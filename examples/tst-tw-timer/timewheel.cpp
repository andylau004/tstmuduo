

#include "timewheel.h"




#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>





using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;



#include <cstdio>
#include <cstdlib>
#include <functional>


using namespace std;
using namespace muduo;
using namespace muduo::net;


#include "timewheel.h"


// need referance it
// https://blog.csdn.net/jasonliuvip/article/details/23888851

time_wheel::time_wheel()
    : cur_slot(0)
{
    for (int i = 0 ; i < N; i ++ ) {
        m_slots[i] = nullptr;
    }
}

//遍历每个槽，并销毁定时器
time_wheel::~time_wheel() {
    for (int i = 0 ; i < N; i ++ ) {

        tw_timer* ptmp = m_slots[i];

        while (ptmp) {
            m_slots[i] = ptmp->next;
            delete_object(ptmp);
            ptmp = m_slots[i];
        }

    }
}

tw_timer* time_wheel::add_timer(int timeout) {
    if (timeout < 0) { return NULL;}

    char tmpbuff[1024] = {0};
    int  ticks         = 0;
    if (timeout < TI) {
        ticks = 1;
    } else {
        ticks = timeout / TI;
    }

    int rotation = ticks / N;   //计算定时器在时间轮上要转动多少圈后触发
    int ts = (cur_slot + ticks);//计算待持入定时器应该被插入的位置

    //创建定时器，它在时间轮转动rotation圈之后触发，且位于第ts个槽上
    tw_timer* newTimer = new tw_timer(rotation, ts);

    //如果槽为空，则它新定时器插入，并设置为该槽的头节点
    if (!m_slots[ts]) {
        sprintf(tmpbuff, "000000 add timer, rotation is %d, ts is %d, cur_slot is %d",
                rotation, ts, cur_slot);
        LOG_INFO << tmpbuff;
        m_slots[ts] = newTimer;
        LOG_INFO << "addr1 = " << &m_slots[ts];
    } else {
        sprintf(tmpbuff, "111111 add timer, rotation is %d, ts is %d, cur_slot is %d",
                rotation, ts, cur_slot);
        newTimer->next = m_slots[ts];
        m_slots[ts]->prev = newTimer;
        m_slots[ts] = newTimer;
    }
    return newTimer;
}
void time_wheel::tick() {
    tw_timer* ptmp = m_slots[cur_slot];
//    LOG_INFO << "current slot=" << cur_slot << " ptmp=" << ptmp << " addr2=" << &m_slots[cur_slot];

    while( ptmp )
    {
        LOG_INFO << "tick the timer once, ptmp=" << ptmp;
        if ( ptmp->rotation > 0 ) {
            ptmp->rotation --;// 如果 有循环数, 先减一, 等下一轮
            ptmp = ptmp->next;
        } else {
            // 如果定时器队列指针不为空,则说明该队列中的定时器都到时,需要触发执行
            // 如果ptmp为空,说明,该槽空间内没有需要执行的定时器对象
            ptmp->cb_func( ptmp->user_data );

            if ( ptmp == m_slots[cur_slot] )
            {
                LOG_INFO << "delete header in cur_slot";
                m_slots[cur_slot] = ptmp->next;
                delete_object(ptmp);

                if ( m_slots[cur_slot] ) {
                    m_slots[cur_slot]->prev = NULL;
                }
                ptmp = m_slots[cur_slot];
            }
            else
            {
                ptmp->prev->next = ptmp->next;
                if ( ptmp->next ) {
                    ptmp->next->prev = ptmp->prev;
                }
                tw_timer* tmp2 = ptmp->next;
                delete ptmp;
                ptmp = tmp2;
            }
        }
    }

    cur_slot = (++cur_slot) % N;
//    LOG_INFO << "cur_slot calc afer=" << cur_slot;
}
void time_wheel::del_timer(tw_timer *del_timer) {
    if (!del_timer) return;

    int ts = del_timer->time_solt;
    if (del_timer == m_slots[ts]) {
        m_slots[ts] = m_slots[ts]->next;

        if (m_slots[ts])
            m_slots[ts]->prev = nullptr;
    } else {
        del_timer->prev->next = del_timer->next;
        if (del_timer->next)
            del_timer->next->prev = del_timer->prev;
    }

    delete_object(del_timer);
}














