

#include "c11fun.h"




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





using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;



#include <cstdio>
#include <cstdlib>
#include <functional>


using namespace std;
using namespace muduo;
using namespace muduo::net;


class defer {
public:
    defer(const boost::function < void() >& deferFunc) :
        m_defer_func(deferFunc) {

    }
    ~defer() {
        m_defer_func();
//        m_defer_func = nullptr;
    }
    boost::function < void() > m_defer_func;
};


void tst_c11fun_entry() {
    OutputDbgInfo tmpOut( "tst_c11fun_entry begin", "tst_c11fun_entry end" );
    FILE* fileptr = fopen( "datafile.txt", "wb" );
    defer releaseFile = boost::function < void() >([&]()
    {
        fclose(fileptr);
        printf("closed fileptr\n");
        getchar();
    });
    if (fileptr) {
        fprintf(fileptr, "write data for test\n");
    } else {
        printf( "openfile failed!!!\n" );
        exit(1);
    }
}


