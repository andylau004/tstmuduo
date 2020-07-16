// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_ASYNCLOGGING_H
#define MUDUO_BASE_ASYNCLOGGING_H

#include <atomic>
#include <vector>

#include "muduo/base/BlockingQueue.h"
#include "muduo/base/BoundedBlockingQueue.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"
#include "muduo/base/LogStream.h"


namespace muduo
{

class AsyncLogging : noncopyable
{
public:

    AsyncLogging(const string& basename,
                 off_t rollSize,
                 int flushInterval = 3);

    ~AsyncLogging()
    {
        if (running_)
        {
            stop();
        }
    }

    void append(const char* logline, int len);

    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() NO_THREAD_SAFETY_ANALYSIS
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:

    void threadFunc();

    typedef muduo::detail::FixedBuffer<muduo::detail::kLargeBuffer> Buffer;
    // 用unique_ptr管理buffer，持有对对象的独有权，不能进行复制操作只能进行移动操作（效率更高）
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;// 指向buffer的指针

    const int flushInterval_;// 定期（flushInterval_秒）将缓冲区的数据写到文件中
    std::atomic<bool> running_;
    const string basename_;
    const off_t rollSize_;
    muduo::Thread thread_;
    muduo::CountDownLatch latch_;// 倒计时计数器初始化为1，用于指示什么时候日志记录器才能开始正常工作
    muduo::MutexLock mutex_;
    muduo::Condition cond_ GUARDED_BY(mutex_);
    BufferPtr currentBuffer_ GUARDED_BY(mutex_);//当前缓冲区   4M大小
    BufferPtr nextBuffer_ GUARDED_BY(mutex_);//预备缓冲区，主要是在调用append向当前缓冲添加日志消息时，如果当前缓冲放不下，当前缓冲就会被移动到前端缓冲队列中国，此时预备缓冲区用来作为新的当前缓冲
    BufferVector buffers_ GUARDED_BY(mutex_);//缓冲区队列
};

}  // namespace muduo

#endif  // MUDUO_BASE_ASYNCLOGGING_H
