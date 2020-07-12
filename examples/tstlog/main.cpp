
#include "tstlog.h"

#include <stdio.h>
#include <iostream>
#include <sstream>

#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include "muduo/base/common.h"
#include "muduo/base/ThreadPool.h"
#include "muduo/base/Atomic.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"




using namespace std;
using namespace muduo;
using namespace muduo::net;



muduo::AtomicInt64 g_succCount;
muduo::AtomicInt64 g_failCount;





int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_log_entry(argc, argv); return 1;
}
