

#pragma once


#include <unordered_map>

#include "muduo/net/TcpServer.h"


namespace google {
namespace protobuf {

class Service;

}  // namespace protobuf
}  // namespace google

namespace muduo
{
namespace net
{

class RpcServer {
public:
    RpcServer(EventLoop* loop,
              const InetAddress& listenAddr);

    void SetThreadNum(int num) { server_.setThreadNum(num); }

    void registerService(::google::protobuf::Service*);
    void start();

private:
    void onConnection(const TcpConnectionPtr& conn);

    // void onMessage(const TcpConnectionPtr& conn,
    //                Buffer* buf,
    //                Timestamp time);
    TcpServer server_;
    std::unordered_map< std::string, ::google::protobuf::Service* > service_;
};


}  // namespace net
}  // namespace muduo


