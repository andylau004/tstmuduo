#include "muduo/net/TcpServer.h"

#include "muduo/base/Atomic.h"
#include "muduo/base/Logging.h"
#include "muduo/base/Thread.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"

#include <utility>

#include <stdio.h>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

int numThreads = 0;

class EchoClient {
public:
    EchoClient(EventLoop* loop, const InetAddress& connAddr, int size)
        : loop_(loop),
          client_(loop, connAddr, "EchoClient"),
          message_(size, 'x')
    {
        client_.setConnectionCallback(
                    boost::bind(&EchoClient::onConnection, this, _1));
        client_.setMessageCallback(
                    boost::bind(&EchoClient::onMesssage, this, _1, _2, _3));
    }
    void connect() {
        client_.connect();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        LOG_INFO << "client, " << conn->localAddress().toIpPort() << " -> "
                  << conn->peerAddress().toIpPort() << " is "
                  << (conn->connected() ? "UP" : "DOWN");

        if (conn->connected())
        {
            conn->setTcpNoDelay(true);
            conn->send(message_);
        }
        else
        {
            loop_->quit();
        }
    }
    void onMesssage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time ) {
        LOG_INFO << "client recv msg=" << buf->peek();
        conn->send(buf);
    }

private:
    EventLoop* loop_;
    TcpClient client_;
    std::string message_;

};

int main(int argc, char* argv[]) {
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();
    if (argc > 1)
    {
        EventLoop loop;
        InetAddress serverAddr(argv[1], 2007);

        int size = 16;
        if (argc > 2)
        {
            size = atoi(argv[2]);
        }

        EchoClient client(&loop, serverAddr, size);
        client.connect();
        loop.loop();
    }
    else
    {
        printf("Usage: %s host_ip [msg_size]\n", argv[0]);
    }
    return 1;
}
