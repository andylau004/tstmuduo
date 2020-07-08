


#ifndef CTHRIFT_SRC_CTHRIFT_CTHRIFT_ASYNC_CALLBACK_H_
#define CTHRIFT_SRC_CTHRIFT_CTHRIFT_ASYNC_CALLBACK_H_

#include <thrift/async/TAsyncChannel.h>
#include "cthrift_client.h"
#include "cthrift/util/cthrift_common.h"

namespace apache {
namespace thrift {
namespace transport {
class TMemoryBuffer;
}
}
}

namespace meituan_cthrift {
class CthriftClientChannel : public apache::thrift::async::TAsyncChannel {
public:
    using apache::thrift::async::TAsyncChannel::VoidCallback;

    explicit CthriftClientChannel(boost::shared_ptr<CthriftClient> sp_cthrift_cli);
    ~CthriftClientChannel();

    virtual void sendAndRecvMessage(
            const VoidCallback &cob,
            apache::thrift::transport::TMemoryBuffer *sendBuf,
            apache::thrift::transport::TMemoryBuffer *recvBuf);

    virtual void sendMessage(const VoidCallback &cob,
                             apache::thrift::transport::TMemoryBuffer *message);
    virtual void recvMessage(const VoidCallback &cob,
                             apache::thrift::transport::TMemoryBuffer *message);

    virtual bool good() const;
    virtual bool error() const;
    virtual bool timedOut() const;

private:
    boost::shared_ptr<CthriftClient> sp_cthrift_client_;
    boost::shared_ptr<CthriftClientWorker> sp_cthrift_client_worker_;

    // 没有实际作用，只是为了复用SharedContSharedPtr结构体
    muduo::MutexLock mutexlock_conn_ready;
    muduo::Condition cond_ready_read;

    boost::shared_ptr<muduo::MutexLock> sp_mutexlock_read_buf;
    TMemBufSharedPtr sp_write_buf;
    boost::shared_ptr<muduo::MutexLock> sp_mutexlock_write_buf;
    TMemBufSharedPtr sp_read_buf;
};
}  // namespace meituan_cthrift

#endif  // CTHRIFT_SRC_CTHRIFT_CTHRIFT_CLIENT_CHANNEL_H_
