

#include "tstnvi.h"

#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield


#include <string>
#include <mutex>

#include <cmath>
#include <future>
#include <functional>

#include <cstdio>
#include <cstdlib>

#include <event.h>

#include <thrift/Thrift.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/transport/TBufferTransports.h>

#include <thrift/server/TServer.h>
#include <thrift/async/TAsyncChannel.h>
#include <thrift/async/TEvhttpClientChannel.h>

#include "gen-cpp/Twitter.h"

#include <thrift/async/TAsyncProtocolProcessor.h>
#include <thrift/async/TEvhttpServer.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/concurrency/Monitor.h>
//#include <server/TSimpleServer.h>
#include <boost/shared_ptr.hpp>

#include "muduo/base/common.h"

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;




using namespace std;
using std::string;
using namespace boost;

using namespace thrift::example;
using namespace apache::thrift::async;


class MyTask : public Runnable {
    public:
        MyTask(Monitor &monitor, int &count) : count_(count), monitor_(monitor) {
        }
        void run() {
            {
                Synchronized s(monitor_);
                count_++;
//                printf("-------------count_=%d\n", count_);
                if (count_ == 10) {
                    monitor_.notify();
                }
                printf("-----------------last=%d\n", count_);
            }
        }
    private:
        int &count_;
        Monitor& monitor_;
};

void tstTask() {
    int num_threads = 5;
    boost::shared_ptr<ThreadManager> tm = ThreadManager::newSimpleThreadManager(num_threads);

    boost::shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());
    tm->threadFactory(threadFactory);
    tm->start();

    Monitor monitor;
    int m = 0;

    std::set<boost::shared_ptr<MyTask> > tasks;
    for (int i = 0; i < 20; i++) {
        tasks.insert(boost::shared_ptr<MyTask>(new MyTask(monitor, m)));
    }

    std::set<boost::shared_ptr<MyTask> >::iterator it;
    for (it = tasks.begin(); it != tasks.end(); it++) {
        tm->add(*it, 50, 50);
    }

    {
        Synchronized s(monitor);
        monitor.wait();
    }
    tm->join();
}

class Parent {
public:
    void read() { this->read_virt();}
    virtual void read_virt() { }
    void write() { this->write_virt(); }
    virtual void write_virt() {}
};


class DefaultSon : public Parent {
public:
    virtual void read_virt() {
        std::cout << "DefaultSon read" << std::endl;
    }
    virtual void write_virt() {
        std::cout << "DefaultSon write" << std::endl;
    }
};

template <class Me, class super_ = DefaultSon>
class VirtualParent : public super_ {
public:
    virtual void read_virt() {
        return static_cast<Me *>(this)->read();
    }
    virtual void write_virt() {
        return static_cast<Me *>(this)->write();
    }
};
class GoodSon : public VirtualParent<GoodSon> {
public:
    void read() {
        std::cout << "GoodSon read" << std::endl;
    }
    void write() {
        std::cout << "GoodSon write" << std::endl;
    }
};
class BadSon : public VirtualParent<BadSon> {
public:
    void read() {
        std::cout << "BadSon read" << std::endl;
    }
    void write() {
        std::cout << "BadSon read" << std::endl;
    }
};


void tstnvi() {
    tstTask();
    return;

    boost::shared_ptr<Parent> p(boost::make_shared< GoodSon >());
    p->read();
    p->write();

    boost::shared_ptr<Parent> q(boost::make_shared< BadSon >());
    q->read();
    q->write();

}
