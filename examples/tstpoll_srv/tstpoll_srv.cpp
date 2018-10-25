


#include "tstpoll_srv.h"

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

void startPollServer() {

    const char* srv_ip = "";
    uint16_t srv_port = 9090;

    int ret = 0;
    struct sockaddr_in  address;
    ::bzero( &address, sizeof( address ) );

    address.sin_family = AF_INET;
    inet_pton( AF_INET, srv_ip, &address.sin_addr );
    address.sin_port  = htons( srv_port );

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert ( listenfd > 0 );

    setReuseAddr( listenfd, 1 );
    setReusePort( listenfd, 1 );
    ret = ::bind( listenfd, (struct sockaddr*)&address, sizeof(address) );
    assert ( ret != -1 );

    ret = ::listen( listenfd, 5 );

    client_data* users = new client_data[FD_LIMIT];
    pollfd fds[USER_LIMIT + 1];
    int user_counter = 0;

    for ( int i = 0 ; i <= USER_LIMIT; ++i ) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = listenfd;
    fds[0].events = POLLIN | POLLOUT;
    fds[0].revents= 0;

    while ( 1 ) {
        ret = ::poll( fds, user_counter + 1, -1 );
        if ( ret < 0 ) {
            printf( "poll failed!!!\n" ); break;
        }

        for( int i=0; i < user_counter + 1; ++i ) {//这是poll的特征，遍历全部注册文件描述符(+1是由于多了监听描述符的缘故)

            if ( ( fds[i].fd == listenfd ) && ( fds[i].revents & POLLIN ) ) {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if ( connfd < 0 ) {
                    printf("accept errno is : %d\n", errno );
                    continue;
                }
                //如果请求太多，则关闭新到的连接
                if ( user_counter >= USER_LIMIT ) {
                    const char* info = "too many users\n";
                    printf("%s\n", info );
                    send( connfd, info, strlen( info ), 0 );
                    close( connfd );
                    continue;
                }
                //对于新的连接，同时修改fds users所组。前进文已经提到，users[connfd]
                //对应于新连接文件描述connfd 的客户数据。
                user_counter++;
                users[connfd].address = client_address;
                setnonblocking( connfd );
                fds[user_counter].fd = connfd;
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user,now have %d users\n", user_counter );
            } else if ( fds[i].revents & POLLERR ) {
                printf("get an error from %d\n", fds[i].fd );
                char errors[ 100 ];
                memset( errors, '\0', 100 );
                socklen_t length = sizeof( errors );
                if( getsockopt( fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &length ) < 0 ) {
                    printf( "get socket option failed\n" );
                }
                continue;
            } else if( fds[i].revents & POLLRDHUP ) {
                users[fds[i].fd] = users[fds[user_counter].fd];
                close( fds[i].fd );
                fds[i] = fds[user_counter];
                i--;
                user_counter--;
                printf("a client left \n");
            } else if( fds[i].revents & POLLIN ) {//用户连接可读事件发生，表示用户有数据发送到来
                printf ( "pollin trig\n" );
                int connfd = fds[i].fd;
                memset( users[connfd].buf, '\0', BUFFER_SIZE );

                ret = ::recv( connfd, users[connfd].buf, BUFFER_SIZE-1, 0 );
                printf("get %d bytes of client data %s from fd:%d\n", ret, users[connfd].buf, connfd );
                if( ret < 0 ) {
                    if( errno != EAGAIN ) {
                        printf( "recv failed! errno=%d\n", errno );
                        close( connfd );
                        users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                } else if( ret == 0 ) {
                    printf( "recv data ret=0\n" );
                } else {
                    printf ( "cur user_counter=%d\n", user_counter );
                    int j;
                    //如果接收到客户数据，则通知其他socket 连接准备写数据
                    for( j = 1; j <= user_counter; ++j ) {
                        if( fds[j].fd == connfd ) {
                            fds[j].events |= ~POLLIN;
                            fds[j].events |= POLLOUT;
//                            memset(users[connfd].buf, 0, array_size(users[connfd].buf));
//                            strcpy( users[connfd].buf, "srv response=");
                            users[fds[j].fd].write_buff = users[connfd].buf;
                            continue;
                        }
//                        fds[j].events |= ~POLLIN;
//                        fds[j].events |= POLLOUT;
//                        users[fds[j].fd].write_buff = users[connfd].buf;
                    }// for end
                }
            }// POLLIN end
            else if( fds[i].revents & POLLOUT )  {//被置为POLLOUT事件后意味着有某个用户的数据需要发送给当前用户
                printf ( "pollout trig\n" );
                int connfd = fds[i].fd;
                if( !users[connfd].write_buff ) {
                    printf ( "write_buff is null i=%d\n", i );
                    fds[i].events |= ~POLLOUT;
                    fds[i].events |= POLLIN;
                    break;
                    continue;
                }
                ret = send( connfd, users[connfd].write_buff, strlen(users[connfd].write_buff), 0 );
                users[connfd].write_buff = NULL;//恢复当前用户数据

//                printf( "send connfd: %d sendlen: %d content: %s\n", connfd, ret, users[connfd].write_buff );
                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }// POLLOUT end

        }

    }

}

int tst_poll_entry(int argc, char *argv[]) {

    startPollServer();
    return 1;


    struct pollfd fds[2];
    int ret;

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    /* watch stdout for ability to write */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;

    ret = ::poll( fds, 2, TIMEOUT * 1000 );
    if ( ret == -1 ) {
        perror(" -------------poll failed!!! \n");
        return -1;
    }
    if (!ret) {
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }

    if (fds[0].revents & POLLIN)
        printf ("stdin is readable\n");

    if (fds[1].revents & POLLOUT)
        printf ("stdout is writable\n");

    return 0;
}



