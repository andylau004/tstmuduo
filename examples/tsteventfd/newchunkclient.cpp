

#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <err.h>


#include <sys/queue.h>
#include <evhttp.h>
#define MAX_CONN 10240


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/queue.h>
#include <event.h>

#include <string>

#include <string.h>


#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/keyvalq_struct.h>

#include <event2/dns.h>
#include <event2/thread.h>



#include <sys/eventfd.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>             /* Definition of uint64_t */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>



//struct evhttp_request *requests[MAX_CONN];
//struct evbuffer *bufs[MAX_CONN];
//int pos[MAX_CONN], num;

//void on_close(struct evhttp_connection *req, void *arg)
//{
//    int* p = (int*) arg;
//    struct evhttp_request *p2 = requests[*p];
//    struct evbuffer *p3 = bufs[*p];
//    printf("%s closed.\n", p2->remote_host);
//    requests[*p] = NULL;
//    if (p3 != NULL) evbuffer_free(p3);
//}

///* for comet recv */
//void comet_handler(struct evhttp_request *req, void *arg)
//{
//    int i, j;
//    requests[num] = req;
//    struct evbuffer *buf;

//    req->minor = 0;
//    evhttp_add_header(req->output_headers, "Content-Type", "text/plain");
//    evhttp_send_reply_start(req, HTTP_OK, "OK");

//    /* Set a callback for connection close. */
//    evhttp_connection_set_closecb(req->evcon, on_close, &pos[num]);

//    buf = evbuffer_new();
//    bufs[num] = buf;

//    for (j = 0; j < 10; j++)
//        evbuffer_add_printf(buf, "Hi %s, this is a comet server based on libevent, visit http://hi.baidu.com/jabber for more information!\r\n", req->remote_host);
//    evhttp_send_reply_chunk(req, buf);

//    for (i = 0; i < num; i++)
//    {
//        if (requests[i] == NULL)
//            continue;
//        evbuffer_add_printf(bufs[i], "Oops, %s requests!\r\n", req->remote_host);
//        evhttp_send_reply_chunk(requests[i], bufs[i]);
//    }

//    if (num++ == MAX_CONN) err(1, "exceed max conn, TODO.");
//}

///* for comet send, GET /id */
//void notify_handler(struct evhttp_request *req, void *arg)
//{
//    struct evbuffer *buf;
//    int nudge;

//    buf = evbuffer_new();
//    if (buf == NULL)
//        err(1, "failed to create response buffer");
//    const char *uri = evhttp_request_uri(req);
//    evbuffer_add_printf(buf, "Requested: %s, send a nudge to %s\n", uri, uri + 1);

//    nudge = atoi(uri + 1);
//    if (requests[nudge] != NULL)
//    {
//        evbuffer_add_printf(bufs[nudge], "Oops, received a nudge from %s!\r\n", req->remote_host);
//        evhttp_send_reply_chunk(requests[nudge], bufs[nudge]);
//    }
//    evhttp_send_reply(req, HTTP_OK, "OK", buf); // TODO: free buf
//}

////int main(int argc, char **argv)
//int newchukclient()
//{
//    struct evhttp *httpd;
//    int i;
//    for (i = 0; i < 1024; i++)
//    {
//        requests[i] == NULL;
//        bufs[i] = NULL;
//        pos[i] = i;
//    }
//    num = 0;

//    event_init();
//    httpd = evhttp_start("0.0.0.0", 8080);

//    /* Set a comet callback for requests to "/". */
//    evhttp_set_cb(httpd, "/", comet_handler, NULL);

//    /* Set a send callback for all other requests. */
//    evhttp_set_gencb(httpd, notify_handler, NULL);

//    event_dispatch();

//    /* Not reached in this code as it is now. */
//    evhttp_free(httpd);
//    return 0;
//}





#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)


int tst_some_event_fd_1(int argc, char *argv[])
{
    int efd, j;
    uint64_t u;
    ssize_t s;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    efd = eventfd(0, 0);
    if (efd == -1)
        handle_error("eventfd");

    switch (fork()) {
    case 0:
        for (j = 1; j < argc; j++) {
            printf("Child writing %s to efd\n", argv[j]);

            u = strtoull(argv[j], NULL, 0);

            /* strtoull() allows various bases */
            s = write(efd, &u, sizeof(uint64_t));
            if (s != sizeof(uint64_t))
                handle_error("write");
        }
        printf("Child completed write loop\n");
        exit(EXIT_SUCCESS);

    default:
        sleep(2);

        printf("Parent about to read\n");

        s = read(efd, &u, sizeof(uint64_t));
        if (s != sizeof(uint64_t))
            handle_error("read");

        printf("Parent read %llu (0x%llx) from efd\n",
               (unsigned long long) u, (unsigned long long) u);

        exit(EXIT_SUCCESS);

    case -1:
        handle_error("fork");
    }

    return 1;
}

//gcc main.c -o main
//./main  1 2 4 7 14
//child writing 1 to efd
//child writing 2 to efd
//child writing 4 to efd
//child writing 7 to efd
//child writing 14 to efd
//child completed write loop
//parent about to read
//parent read 28 (0x1c) from efd





#define handle_error(...)  \
    do { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); } while (0)


#define log(msg, ...) \
    do { time_t t; time(&t); fprintf(stdout, "[%ld] "msg"\n", t, __VA_ARGS__); fflush(stdout); } while(0)


#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 8
#define MAX_EVENTS_SIZE 1024


typedef struct thread_info {
    pthread_t thread_id;
    int rank;
    int epfd;
}thread_info_t;



static void* consumer_routine( void* data ) {
    thread_info_t* c = (thread_info_t*) data;
    struct epoll_event *events;

    int epfd = c->epfd;
    int nfds = -1;
    int i = -1;
    uint64_t result;

    log("Greetings from [consumer-%d]", c->rank);

    events = (epoll_event*)calloc(MAX_EVENTS_SIZE, sizeof(epoll_event));
    if (events == NULL) handle_error("calloc epoll events\n");

    for (;;) {

        nfds = epoll_wait(epfd, events, MAX_EVENTS_SIZE, 1000); // poll every second
        for (i = 0; i < nfds; i++) {
            if ( events[i].events & EPOLLIN ) {
                log("[consumer-%d] got event from fd-%d", c->rank, events[i].data.fd);

                // consume events (reset eventfd)
                read(events[i].data.fd, &result, sizeof(uint64_t));

                close(events[i].data.fd);   // NOTE: need to close here 否则会有未释放的fd

            } else {
                printf( "unknown events[%d].events=%lu\n", i, events[i].events );
            }
        }

    }

}

static void* producer_routine( void* data ) {
    thread_info_t *p = (thread_info_t *)data;

    struct epoll_event event;
    int epfd = p->epfd;
    int efd = -1;
    int ret = -1;

    log("Greetings from [producer-%d]", p->rank);
    while (1) {
        sleep(1);
        // create eventfd (no reuse, create new every time)
        efd = eventfd(1, EFD_CLOEXEC|EFD_NONBLOCK);
        if (efd == -1) handle_error("eventfd create: %s", strerror(errno));
        event.data.fd = efd;
        event.events = EPOLLIN | EPOLLET;
        ret = epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &event);
        if (ret != 0) handle_error("epoll_ctl");
        // trigger (repeatedly)
        write(efd, (void *)0xffffffff, sizeof(uint64_t));
    }

}


void  start_work() {
    struct thread_info *p_list = NULL, *c_list = NULL;
    int epfd = -1;
    int ret = -1, i = -1;

    epfd = epoll_create1(EPOLL_CLOEXEC);
    if (epfd == -1) handle_error("epoll_create1: %s", strerror(errno));

    // producers
    p_list = (struct thread_info*)calloc(NUM_PRODUCERS, sizeof(thread_info_t));
    if (!p_list) handle_error("calloc");

    for ( int i = 0; i < NUM_PRODUCERS; ++i ) {
        p_list[i].rank = i;
        p_list[i].epfd = epfd;
        ret = pthread_create( &(p_list[i].thread_id), nullptr, producer_routine, &p_list[i] );
        if (ret != 0) handle_error("pthread_create");
    }

    c_list = (struct thread_info*)calloc(NUM_CONSUMERS, sizeof(thread_info_t));
    if (!c_list) handle_error("calloc");

    for ( int i = 0; i < NUM_CONSUMERS; ++i ) {
        c_list[i].rank = i;
        c_list[i].epfd = epfd;
        ret = pthread_create( &(c_list[i].thread_id), nullptr, consumer_routine, &c_list[i] );
        if (ret != 0) handle_error("pthread_create");
    }

    for ( int i = 0; i < NUM_PRODUCERS; ++i ) {
        ret = pthread_join(p_list[i].thread_id, nullptr);
        if (ret != 0)  handle_error("pthread_join");
    }

    for ( int i = 0; i < NUM_CONSUMERS; ++i ) {
        ret = pthread_join(c_list[i].thread_id, nullptr);
        if (ret != 0)  handle_error("pthread_join");
    }

    free(p_list);
    free(c_list);
    return ;
}


