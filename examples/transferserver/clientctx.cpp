
#include "clientctx.hpp"


#include <assert.h>
#include <unistd.h>
#include <iostream>

#include "threadex.hpp"
#include "iothread.hpp"
#include "frametransport.hpp"


//#include "approcessor.h"
//#include "bgprocessor.h"
//#include "javaprocessor.h"
#include "testprocessor.h"

//#include "logs/log.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"


//#include "sessionmanager.h"

//extern log4cplus::Logger gLogger;


ClientCtx::ClientCtx(IOThread * thread, TypeProcess tp, TypeProtocol pro) :
    isInUse(false),
    m_typeProcess(tp),
    m_protocol(pro),
    m_sessionID(0),
    m_notifyID(0),
    m_idle_time(0),
    m_thread(thread),
    m_ctx(NULL),
    m_transport(NULL),
    m_processor(NULL),
    m_ticket(""), m_uIdx (0) {
//        pthread_mutex_init(&m_clientLock, NULL);
}

ClientCtx::~ClientCtx() {
    printf( "clientctx destruction, this=%p\n", this);
//    LOG_INFO << "clientctx destruction, this=" << this;
    delete_object(m_ctx);
    delete_object(m_transport);
    delete_object(m_processor);
//    pthread_mutex_destroy(&m_clientLock);
}

/// 暂时不处理process不一致的状况, 保证process一定是一致的
bool ClientCtx::init(Item_t * item) {
    if (item == NULL || m_typeProcess == P_UNKNOWN) {
        LOG_ERROR << " item or type is error" << m_typeProcess;
        return false;
    }
    isInUse = true;
    m_sessionID = 0;
    m_notifyID = 0;
    m_ticket = "";

    if (m_ctx == NULL) {
        m_ctx = new (std::nothrow) conn_ctx_t;
        if (m_ctx == NULL) {
            LOG_ERROR << " new ctx error";
            return false;
        }
    }
    m_ctx->m_sfd   = item->m_socket;
    m_ctx->m_flags = 0;

    if (m_transport == NULL) {
        m_transport = new (std::nothrow) FrameTransport(512);
        if (m_transport == NULL) {
            LOG_ERROR << " can't new transport ";
            return false;
        }
        m_transport->SignalCloseSocket.connect(this, &ClientCtx::on_closesocket);
        m_transport->SignalClientCloseSocket.connect(this, &ClientCtx::on_clientclosesocket);
        m_transport->SignalReadSocketDone.connect(this, &ClientCtx::on_readsocketdone);
        m_transport->SignalSetWriteState.connect(this, &ClientCtx::on_setwritestate);
        m_transport->SignalSetReadState.connect(this, &ClientCtx::on_setreadstate);

//        m_transport->SingalRecvSomeData.connect(this, &ClientCtx::on_recvsomedata);
    }
    if (!m_transport->init()) {
        LOG_ERROR << " bad transport init " << m_ctx->m_sfd;
        return false;
    }
    m_inputProtocol  = m_thread->getInputProtocolFactory()->getProtocol(m_transport->getInputTransport());
    m_outputProtocol = m_thread->getOutputProtocolFactory()->getProtocol(m_transport->getOutputTransport());

    if (m_processor == NULL) {
        if (P_TEST_PROCESS == m_typeProcess) {
            m_processor = new (std::nothrow) TestProcessor(this, m_protocol);
        }
        else if (m_typeProcess == P_APPROCESS) {
//            m_processor = new (std::nothrow) APProcessor(this, m_protocol);
        } else if (m_typeProcess == P_BGPROCESS) {
//            m_processor = new (std::nothrow) BGProcessor(this, m_protocol);
        } else if (m_typeProcess == P_JAVAPROCESS) {
//            m_processor = new (std::nothrow) JAVAProcessor(this, m_protocol);
        } else {
            LOG_ERROR << " unknown process type...type=" << m_typeProcess;
            return false;
        }
        if (m_processor == NULL) {
            LOG_ERROR << " can't new processor type:" << m_typeProcess;
            return false;
        }
        m_processor->SignalWriteSocket.connect(this, &ClientCtx::on_writesocket);
        m_processor->SignalCloseSocket.connect(this, &ClientCtx::on_closesocket);
        m_processor->SignalRecycleCtx.connect(this, &ClientCtx::on_recyclectx);
    }
    if (!m_processor->init()) {
        LOG_ERROR << " can't init process ";
        return false;
    }
    LOG_INFO << " new init fd " << m_ctx->m_sfd
             << " event " << &m_ctx->m_event
             << " type " << m_typeProcess
             << " protocol " << m_protocol
             << " ctx " << this;
    return set_ctx_read_ctxnew();
}

void ClientCtx::on_closesocket() {
    //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << ", close socket " << m_ctx->m_sfd << "," << (int)isRecycle);
    close_connection(true);
}

void ClientCtx::on_clientclosesocket() {
    bool isRecycle = true;
    if (m_processor) {
//        isRecycle = m_processor->close_apservice();
    } else {
        LOG_INFO << "m_processor is NULL!";
    }
    //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << ", close socket " << m_ctx->m_sfd << "," << (int)isRecycle);
    close_connection(isRecycle);
}

void ClientCtx::on_recyclectx() {
    recycle_ctx();
}

void ClientCtx::on_readsocketdone() {
    LOG_INFO << ", finish to read socket, will process, --sd start:" << m_ctx->m_sfd << " ctx " << this;
    //set_ctx_idle();
    set_ctx_read_ctxnew();
    bool ret = false;
    try {
        ret = m_processor->process(m_inputProtocol, m_outputProtocol, this);
    } catch(std::exception& e) {
        ret = false;
        LOG_ERROR << " bad process " << m_ctx->m_sfd << ":" << e.what();
    } catch(...) {
        ret = false;
        LOG_ERROR << " bad process " << m_ctx->m_sfd;
    }
    //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << ", process call end, ret:" << (int)ret << ", --sd end:" << m_ctx->m_sfd);
    if (!ret) {
        LOG_ERROR << ", process call error " << m_ctx->m_sfd;
        on_closesocket();
    }
}

void ClientCtx::on_setwritestate() {
    if (set_ctx_write_ctxnew()) {
        //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << ", good callback sd:" << m_ctx->m_sfd);
    } else {
        //LOG4CPLUS_ERROR(gLogger, __FUNCTION__ << ", @@@ bad callback sd:" << m_ctx->m_sfd);
        on_closesocket();
    }
}

void ClientCtx::on_setreadstate() {
    set_ctx_read_ctxnew();
}

// 收到数据刷新时间轮
void ClientCtx::on_recvsomedata() {
    m_thread->AddOneCtx(this/*thisPtr*/);
}

void ClientCtx::on_writesocket() {
    LOG_INFO << ", sd:" << m_ctx->m_sfd << " ctx " << this;
    m_transport->write_socket(m_ctx->m_sfd);
}

void ClientCtx::pushMessage(int64_t sessionID) {
    if (sessionID <= 0 || sessionID != m_sessionID) {
        return;
    }

    LOG_INFO << " get msg session id " << sessionID;
    if (m_processor) {
        bool ret = false;
        m_transport->getOutputTransport()->resetBuffer();
        m_transport->getOutputTransport()->getWritePtr(4);
        m_transport->getOutputTransport()->wroteBytes(4);

        m_transport->reset_write_state();
        ret = m_processor->process(m_inputProtocol, m_outputProtocol, this, m_sessionID);
        if (!ret) {
            LOG_ERROR << " can't process " << sessionID;
            on_closesocket();
        }
    } else {
        LOG_ERROR << " processor is nil " << sessionID;
    }
}

void ClientCtx::setSessionID(int64_t sessionID) {
    if (sessionID <= 0) return;
    muduo::MutexLockGuard lock(m_clientLock);
    m_sessionID = sessionID;
    if (m_sessionID > 0) {
        m_thread->setPushConnection(m_sessionID, this);
    }
}

void ClientCtx::setClientTicket(const std::string &ticket) {
    muduo::MutexLockGuard lock(m_clientLock);
    m_ticket = ticket;
}

void ClientCtx::setClientNotifyID(int64_t notifyID) {
    muduo::MutexLockGuard lock(m_clientLock);
    m_notifyID = notifyID;
}

int64_t ClientCtx::getSessionID() {
    int64_t id = 0;
    muduo::MutexLockGuard lock(m_clientLock);
    id = m_sessionID;
    return id;
}

int64_t ClientCtx::getClientNotifyID() {
    int64_t id = 0;
    muduo::MutexLockGuard lock(m_clientLock);
    id = m_notifyID;
    return id;
}

std::string ClientCtx::getClientTicket() {
    std::string ticket;
    muduo::MutexLockGuard lock(m_clientLock);
    ticket = m_ticket;
    return ticket;
}

IOThread * ClientCtx::get_iothread() {
    return m_thread;
}

bool ClientCtx::set_ctx_read() {
    return set_ctx_flags(EV_PERSIST | EV_READ);
}

bool ClientCtx::set_ctx_write() {
    return set_ctx_flags(EV_PERSIST | EV_WRITE);
}

bool ClientCtx::set_ctx_idle() {
    return set_ctx_flags(0);
}

bool ClientCtx::set_ctx_read_ctxnew() {
    return set_ctx_flags_ctxnew(EV_PERSIST | EV_READ);
}

bool ClientCtx::set_ctx_write_ctxnew() {
    return set_ctx_flags_ctxnew(EV_PERSIST | EV_WRITE);
}

bool ClientCtx::set_ctx_idle_ctxnew() {
    return set_ctx_flags_ctxnew(0);
}

bool ClientCtx::set_ctx_flags_ctxnew(short flags) {
    if (m_ctx == NULL) {
        LOG_ERROR << " m_ctx error";
        return false;
    }

    LOG_DEBUG << " old " << m_ctx->m_flags << " new " << flags << " event " << &m_ctx->m_event;
    if (m_ctx->m_flags == flags) {
        return true;
    }

    event_thread *t = m_thread->get_thread();
    if (t == nullptr) {
        return false;
    }

    if (m_ctx->m_flags != 0) {
        if (event_del(&m_ctx->m_event) == -1) {
            LOG_ERROR << " event_del error";
            return false;
        }
    }

    m_ctx->m_flags = flags;
    if (!m_ctx->m_flags) {
        LOG_INFO << " no pending event ";
        return true;
    }

    ::event_set(&m_ctx->m_event, m_ctx->m_sfd, m_ctx->m_flags, ClientCtx::connection_event_handler, this);
    ::event_base_set(t->get_eventbase(), &m_ctx->m_event);
    if (::event_add(&m_ctx->m_event, nullptr) == -1) {
        LOG_ERROR << " event add error!!!";
        return false;
    }
    return true;
}

bool ClientCtx::set_ctx_flags(short flags) {
//    if (m_ctx == NULL) {
//        return false;
//    }

//    LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << " old flag " << m_ctx->m_flags
//                    << " new flag " << flags << " fd " << m_ctx->m_sfd);

//    if (m_ctx->m_flags == flags) {
//        return true;
//    }

//    Thread * t = m_thread->get_thread();
//    if (t == NULL) {
//        return false;
//    }
//    if (m_ctx->m_flags != 0) {
//        m_ctx->m_flags = t->set_eventcallback(&m_ctx->m_event, m_ctx->m_sfd,
//                                              flags, connection_event_handler, this, true);
//    } else {
//        m_ctx->m_flags = t->set_eventcallback(&m_ctx->m_event, m_ctx->m_sfd,
//                                              flags, connection_event_handler, this, false);
//    }

//    return m_ctx->m_flags == flags;
    return true;
}

void ClientCtx::close_connection(bool isRecycle) {
    //set_ctx_idle();
    if (event_del(&m_ctx->m_event) == -1) {
        LOG_ERROR << " event_del failed!!!";
    }

    LOG_ERROR << ", close socket " << m_ctx->m_sfd << "," << (int)isRecycle;
    if (m_ctx->m_sfd != INVALID_SOCKET) {
        shutdown(m_ctx->m_sfd, SHUT_RDWR);
        close(m_ctx->m_sfd);
        m_ctx->m_sfd = INVALID_SOCKET;
    }
    if (m_sessionID > 0) {
        LOG_ERROR << " return sid " << m_sessionID;
        m_thread->deletePushConnection(m_sessionID, this);
//        _sm.on_clt_disconnect(m_sessionID);
    }

    setSessionID(0);
    setClientNotifyID(0);
    setClientTicket("");
    if (isRecycle) {
        isInUse = false;
        m_thread->return_clientctx(this);
    }
    LOG_INFO << ", is return ctx=" << this;
}

void ClientCtx::recycle_ctx() {
    //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << ", recycle socket " << m_ctx->m_sfd);
    isInUse = false;
    m_thread->return_clientctx(this);
}

void ClientCtx::connection_event_handler(SOCKET fd, short which, void * v) {
    ClientCtx* ctx = reinterpret_cast<ClientCtx*>(v);
    if (ctx == NULL) {
        LOG_ERROR << ", socket " << fd;
        return;
    }
    if (fd != ctx->m_ctx->m_sfd) {
        LOG_ERROR << " bad fd " << fd << " old " << ctx->m_ctx->m_sfd << " " << ctx
                  << " which " << which << " event " << &ctx->m_ctx->m_event;
        return;
    }
    ctx->m_transport->work_socket(fd);
}
