


#include "tstidleconnection_srv.h"

//#include <stdio.h>
//#include <iostream>
//#include <string>

//#include <list>

//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/scoped_ptr.hpp>

#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>


#include <list>


#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>


#include "muduo/net/Callbacks.h"
#include "muduo/base/common.h"
#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"
//#include "muduo/base/ThreadPool.h"

#include "muduo/base/libevent.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
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


// RFC 862
class EchoServer
{
public:
    EchoServer(EventLoop* loop,
               const InetAddress& listenAddr,
               int idleSeconds);
    void start() {
        server_.start();
    }

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,
                   Buffer* buf,
                   Timestamp time);
    void onTimer();
    void dumpConnectionList() const;

    typedef boost::weak_ptr<TcpConnection> WeakTcpConnectionPtr;
    typedef std::list<WeakTcpConnectionPtr> WeakConnectionList;

    struct Node : public muduo::copyable
    {
        Timestamp lastReceiveTime;
        WeakConnectionList::iterator position;
    };

    int idleSeconds_ = 0;
    TcpServer server_;
    WeakConnectionList connectionList_;
};

EchoServer::EchoServer(EventLoop* loop,
                       const InetAddress& listenAddr,
                       int idleSeconds)
    : server_(loop, listenAddr, "EchoServer"), idleSeconds_(idleSeconds)
{
    server_.setConnectionCallback( boost::bind(&EchoServer::onConnection, this, _1) );
    server_.setMessageCallback( boost::bind(&EchoServer::onMessage, this, _1, _2, _3) );

    loop->runEvery((double)1.0, boost::bind(&EchoServer::onTimer, this) );
    dumpConnectionList();
}
void EchoServer::onConnection(const TcpConnectionPtr &conn) {
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected()) {
        Node newNode;
        newNode.lastReceiveTime = Timestamp::now();

        connectionList_.push_back(conn);

        newNode.position = --connectionList_.end();
        conn->setContext(newNode);
    } else {
        assert(!conn->getContext().empty());
        const Node& node = boost::any_cast<const Node&> (conn->getContext());
        connectionList_.erase(node.position);
    }
    dumpConnectionList();
}
void EchoServer::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time) {
    std::string recvMsg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << recvMsg.size()
             << " bytes at " << time.toString();
    conn->send(recvMsg);

    assert(!conn->getContext().empty());
    Node* node = boost::any_cast<Node>(conn->getMutableContext());
    node->lastReceiveTime = time;
    connectionList_.splice(connectionList_.end(), connectionList_, node->position);
    assert(node->position == --connectionList_.end());

    dumpConnectionList();
}
void EchoServer::onTimer() {
    dumpConnectionList();

    Timestamp now = Timestamp::now();

}
void EchoServer::dumpConnectionList() const
{
    LOG_INFO << "size = " << connectionList_.size();

    for (WeakConnectionList::const_iterator it = connectionList_.begin();
         it != connectionList_.end(); ++it)
    {
        TcpConnectionPtr conn = it->lock();
        if (conn)
        {
            printf("conn %p\n", get_pointer(conn));
            const Node& n = boost::any_cast<const Node&>(conn->getContext());
            printf("    time %s\n", n.lastReceiveTime.toString().c_str());
        }
        else
        {
            printf("expired\n");
        }
    }
}


int tst_idleconnection_srv_entry(int argc, char *argv[]) {

    return 0;
}


