

#include "resolver.pb.h"

#include <google/protobuf/stubs/callback.h>
#include <google/protobuf/descriptor.h>

#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

// /usr/local/include/google/protobuf/stubs/callback.h

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/protorpc/RpcChannel.h"

#include "muduo/net/Callbacks.h"

#include <arpa/inet.h>  // inet_ntop

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;


class RpcClient : noncopyable {
public:
    RpcClient(EventLoop* loop, const InetAddress& serverAddr)
        : loop_(loop),
          client_(loop, serverAddr, "RpcClient"),
          channel_(new RpcChannel),
          got_(0),
          total_(0),
          stub_(muduo::get_pointer(channel_))
    {
        client_.setConnectionCallback(
                    boost::bind(&RpcClient::onConnection, this, _1));
        client_.setMessageCallback(
                    boost::bind(&RpcChannel::onMessage, muduo::get_pointer(channel_), _1, _2, _3));
    }
    void connect() {
        client_.connect();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        if (conn->connected()) {
            channel_->setConnection(conn);
            total_ = 4;
            resolve("www.example.com");
            resolve("www.chenshuo.com");
            resolve("www.google.com");
            resolve("acme.chenshuo.org");
        } else {
            loop_->quit();
        }
    }
    void resolve(const std::string& host) {
        resolver::ResolveRequest req;
        req.set_address(host);

        resolver::ResolveResponse* response = new resolver::ResolveResponse;
        stub_.Resolve(NULL, &req, response,
                      ::google::protobuf::NewCallback(this, &RpcClient::resolved, response, host));
    }
    void resolved(resolver::ResolveResponse* resp, std::string host) {
        if (resp->resolved())
        {
            char buf[32];
            uint32_t ip = resp->ip(0);
            inet_ntop(AF_INET, &ip, buf, sizeof buf);

            LOG_INFO << "resolved " << host << " : " << buf << "\n"
                     << resp->DebugString();
        }
        else
        {
            LOG_INFO << "resolved " << host << " failed";
        }

        if (++got_ >= total_)
        {
            client_.disconnect();
        }
    }

private:
    EventLoop* loop_;
    TcpClient  client_;
    RpcChannelPtr channel_;
    int got_;
    int total_;
    resolver::ResolverService::Stub stub_;
};

int main(int argc, char* argv[]) {
    {
        resolver::ResolveRequest req;
        std::string host = "myhost";
        req.set_address(host);
        req.set_name("michel jordan");
        req.set_number(23);
        req.set_age(30);

        std::string p;
        google::protobuf::TextFormat::PrintToString(req, &p);
        printf("p=%s\n", p.c_str());
        return 1;
    }

    if (argc < 2) {
        LOG_INFO << "lack dst ip";
        return 1;
    }
    EventLoop loop;
    InetAddress serverAddr(argv[1], 2053);

    RpcClient rpcClient(&loop, serverAddr);
    rpcClient.connect();
    loop.loop();
    return 1;
}
