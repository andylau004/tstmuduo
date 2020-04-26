

#include "examples/protobuf/codec/codec.h"

#include "muduo/base/Logging.h"
#include "muduo/net/Endian.h"
#include "muduo/net/protorpc/google-inl.h"

#include <google/protobuf/descriptor.h>

#include <zlib.h>  // adler32

using namespace muduo;
using namespace muduo::net;

void ProtobufCodec::fillEmptyBuffer(Buffer* retbuf, const google::protobuf::Message& message)
{
    assert(retbuf->readableBytes() == 0);

    const std::string& typeName = message.GetTypeName();
    int32_t nameLen = static_cast<int32_t>(typeName.size()+1);

    retbuf->appendInt32(namelen);
    retbuf->append(typeName.c_str(), nameLen);

    // code copied from MessageLite::SerializeToArray() and MessageLite::SerializePartialToArray().
    GOOGLE_DCHECK(message.IsInitialized()) << InitializationErrorMessage("serialize", message);


}
