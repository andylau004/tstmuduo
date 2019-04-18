
#pragma once


#include <thrift/protocol/TProtocol.h>

#include <string>
#include <map>

#include "processor.h"



using namespace apache::thrift;
using namespace apache::thrift::protocol;



class TestProcessor : public Processor {
public:
    TestProcessor(ClientCtx * ctx, TypeProtocol proc);
    ~TestProcessor();

    virtual bool dispatchCall(protocol::TProtocol * in,
                              protocol::TProtocol * out,
                              const std::string & fname,
                              int32_t seqid,
                              void * callContext);

    virtual bool process(boost::shared_ptr<TProtocol> in,
                         boost::shared_ptr<TProtocol> out,
                         void * connectionContext);

    virtual bool process(boost::shared_ptr<TProtocol> in,
                         boost::shared_ptr<TProtocol> out,
                         void * connectionContext,
                         int64_t flag);

    virtual bool init();

public:
    void process_SendPhoto(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext);
    void return_SendPhoto(int32_t, protocol::TProtocol*, void*);

    void process_Add(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext);
    void return_Add(int32_t, protocol::TProtocol*, void*);

    void process_login_im(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext);
    void return_login_im(int32_t, protocol::TProtocol*, void*);

//    void process_SendEmptyRequest(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext);
//    void return_SendEmptyRequest(int32_t, protocol::TProtocol*, void*);

protected:
    virtual void set_transport(void *ctx);
 private:
     //bool m_isinit;
//     APServiceHandler * m_apservicehandler;
 };


