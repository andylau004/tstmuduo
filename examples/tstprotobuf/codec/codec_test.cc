

#include "examples/tstprotobuf/codec/codec.h"
#include "muduo/net/Endian.h"
#include "examples/tstprotobuf/codec/query.pb.h"


#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


#include <stdio.h>
#include <zlib.h>  // adler32

using namespace muduo;
using namespace muduo::net;


void print(const Buffer& buf) {
    printf("encoded to %zd bytes\n", buf.readableBytes());
    for (size_t i = 0; i < buf.readableBytes(); i ++ ) {
        unsigned char ch = static_cast<unsigned char>(buf.peek()[i]);

        printf("%2zd:  0x%02x  %c\n", i, ch, isgraph(ch) ? ch : ' ');
    }
}

void testQuery()
{
    muduo::Query query;
    query.set_id(1);
    query.set_questioner("Chen Shuo");
    query.add_question("Running?");

    Buffer buf;
    ProtobufCodec::fillEmptyBuffer(&buf, query);
    print(buf);

    const int32_t len = buf.readInt32();
    assert(len == static_cast<int32_t>(buf.readableBytes()));

    ProtobufCodec::ErrorCode errorCode = ProtobufCodec::kNoError;
    MessagePtr message = ProtobufCodec::parse(buf.peek(), len, &errorCode);
    assert(errorCode == ProtobufCodec::kNoError);
    assert(message != NULL);
    message->PrintDebugString();
    assert(message->DebugString() == query.DebugString());

    boost::shared_ptr<muduo::Query> newQuery = down_pointer_cast<muduo::Query>(message);
    assert(newQuery != NULL);
}

void testEmpty()
{
    muduo::Empty empty;

    Buffer buf;
    ProtobufCodec::fillEmptyBuffer(&buf, empty);
    print(buf);

    const int32_t len = buf.readInt32();
    assert(len == static_cast<int32_t>(buf.readableBytes()));

    ProtobufCodec::ErrorCode errorCode = ProtobufCodec::kNoError;
    MessagePtr message = ProtobufCodec::parse(buf.peek(), len, &errorCode);
    assert(message != NULL);
    message->PrintDebugString();
    assert(message->DebugString() == empty.DebugString());
}

void testBadBuffer()
{
    muduo::Empty empty;
    empty.set_id(43);

    Buffer buf;
    ProtobufCodec::fillEmptyBuffer(&buf, empty);
    // print(buf);

    const int32_t len = buf.readInt32();
    assert(len == static_cast<int32_t>(buf.readableBytes()));

    {
        std::string data(buf.peek(), len);
        ProtobufCodec::ErrorCode errorCode = ProtobufCodec::kNoError;
        MessagePtr message = ProtobufCodec::parse(data.c_str(), len - 1, &errorCode);
        LOG_INFO << "message=" << message.get();
        LOG_INFO << "errorCode=" << errorCode;
    }
    {
        std::string data(buf.peek(), len);
        ProtobufCodec::ErrorCode errorCode = ProtobufCodec::kNoError;
        data[len-1]++;
        MessagePtr message = ProtobufCodec::parse(data.c_str(), len, &errorCode);
        LOG_INFO << "message=" << message.get();
        LOG_INFO << "errorCode=" << errorCode;
    }
    {
        std::string data(buf.peek(), len);
        ProtobufCodec::ErrorCode errorCode = ProtobufCodec::kNoError;
        data[0]++;
        MessagePtr message = ProtobufCodec::parse(data.c_str(), len, &errorCode);
        LOG_INFO << "message=" << message.get();
        LOG_INFO << "errorCode=" << errorCode;
    }


}

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

//    testQuery();
//    puts("");

    testEmpty();
    puts("");

    testBadBuffer();
    puts("");
    return 1;
}
