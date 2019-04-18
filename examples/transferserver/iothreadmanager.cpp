

#include "iothreadmanager.hpp"

#include <unistd.h>
#include <iostream>

#include "iothread.hpp"

#include "muduo/base/Logging.h"

//#include "logs/log.h"

//extern log4cplus::Logger gLogger;



IOThreadManager::IOThreadManager(int iothread) :
    m_nthreads(iothread),
    m_nextthreads(0) {

}

IOThreadManager::~IOThreadManager() {
    destroy();
}

bool IOThreadManager::start(TypeProcess tp, TypeProtocol proc) {
    //if (m_nthreads <= 0 || m_nthreads > MAX_THREAD_COUNT) {
    //    m_nthreads = MAX_THREAD_COUNT;
    //}
    int i = 1;
    int j = 0;
    while (j++ < m_nthreads) {
        IOThread * t = new IOThread(i, tp, proc);
        if (t->start()) {
            ++i;
            m_threads.push_back(t);
        } else {
            delete t;
        }
    }
    m_nthreads = (int)m_threads.size();
    return m_nthreads > 0;
}

// 大量连接的情况下，不确定是否会造成大量的内存碎片，可能这里重复利用比较好
Item_t * IOThreadManager::get_item() {
    Item_t * item = new Item_t;
    return item;
}


// 这里没做安全的检查和控制
// 1. 异常 失败 检查
// 2. io线程饱和检查
bool IOThreadManager::dispatch_item(Item_t * item) {
    if (item == NULL || m_nthreads <= 0) {
        return false;
    }

    int selected = m_nextthreads;
    m_nextthreads = (m_nextthreads + 1) % m_nthreads;

    IOThread * t = m_threads[selected];
    LOG_INFO << "dispatch io_tid=" << t->get_tid() << ", socket=" << item->m_socket;
    if (t == NULL) {
        return false;
    }
    if (!t->set_item(item)) {
        LOG_ERROR << "set_item failed!!!";
        return false;
    }
    return true;
//    char buf[1];
//    buf[0] = 'c';
//    // 这里失败的时候，理论上是应该删除item的(也可能不需要)请慎重思考
//    if (write(t->get_notifysendfd(), buf, 1) != 1) {
//        // 是否应当删除item;
//        LOG_ERROR << " send fd error " << t->get_tid() << " socket " << item->m_socket;
//        return false;
//    }
//    return true;
}

void IOThreadManager::destroy() {
    for (size_t i = 0; i < m_threads.size(); ++i) {
        IOThread * t = m_threads[i];
        delete_object(t);
    }
}
