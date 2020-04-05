





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


#include "muduo/base/common.h"



using namespace std;
using namespace muduo;
using namespace muduo::net;


#include "tstpoll_cli.h"





using namespace muduo;
using namespace muduo::net;

class Client;

class Session
{
public:
    Session(EventLoop* loop,
            const InetAddress& serverAddr,
            const string& name,
            Client* owner)
        : client_(loop, serverAddr, name),
          owner_(owner),
          bytesRead_(0),
          bytesWritten_(0),
          messagesRead_(0)
    {
        client_.setConnectionCallback(boost::bind(&Session::onConnection, this, _1));
        client_.setMessageCallback(boost::bind(&Session::onMessage, this, _1, _2, _3));
    }

    void start()
    {
        client_.connect();
    }

    void stop()
    {
        client_.disconnect();
    }

    int64_t bytesRead() const
    {
        return bytesRead_;
    }

    int64_t messagesRead() const
    {
        return messagesRead_;
    }

private:
    void onConnection(const TcpConnectionPtr& conn);

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
    {
        ++messagesRead_;
        bytesRead_ += buf->readableBytes();
        bytesWritten_ += buf->readableBytes();
        conn->send(buf);
    }

    TcpClient client_;
    Client* owner_;
    int64_t bytesRead_;
    int64_t bytesWritten_;
    int64_t messagesRead_;
};

class Client
{
public:
    Client(EventLoop* loop,
           const InetAddress& serverAddr,
           int blockSize,
           int sessionCount,
           int timeout,
           int threadCount)
        : loop_(loop),
          threadPool_(loop, "pingpong-client"),
          sessionCount_(sessionCount),
          timeout_(timeout)
    {
        loop->runAfter(timeout, boost::bind(&Client::handleTimeout, this));
        if (threadCount > 1)
        {
            threadPool_.setThreadNum(threadCount);
        }
        threadPool_.start();

        for (int i = 0; i < blockSize; ++i)
        {
            message_.push_back(static_cast<char>(i % 128));
        }

        for (int i = 0; i < sessionCount; ++i)
        {
            char buf[32];
            snprintf(buf, sizeof buf, "C%05d", i);
            Session* session = new Session(threadPool_.getNextLoop(), serverAddr, buf, this);
            session->start();
            sessions_.emplace_back(session);
        }
    }

    const string& message() const
    {
        return message_;
    }

    void onConnect()
    {
        if (numConnected_.incrementAndGet() == sessionCount_)
        {
            LOG_WARN << "all connected";
        }
    }

    void onDisconnect(const TcpConnectionPtr& conn)
    {
        if (numConnected_.decrementAndGet() == 0)
        {
            LOG_WARN << "all disconnected";

            int64_t totalBytesRead = 0;
            int64_t totalMessagesRead = 0;
            for (const auto& session : sessions_)
            {
                totalBytesRead += session->bytesRead();
                totalMessagesRead += session->messagesRead();
            }
            LOG_WARN << totalBytesRead << " total bytes read";
            LOG_WARN << totalMessagesRead << " total messages read";
            LOG_WARN << static_cast<double>(totalBytesRead) / static_cast<double>(totalMessagesRead)
                     << " average message size";
            LOG_WARN << static_cast<double>(totalBytesRead) / (timeout_ * 1024 * 1024)
                     << " MiB/s throughput";
            conn->getLoop()->queueInLoop(boost::bind(&Client::quit, this));
        }
    }

private:

    void quit()
    {
        loop_->queueInLoop(boost::bind(&EventLoop::quit, loop_));
    }

    void handleTimeout()
    {
        LOG_WARN << "stop";
        for (auto& session : sessions_)
        {
            session->stop();
        }
    }

    EventLoop* loop_;
    EventLoopThreadPool threadPool_;
    int sessionCount_;
    int timeout_;
    std::vector<std::unique_ptr<Session>> sessions_;
    string message_;
    AtomicInt32 numConnected_;
};

void Session::onConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        conn->setTcpNoDelay(true);
        conn->send(owner_->message());
        owner_->onConnect();
    }
    else
    {
        owner_->onDisconnect(conn);
    }
}

void tst_muduo_client_1() {

    //    if (argc != 7)
    //    {
    //        fprintf(stderr, "Usage: client <host_ip> <port> <threads> <blocksize> ");
    //        fprintf(stderr, "<sessions> <time>\n");
    //    }
    //    else
    //    {
            LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
            Logger::setLogLevel(Logger::WARN);

            DeferFunctor pfnExit = boost::function < void() >([&]() {
                printf("client main function done -----------------\n");
            });

            const char* ip = "127.0.0.1";
            uint16_t port = 9099;
            int threadCount = 1;
            int blockSize = 3;
            int sessionCount = 1;
            int timeout = 3;

            EventLoop loop;
            InetAddress serverAddr(ip, port);
            Client client(&loop, serverAddr, blockSize, sessionCount, timeout, threadCount);
            loop.loop();
    //    }
//            return 1;

}

int main(int argc, char* argv[])
{
    tst_muduo_client_1();
    return 1;
//        return tst_poll_cli_entry( argc, argv );

//        //    EventLoop loop;
//        //    InetAddress serverAddr("127.0.0.1", 8888);
//        //    TestClient client(&loop, serverAddr);
//        //    client.connect();
//        //    loop.loop();


}



//int main(int argc, char *argv[])
//{
//    Logger::setLogLevel(Logger::DEBUG);
//    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

//    return 1;
//}

















