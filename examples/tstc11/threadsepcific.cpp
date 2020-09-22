
#include "threadsepcific.h"


#include <iostream>
#include <sys/eventfd.h>


#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/tss.hpp>


#include "muduo/base/common.h"
#include "muduo/base/Logging.h"


boost::mutex gMtx;
boost::thread_specific_ptr<int> gDataPtr;

struct Count {

    Count(int id) : id_(id) {
    }

    void operator ()() {
        if (gDataPtr.get() == 0) {
            gDataPtr.reset(new int(1943));
        }
        for (int i = 0; i < 10; i ++) {
            (*gDataPtr)++;

            boost::mutex::scoped_lock lock(gMtx);
            LOG_INFO << id_ << ": " << *gDataPtr;
        }
    }

    int id_;
};


int VarintLength(int v) {
    int len = 1;
    while (v >= 128) {
        v >>= 7;
        len++;
    }
    return len;
}

void tst_boost_specfic_fun() {

    {
        LOG_INFO << "VarintLength=" << VarintLength(256);
        return ;
    }

    auto pfnCreate = []()
    {
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evtfd < 0)
        {
            LOG_SYSERR << "Failed in eventfd";
            abort();
        }
        return evtfd;
    };

    int fds[2];
    LOG_INFO << "sizeof(fds)=" << sizeof(fds);
    LOG_INFO << "sizeof(eventfd)=" << sizeof(pfnCreate());
    return ;

    int cron_interval_ = 3500;
    cron_interval_ = 3000;
    LOG_INFO << "cron / 1000 = " << cron_interval_ / 1000;
    LOG_INFO << "(cron_interval_ % 1000) = " << (cron_interval_ % 1000);

    return ;

    boost::thread t1(Count(1));
    boost::thread t2(Count(2));
    t1.join();
    t2.join();

}
