



#pragma once



#include "muduo/base/Timestamp.h"
#include "muduo/net/protobuf/ProtobufCodecLite.h"


namespace muduo {
namespace net {

class Buffer;
class TcpConnection;

typedef boost::shared_ptr < TcpConnection > TcpConnectionPtr;

class RpcMessage;
typedef boost::shared_ptr < RpcMessage > RpcMessagePtr;

extern const char rpctag[];

// frame format
//
// Field     Length  Content
//
// size      4-byte  N+8
// "RPC0"    4-byte
// payload   N-byte
// checksum  4-byte  adler32 of "RPC0"+payload
//
typedef ProtobufCodecLiteT< RpcMessage, rpctag > RpcCodec;


}
}
