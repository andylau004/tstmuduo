
#include "muduo/net/protorpc/RpcCodec.h"

#include "muduo/base/Logging.h"

#include "muduo/net/TcpConnection.h"

#include "muduo/net/protorpc/rpc.pb.h"
#include "muduo/net/protorpc/google-inl.h"

using namespace muduo;
using namespace net;


namespace {
int ProtobufVersionCheck()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  return 0;
}
int dummy __attribute__ ((unused)) = ProtobufVersionCheck();
}

namespace muduo
{
namespace net
{
const char rpctag [] = "RPC0";
}
}

