


#include "iothread.hpp"

#include <pthread.h>

#include <unistd.h>
#include <assert.h>

#include <deque>
#include <queue>
#include <iostream>
#include <atomic>

#include <boost/unordered_map.hpp>
#include <boost/get_pointer.hpp>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>

#include "common_stucts.hpp"
#include "threadex.hpp"
#include "clientctx.hpp"
#include "iothread.hpp"

#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "../../muduo/base/event_watcher.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;

using namespace boost;

//using namespace apache::thrift;
//extern log4cplus::Logger gLogger;


typedef struct conn_queue_s {
    MutexLock               m_newlock;
    std::deque<Item_t *>    m_connQueue;

    conn_queue_s()  { }
    ~conn_queue_s() { }

//    pthread_mutex_t         m_newlock;
//    std::deque<Item_t *>    m_connQueue;
//    void lock()     {
//        pthread_mutex_lock(&m_newlock);
//        LOG_INFO << ( "----------lock" );
//    }
//    void unlock()   {
//        pthread_mutex_unlock(&m_newlock);
//        LOG_INFO << ( "----------unlock" );
//    }
//    conn_queue_s()  { pthread_mutex_init(&m_newlock, NULL); }
//    ~conn_queue_s() { pthread_mutex_destroy(&m_newlock); }
} conn_queue_t;



class IOThreadImpl {
public:
    //typedef std::deque<ClientCtx*> CCQ; // TODO(H.haiwei) pop_front 竟然会失败
    typedef std::queue<ClientCtx*> CCQ;

public:
    typedef boost::weak_ptr<ClientCtx> WeakClientCtxPtr;

    struct EntryCtx : public muduo::copyable
    {
        explicit EntryCtx(const WeakClientCtxPtr& weakCtx) : wk_client_ctx_(weakCtx) {
        }
        ~EntryCtx() {
//            printf( "--------------EntryCtx Obj Destroying beg--------------\n" );
            OutputDbgInfo tmp("--------------EntryCtx Obj Destroying beg--------------",
                              "--------------EntryCtx Obj Destroying end--------------");
             boost::shared_ptr<ClientCtx> ctxptr = wk_client_ctx_.lock();
             if (ctxptr) {
                 ctxptr->on_closesocket();
             }
//             printf( "--------------EntryCtx Obj Destroying end--------------\n" );

             LOG_INFO << " ctxptr GetIdx=" << ctxptr->GetIdx() << " usecount=" << ctxptr.use_count();
        }
        WeakClientCtxPtr wk_client_ctx_;
    };
    typedef std::shared_ptr<EntryCtx>  EntryCtxPtr;
    typedef std::weak_ptr<EntryCtx>    WeakEntryCtxPtr;

    typedef std::unordered_set<EntryCtxPtr>    Bucket;
    typedef boost::circular_buffer<Bucket>     WeakConnectionList;

    WeakConnectionList connectionBuckets_;


    typedef boost::shared_ptr<ClientCtx>  ClientCtxPtr;


    struct event m_idle_time_event;
    struct event m_notifyEvent;

public:
    IOThreadImpl(int tid, TypeProcess tp, TypeProtocol proc);
    ~IOThreadImpl();

    bool set_item(Item_t* item);
    bool start(IOThread * t);
    void return_clientctx(ClientCtx * ctx);
    int get_notifysendfd() const    { return m_notifySendfd; }
    int get_tid() const             { return m_tid; }

    event_thread * get_thread()       { return m_thread; }
    IOThread * get_iothread()   { return m_iothread; }

    TProtocolFactoryPtr getInputProtocolFactory()   { return m_inputProtocolFactory; }
    TProtocolFactoryPtr getOutputProtocolFactory()  { return m_outputProtocolFactory; }

    evutil_socket_t getNotificationPushSendFD() const {
        //LOG4CPLUS_DEBUG(sLogger, __FUNCTION__ << " id " << notificationPushPipeFDs_[1]);
        return notificationPushPipeFDs_[1];
    }
    evutil_socket_t getNotificationPushRecvFD() const { return notificationPushPipeFDs_[0]; }

    void setPushConnection(int64_t id, ClientCtx *ctx);
    void deletePushConnection(int64_t id, ClientCtx * ctx);
    void monitor_idle_ctx();

    void set_isSetTimer(bool set) {m_isSetTimer = set;}
    bool get_isSetTimer() {return m_isSetTimer;}
    struct event * get_timeEvent(){return &m_timerEvent;}

    void AddOneCtx(const ClientCtx* pCtx/*const boost::shared_ptr<ClientCtx>& oneCtx*/);

private:
    bool init(int tid, IOThread * t);
    bool register_notifyevent();
    bool register_timerevent();

    bool create_notifypipe();

    Item_t * get_connitem();
    void check_timer();

    ClientCtx * get_clientctx();
    bool create_connection(Item_t * item);

    void pushMessage(int64_t sessionID);

    static void thread_handle_newconnection(int fd, short which, void *arg, void *user_data);
    static void thread_check_timer(int fd, short which, void *arg);
    static void thread_notifypush_handler(int fd, short which, void *arg);

    static void onTimer(int fd, short which, void *arg);
    void dumpConnectionBuckets() const;

    void SaveNewCtx(const ClientCtxPtr& in_sp);
private:
    atomic_uint64_t idx_connection;
    boost::unordered_map< uint64_t, ClientCtxPtr > id_ctx_map_;
    muduo::MutexLock mtx_id_ctx;


    bool m_isSetTimer;

    TypeProcess  m_typeProcess;
    TypeProtocol m_protocol;

    int m_tid;
    int m_notifyRecvfd;
    int m_notifySendfd;

    // 封装 替代 notificationPipeFDs_
    PipeEventWatcherPtr notify_watcher_;
//    TimerEventWatcher   timer_idle_watcher_;

    evutil_socket_t notificationPushPipeFDs_[2];

    conn_queue_t    *   m_newConns;
    event_thread    *   m_thread;
    IOThread        *   m_iothread; // 不会被删除

    boost::shared_ptr<protocol::TProtocolFactory> m_inputProtocolFactory;
    boost::shared_ptr<protocol::TProtocolFactory> m_outputProtocolFactory;

    boost::unordered_map<int64_t, ClientCtx*> pushConnections_;

    //CCQ         m_activeCtxs;
    CCQ         m_idleCtxs;

//    struct event m_notifyEvent;
    struct event m_timerEvent;
    struct event notificationPushEvent_;

    struct event * ev_timer_;

    int64_t activeCtxnum_;
};

IOThreadImpl::IOThreadImpl(int tid, TypeProcess tp, TypeProtocol proc) :
    m_isSetTimer(false),
    m_typeProcess(tp),
    m_protocol(proc),
    m_tid(tid),
    m_notifyRecvfd(INVALID_SOCKET),
    m_notifySendfd(INVALID_SOCKET),
    m_newConns(NULL),
    m_thread(NULL),
    m_iothread(NULL),
    activeCtxnum_(0),
    ev_timer_ (NULL) {

    connectionBuckets_.resize(3);

    switch(proc) {
    case PRO_JSON:
        m_inputProtocolFactory.reset(new protocol::TJSONProtocolFactory);
        m_outputProtocolFactory.reset(new protocol::TJSONProtocolFactory);
        break;
    case PRO_BINARY:
        m_inputProtocolFactory.reset(new protocol::TBinaryProtocolFactory);
        m_outputProtocolFactory.reset(new protocol::TBinaryProtocolFactory);
        break;
    default:
        m_inputProtocolFactory.reset(new protocol::TBinaryProtocolFactory);
        m_outputProtocolFactory.reset(new protocol::TBinaryProtocolFactory);
        break;
    }

    notificationPushPipeFDs_[0] = INVALID_SOCKET;
    notificationPushPipeFDs_[1] = INVALID_SOCKET;

//    event_loop.runEvery(1.0, boost::bind(&IOThreadImpl::onTimer, this));

}

IOThreadImpl::~IOThreadImpl() {
    delete_object(m_newConns);
    delete_object(m_thread);

    if (notificationPushPipeFDs_[0] > 0)
        close(notificationPushPipeFDs_[0]);
    if (notificationPushPipeFDs_[1] > 0)
        close(notificationPushPipeFDs_[1]);

    // TODO(H.haiwei) 这里并没有真正的删除ClientCtx
    if (ev_timer_) {
        event_free(ev_timer_);
    }
}

bool IOThreadImpl::set_item(Item_t * item) {
    if (item == NULL || m_newConns == NULL) {
        return false;
    }
    {
        MutexLockGuard lock(m_newConns->m_newlock);
        m_newConns->m_connQueue.push_back(item);
//        LOG_INFO << "set_item m_newConns=" << m_newConns;
//        LOG_INFO << "set_item m_newConns->m_connQueue.size=" << m_newConns->m_connQueue.size();
//        m_newConns->lock();
//        m_newConns->m_connQueue.push_back(item);
//        m_newConns->unlock();
    }
    notify_watcher_->Notify();
    return true;
}
bool IOThreadImpl::start(IOThread * t) {
    if (!init(m_tid, t)) {
        return false;
    }
    if (m_newConns == NULL || m_thread == NULL) {
        return false;
    }
    return (m_thread->start(t) == SUCCESS);
}

void IOThreadImpl::return_clientctx(ClientCtx * ctx) {
    if (ctx == NULL) {
        return;
    }
    // 或者考虑下增加一个类似的NGINX判断ctx失效的单子节变量
    //m_idleCtxs.push_back(ctx);
    ctx->set_idle_time();
    m_idleCtxs.push(ctx);
    LOG_INFO << " ctx=" << ctx << ",idlesize=" << m_idleCtxs.size();

    {
        uint64_t tmpId = ctx->GetIdx();
        LOG_INFO << "return_clientctx shoudao close connection Idx=" << tmpId ;

//        MutexLockGuard lock(mtx_id_ctx);
//        boost::unordered_map< uint64_t, ClientCtxPtr >::iterator itfind
//                = id_ctx_map_.find(tmpId);
//        if ( itfind == id_ctx_map_.end() ) {
//            LOG_ERROR << "no found IdxId=" << tmpId << ", id_ctx_map size=" << id_ctx_map_.size();
////            for (auto iter = id_ctx_map_.begin(); iter != id_ctx_map_.end(); ++iter) {
////                LOG_INFO << "mapId=" << iter->first /*<< " mapval=" << iter->second*/;
////            }
//        } else {
//            WeakClientCtxPtr tmpSecond(itfind->second);
//            if (tmpSecond.expired()) {
//                LOG_INFO << "tmpId=" << tmpId << " connection is destroy!!!";
//            } else {
//                LOG_INFO << "before tmpSecond not expired" << " usecount=" << tmpSecond.use_count();
//            }
//            id_ctx_map_.erase( tmpId );
//            LOG_INFO << "after tmpSecond not expired" << " usecount=" << tmpSecond.use_count();
//        }
        ctx->SetIdx(0);
    }

}

bool IOThreadImpl::init(int tid, IOThread * t) {
    m_iothread = t;
    if (m_newConns == NULL) {
        m_newConns = new conn_queue_t;
    }
    if (m_thread == NULL) {
        m_thread = new event_thread(tid);
    }
//    if (!create_notifypipe()) {
//        return false;
//    }
    if (!register_notifyevent()) {
        std::cout << "bad : " << __FUNCTION__ << ",bad:" << __LINE__ << "\n";
        return false;
    }
#if 1  // TODO(H.haiwei) 应该实现，用来释放内存
    if (!register_timerevent()) {
        std::cout << "bad : " << __FUNCTION__ << ",bad:" << __LINE__ << "\n";
        return false;
    }
#endif
    return true;
}

bool IOThreadImpl::register_notifyevent() {
    if (m_thread == nullptr) {
        return false;
    }

    auto tmpFuncPtr = boost::bind(&IOThreadImpl::thread_handle_newconnection, _1, _2, this/*_3*/, _4);
    notify_watcher_.reset(new PipeEventWatcher(m_thread->get_eventbase(), tmpFuncPtr));

    if ( !notify_watcher_->Init() || !notify_watcher_->AsyncWait() ) {
        LOG_ERROR << " notify watcher init failed!";
        return false;
    }

//    struct timeval tm;
//    evutil_timerclear(&tm);
//    tm.tv_sec = 1;

    short code = m_thread->set_eventcallback(&m_idle_time_event, -1,
                                             /*EV_READ | */EV_PERSIST,
                                             IOThreadImpl::onTimer, this, false, 1);
    if (code != (EV_PERSIST /*| EV_READ*/)) {
        LOG_ERROR << " timer watcher AsyncWait failed!";
        return false;
    }

//    ::event_set(&m_idle_time_event, -1, EV_PERSIST, IOThreadImpl::onTimer, this);
//    ::event_base_set(m_thread->get_eventbase(), &m_idle_time_event);
//    ::event_add(m_idle_time_event, &tm);

//    GlobalOutput.printf( "%s registered for notify.", (listenSocket_ >= 0 ? ("listen thread") : ("work iothread")) );

//    short code;
//    short code = m_thread->set_eventcallback(&m_notifyEvent, m_notifyRecvfd,
//                                             EV_READ | EV_PERSIST,
//                                             IOThreadImpl::thread_handle_newconnection, this);
//    if (code != (EV_PERSIST | EV_READ)) {
//        LOG_ERROR << " notify watcher AsyncWait failed!";
//        return false;
//    }

//    code = m_thread->set_eventcallback(&notificationPushEvent_,
//                                       notificationPushPipeFDs_[0],
//                                       EV_READ | EV_PERSIST,
//                                       IOThreadImpl::thread_notifypush_handler, this);
//    if (code != (EV_PERSIST | EV_READ)) {
//        return false;
//    }
    return true;
}

bool IOThreadImpl::register_timerevent() {
#if 0
    struct timeval val = {600, 0};

    ev_timer_ = event_new(m_pEventBase, -1, EV_PERSIST, event_timeout, NULL);

    m_isSetTimer = m_thread->set_timercallback(&m_timerEvent,
                                               IOThreadImpl::thread_check_timer,
                                               this, &val, m_isSetTimer);
    return m_isSetTimer;
#else
    struct timeval val = {600, 0};

    ev_timer_ = m_thread->set_forevertimer(&IOThreadImpl::thread_check_timer, this, &val);
    return ev_timer_ != NULL;
#endif
}

Item_t * IOThreadImpl::get_connitem() {
    if (m_newConns == NULL) {
        return NULL;
    }

    Item_t * item = NULL;
//    LOG_INFO << "get_connitem m_newConns=" << m_newConns;
//    LOG_INFO << "get_connitem m_newConns->m_connQueue.size=" << m_newConns->m_connQueue.size();

    muduo::MutexLockGuard tmp(m_newConns->m_newlock);
//    LOG_INFO << "lock beg";
    while (!m_newConns->m_connQueue.empty()) {
        item = m_newConns->m_connQueue.front();
        m_newConns->m_connQueue.pop_front();
        if (item != NULL) {
//            LOG_INFO << " get socket -ID- " << item->m_socket;
            break;
        }
    }
//    LOG_INFO << "lock end";
    return item;
}

bool IOThreadImpl::create_notifypipe() {
//    int fds[2];
//    if (pipe(fds) != 0) {
//        LOG_ERROR << " bad notify ";
//        return false;
//    }
//    m_notifyRecvfd = fds[0];
//    m_notifySendfd = fds[1];
//    if (evutil_socketpair(AF_LOCAL, SOCK_STREAM, 0, notificationPushPipeFDs_) == -1) {
//        LOG_ERROR << " bad ";
//        return false;
//    }

//    if (evutil_make_socket_nonblocking(notificationPushPipeFDs_[0]) < 0  ||
//            evutil_make_socket_nonblocking(notificationPushPipeFDs_[1] < 0)) {
//        close(notificationPushPipeFDs_[0]);
//        close(notificationPushPipeFDs_[1]);
//        notificationPushPipeFDs_[0] = INVALID_SOCKET;
//        notificationPushPipeFDs_[1] = INVALID_SOCKET;
//        LOG_ERROR << " bad nonblocking";
//        return false;
//    }

//    for (int i = 0; i < 2; ++i) {
//  #if LIBEVENT_VERSION_NUMBER < 0x02000000
//      int flags;
//      if ((flags = FCNTL(notificationPushPipeFDs_[i], F_GETFD, 0)) < 0
//          || FCNTL(notificationPushPipeFDs_[i], F_SETFD, flags | FD_CLOEXEC) < 0) {
//  #else
//      if (evutil_make_socket_closeonexec(notificationPushPipeFDs_[i]) < 0) {
//  #endif
//        close(notificationPushPipeFDs_[0]);
//        close(notificationPushPipeFDs_[1]);
//        notificationPushPipeFDs_[0] = INVALID_SOCKET;
//        notificationPushPipeFDs_[1] = INVALID_SOCKET;
//        LOG_ERROR << "bad closeonexec";
//        return false;
//      }
//    }

//    LOG_INFO << " notifyFD " << notificationPushPipeFDs_[0]
//             << " send " << notificationPushPipeFDs_[1]
//             << " id " << get_tid();
    return true;
}

void IOThreadImpl::check_timer() {
    monitor_idle_ctx();
}

ClientCtx * IOThreadImpl::get_clientctx() {
    ClientCtx * ctx = NULL;
    while(!m_idleCtxs.empty()) {
        ctx = m_idleCtxs.front();
        m_idleCtxs.pop();
        //LOG4CPLUS_DEBUG(gLogger, __FUNCTION__ << " size " << m_idleCtxs.size());
        if (ctx != NULL) {
            if (ctx->ctxIsInUse()) {
                LOG_ERROR << " ~~~~~~~~~ why ctx in use " << ctx;
                ctx = NULL;
                continue;
            }
            break;
        }
    }
    if (ctx == NULL) {
        ctx = new (std::nothrow) ClientCtx(m_iothread, m_typeProcess, m_protocol);
        if (ctx) {
            activeCtxnum_++;
            LOG_INFO << " new ctx=" << ctx << ",ctx totalNum=" << activeCtxnum_;
        }
    } else {
        LOG_INFO << " use old ctx:" << ctx << ",size:" << m_idleCtxs.size();
    }
    return ctx;
}

void IOThreadImpl::thread_handle_newconnection(int fd, short which, void *arg, void *user_data) {
    UNUSED(which);
    IOThreadImpl * impl = reinterpret_cast<IOThreadImpl*>(arg);
    if (impl == NULL || !user_data) {
        LOG_ERROR << " bad " << fd;
        return;
    }

    Item_t* item = NULL;
    char* puser = (char*)user_data;

    switch (puser[0]) {
        case 'c':
            item = impl->get_connitem();
            if (item == NULL) {
                LOG_ERROR << "item == nullptr";
                return;
            }
            LOG_INFO << "recv new connetction, fd=" << fd << " itemsocket=" << item->m_socket;
            if (!impl->create_connection(item)) {
                LOG_ERROR << ", close itemsocket=" << item->m_socket;
                close(item->m_socket);
            }
            delete_object(item);
            break;
        default:
            LOG_ERROR << "default unknown buf..";
            return;
    }
}

// 保存 clientctx 指针到容器，引用计数=2
void IOThreadImpl::SaveNewCtx(const ClientCtxPtr& in_sp) {
    idx_connection++;
    uint64_t uId = idx_connection;
    in_sp->SetIdx(uId);
    LOG_INFO << "new connection Idx=" << uId << " SaveNewCtx in_sp.use_count=" << in_sp.use_count();
    {
        MutexLockGuard lock(mtx_id_ctx);
        id_ctx_map_.insert( make_pair(uId, in_sp) );
    }
//    printf( "SaveNewCtx in_sp.use_count=%d\n", in_sp.use_count() );
}
bool IOThreadImpl::create_connection(Item_t * item) {
    ClientCtx * ctx = get_clientctx();
    if (ctx == NULL) {
        return false;
    }
    bool ret = true;
    if (!ctx->init(item)) {
        ret = false;
        activeCtxnum_--;
        LOG_ERROR << " init ctx error, ctx=" << ctx << ", activeNum=" << activeCtxnum_;
        delete_object(ctx);
    } else {
        ret == true;
//        boost::shared_ptr<ClientCtx> sp_ctx(ctx);
//        SaveNewCtx(sp_ctx);

//        EntryCtxPtr new_entry(new EntryCtx(sp_ctx));
//        connectionBuckets_.back().insert(new_entry);

//        dumpConnectionBuckets();

//        WeakEntryCtxPtr weak_new_entry(new_entry);
//        ctx->setContext(weak_new_entry);

//        printf( "after SaveNewCtx sp_ctx.use_count=%d\n", sp_ctx.use_count() );
    }
    return ret;
}
void IOThreadImpl::AddOneCtx(const ClientCtx* pCtx/*const boost::shared_ptr<ClientCtx>& oneCtx*/) {
    LOG_INFO << "add one ctx, pctx=" << pCtx;
//    assert(!conn->getContext().empty());
    WeakEntryCtxPtr weakEntry(boost::any_cast<WeakEntryCtxPtr>(pCtx->getContext()));
    EntryCtxPtr one_entry(weakEntry.lock());
    if (one_entry)
    {
        connectionBuckets_.back().insert(one_entry);
        dumpConnectionBuckets();
    }
}
void IOThreadImpl::onTimer(int fd, short which, void *arg)
{
    IOThreadImpl* pThis = (IOThreadImpl*)arg;
    pThis->connectionBuckets_.push_back(Bucket());
    pThis->dumpConnectionBuckets();
}

void IOThreadImpl::pushMessage(int64_t sessionID) {
    if (sessionID <= 0)
        return;
//    boost::unordered_map<int64_t, ClientCtx*>::iterator it = pushConnections_.find(sessionID);
//    if (it == pushConnections_.end()) {
//        LOG_ERROR << " can't find sid " << sessionID;
//        return;
//    }
//    ClientCtx * ctx = it->second;
//    if (ctx == NULL) {
//        pushConnections_.erase(sessionID);
//        return;
//    }
//    ctx->pushMessage(sessionID);
}

void IOThreadImpl::thread_check_timer(int /*fd*/, short /*which*/, void *arg) {
    IOThreadImpl * impl = reinterpret_cast<IOThreadImpl*>(arg);
    if (impl == NULL) {
        return;
    }
    impl->check_timer();

#if 0
    struct timeval val = {600, 0};
    bool isSetTimer = impl->get_thread()->set_timercallback(impl->get_timeEvent(),
                                   IOThreadImpl::thread_check_timer,
                                   arg, &val, impl->get_isSetTimer());
    impl->set_isSetTimer(isSetTimer);
#endif
}

void IOThreadImpl::thread_notifypush_handler(int fd, short /*which*/, void *arg) {
    IOThreadImpl * impl = reinterpret_cast<IOThreadImpl*>(arg);
    if (impl == NULL) {
        return;
    }

    const int kSize = sizeof(int64_t);
    ClientNotifyInfo info;
    int nBytes = recv(fd, info.buf, kSize, 0);
    if (nBytes != kSize) {
        return;
    }
    LOG_INFO << " sid " << info.sessionID;
    impl->pushMessage(info.sessionID);
}

void IOThreadImpl::setPushConnection(int64_t id, ClientCtx *ctx) {
    if (id <= 0 || ctx == NULL)
        return;
    LOG_INFO << " set session id " << id << " ctx " << ctx;
    pushConnections_[id] = ctx;
}

void IOThreadImpl::deletePushConnection(int64_t id, ClientCtx * ctx) {
    boost::unordered_map<int64_t, ClientCtx*>::iterator it = pushConnections_.find(id);
    if (it != pushConnections_.end() && it->second == ctx) {
        pushConnections_.erase(id);
    }
}

void IOThreadImpl::monitor_idle_ctx() {
    LOG_INFO << ",start--> idleCtx number=" << m_idleCtxs.size()
             << ",activeCtx number=" << activeCtxnum_;

    //if (m_idleCtxs.size() > IDLE_CTX_REMOVE) {
    if (!m_idleCtxs.empty()) {
        ClientCtx *tmp = NULL;
        time_t curr = time(NULL);
#if 0
        for (int i = 0 ; i < IDLE_CTX_REMOVE; i++) { //remove IDLE_CTX_REMOVE max
            tmp =  m_idleCtxs.front();

            if (tmp == NULL) {//error
                m_idleCtxs.pop();
                LOG4CPLUS_ERROR(gLogger, __FUNCTION__<<" tmp==NULL");
                continue;
            }

            if ((curr - tmp->get_idle_time()) >= IDLE_CTX_TIMEOUT) {
                m_idleCtxs.pop();
                activeCtxnum_--;

                LOG4CPLUS_INFO(gLogger, __FUNCTION__<<", remove ctx=" << tmp
                               << ",activeNum=" << activeCtxnum_);
                delete(tmp);
                tmp = NULL;
            } else {
                tmp = NULL;
                break;
            }
        }
#endif
        while (!m_idleCtxs.empty()) {
            tmp = m_idleCtxs.front();
            if (tmp == NULL) {
                m_idleCtxs.pop();
                continue;
            }
            if (labs(curr - tmp->get_idle_time()) >= IDLE_CTX_TIMEOUT) {
                m_idleCtxs.pop();
                delete_object(tmp);
                tmp = NULL;
                activeCtxnum_--;
            } else {
                LOG_INFO << ",middle--> currentTime=" << (int64_t)curr
                         << ",ctxTime=" << (int64_t)tmp->get_idle_time();
                tmp = NULL;
                break;
            }
        }
        LOG_INFO << ",end--> idleCtx number=" << m_idleCtxs.size()
                 << ",activeCtx number=" << activeCtxnum_;
    }

}


void IOThreadImpl::dumpConnectionBuckets() const
{
    LOG_INFO << "";
    LOG_INFO << "dump Connection Buckets beg";
    LOG_INFO << "size = " << connectionBuckets_.size();
    int idx = 0;
    for (WeakConnectionList::const_iterator bucketI = connectionBuckets_.begin();
         bucketI != connectionBuckets_.end();
         ++bucketI, ++idx)
    {
        const Bucket& bucket = *bucketI;
        printf("[%d] len = %zd : ", idx, bucket.size());
        for (const auto& it : bucket)
        {
            bool connectionDead = it->wk_client_ctx_.expired();
            printf("%p(%ld)%s, ", boost::get_pointer(it), it.use_count(), connectionDead ? " DEAD" : "");
        }
        puts("");
    }
    LOG_INFO << "dump Connection Buckets end";
    LOG_INFO << "";
}


/****************************************************/
/// IOThread
/****************************************************/

void IOThread::AddOneCtx(const ClientCtx* pCtx/*const boost::shared_ptr<ClientCtx>& oneCtx*/) {
    m_impl->AddOneCtx(pCtx/*oneCtx*/);
}

IOThread::IOThread(int id, TypeProcess tp, TypeProtocol pro) : m_impl(NULL) {
    m_impl = new IOThreadImpl(id, tp, pro);
}

IOThread::~IOThread() {
    delete_object(m_impl);
}

bool IOThread::set_item(Item_t * item) {
    if (m_impl) {
        return m_impl->set_item(item);
    }
    return false;
}

int IOThread::get_notifysendfd() {
    return m_impl->get_notifysendfd();
}

int IOThread::get_tid() const {
    return m_impl->get_tid();
}

bool IOThread::start() {
    if (m_impl) {
        return m_impl->start(this);
    }
    return false;
}

void IOThread::return_clientctx(ClientCtx* ctx) {
    if (m_impl) {
        m_impl->return_clientctx(ctx);
    }
}

void IOThread::setPushConnection(int64_t id, ClientCtx *ctx) {
    if (m_impl) {
        m_impl->setPushConnection(id, ctx);
    }
}

void IOThread::deletePushConnection(int64_t id, ClientCtx * ctx) {
    if (m_impl) {
        m_impl->deletePushConnection(id, ctx);
    }
}

TProtocolFactoryPtr IOThread::getInputProtocolFactory() {
    return m_impl->getInputProtocolFactory();
}

TProtocolFactoryPtr IOThread::getOutputProtocolFactory() {
    return m_impl->getOutputProtocolFactory();
}

evutil_socket_t IOThread::getNotificationPushSendFD() const {
     return m_impl->getNotificationPushSendFD();
}

evutil_socket_t IOThread::getNotificationPushRecvFD() const {
    return m_impl->getNotificationPushRecvFD();
}

event_thread * IOThread::get_thread() {
    if (m_impl) {
        return m_impl->get_thread();
    }
    return NULL;
}
