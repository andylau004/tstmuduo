
#pragma once



#include "muduo/base/Atomic.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/protorpc/RpcCodec.h"

#include <google/protobuf/service.h>

#include <map>
#include <unordered_map>
#include <boost/shared_ptr.hpp>



namespace google {
namespace protobuf {

class Descriptor;
class ServiceDescriptor;
class MethodDescriptor;
class Message;
class Closure;
class RpcController;
class Service;

}// namespace protobuf
}// namespace google


namespace muduo
{
namespace net
{

class RpcChannel : public ::google::protobuf::RpcChannel
{
public:
    RpcChannel();
    explicit RpcChannel(const TcpConnectionPtr& conn);
    ~RpcChannel() /*override*/;

    void setConnection(const TcpConnectionPtr& conn)
    {
        conn_ = conn;
    }

    void setServices(const std::unordered_map<std::string, ::google::protobuf::Service*>* services)
    {
        services_ = services;
    }

    void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                    ::google::protobuf::RpcController* controller,
                    const ::google::protobuf::Message* request,
                    ::google::protobuf::Message* response,
                    ::google::protobuf::Closure* done) /*override*/;

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);

private:
    void onRpcMessage(const TcpConnectionPtr& conn,
                      const RpcMessagePtr& messagePtr,
                      Timestamp receiveTime);

    void doneCallback(::google::protobuf::Message* response, int64_t id);

    struct OutstandingCall
    {
        ::google::protobuf::Message* response;
        ::google::protobuf::Closure* done;
    };

    RpcCodec codec_;
    TcpConnectionPtr conn_;
    AtomicInt64 id_;

    MutexLock mutex_;
    std::map<int64_t, OutstandingCall> outstandings_ GUARDED_BY(mutex_);

    const std::unordered_map/*map*/<std::string, ::google::protobuf::Service*>* services_;
};
typedef boost/*std*/::shared_ptr<RpcChannel> RpcChannelPtr;

}  // namespace net
}  // namespace muduo
