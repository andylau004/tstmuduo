





#include <iostream>
#include <sstream>
#include <stdio.h>

//#include "tst_1.h"

#include "codeconvert.h"

#include "muduo/base/Logging.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpClient.h"

#include <boost/bind.hpp>



#include <muduo/base/TimeZone.h>
#include <muduo/net/EventLoop.h>

#include <cthrift/cthrift_svr.h>
#include "Echo.h"

using namespace std;

using namespace echo;
using namespace muduo;
using namespace muduo::net;
using namespace meituan_cthrift;



//1K 数据echo测试
void Work(const string &str_svr_appkey,
          const string &str_cli_appkey,
          const int32_t &i32_timeout_ms,
          muduo::CountDownLatch *p_countdown) {

    //建议CthriftClient生命期也和线程保持一致，不要一次请求创建销毁一次
    CthriftClient cthrift_client("172.17.0.2", int16_t(16888)/*str_svr_appkey*/, i32_timeout_ms);

    //cthrift_client.SetFilterService("echo.Echo");

    //设置client appkey，方便服务治理识别来源
    if (SUCCESS != cthrift_client.SetClientAppkey(str_cli_appkey)) {
        std::cout << "SetClientAppkey error" << endl;
        p_countdown->countDown();
        return;
    }

    //显示调用初始化，初始化内部流程
    printf("bbbbbbbbbbbbbbbbbbbbbbb\n");
    if (SUCCESS != cthrift_client.Init()) {
        std::cout << "Init error" << endl;
        p_countdown->countDown();
        return;
    }
    printf("cccccccccccccccccccccc\n");

    // 确保业务EchoClient的生命期和线程生命期同，不要一次请求创建销毁一次EchoClient！！
    EchoClient client(cthrift_client.GetCthriftProtocol());
    string strRet;
    string str_tmp;
    size_t sz;

    char buf[1025];  //1K数据
    memset(buf, 1, sizeof(buf));
    int num = 1000;

    for (int i = muduo::CurrentThread::tid() * num;
         i < muduo::CurrentThread::tid() * num + num; i++) {

        try {
            str_tmp = boost::lexical_cast<std::string>(i);
        } catch (boost::bad_lexical_cast &e) {
            std::cout << "boost::bad_lexical_cast :" << e.what() << "i : " << i;
            continue;
        }

        sz = str_tmp.size();
        str_tmp += string(buf, 0, sizeof(buf) - 1 - sz);

        try {
            //标准RPC调用
            client.echo(strRet, str_tmp);
        } catch (TException &tx) {
            std::cout << "ERROR: " << tx.what() << endl;
            p_countdown->countDown();
            return;
        }

        if (str_tmp != strRet) {
            std::cout << "tid: " << muduo::CurrentThread::tid() << "strRet " << strRet
                 << " str_tmp " << "test" + str_tmp << endl;
            p_countdown->countDown();
            return;
        }
    }

    std::cout << "tid: " << muduo::CurrentThread::tid() << " END" << endl;
    std::cout << "**********run rpc for echo 1000 times suceess*********" << endl;

    p_countdown->countDown();
}


int main(int argc, char *argv[])
{
    muduo::Logger::setLogLevel(muduo::Logger::INFO);
    LOG_INFO << "pid = " << getpid() << ", tid = " << muduo::CurrentThread::tid();


    string str_svr_appkey("com.sankuai.inf.newct"); //服务端的appkey
    string str_cli_appkey("com.sankuai.inf.newct.client"); //客户端的appkey
    int32_t i32_timeout_ms = 20;

//    switch (argc) {
//      case 1:std::cout << "no input arg, use defalut" << std::endl;
//        break;

//      case 4:str_svr_appkey.assign(argv[1]);
//        str_cli_appkey.assign(argv[2]);
//        i32_timeout_ms = static_cast<int32_t>(atoi(argv[3]));
//        break;
//      default:
//        cerr << "prog <svr appkey> <client appkey> <timeout ms> but argc " << argc
//             << endl;
//        exit(-1);
//    }

    std::cout << "svr appkey " << str_svr_appkey << std::endl;
    std::cout << "client appkey " << str_cli_appkey << std::endl;
    std::cout << "timeout ms " << i32_timeout_ms << std::endl;

    //10个线程并发
    int32_t i32_thread_num = 2;  //线程数视任务占用CPU时间而定，建议不要超过2*CPU核数
    muduo::CountDownLatch countdown_thread_finish(i32_thread_num);
    for (int i = 0; i < i32_thread_num; i++) {
      muduo::net::EventLoopThread *pt = new muduo::net::EventLoopThread;
      pt->startLoop()->runInLoop(boost::bind(Work,
                                             str_svr_appkey, //服务端Appkey必须填写，不可为空，寻求服务
                                             str_cli_appkey, //客户端Appkey必须填写，不可为空，以便于问题追踪
                                             i32_timeout_ms,
                                             &countdown_thread_finish));
    }

    countdown_thread_finish.wait();

    std::cout << "exit" << std::endl;

    return 1;
}


















