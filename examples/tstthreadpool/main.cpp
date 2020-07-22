
#include "tpwork.h"


#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include "muduo/base/common.h"
#include "muduo/base/Thread.h"
#include "muduo/base/ThreadPool.h"
#include "muduo/base/Atomic.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;

//#include <gtest/gtest.h>

#include "ched_ssl.h"



int test_fun(int a) {
    return a + 1;
}

/*
// 单元测试
TEST(FunTest, HandlesZeroInput) {
  EXPECT_EQ(1, test_fun(0));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/

pthread_key_t p_key;

struct test_struct {
    int i;
    float k;
};
void child1()
{
    struct test_struct struct_data;
    struct_data.i = 10;
    struct_data.k = 3.1415;
    pthread_setspecific(p_key, &struct_data);

    printf("child1--address of struct_data is --> 0x%p\n", &(struct_data));
    printf("child1--from pthread_getspecific(p_key) get the pointer and it points to --> 0x%p\n", (struct test_struct *)pthread_getspecific(p_key));
    printf("child1--from pthread_getspecific(p_key) get the pointer and print it's content:\nstruct_data.i:%d\nstruct_data.k: %f\n",
        ((struct test_struct *)pthread_getspecific(p_key))->i, ((struct test_struct *)pthread_getspecific(p_key))->k);
    printf("------------------------------------------------------\n");
}
void child2()
{
    int temp = 20;
    sleep(2);
    printf("child2--temp's address is 0x%p\n", &temp);
    pthread_setspecific(p_key, &temp);
    printf("child2--from pthread_getspecific(p_key) get the pointer and it points to --> 0x%p\n", (int *)pthread_getspecific(p_key));
    printf("child2--from pthread_getspecific(p_key) get the pointer and print it's content --> temp:%d\n", *((int *)pthread_getspecific(p_key)));
}


void func1()
{
    int *tmp = (int*)pthread_getspecific(p_key);//同一线程内的各个函数间共享数据。
//    printf("%d is runing in %s\n", *tmp, __func__);
    LOG_INFO << "222, tmp=" << *tmp << " is runing, in Function: " << __func__;
}

void* keyThreadProc(void* arg) {
    pthread_setspecific(p_key, arg);

    int *tmp = (int*)pthread_getspecific(p_key);//获得线程的私有空间
    LOG_INFO << "111, tmp=" << *tmp << " is runing";

    *tmp = (*tmp)*100;//修改私有变量的值

    func1();
    return (void*)0;
}
void tst_pthread_key() {
    pthread_key_create(&p_key, NULL); // 这里是构建一个pthread_key_t类型，确实是相当于一个key

    int a = 1;
    int b = 2;
    muduo::Thread t1(boost::bind(&keyThreadProc, &a));
    muduo::Thread t2(boost::bind(&keyThreadProc, &b));
    t1.start();
    t2.start();
    t1.join();
    t2.join();

    pthread_key_delete(p_key);
    return;

#if 0
    pthread_key_create(&key, NULL); // 这里是构建一个pthread_key_t类型，确实是相当于一个key

    muduo::Thread t1(child1);
    muduo::Thread t2(child2);
    t1.start();
    t2.start();
    t1.join();
    t2.join();

    pthread_key_delete(key);
return;
#endif

#if 0
//    pthread_t tid1, tid2;
//    pthread_key_create(&key, NULL); // 这里是构建一个pthread_key_t类型，确实是相当于一个key
//    pthread_create(&tid1, NULL, child1, NULL);
//    pthread_create(&tid2, NULL, child2, NULL);
//    pthread_join(tid1, NULL);
//    pthread_join(tid2, NULL);
//    pthread_key_delete(key);
#endif
}

bool get_int_value(const rapidjson::Document& d, const char* name, int& value) {
    if (!d.IsObject()) {
        value = 0;
        printf("d is not Object !!!!\n");
        return false;
    }
    if (d.HasMember(name)) {
        if (d[name].IsInt()) {
            value = d[name].GetInt();
            return true;
        }
    }
    value = 0 ;
    return false;
}

muduo::AtomicInt64 g_succCount;
muduo::AtomicInt64 g_failCount;

void tst_parse_crt() {

//    char* poutbuffer = NULL;
//    std::string strfile = "zhqtest123.crt";

//    int iret = ParseCrt(0, strfile.c_str(), &poutbuffer);
////    LOG_INFO << "iret=" << iret << " outbuf=" << poutbuffer;

//    rapidjson::Document jsonReader;
//    jsonReader.Parse(poutbuffer);
//    if (jsonReader.HasParseError()) {// 判断解析是否成功
//        LOG_INFO << "tst_parse_crt parse json failed, injson=" << poutbuffer;
//        free(poutbuffer);
//        g_failCount.add(1);
//        return;
//    }

//    int iunl = 0;
//    get_int_value(jsonReader, "unl", iunl);
////    LOG_INFO << "unl=" << iunl;
////    printf("iunl=%d\n", iunl);
//    free(poutbuffer);
//    g_succCount.add(1);
}

void tstMultiParseCrt(int argc, char *argv[]) {
    if (argc < 3) {
        printf("argc < 3, need threadnum tasknum\n");
        return ;
    }

    int threadNum = atoi(argv[1]);
    int taskNum = atoi(argv[2]);
    if (threadNum <= 0) {
        LOG_INFO << "thread num invalid";
        return;
    }
    if (taskNum <= 0) {
        LOG_INFO << "task num invalid";
        return;
    }
    printf("threadNum=%d taskNum=%d\n", threadNum, taskNum);

    muduo::ThreadPool pool("MainThreadPool");
    pool.start(threadNum);
    for (int i = 0; i < taskNum; ++i)
    {
//        char buf[32];
//        snprintf(buf, sizeof buf, "task %d", i);
        pool.run(boost::bind(tst_parse_crt));
    }

    muduo::CountDownLatch latch(1);
    pool.run(boost::bind(&muduo::CountDownLatch::countDown, &latch));
    latch.wait();
    pool.stop();
    LOG_INFO << "succ count=" << g_succCount.get();
    LOG_INFO << "fail count=" << g_failCount.get();
}


void WorkerFun(int idx, muduo::CountDownLatch *p_countdown) {

//    LOG_INFO << "workerIdx=" << idx << " is running...";
    DeferFunctor pfnExit = boost::function < void() >([&]() {
        LOG_INFO << "workerIdx=" << idx << " is exiting...";
    });

    if (idx == 2) {
        LOG_INFO << "I am idx2 thread, sleep moment";
        sleep(5);
    }

    p_countdown->countDown();
}

void tst_countdown_fun() {
    int numThread = 4;

    muduo::CountDownLatch cdlatch(numThread);

    for (int i = 0; i < numThread; i ++) {
        muduo::net::EventLoopThread* pt = new muduo::net::EventLoopThread;
        pt->startLoop()->runInLoop(boost::bind(&WorkerFun,
                                               i,
                                               &cdlatch));
    }

//    LOG_INFO << "before cdlatch wait";
    cdlatch.wait();
    LOG_INFO << "after cdlatch wait";

}

int main(int argc, char *argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_pthread_key(); return 1;

    tst_countdown_fun(); return 1;

    tst_threadpoolWork_entry();  return 1;

    tstMultiParseCrt(argc, argv); return 1;

}
