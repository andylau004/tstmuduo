
#pragma once


#include <functional>
#include <list>
#include <thread>
#include <mutex>

#include <boost/function.hpp>


typedef struct TimePos_
{
    int ms_pos;
    int s_pos;
    int min_pos;
}TimePos;

typedef struct EventInfo_
{
    int interval;
    boost::function<void(void)> call_back;
    TimePos time_pos;
    int timer_id;

}EventInfo;


/* ----------------------------------------------------------
 *
 *  这里实现的是一个毫秒到分钟级别的三成时间轮定时器。InitTimerWheel 中有两个参数，第一个表示支持的最小时间粒度单位毫秒，第二个参数是支持的最大分钟级别。

时钟原理说明：

1.1. 初始化一个三层时间轮:毫秒刻盘：1000/step_ms 个MSList, 秒刻盘：60个SList, 时刻盘：max_min个MinList;

1.2. MSTick由外界推动,每跳一轮(1000/step_ms格),MSTick复位至0,同时STick跳1格;

1.3. 同理STick每跳一轮(60格),STick复位至0,同时MinTick跳1格;

1.4. 最高层：MinTick跳一轮(max_min格）,MinTick复位至0，一个时间轮完整周期完成.

2.事件原理说明：

2.1. 设置时间为TimeOut的事件时,根据TimeOut算出发生此事件时刻的指针位置{TriggerMin,TriggerS,TriggerMS};

2.2. 用{TriggerMin,TriggerS,TriggerMS}与当前指针{NowMin,NowS,NowMS}对比得出事件存放在哪一个指针(Tick);

2.3. 所有层的指针每跳到下一格(Tick01)都会触发格子的事件列表,处理每一个事件Event01：

2.3.1 根据事件Event01的剩余TimeOut算出Event01应该存在上一层(跳得更快)层的位置Pos;

2.3.2 把事件更新到新的Pos(更新TimeOut);

2.3.3 重复处理完Tick01里面所有的事件;

2.3.4 清空Tick01的事件;

2.3.5 最底层(跳最快)层所有的事件遇到指针Tick都会立即执行;

需要指出的是，这里和我所贴的博客中的实现是有点不同的，它所叙述的是一个时分秒级别的定时器，但是我们这里进行了降级，实现的是一个 毫秒，秒，分钟级别的定时器。
因为个人感觉，这种级别的定时器使用的概率会更大一些
 * */

class TimeWheel
{
public:
    TimeWheel();
    ~TimeWheel();
public:
    /*step 以毫秒为单位，表示定时器最小时间粒度
     *max_timer 表示定时器所能接受的分钟时间间隔
     */
    int InitTimerWheel(int step,int max_min);
    int AddTimer(int interval, boost::function<void(void)>& call_back);
    int DeleteTimer(int timer_id);

private:
    int DoLoop();
    int GenerateTimerID();
    int InsertTimer(int diff_ms,EventInfo& einfo);
    int GetNextTrigerPos(int interval,TimePos& time_pos);
    int GetMS(TimePos time_pos);
    int DealTimeWheeling(std::list<EventInfo> leinfo);
private:
    std::list<EventInfo> *_pCallbackList = nullptr;
    std::mutex _mutex;

    TimePos _time_pos;

    int _lowCount = 0;
    int _midCount = 0;
    int _highCount = 0;

    int _step_ms = 0;

    int _timer_count = 0;

};
