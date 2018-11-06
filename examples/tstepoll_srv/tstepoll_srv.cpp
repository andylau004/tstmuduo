


#include "tstepoll_srv.h"

//#include <stdio.h>
//#include <iostream>
//#include <string>

//#include <list>

//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/scoped_ptr.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>

#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"
//#include "muduo/base/ThreadPool.h"

#include "muduo/base/libevent.h"


using namespace std;
using namespace muduo;



#define USER_LIMIT 5  // MAX user number
#define TIMEOUT 5
#define BUFFER_SIZE 256
#define FD_LIMIT 65535 /* 文件描述符数量限制 */



struct client_data {
    sockaddr_in address;
    char* write_buff;
    char  buf[ BUFFER_SIZE ];
};

int setnonblocking( const int& fdsock ) {
    if (evutil_make_socket_nonblocking(fdsock) < 0) {
        std::cout << "set fd=" << fdsock << " nonblocking failed." << std::endl;
        EVUTIL_CLOSESOCKET(fdsock);
        return -1;
    }
    return fdsock;
//    int old_option = fcntl( fd, F_GETFL );
//    int new_option = old_option | O_NONBLOCK;
//    fcntl( fd, F_SETFL, new_option );
//    return old_option;
}

void  setReuseAddr(const int& sockfd_, bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
    // FIXME CHECK
}

void  setReusePort(const int& sockfd_, bool on)
{
//#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on) {
//        LOG_SYSERR << "SO_REUSEPORT failed.";
        printf( "SO_REUSEPORT failed.\n" );
    }
//#else
//    if (on)
//    {
//        LOG_ERROR << "SO_REUSEPORT is not supported.";
//    }
//#endif
}

#define IPADDRESS   "127.0.0.1"
#define PORT        8787
#define MAXSIZE     4096
#define LISTENQ     2048
#define FDSIZE      1000
#define EPOLLEVENTS 512

//函数声明
//创建套接字并进行绑定
int socket_bind(const char* ip,int port);
//IO多路复用epoll
void do_epoll(int listenfd);
//事件处理函数
void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf);
//处理接收到的连接
void handle_accpet(int epollfd,int listenfd);
//读处理
void do_read(int epollfd,int fd,char *buf);
//写处理
void do_write(int epollfd,int fd,char *buf);
//添加事件
void add_event(int epollfd,int fd,int state);
//修改事件
void modify_event(int epollfd,int fd,int state);
//删除事件
void delete_event(int epollfd,int fd,int state);

void handle_accpet(int epollfd,int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t  cliaddrlen;
    clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
    if (clifd == -1)
        perror("accpet error:");
    else
    {
        printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);
        //添加一个客户描述符和事件
        add_event(epollfd,clifd,EPOLLIN);
    }
}
int socket_bind(const char* ip,int port)
{
    int  listenfd;
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if (listenfd == -1) {
        perror("socket error:");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1) {
        perror("bind error: ");
        exit(1);
    }
    return listenfd;
}
void do_epoll(int listenfd) {
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    int numEvents;
    char buf[MAXSIZE] = { 0 };

    epollfd = epoll_create(FDSIZE);
    //添加监听描述符事件
    add_event(epollfd,listenfd,EPOLLIN);
    for ( ; ; ) {
        //获取已经准备好的描述符事件
        numEvents = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, numEvents, listenfd, buf);
    }
    close(epollfd);
}
void do_read(int epollfd,int fd,char *buf) {
    ssize_t irecv = 0;
    irecv = ::recv( fd, buf, MAXSIZE-1, 0 );
    if (irecv == -1) {
        perror("read error...!!!\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    } else if ( irecv == 0 ) {
        perror("client close...!!!\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    } else {
        printf("read message is : %s\n", buf);
        strcat(buf, " ------ srv response");
        //修改描述符对应的事件，由读改为写
        modify_event(epollfd, fd, EPOLLOUT);
    }
}
void do_write(int epollfd,int fd,char *buf) {
    ssize_t iwrite = 0;
    iwrite = ::send( fd, buf, strlen(buf), 0 );
    if ( iwrite == 0 ) {
        perror("write error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    } else {
        modify_event(epollfd, fd, EPOLLIN);
    }
    memset(buf, 0, MAXSIZE);
}
void add_event(int epollfd,int fd,int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}
void delete_event(int epollfd,int fd,int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}
void modify_event(int epollfd,int fd,int state) {
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}

void handle_events(int epollfd,struct epoll_event *events,int numEvents,int listenfd,char *buf)
{
    int i;
    int fd;

    for (i = 0; i < numEvents; ++i)
    {
        fd = events[i].data.fd;

        //根据描述符的类型和事件类型进行处理
        if ( (fd == listenfd) && (events[i].events && EPOLLIN) )
            handle_accpet(epollfd, fd);
        else if (events[i].events & EPOLLIN) {
            do_read(epollfd, fd, buf);
        } else if (events[i].events & EPOLLOUT) {
            do_write(epollfd, fd, buf);
        } else {
            printf( "unknown events=%d\n", events[i].events );
        }
    }
}
int tst_epollsrv_entry(int argc, char *argv[]) {
    int listenfd = socket_bind(IPADDRESS,PORT);
    listen(listenfd,LISTENQ);
    do_epoll(listenfd);
    return 0;
//    startPollServer();
    return 1;
}



