// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include <boost/noncopyable.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Channel.h>

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///

/*---------------------------------------------------------------------------------------------*
timers_和activeTimers_保存的是相同的数据，timers_是按到期时间排序，activeTimers_按照对象地址排序，并且timerQueue只关注最早
的那个定时器，所以当发生可读事件的时候，需要使用getExpired()获取所有的超时事件，因为可能有同一时刻的多个定时器。
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*---------------------------------------------------------------------------------------------*
 * 分析: TimerQueue 中有多个定时器，一次性的和重复的，事件循环开始 EventLoop::loop()                     *
 *   当最早到期定时器超时，poll() 返回timerfd_ 的可读事件(timerfdChannel_)                       *
 *   调用 Channel::handleEvent(),调用readCallback_(receiveTime);                                  *
 *   进而调用 Channel::setReadCallback 注册的TimerQueue::handleRead(), 在函数内先 read 掉 timerfd_ 数据，                     *
 *   避免一直触发可读事件，接着遍历 TimerQueue 中此时所有超时的定时器，调用每个定时器构造时传递的回调函数。                     *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
class TimerQueue : boost::noncopyable
{
public:
    explicit TimerQueue(EventLoop* loop);
    ~TimerQueue();

    ///
    /// Schedules the callback to be run at given time,
    /// repeats if @c interval > 0.0.
    ///
    /// Must be thread safe. Usually be called from other threads.
    TimerId addTimer(const TimerCallback& cb,
                     Timestamp when,
                     double interval);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    TimerId addTimer(TimerCallback&& cb,
                     Timestamp when,
                     double interval);
#endif

    void cancel(TimerId timerId);

private:

    // FIXME: use unique_ptr<Timer> instead of raw pointers.
    // This requires heterogeneous comparison lookup (N3465) from C++14
    // so that we can find an T* in a set<unique_ptr<T>>.
    // set < pair<TimeStamp,Timer*> >，采用 pair 为 key 的原因: 在一个时刻有多个相同的 Timestamp 时间戳超时，
    // 而查找只返回一个，这样即使两个 Timer 的超时时间相同，它们的地址也必须不同。
    typedef std::pair<Timestamp, Timer*> Entry;    //对应一个定时任务
    typedef std::set<Entry>              TimerSet; //定时任务集合，采用set,有key无value，且有序

    typedef std::pair<Timer*, int64_t>   ActiveTimer;
    typedef std::set<ActiveTimer>        ActiveTimerSet;

    // 以下成员函数只可能在其所属的I/O线程中调用，因而不必加锁
    // 服务器性能杀手之一是锁竞争，所以要尽可能少用锁
    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);

    // called when timerfd alarms
    void handleRead(); //timerfd 可读 的回调

    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);

    bool insert(Timer* timer);

    EventLoop* loop_;
    // 通过给 timerfd 一个超时时间实现超时计时，
    // 它内部有 Channel，通过 Channel 管理 timerfd，
    // 然后向EventLoop和Poller注册 timerfd 的可读事件，
    // 当 timerfd 的可读事件就绪时表明一个超时时间点到了，
    // 然后调用 timerfdChannel_ 的可读事件回调 handleRead()，
    // 通过 getExpired() 找出所有的超时事件，然后执行相应的超时回调函数 Timer::run()。
    // 为了复用定时器，每次处理完之后，会检查这些超时定时器是否需要重复定时，如果需要重复，就再次添加到定时器集合中
    const int timerfd_;
    Channel timerfdChannel_; //timerfd 对应的Channel，借此来观察timerfd_ 上的readable事件
    // Timer list sorted by expiration
    TimerSet timers_;

    // for cancel()
    // timers_与activeTimers_保存的是相同的数据
    // timers_是按到期时间排序，activeTimers_是按对象地址排序
    ActiveTimerSet activeTimers_;
    bool callingExpiredTimers_; /* atomic *///是否处于 处理定时器超时回调中
    ActiveTimerSet cancelingTimers_;//保存被注销的定时器
};

}
}
#endif  // MUDUO_NET_TIMERQUEUE_H
