

#include "twfun.h"




#include <cstdio>
#include <cstdlib>


#include <time.h>

#include <functional>

#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>






#include <sys/timerfd.h>
#include <time.h>

#include <sys/epoll.h>
#include <sys/socket.h>

#include <unistd.h>
#include <signal.h>


#include "timewheel.h"



using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;




using namespace std;
using namespace muduo;
using namespace muduo::net;






#define FD_LIMIT 2048
#define MAX_EVENT_NUMBER 1024
#define TIMESLOT 10



client_data *users = nullptr;
time_wheel client_conn_time_wheel;


struct Con {
    string readed;
    size_t written;
    Con() : written(0) {}
};

map<int, Con> cons;
std::string httpRes;

int g_epFd = 0;

bool output_log = true;

#define exit_if(r, ...)                                                                          \
    if (r) {                                                                                     \
        printf(__VA_ARGS__);                                                                     \
        printf("%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); \
        exit(1);                                                                                 \
    }

int g_idx = 0;

int pipefd[2];

// pipefd[0] ===> recvfd
// pipefd[1] ===> sendfd

void setNonBlock(int fd);



int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                   TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0)
    {
        LOG_SYSFATAL << "Failed in timerfd_create";
    }
    return timerfd;
}



int addfd(int fd) {
    epoll_event event;

    event.data.fd = fd;
    event.events  = EPOLLIN | EPOLLET;
    epoll_ctl( g_epFd, EPOLL_CTL_ADD, fd, &event );

    setNonBlock(fd);
}

void sig_handler(int sig)
{
    int save_error = errno;
    int msg = sig;
    send(pipefd[1], (char*)&msg, 1, 0);
    errno = save_error;
}
void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;

    sigfillset(&sa.sa_mask);

    assert(sigaction(sig, &sa, NULL) != -1);
}

void timer_handler() {
    client_conn_time_wheel.tick();
    alarm(1);
}
void timeout_cb(client_data *user_data)
{
    epoll_ctl(g_epFd, EPOLL_CTL_DEL, user_data->client_sock, 0);
    LOG_INFO << "time is out!!! close fd=" << user_data->client_sock;

    assert(user_data);
    close(user_data->client_sock);
}



void setNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags < 0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    exit_if(r < 0, "fcntl failed");
}

void updateEvents(int fd, int events, int op) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events  = events;
    ev.data.fd = fd;

    char tmpbuff[1024] = {0};
    sprintf(tmpbuff, "%s fd %d events read %d write %d",
           op == EPOLL_CTL_MOD ? "[mod]" : "[add]",
           fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);
    LOG_INFO << tmpbuff;

    int r = epoll_ctl(g_epFd, op, fd, &ev);
    exit_if(r, "epoll_ctl failed");
}

void handleAccept(int fd) {
    struct sockaddr_in adr;
    socklen_t rsz = sizeof(adr);

    int cfd = accept(fd, (struct sockaddr *) &adr, &rsz);
    exit_if(cfd < 0, "accept failed");

    sockaddr_in peer, local;
    socklen_t alen = sizeof(peer);

    int r = getpeername(cfd, (sockaddr *) &peer, &alen);
    exit_if(r < 0, "getpeername failed");

    LOG_INFO << "accept a connection from: " << inet_ntoa(adr.sin_addr);

    setNonBlock(cfd);

    // 启动时,内核写缓冲区为空, 会立即触发 EPOLLOUT, 所以去掉该选项,刚收到连接时,因为暂时无数据要写
    // 此例逻辑 等待收到数据后,再触发写
    updateEvents(cfd, EPOLLIN /*| EPOLLOUT*/ | EPOLLET, EPOLL_CTL_ADD);

    users[cfd].client_sock = cfd;

    tw_timer* newtimer = nullptr;
    newtimer = client_conn_time_wheel.add_timer(TIMESLOT);
    if (newtimer) {
        newtimer->user_data = &users[cfd];
        newtimer->cb_func   = timeout_cb;
        LOG_INFO << "cllientFd=" << cfd << " add new timer obj successed!!!";
    } else {
        LOG_INFO << "cllientFd=" << cfd << " add new timer obj failed!!!";
    }

    users[cfd].timerObj = newtimer;
}

void sendRes(int fd) {
    Con &con = cons[fd];
    if (!con.readed.length()) {
        LOG_INFO << "conn.readed.length == 0";
        return;
    } else {
        LOG_INFO << "conn.readed.length=" << con.readed.length();
    }

    size_t left = httpRes.length() - con.written;
    int wd = 0;

    // == 0, 发送48684字节
    // == 1, 发送10000字节
    if ( g_idx == 0 ) {
        left = 48684;
    } else {
//        printf( "g_idx=%d left=%d\n", g_idx, left);
        LOG_INFO << "g_idx=" << g_idx << " left=" << left;
    }

    while ( (wd = ::send(fd, httpRes.data() + con.written, left, 0) ) > 0 ) {
        con.written += wd;
        left -= wd;

        g_idx ++;

        if (output_log)
            LOG_INFO << "write=" << wd << " bytes left=" << left;
    }
    if (left == 0) {
//        printf("left == 0\n");

        if (g_idx == 1) {
            // 故意只发一部分,剩下的设置EPOLLOUT
            updateEvents(fd, EPOLLOUT /*| EPOLLIN*/ | EPOLLET, EPOLL_CTL_MOD);
            g_idx ++;
        } else {
            updateEvents(fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
            // 客户端套接字先不删除
    //        cons.erase(fd);
        }
        return;
    }
    if (wd < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        printf("000 send error for %d: %d %s\n", fd, errno, strerror(errno));
        return;
    }
    if (wd <= 0) {
        printf("111 send error for %d: %d %s\n", fd, errno, strerror(errno));
        close(fd);
        cons.erase(fd);
    }
}
void handleRead(int fd) {
    char buf[4096] = {0};
    int  n = 0;

    while ( (n = ::recv(fd, buf, sizeof(buf), 0)) > 0 ) {
        if (output_log)
            LOG_INFO << "read " << n << " bytes";

        std::string& readed = cons[fd].readed;
        readed.append(buf, n);
        if (readed.length() > 4) {
//            if ( readed.substr(readed.length() - 2, 2) == "\r\n" &&
//                 readed.substr(readed.length() - 4, 4) == "\r\n\r\n" ) {
                LOG_INFO << "aaaaaaaaaaaaaaaaaaa";
                //当读取到一个完整的http请求，测试发送响应
                sendRes(fd);
//            }
        }
    }
    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        printf("000 read %d error: %d %s\n", fd, errno, strerror(errno));
        return;
    }
    //实际应用中，n<0应当检查各类错误，如EINTR
    if (n < 0) {
        printf("111 read %d error: %d %s\n", fd, errno, strerror(errno));
    }
    LOG_INFO << "handleRead close fd=" << fd;
    close(fd);
    cons.erase(fd);
}
void handleWrite(int fd) {
    LOG_INFO << "handleWrite ccccccccccccccccc";
    sendRes(fd);
}
void loop_once(int lfd, int waitms) {
    int ret = 0;
    bool mark_timer = false;

    char tmpbuffer[1024] = {0};
    const int kMaxEvents = 20;

    struct epoll_event activeEvs[100];
    int n = epoll_wait(g_epFd, activeEvs, kMaxEvents, waitms);

    for (int i = 0; i < n; i ++) {
        int fd     = activeEvs[i].data.fd;
        int events = activeEvs[i].events;

        if (events & (EPOLLIN | EPOLLERR)) {
            if (fd == lfd) {
                if (output_log)  LOG_INFO << "handling handleAccept fd=" << fd;

                handleAccept(fd);
            } else if (fd == pipefd[0]) {
                int sig; char recv_buff[1024] = {0};

                ret = recv(fd, recv_buff, array_size(recv_buff), 0);
                if (ret == -1 || ret == 0) { continue; }
                else {
                    for (int i = 0; i < ret; ++i) {
                        switch (recv_buff[i]) {
                        case SIGALRM:
//                            LOG_INFO << "SIGALRM!!!!!!!!!!! fd=" << fd;
                            mark_timer = true;
                            break;
                        case SIGTERM:
                            LOG_INFO << "SIGTERM!!!!!!!!!!! fd=" << fd;
                            break;
                        default:
                            break;
                        }
                    }
                }

            } else {
                if (output_log) LOG_INFO << "handling handleRead fd=" << fd;
                handleRead(fd);
            }
        } else if (events & EPOLLOUT) {
            if (output_log) LOG_INFO << "handling epollout fd=" << fd;
            handleWrite(fd);
        } else {
            LOG_INFO << "unknown event =" << events;
            exit_if(1, tmpbuffer);
        }
    }

    if (mark_timer) {
//        LOG_INFO << "mark_timer=" << mark_timer;
        timer_handler();
        mark_timer = false;
    }
}

void setup_pipefd() {
    int ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    if (ret == -1) {
        printf( "create socketpaire failed!\n" );
        exit(-1);
        return;
    }

    setNonBlock( pipefd[1] );
    addfd( pipefd[0] );
}

void start_et_srv() {
    ::signal(SIGPIPE, SIG_IGN);

    httpRes = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
    LOG_INFO << "httpRes.length=" << httpRes.length();
    for (int i = 0; i < 1048570; i++) {
        httpRes += '\0';
    }
    LOG_INFO << "httpRes.length=" << httpRes.length();

    short port = 80;

    int epollfd = epoll_create(5);
    exit_if(epollfd < 0, "epoll_create failed");
    g_epFd = epollfd;

    setup_pipefd();

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(listenfd < 0, "socket failed");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int one = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<void*>(&one), sizeof(one));

    int r = ::bind(listenfd, (struct sockaddr *) &addr, sizeof(struct sockaddr));
    exit_if(r, "bind to 0.0.0.0:%d failed %d %s", port, errno, strerror(errno));

    r = listen(listenfd, 20);
    exit_if(r, "listen failed %d %s", errno, strerror(errno));

    LOG_INFO << "fd=" << listenfd << " listening at " << port;

    setNonBlock(listenfd);

    addsig(SIGALRM);
    addsig(SIGTERM);

    updateEvents(listenfd, EPOLLIN, EPOLL_CTL_ADD);

    epoll_event events[MAX_EVENT_NUMBER];
    users = new client_data[FD_LIMIT];
    alarm(1);

    bool timeout = false;
    for (;;) {  //实际应用应当注册信号处理函数，退出时清理资源
        loop_once(listenfd, -1/*10000*/);
    }

}


void tst_tw_entry() {
    start_et_srv();
}


