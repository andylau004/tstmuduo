





#include <stdio.h>
#include <iostream>

#include <sstream>


#include <boost/bind.hpp>






//#include "tst_1.h"

#include "codeconvert.h"


//#include "poco/Bugcheck.h"
//#include "poco/Foundation.h"

//#include "poco/Alignment.h"

#include "muduo/net/InetAddress.h"

#include "muduo/net/Channel.h"

#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"

#include "muduo/base/Logging.h"

#include "muduo/net/TcpClient.h"

#include "mysqlinsert.h"
#include "cpp11fun.h"
#include "tsteventfd.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;




int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_event_fd_entry(argc, argv);

//    Logger::setLogLevel(Logger::DEBUG);
//    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
//    EventLoop loop;
//    InetAddress serverAddr("127.0.0.1", 8888);
//    TestClient client(&loop, serverAddr);
//    client.connect();
//    loop.loop();
}




