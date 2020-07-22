// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/base/AsyncLogging.h"
#include "muduo/base/LogFile.h"
#include "muduo/base/Timestamp.h"

#include <stdio.h>

using namespace muduo;

AsyncLogging::AsyncLogging(const string& basename,
                           off_t rollSize,
                           int flushInterval)
    : flushInterval_(flushInterval),
      running_(false),
      basename_(basename),
      rollSize_(rollSize),
      thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
      latch_(1),
      mutex_(),
      cond_(mutex_),
      currentBuffer_(new Buffer),
      nextBuffer_(new Buffer),
      buffers_()
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);// vector预定大小，避免自动增长（效率更高）
}

/*---------------------------------------------------------------------------------------------*
 *                                  Description
 * 前端在生成一条日志消息时，会调用AsyncLogging::append()。
 * 如果currentBuffer_够用，就把日志内容写入到currentBuffer_中，
 * 如果不够用(就认为其满了)，就把currentBuffer_放到已满buffer数组中，
 * 等待消费者线程（即后台线程）来取。则将预备好的另一块缓冲 (nextBuffer_) 移用为当前缓冲区 (currentBuffer_)
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void AsyncLogging::append(const char* logline, int len)
{
    muduo::MutexLockGuard lock(mutex_);
    // 如果当前buffer长度大于要添加日志记录的长度，即当前buffer还有空间，就添加到当前日志。
    if (currentBuffer_->avail() > len)
    {
        currentBuffer_->append(logline, len);
    }
    else// 当前buffer已满
    {
        buffers_.push_back(std::move(currentBuffer_));

        if (nextBuffer_)//如果备用缓冲区不为空，则将预备好的备用缓冲区移用为当前缓冲区
        {
            currentBuffer_ = std::move(nextBuffer_);
        }
        else
        {   //如果前端写入速度太快了，一下子把两块缓冲都用完了，那么只好分配一块新的buffer，作当前缓冲区
            currentBuffer_.reset(new Buffer); // Rarely happens
        }
        currentBuffer_->append(logline, len);
        cond_.notify();//通知后台线程，已经有一个满了的buffer了
    }
}

void AsyncLogging::threadFunc()
{
    assert(running_ == true);
    latch_.countDown();
    LogFile output(basename_, rollSize_, false);
    // BufferPtr 属于 unique_ptr 只能使用 std::move 移动
    BufferPtr newBuffer1(new Buffer);//用来填充移动后的currentBuffer_
    BufferPtr newBuffer2(new Buffer);//用来填充使用后的nextBuffer_
    newBuffer1->bzero();
    newBuffer2->bzero();
    BufferVector buffersToWrite;//要写入 LogFile 的 buffer vector
    buffersToWrite.reserve(16);
    while (running_)
    {
        // buffersToWrite 必须是空的,newBuffer1、newBuffer2必须是空的并且可用
        assert(newBuffer1 && newBuffer1->length() == 0);
        assert(newBuffer2 && newBuffer2->length() == 0);
        assert(buffersToWrite.empty());

        {
            muduo::MutexLockGuard lock(mutex_);
            if (buffers_.empty())  // unusual usage! 主程序中没有写入日志
            {
                // 如果 buffers_ 为空，那么表示没有数据需要写入文件，那么就等待指定的时间
                cond_.waitForSeconds(flushInterval_);//睡眠的时间是日志库flush时间
            }

            //将主程序中正在使用的 currentBuffer_ 存入 buffers_
            //如果因为时间到而醒, 那么currentBuffer_还没满, 也要将之写入LogFile中。
            //如果已经有一个前台buffer满了, 那么在前台线程中就已经把一个前台buffer放到buffers_中
            //此时, 还需要把currentBuffer_放到buffers_中(注意: 前后放置是不同的buffer,
            //因为在前台线程中, currentBuffer_已经被换成nextBuffer_指向的buffer了)
            buffers_.push_back(std::move(currentBuffer_)); //将主程序中正在使用的 currentBuffer_ 存入 buffers_
            currentBuffer_ = std::move(newBuffer1);// 使用 newBuffer1 替换正在使用的 currentBuffer_
            // 使用新的未使用 buffersToWrite 交换 buffers_, 将 buffers_ 中的数据在异步线程中写入 LogFile 中
            buffersToWrite.swap(buffers_);
            if (!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2);// 如果主程序中的 nextBuffer_ 为nullptr, 使用 newBuffer2 代替
            }
        }

        assert(!buffersToWrite.empty());// 从主程序中获得的 buffer vector 不可以是空的

        if (buffersToWrite.size() > 25)// 如果从主程序获得的 buffer vector 长度大于 25, 则删除到只剩下 2 个 buffer
        {
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd larger buffers\n",
                     Timestamp::now().toFormattedString().c_str(), buffersToWrite.size() - 2);
            fputs(buf, stderr);// 输出到标准错误流
            output.append(buf, static_cast<int>(strlen(buf)));
            // buffer vector 只留下两个 buffer 使用 std::move 移动，节省资源
            buffersToWrite.erase(buffersToWrite.begin()+2, buffersToWrite.end());
        }

        // 将 buffer vector 中的 数据写入 LogFile 中
        for (const auto& buffer : buffersToWrite)
        {
            // FIXME: use unbuffered stdio FILE ? or use ::writev ?
            output.append(buffer->data(), buffer->length());
        }

        if (buffersToWrite.size() > 2)
        {
            // drop non-bzero-ed buffers, avoid trashing
            buffersToWrite.resize(2);
        }

        if (!newBuffer1)
        {
            assert(!buffersToWrite.empty());
            newBuffer1 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer1->reset();
        }

        if (!newBuffer2)
        {
            assert(!buffersToWrite.empty());
            newBuffer2 = std::move(buffersToWrite.back());
            buffersToWrite.pop_back();
            newBuffer2->reset();
        }

        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}

