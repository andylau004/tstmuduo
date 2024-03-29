


#include "tpwork.h"


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>


#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"
#include "muduo/base/ThreadLocal.h"
#include "muduo/base/ThreadPool.h"


using namespace std;
using namespace muduo;



class Test : boost::noncopyable
{
public:
    Test()
    {
        printf("tid=%d, constructing %p\n", muduo::CurrentThread::tid(), this);
    }

    ~Test()
    {
        printf("tid=%d, destructing %p %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
    }

    const muduo::string& name() const { return name_; }
    void setName(const muduo::string& n) { name_ = n; }

private:
    muduo::string name_;
};
//定义两个线程特定数据对象，每个线程都有这样的对象
muduo::ThreadLocal<Test> testObj1;
muduo::ThreadLocal<Test> testObj2;

void print()
{
    printf("tid=%d, obj1 %p name=%s\n",
           muduo::CurrentThread::tid(),
           &testObj1.value(),
           testObj1.value().name().c_str());
    printf("tid=%d, obj2 %p name=%s\n",
           muduo::CurrentThread::tid(),
           &testObj2.value(),
           testObj2.value().name().c_str());
}

void threadFunc()
{
    printf("\n");
    printf("threadFunc beg----------------------------------\n");
    print();
    //testObj1.value()返回的是Test类型的引用
    testObj1.value().setName("changed 1");
    testObj2.value().setName("changed 42");
    print();
    printf("threadFunc end----------------------------------\n");
    printf("\n");
}

void tst_threadpoolWork_entry() {
    testObj1.value().setName("main one");
    print();

    // getchar();
    muduo::Thread t1(threadFunc);
    t1.start();
    t1.join();

    testObj2.value().setName("main two");
    print();

    pthread_exit(0);
}

    // 测试muduo线程池
    // muduo::ThreadPool workPool;
    // workPool.start( 4 );
