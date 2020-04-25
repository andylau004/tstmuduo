


#ifndef MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H
#define MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H

#include "muduo/net/Buffer.h"
#include "muduo/net/TcpConnection.h"

#include <google/protobuf/message.h>

// struct ProtobufTransportFormat __attribute__ ((__packed__))
// {
//   int32_t  len;
//   int32_t  nameLen;
//   char     typeName[nameLen];
//   char     protobufData[len-nameLen-8];
//   int32_t  checkSum; // adler32 of nameLen, typeName and protobufData
// }

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

//
// FIXME: merge with RpcCodec
//
class ProtobufCodec : muduo::noncopyable
{
public:

    enum ErrorCode
    {
        kNoError = 0,
        kInvalidLength,
        kCheckSumError,
        kInvalidNameLen,
        kUnknownMessageType,
        kParseError,
    };

    typedef boost::function< void(const muduo::net::TcpConnectionPtr&,
                                  const MessagePtr&,
                                  muduo::Timestamp) > ProtobufMessageCallback;

    typedef boost::function< void (const muduo::net::TcpConnectionPtr&,
                                   const MessagePtr&,
                                   muduo::Timestamp,
                                   ErrorCode) > ErrorCallback;

    explicit ProtobufCodec(const ProtobufMessageCallback& messageCb)
      : messageCallback_(messageCb),
        errorCallback_(defaultErrorCallback)
    {
    }

    ProtobufCodec(const ProtobufMessageCallback& messageCb, const ErrorCallback& errorCb)
      : messageCallback_(messageCb),
        errorCallback_(errorCb)
    {
    }

    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp receiveTime);

    void send(const muduo::net::TcpConnectionPtr& conn,
              const google::protobuf::Message& message) {

    }


private:
    static void defaultErrorCallback(const muduo::net::TcpConnectionPtr&,
                                     muduo::net::Buffer*,
                                     muduo::Timestamp,
                                     ErrorCode);

    ProtobufMessageCallback messageCallback_;
    ErrorCallback errorCallback_;

    const static int kHeaderLen = sizeof(int32_t);
    const static int kMinMessageLen = 2*kHeaderLen + 2; // nameLen + typeName + checkSum
    const static int kMaxMessageLen = 64*1024*1024; // same as codec_stream.h kDefaultTotalBytesLimit

};





#endif  // MUDUO_EXAMPLES_PROTOBUF_CODEC_CODEC_H
