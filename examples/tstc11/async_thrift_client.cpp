


#include "async_thrift_client.h"


#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield


#include <string>
#include <mutex>

#include <cmath>
#include <future>
#include <functional>

#include <cstdio>
#include <cstdlib>

#include <event.h>

#include <thrift/Thrift.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/server/TServer.h>
#include <thrift/async/TAsyncChannel.h>
#include <thrift/async/TEvhttpClientChannel.h>

#include "gen-cpp/Twitter.h"

#include <thrift/async/TAsyncProtocolProcessor.h>
#include <thrift/async/TEvhttpServer.h>
// #include <thrift/asyn_asio/TAsioServer.h>



#include "muduo/base/common.h"

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;


using namespace std;
using std::string;
using namespace boost;

using namespace thrift::example;
using namespace apache::thrift::async;


// https://www.programmersought.com/article/10146319794/
// https://blog.csdn.net/swartz_lubel/article/details/79236145

class testClient : public TwitterCobClient
{
public:
    testClient(boost::shared_ptr< ::apache::thrift::async::TAsyncChannel> channel, TProtocolFactory* protocolFactory)
        : TwitterCobClient(channel, protocolFactory)
    {
    }

    virtual void completed__(bool success)
    {
        if (success)
        {
            printf("respone : %s \n", res.c_str());   // 输出返回结果
        }
        else
        {
            printf("failed to respone\n");
        }
        fflush(0);
    };

    std::string res;
};

//callback function
static void my_recv_sendString(TwitterCobClient *client){
    client->recv_sendString(dynamic_cast<testClient*>(client)->res);
}

static void sendString(testClient & client){
    printf("send string start\n");

    std::function<void(TwitterCobClient*client)> cob = std::bind(&my_recv_sendString, std::placeholders::_1);

    client.sendString(cob, "Hello");
    
    printf("send string end\n");
}

void DoSimpleTest(const std::string& host, int port) {  
    printf("running SimpleTset(%s, %d)..\n", host.c_str(), port);

    event_base* base = event_base_new();
    std::cout << "0000, base=" << base << std::endl;

    boost::shared_ptr< ::apache::thrift::async::TAsyncChannel >  channel1( new TEvhttpClientChannel(host, "/", host.c_str(), port, base) );

    std::cout << "1111" << std::endl;
    testClient client1(channel1, new TBinaryProtocolFactory());
    std::cout << "2222" << std::endl;

    sendString(client1);   // 发送第一个请求
    
    boost::shared_ptr< ::apache::thrift::async::TAsyncChannel>  channel2( new TEvhttpClientChannel(host, "/", host.c_str(), port, base) );
    
    testClient client2(channel2, new TBinaryProtocolFactory() );
    
    sendString(client2);  // 发送第二个请求
    
    event_base_dispatch(base);
    
    event_base_free(base);
    
    printf( "done DoSimpleTest().\n" );
}



class TwitterAsyncHandler : public TwitterCobSvIf {
public:
    TwitterAsyncHandler() {
        // Your initialization goes here
    }
    virtual ~TwitterAsyncHandler() {}

    virtual void sendString(tcxx::function<void(std::string const& _return)> cob, const std::string& data) {
        printf("sendString rec:%s\n", data.c_str());  // 输出收到的数据
        std::string _return = "world";   // 返回world字符串给客户端
        return cob(_return);
    }
};



int asnc_server() {
    std::cout << "asnc_server , beg -----------" << std::endl;
    deferTime ([&](){
        std::cout << "asnc_server , end -----------" << std::endl;
    });
    boost::shared_ptr<TAsyncProcessor>
            underlying_pro( new TwitterAsyncProcessor( boost::shared_ptr<TwitterCobSvIf>(new TwitterAsyncHandler()) ) );

    boost::shared_ptr<TAsyncBufferProcessor>
            processor( new TAsyncProtocolProcessor( underlying_pro, boost::shared_ptr<TProtocolFactory>(new TBinaryProtocolFactory()) ) );
    
    TEvhttpServer server(processor, 14488);
    // apache::thrift::async_asio::TAsioServer server(processor, 14488);
    std::cout << "before start srv" << std::endl;
    server.serve();
    std::cout << "after  start srv" << std::endl;
    return 0;
}  


void tstclient() {
    DoSimpleTest("127.0.0.1", 14488);
    // DoSimpleTest("localhost", 14488);
}

void tst_async_thrift_client() {
    std::cout << "tst_async_thrift_client , beg -----------" << std::endl;
    deferTime ([&](){
        std::cout << "tst_async_thrift_client , end -----------" << std::endl;
    });

    std::thread srvThrd(std::bind(&asnc_server));
    sleep(1);
    std::thread cliThrd(std::bind(&tstclient));
    cliThrd.join();
    srvThrd.join();
}