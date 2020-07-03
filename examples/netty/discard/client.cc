
#include "muduo/net/TcpClient.h"

#include <boost/bind.hpp>

#include "muduo/base/Atomic.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"

#include <utility>

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

using i64 = int64_t;

int numThreads = 0;


class DiscardClient : noncopyable
{
public:
    DiscardClient(EventLoop* loop, const InetAddress& listenAddr, int size)
        : loop_(loop),
          client_(loop, listenAddr, "DiscardClient"),
          message_(size, 'H')
    {
        client_.setConnectionCallback(
            boost::bind(&DiscardClient::onConnection, this, _1));
        client_.setMessageCallback(
            boost::bind(&DiscardClient::onMessage, this, _1, _2, _3));
        client_.setWriteCompleteCallback(
            boost::bind(&DiscardClient::onWriteComplete, this, _1));
//        client_.enableRetry();
    }
    void connect() {
        client_.connect();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        if (conn->connected()) {
            LOG_INFO << "on connect ...";
            conn->setTcpNoDelay(true);
            conn->send(message_);
        } else {
            LOG_INFO << "on connect closed...";
            loop_->quit();
        }
    }
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts) {
        LOG_INFO << "on message ...";
        buf->retrieveAll();
    }
    void onWriteComplete(const TcpConnectionPtr& conn)
    {
        LOG_INFO << "write complete, msg size=" << message_.size();
        conn->send(message_);
    }
private:
    EventLoop* loop_;
    TcpClient  client_;
    std::string message_;
};

int main(int argc, char* argv[]) {
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    if (argc > 1)
    {
        EventLoop loop;
        InetAddress serverAddr(argv[1], 2009);

        int size = 16;
        if (argc > 2) {
            size = atoi(argv[2]);
        }

        DiscardClient client(&loop, serverAddr, size);
        client.connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip [msg_size]\n", argv[0]);
    }
    return 1;
}
