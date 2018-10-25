


#include "tstpoll_cli.h"

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

int setnonblocking( int fd ) {
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

int startPollCli() {

    const char* srv_ip = "172.17.0.2";
    uint16_t srv_port = 9090;

    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server_address ) );
    server_address.sin_family = AF_INET;
    inet_pton( AF_INET, srv_ip, &server_address.sin_addr );
    server_address.sin_port  = htons( srv_port );

    int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sockfd >= 0 );
    if( connect( sockfd, ( struct sockaddr* )&server_address, sizeof( server_address ) ) < 0 ) {
        printf("connection failed\n");
        close( sockfd );
        return 1;
    }

    pollfd fds[2];
    fds[0].fd = 0;//fds[0]是标准输入
    fds[0].events = POLLIN;//注册可写事件
    fds[0].revents = 0;

    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;//注册可写和挂起事件
    fds[1].revents = 0;

    char read_buf[ BUFFER_SIZE ] = { 0 };
    int pipefd[2] = { 0 };
    int ret = pipe( pipefd );
    assert( ret != -1 );

    while( 1 ) {
        ret = ::poll( fds, 2, -1 );
        if( ret < 0 ) {//无限期等待注册事件发生
            printf("poll failed\n");
            break;
        }
        if( fds[1].revents & POLLRDHUP ) {//挂起事件则代表服务器关闭了连接
            printf("server close the connection\n");
            break;
        }
        else if( fds[1].revents & POLLIN ) {// readable
            printf("client recv pollin event\n");

            memset( read_buf, '\0', BUFFER_SIZE );
            recv( fds[1].fd, read_buf, BUFFER_SIZE-1, 0 );//接收服务端发送来的数据(服务端的数据是其它用户发送给它的数据)
            printf("read_buf=%s\n", read_buf);
        }
        if( fds[0].revents & POLLIN ) {//标准输入端可写事件发生(该用户有数据输入并需要发送给服务端)
            printf( "fds[0].revents & POLLIN\n" );
            ret = splice( 0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );//将标准输入的数据零拷贝到管道的写端
            ret = splice( pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE );//将管道的读端数据零拷贝到socket描述符
//            break;
        }
    }

    close( sockfd );
    return 1;

//    int ret = 0;
//    struct sockaddr_in  address;
//    ::bzero( &address, sizeof( address ) );

//    address.sin_family = AF_INET;
//    inet_pton( AF_INET, srv_ip, &address.sin_addr );
//    address.sin_port  = htons( srv_port );

//    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
//    assert ( listenfd > 0 );

//    ret = ::bind( listenfd, (struct sockaddr*)&address, sizeof(address) );
//    assert ( ret != -1 );

//    ret = ::listen( listenfd, 5 );

//    client_data* users = new client_data[FD_LIMIT];
//    pollfd fds[USER_LIMIT + 1];
//    int user_counter = 0;

//    for ( int i = 0 ; i <= USER_LIMIT; ++i ) {
//        fds[i].fd = -1;
//        fds[i].events = 0;
//    }
//    fds[0].fd = listenfd;
//    fds[0].events = POLLIN | POLLOUT;
//    fds[0].revents= 0;

//    while ( 1 ) {
//        ret = ::poll( fds, user_counter + 1, -1 );
//        if ( ret < 0 ) {
//            printf( "poll failed!!!\n" ); break;
//        }

//        for( int i=0; i < user_counter + 1; ++i ) {

//            if ( ( fds[i].fd == listenfd ) && ( fds[i].revents & POLLIN ) ) {
//                struct sockaddr_in client_address;
//                socklen_t client_addrlength = sizeof( client_address );
//                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
//                if ( connfd < 0 ) {
//                    printf("accept errno is : %d\n", errno );
//                    continue;
//                }
//                //如果请求太多，则关闭新到的连接
//                if ( user_counter >= USER_LIMIT ) {
//                    const char* info = "too many users\n";
//                    printf("%s\n", info );
//                    send( connfd, info, strlen( info ), 0 );
//                    close( connfd );
//                    continue;
//                }
//                //对于新的连接，同时修改fds users所组。前进文已经提到，users[connfd]
//                //对应于新连接文件描述connfd 的客户数据。
//                user_counter++;
//                users[connfd].address = client_address;
//                setnonblocking( connfd );
//                fds[user_counter].fd = connfd;
//                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
//                fds[user_counter].revents = 0;
//                printf("comes a new user,now have %d users\n", user_counter );
//            } else if ( fds[i].revents & POLLERR ) {
//                printf("get an error from %d\n", fds[i].fd );
//                char errors[ 100 ];
//                memset( errors, '\0', 100 );
//                socklen_t length = sizeof( errors );
//                if( getsockopt( fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &length ) < 0 ) {
//                    printf( "get socket option failed\n" );
//                }
//                continue;
//            } else if( fds[i].revents & POLLRDHUP ) {
//                users[fds[i].fd] = users[fds[user_counter].fd];
//                close( fds[i].fd );
//                fds[i] = fds[user_counter];
//                i--;
//                user_counter--;
//                printf("a client left \n");
//            } else if( fds[i].revents & POLLIN ) {// POLLIN beg
//                printf ( "POLLIN trig\n" );
//                int connfd = fds[i].fd;
//                memset( users[connfd].buf, '\0', BUFFER_SIZE );
//                ret = ::recv( connfd, users[connfd].buf, BUFFER_SIZE-1, 0 );
//                printf("get %d bytes of client data %s from fd:%d\n", ret, users[connfd].buf, connfd );
//                if( ret < 0 ) {
//                    if( errno != EAGAIN ) {
//                        printf( "recv failed! errno=%d\n", errno );
//                        close( connfd );
//                        users[fds[i].fd] = users[fds[user_counter].fd];
//                        fds[i] = fds[user_counter];
//                        i--;
//                        user_counter--;
//                    }
//                } else if( ret == 0 ) {
//                    printf( "recv data ret=0\n" );
//                } else {
//                    printf ( "cur user_counter=%d\n", user_counter );
//                    int j;
//                    //如果接收到客户数据，则通知其他socket 连接准备写数据
//                    for( j = 1; j <= user_counter; ++j ) {
//                        if( fds[j].fd == connfd ) {
//                            continue;
//                        }
//                        fds[j].events |= ~POLLIN;
//                        fds[j].events |= POLLOUT;
//                        users[fds[j].fd].write_buff = users[connfd].buf;
//                    }// for end
//                }
//            }// POLLIN end
//            else if( fds[i].revents & POLLOUT )  {// POLLOUT beg
//                printf ( "POLLOUT trig\n" );
//                int connfd = fds[i].fd;
//                if( ! users[connfd].write_buff ) {
//                    continue;
//                }
//                ret = send( connfd, users[connfd].write_buff, strlen( users[connfd].write_buff ), 0 );
//                users[connfd].write_buff = NULL;

////                printf( "send connfd: %d sendlen: %d content: %s\n", connfd, ret, users[connfd].write_buff );
//                fds[i].events |= ~POLLOUT;
//                fds[i].events |= POLLIN;
//            }// POLLOUT end

//        }

//    }

}

int tst_poll_cli_entry(int argc, char *argv[]) {
    startPollCli();
    return 1;
}



