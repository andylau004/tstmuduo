#pragma once

#include "muduo/base/noncopyable.h"
#include "muduo/base/StringPiece.h"
#include "muduo/base/Timestamp.h"
#include "muduo/net/InetAddress.h"

#include <functional>
#include <map>
#include <memory>

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


extern "C"
{
    struct hostent;
    struct ares_channeldata;
    typedef struct ares_channeldata* ares_channel;
}

namespace muduo
{
namespace net
{
class Channel;
class EventLoop;
}
}

namespace cdns
{

class Resolver : public muduo::noncopyable {
public:
    typedef boost::function< void(const muduo::net::InetAddress&) > Callback;
    enum Option {
        kDNSandHostsFile,
        kDNSonly,
    };

    explicit Resolver(muduo::net::EventLoop* loop, Option opt=kDNSandHostsFile);
    ~Resolver();

    bool resolve(muduo::StringArg hostname, const Callback& cb);

private:
    struct QueryData
    {
        Resolver* owner;
        Callback callback;
        QueryData(Resolver* o, const Callback& cb)
            : owner(o), callback(cb)
        {
        }
    };

    muduo::net::EventLoop* loop_;
    ares_channel ctx_;
    bool timerActive_;
    typedef std::map<int, std::unique_ptr<muduo::net::Channel>> ChannelList;
    ChannelList channels_;

    void onRead(int sockfd, muduo::Timestamp t);
    void onTimer();
    void onQueryResult(int status, struct hostent* result, const Callback& cb);
    void onSockCreate(int sockfd, int type);
    void onSockStateChange(int sockfd, bool read, bool write);

    static void ares_host_callback(void* data, int status, int timeouts, struct hostent* hostent);
    static int ares_sock_create_callback(int sockfd, int type, void* data);
    static void ares_sock_state_callback(void* data, int sockfd, int read, int write);
};

}



