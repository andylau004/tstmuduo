

#pragma once


#include <thrift/TDispatchProcessor.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "sigslot.h"
#include "muduo/base/common.h"

using namespace apache::thrift;


class IOThread;
class ClientCtx;
class Processor : virtual public TDispatchProcessor {
public:
    Processor(ClientCtx * ctx, TypeProtocol pro);
    virtual ~Processor();

    virtual bool dispatchCall(protocol::TProtocol* iprot,
                              protocol::TProtocol* oprot,
                              const std::string& fname,
                              int32_t seqid, void* callContext);

    virtual void set_transport(void* ctx);
    void set_close_connection(void* ctx);

    IOThread * get_iothread();
    /**
     *  这里用来组装返回用户的信息
     */
    void return_result(void* result);

    virtual bool init();

    void recycle_ctx();
    bool close_apservice();

    TypeProtocol get_protocol() const { return m_protocol; }

    template <class T>
    void read_helper_fun(T& in_args, protocol::TProtocol* iprot) {
//      bg::BGService_getAllUserId_args args;
        in_args.read(iprot);
        iprot->readMessageEnd();
        iprot->getTransport()->readEnd();
    }

    template <class T>
    void return_helper_fun(T& result, const char* functionName, int32_t seqid, protocol::TProtocol* oprot) {
        oprot->writeMessageBegin(functionName, ::apache::thrift::protocol::T_REPLY, seqid);
        result.write(oprot);
        oprot->writeMessageEnd();
        oprot->getTransport()->writeEnd();
        oprot->getTransport()->flush();
        set_transport(NULL);
    }


public:
    sigslot::signal0<> SignalWriteSocket;
    sigslot::signal0<> SignalCloseSocket;
    sigslot::signal0<> SignalRecycleCtx;

protected:
    boost::function<void(int32_t, protocol::TProtocol*, void*)> m_resultCallback; /**< 操作完成的回调 */
    ClientCtx * m_clientctx; /**< 这个不属于Processor, 不能被创建, 也不能被删除 */
    TypeProtocol m_protocol;
    boost::function0<bool> m_closeService;
};


