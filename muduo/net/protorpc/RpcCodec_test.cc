


#include <stdio.h>

#include "muduo/net/protorpc/RpcCodec.h"
#include "muduo/net/protorpc/rpc.pb.h"
#include "muduo/net/protobuf/ProtobufCodecLite.h"
#include "muduo/net/Buffer.h"

#include "muduo/base/Logging.h"

using namespace muduo;
using namespace muduo::net;

void rpcMessageCallback(const TcpConnectionPtr&,
                        const RpcMessagePtr&,
                        Timestamp)
{
}

MessagePtr g_msgptr;
void messageCallback(const TcpConnectionPtr&,
                     const MessagePtr& msg,
                     Timestamp)
{
    g_msgptr = msg;
}

void print(const Buffer& buf)
{
    printf("encoded to %zd bytes\n", buf.readableBytes());
    for (size_t i = 0; i < buf.readableBytes(); ++i)
    {
        unsigned char ch = static_cast<unsigned char>(buf.peek()[i]);

        printf("%2zd:  0x%02x  %c\n", i, ch, isgraph(ch) ? ch : ' ');
    }
}

char rpctag[] = "RPC0";




void testBufferAppendRetrieve() {
    Buffer buf;
    LOG_INFO << "readableBytes=" << buf.readableBytes();
    LOG_INFO << "writeableBytes=" << buf.writableBytes();
    LOG_INFO << "prependableBytes=" << buf.prependableBytes();

    std::string str(200, 'x');
    LOG_INFO << "str=" << str;
    buf.append(str);
    LOG_INFO << "readableBytes=" << buf.readableBytes();
    LOG_INFO << "writableBytes=" << buf.writableBytes();
    LOG_INFO << "t1=" << Buffer::kInitialSize - str.size();
    LOG_INFO << "prependableBytes=" << buf.prependableBytes();

}


int main() {
    testBufferAppendRetrieve();
    return 1;

    RpcMessage message;
    message.set_type(REQUEST);
    message.set_id(2);
    char wire[] = "\0\0\0\x13" "RPC0" "\x08\x01\x11\x02\0\0\0\0\0\0\0" "\x0f\xef\x01\x32";
    std::string expected(wire, sizeof(wire) - 1);
    std::string s1, s2;
    Buffer buf1, buf2;
    {
        RpcCodec codec(rpcMessageCallback);
        codec.fillEmptyBuffer(&buf1, message);
        print(buf1);
        s1 = buf1.toStringPiece().as_string();
        printf("len=%d s1=%s\n", s1.length(), s1.c_str());
    }

    return 1;
}





