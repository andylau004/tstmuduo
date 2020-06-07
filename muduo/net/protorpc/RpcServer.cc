

//#include "RpcServer.h"

#include "muduo/net/protorpc/RpcServer.h"

#include "muduo/base/Logging.h"
#include "muduo/net/protorpc/RpcChannel.h"

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

using namespace muduo;
using namespace muduo::net;


RpcServer::RpcServer(EventLoop *loop,
                     const InetAddress &listenAddr)
    : server_(loop, listenAddr, "RpcServer")
{
    server_.setConnectionCallback(
                boost::bind(&RpcServer::onConnection, this, _1));
}
void RpcServer::registerService(google::protobuf::Service * service)
{
    const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
    service_[desc->full_name()] = service;
}
void RpcServer::start()
{
    server_.start();
}
void RpcServer::onConnection(const TcpConnectionPtr &conn)
{
    LOG_INFO << "RpcServer - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected()) {
        RpcChannelPtr channel(new RpcChannel(conn));
        channel->setServices(&service_);
        conn->setMessageCallback(boost::bind(&RpcChannel::onMessage, muduo::get_pointer(channel), _1, _2, _3));
        conn->setContext(channel);
    } else {
        conn->setContext(RpcChannelPtr());
        // FIXME:
    }
}
//RpcServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time) {

//}
