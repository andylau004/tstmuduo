


// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/CurrentThread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TimerId.h>

namespace muduo
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

///
/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.

/*

           class EventLoop: 事件循环。先看一个调用链：EventLoop::loop()->Poller::poll()通过此调用链获得一个vector<Channel*> activeChannels_的就绪事件集合，再遍历该容器，执行每个Channel的Channel::handleEvent()完成相应就绪事件回调。至此一个完整的Reactor模式即完成。注意这里的Reactor遵循one
 loop per thread即所在一个线程中完成，故并没有涉及到线程同步机制，以后可能会有其它线程调用这三个类，则通过线程转移函数将一些操作转移到一个线程中完成。(若一个函数既可能加锁情况下使用有可能在未加锁情况下使用，那么就拆成两个函数，需要加锁的函数去调用不需要加锁的函数。线程转移实现就可以通过两个函数实现，如：假设类one隶属于线程B，线程A调用one的方法fun，fun向one注册一个回调，从而将具体操作转移到one的所属线程B中去执行。)
*/
class EventLoop : boost::noncopyable
{
public:
    typedef boost::function<void()> Functor;

    EventLoop();
    ~EventLoop();  // force out-line dtor, for scoped_ptr members.

    ///
    /// Loops forever.
    ///
    /// Must be called in the same thread as creation of the object.
    ///
    void loop();

    /// Quits loop.
    ///
    /// This is not 100% thread safe, if you call through a raw pointer,
    /// better to call through shared_ptr<EventLoop> for 100% safety.
    void quit();

    ///
    /// Time when poll returns, usually means data arrival.
    ///
    Timestamp pollReturnTime() const { return pollReturnTime_; }

    int64_t iteration() const { return iteration_; }

    /// Runs callback immediately in the loop thread.
    /// It wakes up the loop, and run the cb.
    /// If in the same loop thread, cb is run within the function.
    /// Safe to call from other threads.
    void runInLoop(const Functor& cb);
    /// Queues callback in the loop thread.
    /// Runs after finish pooling.
    /// Safe to call from other threads.
    void queueInLoop(const Functor& cb);

    size_t queueSize() const;

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    void runInLoop(Functor&& cb);
    void queueInLoop(Functor&& cb);
#endif

    // timers

    ///
    /// Runs callback at 'time'.
    /// Safe to call from other threads.
    ///
    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    ///
    /// Runs callback after @c delay seconds.
    /// Safe to call from other threads.
    ///
    TimerId runAfter(double delay, const TimerCallback& cb);
    ///
    /// Runs callback every @c interval seconds.
    /// Safe to call from other threads.
    ///
    TimerId runEvery(double interval, const TimerCallback& cb);
    ///
    /// Cancels the timer.
    /// Safe to call from other threads.
    ///
    void cancel(TimerId timerId);

#ifdef __GXX_EXPERIMENTAL_CXX0X__
//    TimerId runAt(const Timestamp& time, TimerCallback&& cb);
//    TimerId runAfter(double delay, TimerCallback&& cb);
//    TimerId runEvery(double interval, TimerCallback&& cb);
#endif

    // internal usage
    void wakeup();
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
    bool hasChannel(Channel* channel);

    // pid_t threadId() const { return threadId_; }
    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    pid_t IoEventLoopTid() { return threadId_; }

    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
    // bool callingPendingFunctors() const { return callingPendingFunctors_; }
    bool eventHandling() const { return eventHandling_; }

    void setContext(const boost::any& context)
    { context_ = context; }

    const boost::any& getContext() const
    { return context_; }

    boost::any* getMutableContext()
    { return &context_; }

    static EventLoop* getEventLoopOfCurrentThread();

private:
    void abortNotInLoopThread();
    void handleRead();  // waked up
    void doPendingFunctors();

    void printActiveChannels() const; // DEBUG

    typedef std::vector<Channel*> ChannelList;

    bool looping_; /* atomic */
    bool quit_; /* atomic and shared between threads, okay on x86, I guess. */
    bool eventHandling_; /* atomic */
    bool callingPendingFunctors_; /* atomic */
    int64_t iteration_;
    const pid_t threadId_;
    Timestamp pollReturnTime_;
    boost::scoped_ptr<Poller> poller_;
    boost::scoped_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    // unlike in TimerQueue, which is an internal class,
    // we don't expose Channel to client.
    boost::scoped_ptr<Channel> wakeupChannel_;//wakeupfd所对应的通道，该通道会纳入到poller来管理
    boost::any context_;

    // scratch variables
    ChannelList activeChannels_; //Poller返回的活动通道，vector<channel*>类型
    Channel* currentActiveChannel_; //当前正在处理的活动通道

    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_; // @GuardedBy mutex_
};

}
}
#endif  // MUDUO_NET_EVENTLOOP_H





