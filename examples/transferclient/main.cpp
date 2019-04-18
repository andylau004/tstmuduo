


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


#include "transferclient.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;




int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_transfer_client_entry();

    return 1;
}
