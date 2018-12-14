


#include "chargenfun.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>


#include <list>


#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>


#include "muduo/net/Callbacks.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"

#include "muduo/base/libevent.h"

#include "muduo/net/TcpServer.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;



#define USER_LIMIT 5  // MAX user number
#define TIMEOUT 5
#define BUFFER_SIZE 256
#define FD_LIMIT 65535 /* 文件描述符数量限制 */



struct client_data {
    sockaddr_in address;
    char* write_buff;
    char  buf[ BUFFER_SIZE ];
};

int setnonblocking( const int& fdsock ) {
    if (evutil_make_socket_nonblocking(fdsock) < 0) {
        std::cout << "set fd=" << fdsock << " nonblocking failed." << std::endl;
        EVUTIL_CLOSESOCKET(fdsock);
        return -1;
    }
    return fdsock;
//    int old_option = fcntl( fd, F_GETFL );
//    int new_option = old_option | O_NONBLOCK;
//    fcntl( fd, F_SETFL, new_option );
//    return old_option;
}

void  setReuseAddr(const int& sockfd_, bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
    // FIXME CHECK
}

void  setReusePort(const int& sockfd_, bool on)
{
//#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT,
                           &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0 && on) {
//        LOG_SYSERR << "SO_REUSEPORT failed.";
        printf( "SO_REUSEPORT failed.\n" );
    }
//#else
//    if (on)
//    {
//        LOG_ERROR << "SO_REUSEPORT is not supported.";
//    }
//#endif
}


// RFC 864
class ChargenServer
{
public:
    ChargenServer(muduo::net::EventLoop* loop,
                  const muduo::net::InetAddress& listenAddr,
                  bool print = false);

    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time);

    void onWriteComplete(const muduo::net::TcpConnectionPtr& conn);
    void printThroughput();

    muduo::net::TcpServer server_;

    muduo::string message_;
    int64_t transferred_;
    muduo::Timestamp startTime_;
};

ChargenServer::ChargenServer(EventLoop* loop,
                             const InetAddress& listenAddr,
                             bool print)
  : server_(loop, listenAddr, "ChargenServer"),
    transferred_(0),
    startTime_(Timestamp::now())
{
    server_.setConnectionCallback( boost::bind(&ChargenServer::onConnection, this, _1) );
    server_.setMessageCallback(
                boost::bind(&ChargenServer::onMessage, this, _1, _2, _3) );
    server_.setWriteCompleteCallback(
                boost::bind(&ChargenServer::onWriteComplete, this, _1) );
    if (print) {
        loop->runEvery( 3.0, boost::bind(&ChargenServer::printThroughput, this) );
    }

    string line;
    for (int i = 33; i < 127; ++i)
    {
        line.push_back(char(i));
    }
    line += line;
    LOG_INFO << "size=" << line.size() << ", line=" << line;

    for (size_t i = 0; i < 127-33; ++i)
    {
//        LOG_INFO << "line.substr(i, 72)=" << line.substr(i, 72);
        message_ += line.substr(i, 72) + '\n';
    }
    LOG_INFO << "message_.size()=" << message_.size();
}

void ChargenServer::start() { server_.start(); }

void ChargenServer::onConnection(const TcpConnectionPtr& conn) {
    LOG_INFO << "ChargenServer - client_ip: " << conn->peerAddress().toIpPort() << " -> listen: "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        conn->setTcpNoDelay(true);
        conn->send(message_);
    }
}
void ChargenServer::onMessage(const TcpConnectionPtr& conn,
                              Buffer* buf,
                              Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " discards " << msg.size()
             << " bytes received at " << time.toString();
}
void ChargenServer::onWriteComplete(const TcpConnectionPtr& conn)
{
    transferred_ += message_.size();
//    conn->send(message_);
}

void ChargenServer::printThroughput()
{
    char tmpbuffer[2048] = {0};
    Timestamp endTime = Timestamp::now();

    double time = timeDifference(endTime, startTime_);
    sprintf(tmpbuffer, "%4.3f MiB/s", static_cast<double>(transferred_)/time/1024/1024);
    LOG_INFO << tmpbuffer;

    transferred_ = 0;
    startTime_ = endTime;
}


int tst_chargen_entry(int argc, char *argv[]) {
    EventLoop loop;
    muduo::net::InetAddress listenAddr(9930);

    ChargenServer newServer(&loop, listenAddr, true);
    newServer.start();

    loop.loop();
    return 0;
}



