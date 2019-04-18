

#include "tst_emplaceback.h"



#include <cstdio>
#include <cstdlib>
#include <functional>

#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield

#include <atomic>


#include <unordered_set>
#include <boost/circular_buffer.hpp>


#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>



#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>





#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"
#include "muduo/base/timeinterval.h"


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

using namespace boost;




void tst_eb_fun() {


    {
        std::vector<std::string> v;
        int count = 10000000;
        v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

        TIME_INTERVAL_SCOPE("push_back string");
        for (int i = 0; i < count; i++)
        {
//            std::string temp("ceshi");
            v.push_back("ceshi"/*temp*/);// push_back(const string&)，参数是左值引用
        }
    }

    {
        std::vector<std::string> v;
        int count = 10000000;
        v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

        TIME_INTERVAL_SCOPE("emplace_back string");
        for (int i = 0; i < count; i++)
        {
//            std::string temp("ceshi");
            v.emplace_back("ceshi"/*temp*/);// push_back(const string&)，参数是左值引用
        }
    }



    {
        std::vector<std::string> v;
        int count = 10000000;
        v.reserve(count);       //预分配十万大小，排除掉分配内存的时间

        TIME_INTERVAL_SCOPE("emplace_back string");
        for (int i = 0; i < count; i++)
        {
//            std::string temp("ceshi");
            v.emplace_back("ceshi"/*temp*/);// push_back(const string&)，参数是左值引用
        }
    }


}




void tst_emplace_back_entry() {
    OutputDbgInfo tmpOut( "tst_emplace_back_entry begin", "tst_emplace_back_entry end" );

    std::vector< int > v_ints;
    v_ints.reserve( 1000 );
    printf ( "v_ints.size=%lu v_ints.capacity=%lu\n", v_ints.size(), v_ints.capacity() );
    return ;

    tst_eb_fun();
}























