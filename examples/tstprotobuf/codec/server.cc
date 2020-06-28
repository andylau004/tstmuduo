#include "examples/tstprotobuf/codec/codec.h"
#include "examples/tstprotobuf/codec/dispatcher.h"
#include "examples/tstprotobuf/codec/query.pb.h"

#include "muduo/base/Logging.h"
#include "muduo/base/Mutex.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/TcpServer.h"

#include <stdio.h>
#include <unistd.h>

#include <boost/function.hpp>
//#include <boost/bind/bind.hpp>
#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

typedef boost::shared_ptr<muduo::Query> QueryPtr;
typedef boost::shared_ptr<muduo::Answer> AnswerPtr;

class QueryServer : public muduo::noncopyable
{
public:
    QueryServer(EventLoop* loop,
                const InetAddress& listenAddr)
        : server_(loop, listenAddr, "QueryServer"),
          dispatcher_(boost::bind(&QueryServer::onUnknownMessage, this, _1, _2, _3)),
          codec_(boost::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_, _1, _2, _3))
    {
        dispatcher_.registerMessageCallback<muduo::Query>(
                    boost::bind(&QueryServer::onQuery, this, _1, _2, _3));
        dispatcher_.registerMessageCallback<muduo::Answer>(
                    boost::bind(&QueryServer::onAnswer, this, _1, _2, _3));
        server_.setConnectionCallback(
                    boost::bind(&QueryServer::onConnection, this, _1));
        server_.setMessageCallback(
                    boost::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
    }
    void start() {
        server_.start();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {
        LOG_INFO << conn->peerAddress().toIpPort() << " -> "
                 << conn->localAddress().toIpPort() << " is "
                 << (conn->connected() ? "UP" : "DOWN");
    }
    void onUnknownMessage(const muduo::net::TcpConnectionPtr& conn,
                          const MessagePtr& message,
                          muduo::Timestamp tm) {
        LOG_INFO << "query server recv, unknown message, type=" << message->GetTypeName();
        conn->shutdown();
    }
    void onQuery(const muduo::net::TcpConnectionPtr& conn,
                 const QueryPtr& message,
                 muduo::Timestamp tm) {
        LOG_INFO << "onQuery:\n" << message->GetTypeName() << message->DebugString();
        muduo::Answer answer;
        answer.set_id(1);
        answer.set_questioner("test answer");
        answer.set_answerer("fake news...");
        answer.add_solution("shame on you!");
        answer.add_solution("Win!");
        codec_.send(conn, answer);

        conn->shutdown();
    }
    void onAnswer(const muduo::net::TcpConnectionPtr& conn,
                  const AnswerPtr& message,
                  muduo::Timestamp tm) {
        LOG_INFO << "onAnswer: " << message->GetTypeName();
        conn->shutdown();
    }

private:
    TcpServer server_;
    ProtobufDispatcher dispatcher_;
    ProtobufCodec codec_;
};

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        EventLoop loop;
        uint16_t port = static_cast<uint16_t>(atoi(argv[1]));

        InetAddress serverAddr(port);
        QueryServer server(&loop, serverAddr);

        server.start();
        loop.loop();
    }
    else
    {
        printf("Usage: %s port\n", argv[0]);
    }
    return 1;
}
