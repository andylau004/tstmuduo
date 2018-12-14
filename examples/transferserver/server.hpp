


#pragma once

#include <stdio.h>
#include "muduo/base/common.h"



class ServerImpl;
class Server {
public:
    Server(TypeProcess tp, TypeProtocol pro, int port, int nthread = 0);
    ~Server();

    bool serve();
private:
    bool init(TypeProcess tp, TypeProtocol pro, int port, int nthread);
    ServerImpl * m_impl;
};




