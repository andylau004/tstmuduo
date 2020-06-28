
#include "examples/tstprotobuf/codec/dispatcher_lite.h"

#include "examples/tstprotobuf/codec/query.pb.h"

#include "muduo/base/Logging.h"


using std::cout;
using std::endl;


void onUnknownMessageType(const muduo::net::TcpConnectionPtr& conn,
                          const MessagePtr& message,
                          muduo::Timestamp ts) {
    LOG_INFO << "UnknowMessageType.typename=" << message->GetTypeName();
}
void onQuery(const muduo::net::TcpConnectionPtr& conn,
             const MessagePtr& message,
             muduo::Timestamp ts) {
    LOG_INFO << "onQuery.typename=" << message->GetTypeName();
    boost::shared_ptr<muduo::Query> query = muduo::down_pointer_cast<muduo::Query>(message);
    LOG_INFO << "onQuery.ptr=" << query.get();
}
void onAnswer(const muduo::net::TcpConnectionPtr&,
              const MessagePtr& message,
              muduo::Timestamp)
{
    LOG_INFO << "onAnswer: " << message->GetTypeName();
    boost::shared_ptr<muduo::Answer> answer = muduo::down_pointer_cast<muduo::Answer>(message);
    assert(answer != NULL);
}

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    {
        boost::shared_ptr<::google::protobuf::Message> msg(new muduo::Query);
        boost::shared_ptr<muduo::Query> query(muduo::down_pointer_cast<muduo::Query>(msg));
        if (msg == query) {
            LOG_INFO  << "msg == query" << ", querycount=" << query.use_count() << ", msgcount=" << msg.use_count();
        } else {
            LOG_INFO  << "msg != query";
        }
    }

    ProtobufDispatcherLite dispatcher(onUnknownMessageType);
    dispatcher.registerMessageCallback(muduo::Query::descriptor(), onQuery);
    dispatcher.registerMessageCallback(muduo::Answer::descriptor(), onAnswer);

    muduo::net::TcpConnectionPtr conn;
    muduo::Timestamp t;

    boost::shared_ptr<muduo::Query> query(new muduo::Query);
    boost::shared_ptr<muduo::Answer> answer(new muduo::Answer);
    boost::shared_ptr<muduo::Empty> empty(new muduo::Empty);

    dispatcher.onProtobufMessage(conn, query, t);
    dispatcher.onProtobufMessage(conn, answer, t);
    dispatcher.onProtobufMessage(conn, empty, t);
    return 1;
}
