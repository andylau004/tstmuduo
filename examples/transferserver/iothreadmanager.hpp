

#pragma once


#include <stdio.h>
#include <vector>

#include "muduo/base/common.h"
#include "common_stucts.hpp"



class IOThread;

class IOThreadManager {
public:
    IOThreadManager(int iothread = 5);
    ~IOThreadManager();

    bool start(TypeProcess tp, TypeProtocol proc);
    Item_t * get_item();
    bool dispatch_item(Item_t * item);
private:
    void destroy();
private:
    int m_nthreads;
    int m_nextthreads;
    std::vector<IOThread*> m_threads;
};
