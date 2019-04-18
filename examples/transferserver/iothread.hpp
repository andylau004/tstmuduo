



#pragma once



#include <stdio.h>
#include <event.h>

#include <thrift/protocol/TProtocol.h>


#include "muduo/base/common.h"
#include "common_stucts.hpp"


#include "muduo/base/Logging.h"


using namespace apache::thrift;



typedef boost::shared_ptr<protocol::TProtocolFactory> TProtocolFactoryPtr;


class IOThreadImpl;
//class Thread;
class event_thread;
class ClientCtx;
class IOThread {
public:
    IOThread(int id, TypeProcess tp, TypeProtocol pro);
    ~IOThread();

    bool set_item(Item_t * item);
    int  get_notifysendfd();
    int  get_tid() const;
    bool start();

    void return_clientctx(ClientCtx* ctx);
    void setPushConnection(int64_t id, ClientCtx * ctx);
    void deletePushConnection(int64_t id, ClientCtx * ctx);

    TProtocolFactoryPtr getInputProtocolFactory();
    TProtocolFactoryPtr getOutputProtocolFactory();

    evutil_socket_t getNotificationPushSendFD() const;
    evutil_socket_t getNotificationPushRecvFD() const;

    event_thread * get_thread();

public:
    void  AddOneCtx(const ClientCtx* pCtx/*const boost::shared_ptr<ClientCtx>& oneCtx*/);

private:
    IOThreadImpl * m_impl;
};


