
#pragma once



#include <stdio.h>
#include <stdint.h>

#include <thrift/transport/TTransport.h>
#include <thrift/transport/TBufferTransports.h>

using namespace apache::thrift;

#include "muduo/base/common.h"
#include "sigslot.h"


/**
 *  sigslot 使得服务更像单独的模块了，但是如果性能不行的话，应该用真实的指针去处理
 */
class Transport {
    typedef boost::shared_ptr<transport::TMemoryBuffer> TMemoryBufferPtr;
public:
    Transport(uint32_t readBufferSize, uint32_t writeBufferSize);
    explicit Transport(uint32_t size = 512);
    virtual ~Transport();

    virtual bool init();

    virtual void work_socket(int sfd) = 0;
    virtual void write_socket(int sfd) = 0;
    virtual void reset_write_state();

    sigslot::signal0<> SignalCloseSocket;
    sigslot::signal0<> SignalClientCloseSocket;
    sigslot::signal0<> SignalReadSocketDone;
    sigslot::signal0<> SignalSetWriteState;
    sigslot::signal0<> SignalSetReadState;

    // 通知收到客户端数据
    sigslot::signal0<> SingalRecvSomeData;

    TMemoryBufferPtr getInputTransport()    { return m_inMemBuffer; }
    TMemoryBufferPtr getOutputTransport()   { return m_outMemBuffer; }
protected:
    int32_t read(uint8_t * buf, uint32_t len);
    int32_t write(const uint8_t * buf, uint32_t len);
protected:
    uint8_t * m_readBuffer;
    uint8_t * m_writeBuffer; /**< 不能被删除，由TMemoryBuffer管理 */

    uint32_t m_readBufferSize;
    uint32_t m_writeBufferSize;
    uint32_t m_readBufferPos;
    uint32_t m_writeBufferPos;

    SOCKET   m_socket;

    TMemoryBufferPtr m_inMemBuffer;
    TMemoryBufferPtr m_outMemBuffer;
};

