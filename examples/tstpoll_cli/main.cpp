





#include <iostream>
#include <sstream>


//#include "tst_1.h"

#include "codeconvert.h"


#include "muduo/net/InetAddress.h"

#include "muduo/net/Channel.h"

#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"

#include "muduo/base/Logging.h"

#include "muduo/net/TcpClient.h"

#include "discardserver.h"

#include "quicksort.h"
#include "tst_signal.h"
#include "mysqlinsert.h"
#include "cpp11fun.h"


#include <boost/bind.hpp>

#include <stdio.h>



using namespace std;
using namespace muduo;
using namespace muduo::net;


#include "tstpoll_cli.h"



int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    return tst_poll_cli_entry( argc, argv );


//    EventLoop loop;
//    InetAddress serverAddr("127.0.0.1", 8888);
//    TestClient client(&loop, serverAddr);
//    client.connect();
//    loop.loop();


}

















