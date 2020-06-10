


//#include "examples/tstprotobuff/resolver/resolver.pb.h"
#include "resolver.pb.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/protorpc/RpcServer.h"
#include "examples/cdns/Resolver.h"

#include <unistd.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

namespace resolver
{

class ResolverServiceImpl : public ResolverService {
public:
    ResolverServiceImpl(EventLoop* loop)
        : resolver_(loop, cdns::Resolver::kDNSonly)
    {
    }

    virtual void Resolve(::google::protobuf::RpcController* controller,
                         const ::resolver::ResolveRequest* request,
                         ::resolver::ResolveResponse* response,
                         ::google::protobuf::Closure* done) {
        LOG_INFO << "ResolverServiceImpl::Resolve req addr=" << request->address();
        bool succ = false;
        succ = resolver_.resolve(request->address(),
                                 boost::bind(&ResolverServiceImpl::doneCallback,
                                             this,
                                             request->address(),
                                             _1,
                                             response,
                                             done));
        if (!succ) {
            response->set_resolved(false);
            done->Run();
        }
    }

private:
    void doneCallback(const std::string& host,
                      const muduo::net::InetAddress& address,
                      ::resolver::ResolveResponse* response,
                      ::google::protobuf::Closure* done) {
        LOG_INFO << "ResolverServiceImpl::doneCallback host=" << host;
        int32_t ip = address.ipNetEndian();
        if (ip) {
            response->set_resolved(true);
            response->add_ip(ip);
            response->add_port(address.portNetEndian());
        } else {
            response->set_resolved(false);
        }
        done->Run();
    }
private:
    cdns::Resolver resolver_;
};

}

int main() {
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(2053);
    resolver::ResolverServiceImpl impl(&loop);

    const google::protobuf::ServiceDescriptor* desc = impl.GetDescriptor();
    LOG_INFO << "fullname=" << desc->full_name();

    RpcServer server(&loop, listenAddr);
    server.registerService(&impl);
    server.start();
    loop.loop();
}
