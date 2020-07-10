
#include "processor.h"



#include "clientctx.hpp"

Processor::Processor(ClientCtx * ctx, TypeProtocol pro) : m_protocol(pro) {
    m_clientctx = ctx;
}

Processor::~Processor() {}


bool Processor::dispatchCall(protocol::TProtocol *iprot,
                             protocol::TProtocol *oprot,
                             const std::string &fname,
                             int32_t seqid, void *callContext) {
    UNUSED(iprot);
    UNUSED(oprot);
    UNUSED(fname);
    UNUSED(seqid);
    UNUSED(callContext);
    return true;
}

IOThread* Processor::get_iothread() {
    if (m_clientctx == NULL) {
        return NULL;
    }
    return m_clientctx->get_iothread();
}

void Processor::set_transport(void *ctx) {
    UNUSED(ctx);
    SignalWriteSocket();
}

void Processor::set_close_connection(void *ctx) {
    UNUSED(ctx);
    SignalCloseSocket();
}

void Processor::return_result(void* result) {
    if (m_resultCallback.empty()) {
        SignalCloseSocket();
        return;
    }
    m_resultCallback(0, NULL, result);
}

bool Processor::init() {
    m_resultCallback.clear();
    m_closeService.clear();
    return true;
}

void Processor::recycle_ctx() {
    SignalRecycleCtx();
}

bool Processor::close_apservice() {
//    if (!m_closeService.empty()) {
//        return m_closeService();
//    }
    return true;
}

