
#include "tsteventfd.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>


#include <iostream>
#include <sstream>

#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>



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



#include "muduo/base/thrift_helper.h"
#include "muduo/base/thrift_connection_pool.h"


int g_efd = -1;

event_base* mainbase_ = nullptr;




void* read_thread(void* dummy) {
    int ret = 0;
    uint64_t count = 0;
    int ep_fd = -1;
    struct epoll_event events[10];

    if (g_efd <= 0) {
        printf("efd not inited.\n");
        goto fail;
    }

    ep_fd = epoll_create(1024);
    if (ep_fd <= 0) {
        printf("efd not inited.\n");
        goto fail;
    }

    {
        struct epoll_event read_event;
        read_event.events = EPOLLHUP | EPOLLERR | EPOLLIN;
        read_event.data.fd = g_efd;
        ret = epoll_ctl( ep_fd, EPOLL_CTL_ADD, g_efd, &read_event );
        if (ret < 0) {
            printf("epoll ctl failed.\n");
            goto fail;
        }
    }

    while (1) {
        ret = ::epoll_wait(ep_fd, &events[0], 10, -1/*2000*/);
        if (ret > 0) {
            int i = 0;

            for (; i < ret; ++i) {

                if (events[i].events & EPOLLHUP) {
                    printf("epoll eventfd has epoll hup.\n");
                    goto fail;
                }
                else if (events[i].events & EPOLLERR) {
                    printf("epoll eventfd has epoll error.\n");
                    goto fail;
                } else if (events[i].events & EPOLLIN) {
                    int event_fd = events[i].data.fd;

                    ret = ::read(event_fd, &count, sizeof(count));
                    if (ret < 0) {
                        perror("read fail:");
                        goto fail;
                    } else {
//                        struct timeval tv;
//                        gettimeofday(&tv, NULL);
//                        printf("success read from efd, read %d bytes(%llu) at %lds %ldus\n",
//                               ret, count, tv.tv_sec, tv.tv_usec);
                        printf("success read from efd, read ret=%d count=(%llu) events[%d].events=%d\n",
                               ret, count, i, events[i].events);
                    }

                } else {
                    printf( "unknown type events[%d].events=%d ret=%d\n", i, events[i].events, ret );
                }

            }// end for

        } else {
            perror("epoll wait error!!!\n");
            goto fail;
        }
    }

fail:
    if (ep_fd >= 0) {
        close(ep_fd);
        ep_fd = -1;
    }
    return nullptr;
}

int write_data() {
    int64_t ucount = 0;

    ::sleep(3);
    int iwheel = 0;
    for (int i = 0; i < 5; ++i) {
        ucount = 4;

        int ret = ::write( g_efd, &ucount, sizeof(ucount) );
        if (ret < 0 ) {
            printf( "write data failed!!! sizeof(ucount)=%d g_efd=%d errCode=%s ret=%d\n",
                    sizeof(ucount), g_efd, strerror_tl(errno), ret );
            return -1;
        } else {
            struct timeval tv;
            gettimeofday(&tv, NULL);

            printf( "success write to efd, write %d bytes(%llu) at %lds %ldus\n",
                    ret, ucount, tv.tv_sec, tv.tv_usec );
        }

        iwheel ++;

        if (iwheel>0 && (iwheel % 2) == 0) {
            ::sleep(10);
        }

    }
}

void OnReadSignal(evutil_socket_t fd, short which, void * v) {
    if (v == NULL) {
//        printf( "recv v==nullptr\n" );
//        return;
    }
//    printf( "fd=%d recv signal\n", fd );

    int64_t recvI64 = -1;
    int ret = ::read(fd, &recvI64, sizeof(recvI64));
    if (ret < 0) {
        perror("OnReadSignal read fail:\n");
        return;
    } else {
        printf("OnReadSignal success read from efd, read ret=%d count=(%llu)\n",
               ret, recvI64);
    }

}

// eventfd 绑定libevent事件，等待触发读
int libevent_impl_eventfd() {
    OutputDbgInfo tmp( "libevent_impl_eventfd beg", "libevent_impl_eventfd end" );

    g_efd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
//    g_efd = ::eventfd(0, 0);
    printf("g_efd=%d\n", g_efd);

    struct event fd_event;

    mainbase_ = event_base_new();
    if (mainbase_ == nullptr) {
        printf("event_base_new failed!\n");
        return -1;
    }

    ::event_set(&fd_event, g_efd, EV_READ|EV_PERSIST, OnReadSignal, nullptr);

    if (::event_base_set(mainbase_, &fd_event) < 0) {
        printf( " event_base_set failed!\n" );
        return -1;
    }
    if (::event_add(&fd_event, nullptr) < 0) {
        printf( " event_add failed!\n" );
        return -1;
    }

    std::thread* pWorkThread = new std::thread(&write_data);

    int code = event_base_loop(mainbase_, 0);
    if (code == 0) {
        printf("success\n");
    } else if (code == 1) {
        printf("no events were registered\n");
    } else {
        printf("something wrong\n");
    }
    return 0;
}

// eventfd 绑定epoll事件，等待触发读
int tst_epoll_eventfd() {
    OutputDbgInfo tmp( "tst_epoll_eventfd beg", "tst_epoll_eventfd end" );

    uint64_t ucount = 0;
    pthread_t tid = 0;

    g_efd = ::eventfd(0, 0);

    int ret = ::pthread_create(&tid, nullptr, read_thread, nullptr);
    if ( ret < 0 ) {
        printf( "create thread failed!!!\n" );
        return -1;
    }

    for (int i = 0; i < 5; ++i) {
        ucount = 4;

        ret = ::write( g_efd, &ucount, sizeof(ucount) );
        if (ret < 0 ) {
            printf( "write data failed!!!\n" );
            return -1;
        } else {
            struct timeval tv;
            gettimeofday(&tv, NULL);

            printf("success write to efd, write %d bytes(%llu) at %lds %ldus\n",
                   ret, ucount, tv.tv_sec, tv.tv_usec);
        }
        ::sleep(5);
    }
    printf( "main thread is work done!!!\n" );

failed:
    if (tid > 0) {
        pthread_join(tid, nullptr);
        tid = 0;
    }
    if (g_efd > 0) {
        close(g_efd);g_efd = 0;
    }

    return 0;
}

// 测试 eventfd 事件通知机制
int tst_event_fd_entry(int argc, char *argv[]) {

    // event style
    libevent_impl_eventfd(); return 1;

    // epoll style
    tst_epoll_eventfd(); return 1;
}






