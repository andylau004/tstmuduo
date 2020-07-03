#include "muduo/net/TcpServer.h"

#include <utility>

#include <stdio.h>
#include <unistd.h>

#include <boost/bind.hpp>

#include "muduo/base/common.h"
#include "muduo/base/Atomic.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

using namespace muduo;
using namespace muduo::net;

int numThreads = 0;

class EchoServer {
public:
    EchoServer(EventLoop* loop, const InetAddress& listenAddr)
        : server_(loop, listenAddr, "EchoServer"),
          oldCounter_(0),
          startTime_(Timestamp::now())
    {
        server_.setConnectionCallback(
                    boost::bind(&EchoServer::onConnection, this, _1));
        server_.setMessageCallback(
                    boost::bind(&EchoServer::onMessage, this, _1, _2, _3));
        server_.setThreadNum(numThreads);
        loop->runEvery(3.0, boost::bind(&EchoServer::printThroughput, this));
    }
    void start() {
        LOG_INFO << "start server: " << server_.name() << ", numThreads: " << numThreads;
        server_.start();
    }
    void onConnection(const TcpConnectionPtr& conn) {
        LOG_INFO << conn->peerAddress().toIpPort() << " -> "
                 << conn->localAddress().toIpPort() << " is "
                 << (conn->connected() ? "up" : "down");
        conn->setTcpNoDelay(true);
    }
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) {
       size_t len = buf->readableBytes();
       transferred_.addAndGet(len);
       receivedMessages_.increment();
       conn->send(buf);
    }
    void printThroughput() {
        Timestamp endTime = Timestamp::now();
        i64 newCounter = transferred_.get();
        i64 bytes = newCounter - oldCounter_;
        i64 msgs = receivedMessages_.getAndSet(0);
        double tm = timeDifference(endTime, startTime_);
        printf("%4.3f MiB/s %4.3 Ki Msgs/s %6.2f bytes per msg\n",
               static_cast<double>(bytes)/tm/1024/1024,
               static_cast<double>(msgs)/1024,
               static_cast<double>(bytes)/static_cast<double>(msgs));
        oldCounter_ = newCounter;
        startTime_ = endTime;
    }
private:

private:
    TcpServer server_;
    AtomicInt64 transferred_;
    AtomicInt64 receivedMessages_;
    int64_t oldCounter_;
    Timestamp startTime_;
};

int main(int argc, char* argv[]) {
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    if (argc > 1)
    {
        numThreads = atoi(argv[1]);
    }
    EventLoop loop;
    InetAddress listenAddr(2007);
    EchoServer srv(&loop, listenAddr);
    srv.start();
    loop.loop();
    return 1;
}
