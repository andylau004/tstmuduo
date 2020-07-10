

#include "twfun.h"




#include <cstdio>
#include <cstdlib>
#include <functional>

#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>


#include "timewheel.h"



using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;




using namespace std;
using namespace muduo;
using namespace muduo::net;


// timewheel 实现


void fun100()
{
    cout << "func 100" << endl;
}
void fun200()
{
    cout << "func 200" << endl;
}
void fun500()
{
    cout << "func 500" << endl;
}

void fun1500()
{
    cout << "func 1500" << endl;
}



void tst_twfun_entry() {
    std::string stt = " 123";
    OutputDbgInfo tmpOut( "tst_twfun_entry begin" +stt, "tst_twfun_entry end" +stt);

    boost::function<void(void)> f100 = boost::bind(&fun100);
    boost::function<void(void)> f200 = boost::bind(&fun200);
    boost::function<void(void)> f500 = boost::bind(&fun500);
    boost::function<void(void)> f1500 = boost::bind(&fun1500);

    TimeWheel time_wheel;
    time_wheel.InitTimerWheel(100, 5);
//    int timer1 = time_wheel.AddTimer(100, f100);
    int timer2 = time_wheel.AddTimer(200, f200);
//    int timer3 = time_wheel.AddTimer(500, f500);
//	time_wheel.AddTimer(1500, f1500);

//    bool b = true;
    int nLoop = 0;
    while (1)
    {
        nLoop++;
        this_thread::sleep_for(chrono::milliseconds(300));
//        if (b)
//        {
//            time_wheel.AddTimer(1500, f1500);
//            b = false;
//        }
//        if (nLoop == 3)
//            time_wheel.DeleteTimer(timer1);
    }

}


