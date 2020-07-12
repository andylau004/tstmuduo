


#include "threadex.hpp"

#include <unistd.h>

#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


event_thread::event_thread(int id) : m_status(false),
    m_id(id), m_isrun(false), m_eventBase(nullptr) {
    init();
}

event_thread::~event_thread() {
    destroy();
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

int event_thread::start(bool isasync) {
    int code = create_thread();
    if (code != SUCCESS) {
        return code;
    }

    pthread_mutex_lock(&m_mutex);
    while (!m_isrun) {
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    pthread_mutex_unlock(&m_mutex);
    if (isasync) {
//        sleep(2);
        return SUCCESS;
    }

    join();
    return SUCCESS;
}

void event_thread::join() {
    if (m_status) {
        pthread_join(m_tid, NULL);

        LOG_ERROR << " join s";
//        LOG4CPLUS_WARN(gLogger, __FUNCTION__ << " join s");
        m_status = false;
    } else {
        LOG_ERROR << " join e";
//        LOG4CPLUS_ERROR(gLogger, __FUNCTION__ << " join e");
    }
}


short event_thread::set_eventcallback(struct event * e,
                                      evutil_socket_t fd,
                                      short flags,
                                      event_callback_fn fn,
                                      void *arg,
                                      bool deleteoldflag,
                                      int second) {
    if (e == NULL || m_eventBase == NULL) {
        LOG_ERROR << ",tid:" << m_id << ",>>>>tmp," << flags << ",e NULL or m_eventBase NULL fd " << fd;
        return ERR_EVENT_FALSE;
    }

    if (deleteoldflag) {
        //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__
        //                << ",tid:" << m_id
        //                << ",>>>>tmp," << flags
        //                << ",deleteoldflag true fd " << fd << " e " << e);
        if (event_del(e) == -1) {
            LOG_ERROR << ",tid:" << m_id << ",>>>>tmp," << flags << ",event_del(e) -1";
            return -1;
        }
    }
    if (!flags) {
        //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__
        //                << ",tid:" << m_id
        //                << ",>>>>tmp," << flags <<",flags 0 fd " << fd << " e " << e);
        return flags;
    }

    event_set(e, fd, flags, fn, arg);
    event_base_set(m_eventBase, e);
    if (second <= 0) {
        //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__
        //                << ",tid:" << m_id
        //                << ",>>>>tmp," << flags << ",second<= 0 fd " << fd << " e " << e);
        if (event_add(e, 0) == -1) {
            LOG_ERROR << ",tid:" << m_id << ",>>>>tmp," << flags << ",event_add -1 "  << fd << " e " << e;
            return -1;
        }
    } else {
        //LOG4CPLUS_DEBUG(gLogger,__FUNCTION__
        //                << ",tid:" << m_id
        //                << ",>>>>tmp," << flags << ",second > 0"  << fd << " e " << e);
        timeval t = {second, 0};
        if (event_add(e, &t) == -1) {
            LOG_ERROR << ",tid:" << m_id << ",>>>>tmp," << flags << ",event_add -1" << fd << " e " << e;
            return -1;
        }
    }

    LOG_DEBUG << ",tid:" << m_id << ", flags:" << flags << ",fd:" << fd
              << ",deleteoldflag:" << (int)deleteoldflag
              << ",second:" << second
              << ",e:" << e;
    return flags;
}

bool event_thread::set_timercallback(struct event * e, event_callback_fn fn, void *arg,
                                     struct timeval * time, bool isdeleteold) {
    if (isdeleteold) {
        evtimer_del(e);
    }
    evtimer_set(e, fn, arg);
    event_base_set(m_eventBase, e);
    if (evtimer_add(e, time) == -1) {
        return false;
    }
    return true;
}

struct event* event_thread::set_forevertimer(event_callback_fn fn, void *arg, struct timeval * val) {
    struct event * ev_timer = event_new(m_eventBase, -1, EV_PERSIST, fn, arg);
    if (ev_timer) {
        if (event_add(ev_timer, val) == -1) {
            event_free(ev_timer);
            return NULL;
        }
    }
    return ev_timer;
}

short event_thread::set_eventidle(struct event * e, bool deleteoldflag) {
    if (e == NULL || m_eventBase == NULL) {
        LOG_ERROR << " bad e or base";
        return ERR_EVENT_FALSE;
    }

    if (deleteoldflag) {
        if (event_del(e) == -1) {
            LOG_ERROR << " bad event_del";
            return -1;
        }
        LOG_ERROR << " delete event";
    }
    return 0;
}

void event_thread::init() {
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);
    init_eventbase();
}

void event_thread::init_eventbase() {
    if (m_eventBase == NULL) {
        m_eventBase = event_base_new/*event_init*/();
    }
}

void event_thread::destroy() {
    if (m_status) {
        /* int code = */pthread_cancel(m_tid);
        pthread_join(m_tid, nullptr);
    }

    if (m_eventBase) {
        event_base_free(m_eventBase);
        m_eventBase = nullptr;
    }
}

int event_thread::create_thread() {
    if (m_eventBase == nullptr) {
        return ERR_EVENT_BASE;
    }
    int code = pthread_create(&m_tid, nullptr, &event_thread::thread_event_loop, this);
    return code;
}

void* event_thread::thread_event_loop(void *argv) {
    event_thread * t = reinterpret_cast<event_thread*>(argv);
    if (t == nullptr) {
        return nullptr;
    }

    struct event_base* base = t->get_eventbase();
    if (base == nullptr) {
        return nullptr;
    }

    pthread_mutex_lock(&t->m_mutex);
    t->m_isrun = true;
    pthread_mutex_unlock(&t->m_mutex);
    t->set_threadstatus(true);

    pthread_cond_signal(&t->m_cond);

    int code = event_base_loop(base, 0);
    if (code == 0) {
        printf("success\n");
    } else if (code == 1) {
        printf("no events were registered\n");
    } else {
        printf("something wrong\n");
    }

    printf("thread is finished\n");
    if (t == nullptr) {
        return nullptr;
    }
    t->set_threadstatus(false);
    return nullptr;
}
