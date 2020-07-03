#include "muduo/net/TcpServer.h"

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

int numThreads = 0;

class EchoServer {
public:
//    EchoServer(EventLoop* loop, const InetAddress& listenAddr)
//        :
//    {

//    }
private:

private:
    TcpServer server_;
    AtomicInt64 transferred_;
    AtomicInt64 receivedMessages_;

};

int main(int argc, char* argv[]) {

    return 1;
}
