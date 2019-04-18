


#include "tst-et.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>


#include <list>


#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>



#include <sys/epoll.h>
#include <sys/socket.h>




#include <unordered_set>
#include <boost/circular_buffer.hpp>


#include "muduo/net/Callbacks.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"

#include "muduo/base/libevent.h"

#include "muduo/net/TcpServer.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


using namespace boost;

using namespace std;
using namespace muduo;
using namespace muduo::net;


#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

static const char MESSAGE[] = "Hello, World!\n";
static const int PORT = 9995;
//static void listener_cb(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
//static void conn_writecb(struct bufferevent *, void *);
//static void conn_eventcb(struct bufferevent *, short, void *);
//static void signal_cb(evutil_socket_t, short, void *);


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

void setNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags < 0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    exit_if(r < 0, "fcntl failed");
}

void updateEvents(int efd, int fd, int events, int op) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    ev.events  = events;
    ev.data.fd = fd;

    printf("%s fd %d events read %d write %d\n",
           op == EPOLL_CTL_MOD ? "[mod]" : "[add]",
           fd, ev.events & EPOLLIN, ev.events & EPOLLOUT);

    int r = epoll_ctl(efd, op, fd, &ev);
    exit_if(r, "epoll_ctl failed");
}

void handleAccept(int efd, int fd) {
    struct sockaddr_in adr;
    socklen_t rsz = sizeof(adr);

    int cfd = accept(fd, (struct sockaddr *) &adr, &rsz);
    exit_if(cfd < 0, "accept failed");

    sockaddr_in peer, local;
    socklen_t alen = sizeof(peer);

    int r = getpeername(cfd, (sockaddr *) &peer, &alen);
    exit_if(r < 0, "getpeername failed");

    printf("accept a connection from %s\n", inet_ntoa(adr.sin_addr));

    setNonBlock(cfd);

    // 启动时,内核写缓冲区为空, 会立即触发 EPOLLOUT, 所以去掉该选项,刚收到连接时,因为暂时无数据要写
    // 此例子,逻辑,,是等待收到数据后,再触发写
    updateEvents(efd, cfd, EPOLLIN /*| EPOLLOUT*/ | EPOLLET, EPOLL_CTL_ADD);
}
void sendRes(int fd) {
    Con &con = cons[fd];
    if (!con.readed.length()) {
        printf("conn.readed.length == 0\n");
        return;
    } else {
        printf("conn.readed.length=%d\n", con.readed.length());
    }

    size_t left = httpRes.length() - con.written;
    int wd = 0;

    // == 0, 发送48684字节
    // == 1, 发送10000字节
    if ( g_idx == 0 ) {
        left = 48684;
    } else {
        printf( "g_idx=%d left=%d\n", g_idx, left);
    }

    while ( (wd = ::send(fd, httpRes.data() + con.written, left, 0) ) > 0 ) {
        con.written += wd;
        left -= wd;

        g_idx ++;

        if (output_log)
            printf("write=%d bytes left=%lu\n", wd, left);
    }
    if (left == 0) {
        printf("left == 0\n");

        if (g_idx == 1) {
            // 故意只发一部分,剩下的设置EPOLLOUT
            updateEvents(g_epFd, fd, EPOLLOUT /*| EPOLLIN*/ | EPOLLET, EPOLL_CTL_MOD);
            g_idx ++;
        } else {
            updateEvents(g_epFd, fd, EPOLLIN | EPOLLET, EPOLL_CTL_MOD);
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
void handleRead(int efd, int fd) {
    char buf[4096] = {0};
    int  n = 0;

    while ( (n = ::recv(fd, buf, sizeof(buf), 0)) > 0 ) {
        if (output_log)
            printf("read %d bytes\n", n);

        std::string& readed = cons[fd].readed;
        readed.append(buf, n);
        if (readed.length() > 4) {
//            if ( readed.substr(readed.length() - 2, 2) == "\r\n" &&
//                 readed.substr(readed.length() - 4, 4) == "\r\n\r\n" ) {
                printf( "aaaaaaaaaaaaaaaaaaa\n" );
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
    close(fd);
    cons.erase(fd);
}
void handleWrite(int efd, int fd) {
    printf( "handleWrite ccccccccccccccccc\n" );
    sendRes(fd);
}
void loop_once(int efd, int lfd, int waitms) {
    char tmpbuffer[1024] = {0};
    const int kMaxEvents = 20;

    struct epoll_event activeEvs[100];
    int n = epoll_wait(efd, activeEvs, kMaxEvents, waitms);

//    if (output_log)
//        printf("epoll_wait return %d\n", n);

    for (int i = 0; i < n; i ++) {
        int fd     = activeEvs[i].data.fd;
        int events = activeEvs[i].events;

        if (events & (EPOLLIN | EPOLLERR)) {
            if (fd == lfd) {
                if (output_log)
                    printf("handling handleAccept fd=%d\n", fd);
                handleAccept(efd, fd);
            } else {
                if (output_log)
                    printf("handling handleRead fd=%d\n", fd);
                handleRead(efd, fd);
            }
        } else if (events & EPOLLOUT) {
            if (output_log)
                printf("handling epollout fd=%d\n", fd);
            handleWrite(efd, fd);
        } else {
            sprintf(tmpbuffer, "unknown event =%d\n", events);
            exit_if(1, tmpbuffer);
        }

    }

}

void start_et_srv() {
    ::signal(SIGPIPE, SIG_IGN);

    httpRes = "HTTP/1.1 200 OK\r\nConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: 1048576\r\n\r\n123456";
    printf( "httpRes.length=%lu\n", httpRes.length() );
    for (int i = 0; i < 1048570; i++) {
        httpRes += '\0';
    }
    printf( "httpRes.length=%lu\n", httpRes.length() );

    short port = 80;

    int epollfd = epoll_create(1);
    exit_if(epollfd < 0, "epoll_create failed");
    g_epFd = epollfd;

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

    printf("fd %d listening at %d\n", listenfd, port);
    setNonBlock(listenfd);

    updateEvents(epollfd, listenfd, EPOLLIN, EPOLL_CTL_ADD);

    for (;;) {  //实际应用应当注册信号处理函数，退出时清理资源
        loop_once(epollfd, listenfd, -1/*10000*/);
    }

}
int tst_et_entry(int argc, char *argv[]) {
    int tmp_array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    LOG_INFO << "array_size(tmp_array)=" << array_size(tmp_array) * sizeof(int);

    start_et_srv();
    return 1;
}




