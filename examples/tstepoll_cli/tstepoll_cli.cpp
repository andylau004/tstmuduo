


#include "tstepoll_cli.h"

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


#define MAXSIZE     1024
#define IPADDRESS   "127.0.0.1"
#define SERV_PORT   8787
#define FDSIZE        1024
#define EPOLLEVENTS 512


void handle_connection(int sockfd);
void handle_events(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf);
void do_read(int epollfd,int fd,int sockfd,char *buf);
void do_read(int epollfd,int fd,int sockfd,char *buf);
void do_write(int epollfd,int fd,int sockfd,char *buf);
void add_event(int epollfd,int fd,int state);
void delete_event(int epollfd,int fd,int state);
void modify_event(int epollfd,int fd,int state);


struct client_data {
    sockaddr_in address;
    char* write_buff;
    char  buf[ BUFFER_SIZE ];
};

int setnonblocking( int fd ) {
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}
void handle_connection(int sockfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAXSIZE] = { 0 };
    int ret;
    epollfd = epoll_create(FDSIZE);
    add_event(epollfd,STDIN_FILENO,EPOLLIN);
    for ( ; ; ) {
        ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
        handle_events(epollfd,events,ret,sockfd,buf);
    }
    close(epollfd);
}

void handle_events(int epollfd,struct epoll_event *events,int num,int sockfd,char *buf)
{
    int fd;
    int i;
    for (i = 0;i < num;i++) {
        fd = events[i].data.fd;
        if (events[i].events & EPOLLIN) {
            printf ( "client epollin\n" );
            do_read(epollfd,fd,sockfd,buf);
        }
        else if (events[i].events & EPOLLOUT) {
            printf ( "client epollout\n" );
            do_write(epollfd,fd,sockfd,buf);
        }
    }
}

void do_read(int epollfd,int fd,int sockfd,char *buf)
{
    int nread;
    nread = read(fd,buf,MAXSIZE);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
    }
    else if (nread == 0)
    {
        fprintf(stderr,"server close.\n");
        close(fd);
    }
    else
    {
        if (fd == STDIN_FILENO) {
            printf( "read from stdin, buf=%s\n", buf );
            add_event(epollfd,sockfd,EPOLLOUT);
        }
        else {
            delete_event(epollfd,sockfd,EPOLLIN);
            add_event(epollfd,STDOUT_FILENO,EPOLLOUT);
        }
    }
}

void do_write(int epollfd,int fd,int sockfd,char *buf)
{
    int nwrite;
    nwrite = write(fd,buf,strlen(buf));
    if (nwrite == -1) {
        perror("write error:");
        close(fd);
    } else {
        if (fd == STDOUT_FILENO)
            delete_event(epollfd,fd,EPOLLOUT);
        else
            modify_event(epollfd,fd,EPOLLIN);
    }
    memset(buf,0,MAXSIZE);
}

void add_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

void delete_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

void modify_event(int epollfd,int fd,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}


int tst_epollcli_entry(int argc, char *argv[]) {
    int                 sockfd;
    struct sockaddr_in  servaddr;

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);

    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

    handle_connection(sockfd);
    close(sockfd);
    return 0;
}



