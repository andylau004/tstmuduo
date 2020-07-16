
#pragma once





#include "muduo/base/LogFile.h"
#include "muduo/base/FileUtil.h"
#include "muduo/base/ProcessInfo.h"

#include "muduo/base/BlockingQueue.h"
#include "muduo/base/BoundedBlockingQueue.h"
#include "muduo/base/CountDownLatch.h"
#include "muduo/base/Thread.h"

#include <string>

#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>



template < typename MSG, template < typename > class QUEUE >
class AsyncLoggingT : boost::noncopyable {

public:
    AsyncLoggingT(const std::string& basename, size_t rollSize)
        : running_(false),
          basename_(basename),
          rollSize_(rollSize),
          thread_(boost::bind(&AsyncLoggingT::threadFunc, this), "Logging"),
          latch_(1)
    {
    }
    ~AsyncLoggingT() {
        if (running_) {
            stop();
        }
    }
    void append(const char* logline, size_t len) {
        queue_.put(MSG(logline, len));
    }
    void start() {
        // 在构造函数中latch_的值为1
        // 线程运行之后将latch_的减为0
        running_ = true;
        thread_.start();
        latch_.wait();// 必须等到latch_变为0才能从start函数中返回，这表明初始化已经完成
    }
    void stop() {
        running_ = false;
        queue_.put(MSG());
        thread_.join();
    }
private:
    void threadFunc() {
        latch_.countDown();

        muduo::LogFile output(basename_, rollSize_, false);
        time_t lastFlush = time(nullptr);

        while (1) {
            MSG msg(queue_.take());
            if (msg.empty())
            {
                assert(running_ == false);
                break;
            }
            output.append(msg.data(), msg.length());
        }
        output.flush();
    }

private:
    bool running_;
    std::string basename_;
    size_t rollSize_;
    muduo::Thread thread_;
    muduo::CountDownLatch latch_;
    QUEUE<MSG> queue_;
};

typedef AsyncLoggingT<std::string, muduo::BlockingQueue> AsyncLoggingUnboundedQueue;
typedef AsyncLoggingT<std::string, muduo::BoundedBlockingQueue> AsyncLoggingBoundedQueue;



