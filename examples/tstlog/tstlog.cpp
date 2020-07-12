


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


void tst_log_entry(int argc, char *argv[]) {

    {
        // set max virtual memory to 2GB.
        size_t kOneGB = 1024*1024*1024;
        rlimit rl = { 2.0*kOneGB, 2.0*kOneGB };
        setrlimit(RLIMIT_AS, &rl);
    }

    AsyncLoggingUnboundedQueue log1("log1", kRollSize);

    int which = argc > 1 ? atoi(argv[1]) : 1;

}



