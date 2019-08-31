
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


#include <event2/http.h>
#include <event2/rpc.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>




#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>


#include "getchunkedclient.h"



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



extern int tst_some_event_fd_1(int argc, char *argv[]);

extern void  start_work();



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
        read_event.events = EPOLLHUP | EPOLLERR | EPOLLIN | EPOLLET;
        read_event.data.fd = g_efd;
        ret = epoll_ctl( ep_fd, EPOLL_CTL_ADD, g_efd, &read_event );
        if (ret < 0) {
            printf("epoll ctl failed.\n");
            goto fail;
        }
    }

    while (1) {
        ret = ::epoll_wait(ep_fd, &events[0], 10, -1/*2000*/);
        printf( "epoll wati ret=%d\n", ret );
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
    char tmpbuffer[ 2048 ] = {0};

    ::sleep(3);
    int iwheel = 0;
    for (int i = 0; i < 5; ++i) {
        ucount = 4;

        int ret = ::write( g_efd, &ucount, sizeof(ucount) );
        if (ret < 0 ) {
            sprintf( tmpbuffer, "write data failed!!! sizeof(ucount)=%d g_efd=%d errCode=%s ret=%d",
                     sizeof(ucount), g_efd, strerror_tl(errno), ret );
            LOG_ERROR << tmpbuffer;
            return -1;
        } else {
            struct timeval tv;
            gettimeofday(&tv, NULL);

            sprintf( tmpbuffer, "success write to efd, write %d bytes(%llu) at %lds %ldus",
                     ret, ucount, tv.tv_sec, tv.tv_usec );
            LOG_INFO << tmpbuffer;
        }

        if (iwheel ++ && iwheel>0 && (iwheel % 2) == 0) {
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
    char tmpbuffer[ 2048 ] = {0};

    int64_t recvI64 = -1;
    int ret = ::read(fd, &recvI64, sizeof(recvI64));
    if (ret < 0) {
        perror("OnReadSignal read fail:\n");
        return;
    } else {
        sprintf(tmpbuffer, "OnReadSignal success read from efd, read ret=%d count=(%llu)",
                ret, recvI64);
        LOG_INFO << tmpbuffer;
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
    char tmpbuffer[ 2048 ] = {0};

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
            LOG_ERROR << "write data failed!!!";
            return -1;
        } else {
            struct timeval tv;
            gettimeofday(&tv, NULL);

            sprintf(tmpbuffer,"success write to efd, write %d bytes(%llu) at %lds %ldus",
                    ret, ucount, tv.tv_sec, tv.tv_usec);
            LOG_INFO << tmpbuffer;
        }
        ::sleep(5);
    }
    LOG_INFO << ( "main thread is work done!!!\n" );

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

const char* STR_CRLF = "\r\n";

// evbuffer 测试使用示例
void tst_evbuffer_use() {
    LOG_DEBUG << " tst_evbuffer_use ------------------";
    printf( " tst_evbuffer_use ------------------\n" );

    struct evbuffer* requestBuffer = evbuffer_new();

    int len1 = 0;
    const char* lpszUserData = "12345678";
    len1 = strlen(lpszUserData);
    uint32_t nw_len = ::htonl(len1);

    evbuffer_add( requestBuffer, &nw_len, sizeof(nw_len) );// 4 bytes
    evbuffer_add( requestBuffer, STR_CRLF, strlen(STR_CRLF) );// 2 bytes

    evbuffer_add( requestBuffer, lpszUserData, strlen(lpszUserData) );// 8 bytes
    evbuffer_add( requestBuffer, STR_CRLF, strlen(STR_CRLF) );// 2 bytes

    {
        size_t requestLen = evbuffer_get_length(requestBuffer);
        LOG_INFO << "allsize=" << requestLen;

        char* copy_out_buf = new char[requestLen + 1];
        memset( copy_out_buf, 0, requestLen + 1 );

        ev_ssize_t copy_out_n = evbuffer_copyout( requestBuffer, copy_out_buf, requestLen );
//        printf("data[%s] len[%d]\n" , copy_out_buf, copy_out_n);
        printf_buffer( "copy_out_buf---------------", (unsigned char*)copy_out_buf, copy_out_n );
/*
 * 20190126 05:05:21.834069Z 10044 INFO  allsize=16 - tsteventfd.cpp:331
    ==========================================================
    copy_out_buf---------------: length:16
    08 00 00 00 0d 0a 31 32 33 34 35 36 37 38 0d 0a
    ==========================================================
    20190126 05:05:21.834087Z 10044 DEBUG tst_evbuffer_use  111 ptrFind.pos=4 - tsteventfd.cpp:349
    20190126 05:05:21.834094Z 10044 DEBUG tst_evbuffer_use  222 ptrFind.pos=5 - tsteventfd.cpp:355
    20190126 05:05:21.834099Z 10044 DEBUG tst_evbuffer_use  333 ptrFind.pos=14 - tsteventfd.cpp:359
*/
    }

    {
        size_t buffer_len = evbuffer_get_length(requestBuffer);
        LOG_INFO << "---- buffer_len=" << buffer_len;

        ev_uint32_t record_len = 0;
        char* record = nullptr;

        struct evbuffer_ptr ptrFind = evbuffer_search( requestBuffer, STR_CRLF, strlen(STR_CRLF), nullptr );
        if ( (int)(ptrFind.pos) == -1 ) {
            LOG_ERROR << " no found length str_crlf" ;
            return ;
        } else {
            // copy 4 bytes head.
            evbuffer_copyout( requestBuffer, &record_len, sizeof(ev_uint32_t) );

            record_len = ::ntohl(record_len); // Convert len_buf into host order.
            if (buffer_len < record_len + 4) {
                LOG_ERROR << " want more data, buffer_len: " << buffer_len << " < record_len + 4: " << record_len + 4;
                return; /* The record hasn't arrived */
            }
            LOG_DEBUG << " record_len=" << record_len;

            LOG_DEBUG << " 111 ptrFind.pos=" << ptrFind.pos;
            evbuffer_ptr_set( requestBuffer, &ptrFind, 1, EVBUFFER_PTR_ADD );
            LOG_DEBUG << " 222 ptrFind.pos=" << ptrFind.pos;

            // 检查 "chunk\r\n"
            ptrFind = evbuffer_search( requestBuffer, STR_CRLF, strlen(STR_CRLF), &ptrFind );
            LOG_DEBUG << " 333 ptrFind.pos=" << ptrFind.pos;

            if ( (int)(ptrFind.pos) == -1 ) {
                LOG_ERROR << " no found chunked str_crlf";
                return;
            }

            record = NewAllocMem(record_len);
            if (record == NULL) {
                LOG_ERROR << " alloc new memory failed!!!";
                return;
            }

            evbuffer_drain( requestBuffer, sizeof(ev_uint32_t) );
            evbuffer_drain( requestBuffer, strlen(STR_CRLF) );

            evbuffer_remove( requestBuffer, record, record_len );
            evbuffer_drain( requestBuffer, strlen(STR_CRLF) );

            LOG_INFO << "record.size()=" << record_len << ", record=" << record;

            int iiit = 123;
    //        printf("===evbuffer_search() : print buffer from search result point===\n");
    //        print_buf( requestBuffer, &ptrFind );
        }
    }

    //evbuffer_add , evbuffer_add_printf
//    evbuffer_add( buf , "merry christmas\n" , strlen("merry christmas\n") );
//    evbuffer_add_printf( buf , "hello %s\n" , "world" );

    //evbuffer_get_length
//    size_t len = evbuffer_get_length(buf);
//    printf("===evbuffer_get_length() : [%d]===\n" , len);

//    //evbuffer_get_contiguous_space
//    len = evbuffer_get_contiguous_space(buf);
//    printf("===evbuffer_get_contiguous_space() : [%d]===\n" , len);

//    //evbuffer_readln
//    char * line = evbuffer_readln( buf , &len , EVBUFFER_EOL_ANY );
//    if ( line != NULL) {
//        printf("===evbuffer_readln(): line[%s] len[%d]===\n" , line , len);
//        free(line);
//    }

}


static char const* const CHUNKS[] = {
    "This is funny",
    "but not hilarious.",
    "bwv 1052"
};

struct chunk_req_state {
    struct event_base *base;
    struct evhttp_request *req;
    int i;
};
static void
http_chunked_trickle_cb(evutil_socket_t fd, short events, void *arg)
{
    struct evbuffer *evb = evbuffer_new();
    struct chunk_req_state *state = (struct chunk_req_state *)arg;
    struct timeval when = { 0, 0 };

    evbuffer_add_printf(evb, "%s", CHUNKS[state->i]);
    evhttp_send_reply_chunk(state->req, evb);
    evbuffer_free(evb);

    if (++state->i < (int) (sizeof(CHUNKS)/sizeof(CHUNKS[0]))) {
        event_base_once(state->base, -1, EV_TIMEOUT, http_chunked_trickle_cb, state, &when);
    } else {
        evhttp_send_reply_end(state->req);
        free(state);
    }
}
static void
http_chunked_cb(struct evhttp_request *req, void *arg)
{
    struct timeval when = { 0, 0 };
    struct chunk_req_state *state = (struct chunk_req_state *)malloc(sizeof(struct chunk_req_state));
    printf(("%s: called\n", __func__));

    memset(state, 0, sizeof(struct chunk_req_state));
    state->req  = req;
    state->base = (struct event_base *)arg;

    if (strcmp(evhttp_request_get_uri(req), "/streamed") == 0) {
        evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Length", "39");
    }
    evhttp_send_reply_start(req, HTTP_OK, "Everything is fine");// generate a chunked/streamed reply

    /* but trickle it across several iterations to ensure we're not assuming it comes all at once */
    event_base_once((struct event_base *)arg, -1, EV_TIMEOUT, http_chunked_trickle_cb, state, (struct timeval *)&when);
}
void tst_some_http_server() {
    struct evhttp_bound_socket *handle = NULL;
    evutil_socket_t server_fd = -1;

    struct event_base * m_pEventBase = event_base_new();
    if (!m_pEventBase) {
        printf("event_base_new failed: %s\n", strerror(errno));
        return;
//        goto ErrP;
    }
    struct evhttp* m_pHttpHandler = evhttp_new(m_pEventBase);
    if (!m_pHttpHandler) {
        printf("evhttp_new failed: %s\n", strerror(errno));
        return;
//        goto ErrP;
    }

    handle = evhttp_bind_socket_with_handle(m_pHttpHandler, "0.0.0.0", 10086);
    if (!handle) {
        printf("evhttp_bind_socket_with_handle failed: %s\n", strerror(errno));
        return;
//        goto ErrP;
    }
    server_fd = evhttp_bound_socket_get_fd(handle);
    printf("evhttp_bind_socket_with_handle succeed: %d\n", server_fd);

    evhttp_set_timeout(m_pHttpHandler, 120);
//    evhttp_set_cb(m_pHttpHandler, "/platform/api2/file2text", default_request_cb, &m_pEventBase);
//    evhttp_set_cb(m_pHttpHandler, "/platform/api2/stream2text", stream_request_cb, &m_pEventBase);

    evhttp_set_cb(m_pHttpHandler, "/chunked",  http_chunked_cb, &m_pEventBase);
    evhttp_set_cb(m_pHttpHandler, "/streamed", http_chunked_cb, &m_pEventBase);

    event_base_dispatch(m_pEventBase);

//ErrP:
//    LOG_ERROR << " fatal error bind http socket";
//    if(m_pHttpHandler) evhttp_free(m_pHttpHandler);
//    if(m_pEventBase) event_base_free(m_pEventBase);
//    m_pHttpHandler = NULL;
//    m_pEventBase = NULL;
}

typedef  struct _tag_Test {
    unsigned int chunked:1,		/* a chunked request */
        userdone:1;			/* the user has sent all data */
    int xxx;
}Test;


void demo_asr(const std::string &file0, const std::string &file1, int agent_dn) {
    LOG_INFO << "BEG...";
//    LOG_INFO << " workthread, file0" << file0 << ", file1" << file1;
    LOG_INFO << "agent_dn=" << agent_dn;
    LOG_INFO << "END...";
//    LOG_INFO << " ";
    printf( "\n" );
}

void tst_threads_() {
    std::string file0 = "file____0";
    std::string file1 = "file____1";
    auto concurrency  = 2;

    std::vector<std::thread> threads;
    for ( int j = 0 ; j < 3; ++ j ) {

//        for (int i = 0; i < concurrency; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(21));
            threads.emplace_back(demo_asr, file0, file1, j * 100/* + i*/);
//        }

    }
    std::for_each( threads.begin(), threads.end(), [](std::thread& t){ t.join();} );

    LOG_INFO << "work all done";
}

extern int newchukclient();

class Testcccc {
public:
    Testcccc(int i) {
        m_indx = i;
        LOG_INFO << "Testcccc cst----------------idx=" << m_indx;
    }
    ~Testcccc() { LOG_INFO << "Testcccc dst----------------idx=" << m_indx; }

    void PrintVal() {
        LOG_INFO << "val=" << m_indx;
    }
private:
    int m_indx;
};

void tst_122xxxxx2( ) {
    boost::ptr_vector<Testcccc> arr_objs;

    for (int i = 0; i < 3; ++i ) {
        arr_objs.push_back( new Testcccc(i) );
    }
    for (int i = 0; i < 3; ++i ) {
        arr_objs[i].PrintVal();
    }

//    for_each( arr_objs.begin(), arr_objs.end(), boost::bind(&Testcccc::PrintVal, _1) );
}

// 测试 eventfd 事件通知机制
int tst_event_fd_entry(int argc, char *argv[]) {

    start_work(); return 1;

    tst_some_event_fd_1(argc, argv); return 1;


    tst_epoll_eventfd(); return 1;


    tst_122xxxxx2(); return 1;

    newchukclient(); return 1;

//    Test c11;
//    LOG_INFO << " c11.chunked=" << c11.chunked
//             << " c11.userdone=" << c11.userdone
//             << " c11.xxx=" << c11.xxx;
//    return 1;
    tst_threads_(); return 1;

    tst_some_http_server(); return 1;

    tst_evbuffer_use(); return 1;


    tst_getchunked_url(); return 1;


    // event style
    libevent_impl_eventfd(); return 1;

    // epoll style
}






