
#pragma once



#include <boost/unordered_map.hpp>

#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;

using namespace boost;


class ClientCtx;

class CCliCtxMgr {

public:
    CCliCtxMgr();
    ~CCliCtxMgr();

public:


private:
    boost::unordered_map< uint64_t, boost::shared_ptr< ClientCtx > > id_ctx_map_;

};

