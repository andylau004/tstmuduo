





#include <iostream>

#include <sstream>





#include "tst_1.h"

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
#include "muduo/net/SocketsOps.h"


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





class TestClient
{
public:
    TestClient(EventLoop *loop, const InetAddress &listenAddr)
        : loop_(loop),
          client_(loop, listenAddr, "TestClient"),
          stdinChannel_(loop, 0)
    {
        client_.setConnectionCallback(
                    boost::bind(&TestClient::onConnection, this, _1));
        client_.setMessageCallback(
                    boost::bind(&TestClient::onMessage, this, _1, _2, _3));
        //client_.enableRetry();
        // 标准输入缓冲区中有数据的时候，回调TestClient::handleRead
        stdinChannel_.setReadCallback(boost::bind(&TestClient::handleRead, this));
        stdinChannel_.enableReading();      // 关注可读事件
    }

    void connect() {
        client_.connect();
    }

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
//            printf("onConnection(): new connection [%s] from %s\n",
//                   conn->name().c_str(),
//                   conn->peerAddress().toIpPort().c_str());
            LOG_INFO << "TestClient onConnection(): new connection [" << conn->name() << "] from " << conn->peerAddress().toIpPort();
            std::string strMsg = "abcdefg";

            std::string strall;

            Buffer sendBuffer;
            sendBuffer.appendInt32( strMsg.size() );
            sendBuffer.append(strMsg);

            conn->send( &sendBuffer );

//            exit(-1);
//            return;
//            conn->shutdown();
        }
        else
        {
            LOG_INFO << "TestClient onConnection(): connection [" << conn->name() << " is down";
//            printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
    {
        muduo::string strMsg(buf->retrieveAllAsString());
//        printf("recv msg [%s]\n", strMsg.c_str());
        LOG_INFO << conn->name() << " recv " << strMsg.size() << " bytes, msg=" << strMsg;

        {
            Buffer sendBuffer;
            sendBuffer.appendInt32( strMsg.size() );
            sendBuffer.append(strMsg);

            conn->send( &sendBuffer );
        }
    }

    // 标准输入缓冲区中有数据的时候，回调该函数
    void handleRead()
    {
        char buf[1024] = {0};
        fgets(buf, 1024, stdin);
        buf[strlen(buf) - 1] = '\0';        // 去除\n
        client_.connection()->send(buf);
    }

    EventLoop *loop_;
    TcpClient client_;
    Channel stdinChannel_;      // 标准输入Channel
};

int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    EventLoop loop;
//    InetAddress serverAddr("127.0.0.1", 8888);
    InetAddress serverAddr("172.17.0.2", 10091);
    TestClient client(&loop, serverAddr);
    client.connect();
    loop.loop();
}

//int main()
//{
//    //    Logger::setLogLevel(Logger::DEBUG);
//    //    std::setlocale(LC_ALL, "en_US.utf8");

//    return 1;
//}













