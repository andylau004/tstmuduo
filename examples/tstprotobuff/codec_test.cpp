



#include "codec.h"



#include <iostream>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <string>
#include <stdint.h>

#include <stdlib.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#include <errno.h>

#include <sys/stat.h>

#include <limits.h>
#include <time.h>

#include <boost/function.hpp>

#include "query.pb.h"


void print(const std::string& buf)
{
    printf("encoded to %zd bytes\n", buf.size());
    for (size_t i = 0; i < buf.size(); ++i)
    {
        printf("%2zd:  0x%02x  %c\n",
               i,
               (unsigned char)buf[i],
               isgraph(buf[i]) ? buf[i] : ' ');
    }
}


void testQuery() {
    muduo::Query query;
    query.set_id(1);
    query.set_questioner("Chen Shuo");
    query.add_question("Running?");

    std::string transport = encode(query);
    print(transport);

    int32_t be32 = 0;

    std::copy(transport.begin(), transport.begin() + sizeof be32, reinterpret_cast<char*>(&be32));
    std::cout << "be32=" << be32 << std::endl;

    int32_t len = ::ntohl(be32);
    std::cout << "len=" << len << std::endl;
    assert(len == transport.size() - sizeof(be32));

    // network library decodes length header and get the body of message
    std::string buf = transport.substr(sizeof(int32_t));
    assert(len == buf.size());

    muduo::Query* newQuery = dynamic_cast<muduo::Query*>(decode(buf));
    assert(newQuery != nullptr);
    newQuery->PrintDebugString();
    assert(newQuery->DebugString() == query.DebugString());
    delete newQuery;

    buf[buf.size() - 6]++;  // oops, some data is corrupted
    muduo::Query* badQuery = dynamic_cast<muduo::Query*>(decode(buf));
    assert(badQuery == nullptr);
}
void testAnswer() {
    muduo::Answer answer;
    answer.set_id(1);
    answer.set_questioner("Chen Shuo");
    answer.set_answerer("blog.csdn.net/Solstice");
    answer.add_solution("Jump!");
    answer.add_solution("Win!");

    std::string transport = encode(answer);
    print(transport);

    std::cout << "answer.SpaceUsed=" << answer.SpaceUsed() << std::endl;

    int32_t be32 = 0;
    std::copy(transport.begin(), transport.begin() + sizeof be32, reinterpret_cast<char*>(&be32));
    int32_t len = ::ntohl(be32);
    std::cout << "len=" << len << std::endl;
    assert(len == transport.size() - sizeof(be32));

    // network library decodes length header and get the body of message
    std::string buf = transport.substr(sizeof(int32_t));
    assert(len == buf.size());

    muduo::Answer* newAnswer = dynamic_cast<muduo::Answer*>(decode(buf));
    assert(newAnswer != nullptr);
    newAnswer->PrintDebugString();
    assert(newAnswer->DebugString() == answer.DebugString());
    std::cout << "newanswer beg" << std::endl;
    newAnswer->PrintDebugString();
    std::cout << "newanswer end" << std::endl;

    std::cout << "newAnswer->SpaceUsed=" << newAnswer->SpaceUsed() << std::endl;
    delete newAnswer;

    buf[buf.size() - 6]++;  // oops, some data is corrupted
    muduo::Answer* badAnswer = dynamic_cast<muduo::Answer*>(decode(buf));
    assert(badAnswer == nullptr);
}

void testEmpty() {
    muduo::Empty emptyObj;
    std::string transport = encode(emptyObj);
    print(transport);
    std::cout << "len=" << emptyObj.DebugString().size() << " ttttt=" << emptyObj.DebugString() << std::endl;

    std::string buf = transport.substr(sizeof(int32_t));
    muduo::Empty* newEmpty = dynamic_cast<muduo::Empty*>(decode(buf));
    assert(newEmpty != nullptr);
    newEmpty->PrintDebugString();
    assert(newEmpty->DebugString() == emptyObj.DebugString());
    std::cout << "newtttt=" << newEmpty->DebugString() << std::endl;
    delete newEmpty;
}


class ProtobufDispatcherLite
{
public:
    typedef boost::function<void (google::protobuf::Message* message)> ProtobufMessageCallback;

    explicit ProtobufDispatcherLite(const ProtobufMessageCallback& defaultCb)
        : defaultCallback_(defaultCb)
    {
    }
    void onMessage(google::protobuf::Message* message) const {
        CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
        if (it != callbacks_.end()) {
            it->second(message);
        } else {
            defaultCallback_(message);
        }
    }
    void registerMessageCallback(const google::protobuf::Descriptor* desc, const ProtobufMessageCallback& callback) {
        callbacks_[desc] = callback;
    }
private:
    typedef std::map<const google::protobuf::Descriptor*, ProtobufMessageCallback> CallbackMap;

    CallbackMap callbacks_;
    ProtobufMessageCallback defaultCallback_;
};
void onQuery(google::protobuf::Message* message)
{
    std::cout << "onQuery: " << message->GetTypeName() << std::endl;
    muduo::Query* query = dynamic_cast<muduo::Query*>(message);
    assert(query != nullptr);
}
void onAnswer(google::protobuf::Message* message)
{
    std::cout << "onAnswer: " << message->GetTypeName() << std::endl;
    muduo::Answer* answer = dynamic_cast<muduo::Answer*>(message);
    assert(answer != nullptr);
}
void onUnknownMessageType(google::protobuf::Message* message)
{
    std::cout << "Discarding typename=" << message->GetTypeName() << std::endl;
}


void tst_dispatcher() {
    ProtobufDispatcherLite dispatcher(onUnknownMessageType);
    dispatcher.registerMessageCallback(muduo::Query::descriptor(), onQuery);
    dispatcher.registerMessageCallback(muduo::Answer::descriptor(), onAnswer);

    muduo::Query  q;
    muduo::Answer a;
    muduo::Empty  e;

    dispatcher.onMessage(&q);
    dispatcher.onMessage(&a);
    dispatcher.onMessage(&e);

//    google::protobuf::ShutdownProtobufLibrary();
}
void tst_codec_test() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tst_dispatcher();
    return;

    testQuery();
    puts("");
    testAnswer();
    puts("");
    testEmpty();
    puts("");

    puts("All pass!!!");

    google::protobuf::ShutdownProtobufLibrary();
}


