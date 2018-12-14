
#pragma once


#include <stdio.h>
#include <event.h>

//#ifdef __linux__
#include <pthread.h>
//#endif


/**
 *  带有libevent的thread，职责很简单，注册事件，循环触发事件
 */
class event_thread {
public:
    explicit event_thread(int id);
    ~event_thread();

    void set_identifer(int id) { m_id = id; }
    int get_identifer() const { return m_id; }

    void set_threadstatus(bool isrun) { m_status = isrun; }

    struct event_base * get_eventbase() { return m_eventBase; }

    int start(bool isasync = true);
    void join();

    pthread_t get_ptid() const { return m_tid; }


    /**
     *  注册事件
     *  @param e, event
     *  @param fd, file descriptor
     *  @param flags, event flags
     *  @param fn, event call back
     *  @param arg, call back param
     *  @param deleteoldflag, is delete event old flags
     *  @param return new flags or error
     */
    short set_eventcallback(struct event * e,
                            evutil_socket_t fd,
                            short flags,
                            event_callback_fn fn,
                            void *arg,
                            bool deleteoldflag = false,
                            int second = 0);

    short set_eventidle(struct event * e, bool deleteoldflag = false);

    bool set_timercallback(struct event * e, event_callback_fn fn,
                           void *arg, struct timeval * time,
                           bool isdeleteold = false);
    struct event* set_forevertimer(event_callback_fn fn, void *arg, struct timeval * val);



private:
    void init();
    void init_eventbase();
    void destroy();

    int create_thread();

    static void * thread_event_loop(void * arg);
private:
    bool m_status;   /**< thread status */
    int  m_id;        /**< identifer */
    pthread_t m_tid; /**< pthread id */

    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;
    bool            m_isrun;

    struct event_base * m_eventBase;
};

