





#include <iostream>

#include <sstream>


#include <stdio.h>



#include "codeconvert.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"

#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"

#include "muduo/base/Logging.h"

#include "muduo/net/TcpClient.h"



#include <boost/bind.hpp>



using namespace std;
using namespace muduo;
using namespace muduo::net;



#include "tst-meml.h"
#include "tstperf.h"


int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    return tst_meml_entry( argc, argv );
}

















