


#include "tstlog.h"


#include <sys/resource.h>


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>


#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"
#include "muduo/base/ThreadLocal.h"
#include "muduo/base/ThreadPool.h"


#include "AsyncLoggingQueue.h"


int kRollSize = 500*1000*1000;

void* g_asyncLog = NULL;


using namespace std;
using namespace muduo;



int gCount = 0;

template<typename ASYNC>
void asyncOutput(const char* msg, int len)
{
    gCount ++;
    if (gCount > 10000) abort();

    ASYNC* log = static_cast<ASYNC*>(g_asyncLog);
    log->append(msg, len);
}


template<typename ASYNC>
void bench(ASYNC* log)
{
    g_asyncLog = log;
    log->start();
    muduo::Logger::setOutput(asyncOutput<ASYNC>);

    int cnt = 0;
    const int kBatch = 1000;
    const bool kLongLog = true;
    muduo::string empty = " ";
    muduo::string longStr(3000, 'X');
    longStr += " ";

    for (int t = 0; t < 30; ++t)
    {
        muduo::Timestamp start = muduo::Timestamp::now();
        for (int i = 0; i < kBatch; ++i)
        {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
                     << (kLongLog ? longStr : empty)
                     << cnt;
            ++cnt;
        }
        muduo::Timestamp end = muduo::Timestamp::now();
        printf("%f\n", timeDifference(end, start)*1000000/kBatch);
        struct timespec ts = { 0, 500*1000*1000 };
        nanosleep(&ts, NULL);
    }
}


void tst_log_entry(int argc, char *argv[]) {

    {
        // set max virtual memory to 2GB.
        size_t kOneGB = 1024*1024*1024;
        rlimit rl = { 2.0*kOneGB, 2.0*kOneGB };
        setrlimit(RLIMIT_AS, &rl);
    }

    AsyncLoggingUnboundedQueue log1("log1", kRollSize);

    bench(&log1);

//    int which = argc > 1 ? atoi(argv[1]) : 1;
//    switch (which) {
//    case 1:
//        break;
//    }

}



