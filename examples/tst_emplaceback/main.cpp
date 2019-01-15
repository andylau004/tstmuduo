


#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <memory>



#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


//#include "poco/Bugcheck.h"
//#include "poco/Foundation.h"
//#include "poco/Alignment.h"

#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


#include "tst_emplaceback.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;


#define GCC

class TimeInterval {
public:
    TimeInterval(const std::string& d) : detail(d)
    {
        init();
    }
//    TimeInterval(const char* d) : detail(d)
//    {
//        init();
//    }

    TimeInterval()
    {
        init();
    }

    ~TimeInterval()
    {
#ifdef GCC
        gettimeofday(&end, NULL);
        std::cout << detail
            << 1000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000
            << " ms" << endl;
#else
        end = clock();
        std::cout << detail
            << (double)(end - start) << " ms" << std::endl;
#endif // GCC
    }

protected:
    void init() {
#ifdef GCC
        printf( "gettimeofday logic\n" );
        gettimeofday(&start, NULL);
#else
        printf( "clock logic\n" );
        start = clock();
#endif // GCC
    }
private:
    std::string detail;
#ifdef GCC
    timeval start, end;
#else
    clock_t start, end;
#endif // GCC
};

#define TIME_INTERVAL_SCOPE(d)   std::shared_ptr<TimeInterval> time_interval_scope_begin = std::make_shared<TimeInterval>(d)


int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    TIME_INTERVAL_SCOPE("11111ttttt");
    tst_emplace_back_entry();

    return 1;
}
