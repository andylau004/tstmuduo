


#include <iostream>

#include <sstream>

#include <random>



#include <boost/bind.hpp>

#include <stdio.h>



#include "tst_1.h"

#include "codeconvert.h"


#include "muduo/net/InetAddress.h"

#include "muduo/net/Channel.h"

#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"

#include "muduo/base/Logging.h"

#include "muduo/net/TcpClient.h"

#include "discardserver.h"

#include "quicksort.h"
#include "tst_signal.h"
#include "mysqlinsert.h"
#include "cpp11fun.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;


#include "child.h"
#include "parent.h"
#include "pubhead.h"

#include "muduo/base/convert.h"

#include "muduo/base/Atomic.h"


#include <iostream>
#include <random>
#include <cstdlib>
using namespace std;



class CTestServer {

public:
    CTestServer( EventLoop* loop, const InetAddress &listenAddr )
        : loop_(loop),
          server_(loop, listenAddr, "testserver")
    {

        server_.setConnectionCallback(
                    boost::bind(&CTestServer::onConnection, this, _1) );
        server_.setMessageCallback(
                    boost::bind(&CTestServer::onMessage, this, _1, _2, _3) );
//        server_.setWriteCompleteCallback(
//                    boost::bind(&CTestServer::onWriteComplete, this, _1));

        // 生成数据
        muduo::string line;
        for (int i = 33; i < 127; ++i)
        {
            line.push_back(char(i));
        }
        line += line;

//        printf("line=%s\n", line.c_str());
        for (size_t i = 0; i < 127 - 33; ++i)
        {
            message_ += line.substr(i, 72) + '\n';
        }
        message_ = "12345";
        server_.setThreadNum(1);
//        printf("message_=%s\n", message_.c_str());

//        exit(1);
//        server_.setThreadNum(1);

//        message1_.resize(10);
////        message2_.resize(200);
//        std::fill(message1_.begin(), message1_.end(), 'A');
////        std::fill(message2_.begin(), message2_.end(), 'B');

    }
    void start( ) {
        server_.start();
    }
private:
    void onConnection(const TcpConnectionPtr& conn) {

        if (conn->connected()) {
//            printf("onConnection(): new connection [%s] from %s\n",
//                   conn->name().c_str(),
//                   conn->peerAddress().toIpPort().c_str());

            LOG_INFO << "onConnection(): new connection [" << conn->name() << "] from " << conn->peerAddress().toIpPort();
            conn->setTcpNoDelay(true);
//            conn->send(message_);
//            conn->shutdown();
        } else {
            LOG_INFO << "onConnection(): connection [" << conn->name() << "] is down";
        }

    }
    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf,
                   Timestamp receiveTime)
    {
        muduo::string msg(buf->retrieveAllAsString());
        printf("onMessage(): received %zd bytes msg=%s from connection [%s] at %s\n",
               msg.size(), msg.c_str(),
               conn->name().c_str(),
               receiveTime.toFormattedString().c_str());
        conn->send(msg);
    }
    void onWriteComplete(const TcpConnectionPtr& conn) {
        conn->send(message_);
    }

private:
    EventLoop *loop_;
    TcpServer server_;

    muduo::string message_;
//    muduo::string message1_;
//    muduo::string message2_;

};

void tst_Tcp_server1() {
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    InetAddress listenAddr(8888);
    EventLoop loop;

    CTestServer server(&loop, listenAddr);
    server.start();

    loop.loop();
}


void tst_weakPtr_1() {
    ParentPtr parent(new CParent());
    CChildPtr child(new CChild());
    parent->m_childPtr = child;
    child->m_parentPtr = parent;

    std::cout << "parent->m_childPtr expired=" << parent->m_childPtr.expired() << std::endl;
    {
        CChildPtr tmp_child;
        tmp_child = parent->m_childPtr.lock();

        std::cout << "tmp_child unique=" << tmp_child.unique() << std::endl;
        std::cout << "tmp_child use_cout=" << tmp_child.use_count() << std::endl;
        std::cout << "parent->m_childPtr use_cout=" << parent->m_childPtr.use_count() << std::endl;
    }
}



typedef boost::function<void (const CTest_enabled_shared_Ptr&)> use_shared_PtrCb;

void tst_use_shared_Ptr__2222(const CTest_enabled_shared_Ptr& ObjPtr) {
    std::cout << std::endl;

    std::cout << "tst_use_shared_Ptr__2222 begin" << std::endl;
    std::cout << "use_count=" << ObjPtr.use_count() << std::endl;
    std::cout << "tst_use_shared_Ptr__2222 end" << std::endl;

    std::cout << std::endl;
}
void tst_use_shared_Ptr(const CTest_enabled_shared_Ptr& ObjPtr) {
    std::cout << std::endl;

    std::cout << "tst_use_shared_Ptr begin" << std::endl;
//    ObjPtr->printTag();
    std::cout << "use_count=" << ObjPtr.use_count() << std::endl;
    std::cout << "tst_use_shared_Ptr end" << std::endl;

    tst_use_shared_Ptr__2222( ObjPtr );
    std::cout << std::endl;
}

void tst_shared_Ptr_1() {
    CTest_enabled_shared_Ptr ttt1(new CTest_enabled_shared());

//    std::cout << "ttt1 use_cout=" << ttt1.use_count() << std::endl;
//    ttt1->printTag();
//    std::cout << "ttt1 use_cout=" << ttt1.use_count() << std::endl;

    tst_use_shared_Ptr(ttt1);
//    tst_use_shared_Ptr(ttt1);

}

void tst_shared_Ptr_2() {
    CTest_enabled_shared_Ptr ttt1(new CTest_enabled_shared());

//    std::cout << "ttt1 use_cout=" << ttt1.use_count() << std::endl;
//    ttt1->printTag();
//    std::cout << "ttt1 use_cout=" << ttt1.use_count() << std::endl;

    tst_use_shared_Ptr(ttt1);

}

// 处理超过128字节ios推送逻辑
std::string dealExceed128Bytes( const std::string& tip ) {
    if ( tip.length() <= 128 )
        return tip;

    std::string ret_tip;

    // 截断128字节数据，且追加后缀 ...
    const int FIX_MAX_LEN = 128;
//    if (tip.length() > FIX_MAX_LEN) {
//        result.promtStKey = "newMSG";
//        return;

    printf( "tip.length() > 128, tip.length()=%d\n", tip.length() );
    printf( "FIX_MAX_LEN=%d\n", FIX_MAX_LEN );
//        LOG_INFO( "tip.length() > 128, tip.length()=" << tip.length() );
//        size_t tmplen = tip.length();

        wchar_t tmp_wbuffer[FIX_MAX_LEN + 1]; ::memset( tmp_wbuffer, 0, sizeof(tmp_wbuffer) );

        size_t transferLen = std::mbstowcs(tmp_wbuffer, tip.c_str(), FIX_MAX_LEN);
//        UNUSED(transferLen);
//        LOG_INFO( "transferLen=" << transferLen );
        printf( "transferLen=%d\n", transferLen );

//        std::wstring neww1111 = Convert::convert(tip);
//        std::wcout << "neww1111=" << neww1111 << std::endl;

        std::wstring ws_tmp = tmp_wbuffer;//ws_tmp = neww1111;
//        std::wcout << "ws_tmp=" <<ws_tmp << std::endl;

        std::string slst = Convert::convert(ws_tmp);
//        char lstbuffer[ FIX_MAX_LEN*sizeof(wchar_t) + 1 ];
//        memset( lstbuffer, 0, FIX_MAX_LEN*sizeof(wchar_t) + 1 );
//        std::cout << "aaaaaaaaaa=" << FIX_MAX_LEN*sizeof(wchar_t) + 1 << std::endl;//

//        transferLen = std::wcstombs(lstbuffer, tmp_wbuffer, FIX_MAX_LEN*sizeof(wchar_t) + 1);
//        std::cout << "lst transferLen=" << transferLen << std::endl;//
//        std::cout << "lstbuffer=" << lstbuffer << std::endl;//

        if (slst.length() == tip.length()) {
            ret_tip += slst;
        } else {
            ret_tip += slst;
            ret_tip += "...";
        }

//        LOG_DEBUG( "last_tip=" << ret_tip.c_str() );
//        printf( "slst=%s\n", slst.c_str() );
//        printf( "ret_tip.size=%d&last_tip=%s\n", ret_tip.size(), ret_tip.c_str() );
//    }
    return ret_tip;
}

// 分析字符串内中文字个数
void tst_convert_1() {
    {
        std::string strShow = "www别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦JJ集体哦说www上午去  PK哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊 安咯哦哟...";
        strShow = "别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦集体哦说上午去哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊安咯哦哟";

        strShow = "abcd别个名模意图别个名模意图别个名模意图别个名模意图别个名模意图别个名模意图别个名模意图别个名模意图";

        std::cout << "strShow=" << strShow << std::endl;
        std::cout << "strShowsize=" << strShow.size() << std::endl;

        std::cout << "countChineseCharsUtf8=" << Convert::countChineseCharsUtf8(strShow) << std::endl;
        std::cout << "detectEncoding=" << Convert::getEncodingName(strShow) << std::endl;
//        std::cout << "countChineseCharsGbk=" << Convert::countChineseCharsGbk(strShow) << std::endl;
//        std::cout << "countChineseCharsUcs2=" << Convert::countChineseCharsUcs2(strShow) << std::endl;

        std::string ret = dealExceed128Bytes( strShow );

        std::cout << "ret=" << ret << std::endl;
        std::cout << "ret.size=" << ret.size() << std::endl;
        return ;
    }


    std::string sttt = "别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦集体哦说上午去哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊安咯哦哟哦只有行么额呃呃呃体现一下喷雾器咯哦哦的啦民工别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦集体哦说上午去哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊安咯哦哟哦只有行么别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦集体哦说上午去哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊安咯哦哟别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦集体哦说上午去哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊安咯哦哟哦只有行么额呃呃呃体现一下喷雾器咯哦哦的啦民工别个";

    sttt = "www别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦JJ集体哦说www上午去  PK哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊 安咯哦哟哦只有行么额呃呃呃   体现一下喷雾器咯哦哦的啦 民工别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦JJ集体哦说www上午去  PK哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊 安咯哦哟哦只有行么别个名模意图舞台设计利济路集体推理别个名模意图舞台设计利济路集体推理鸡腿踢腿吱吱吱嘻嘻嘻仔细找是哦里连累哦哦呦喂一直嘻嘻嘻嘻嘻醉咯上课去啦怕怕啊怕怕阿萨很愿意走咯额呃呃呃哦啦JJ集体哦说www上午去  PK哦哦你呃呃呃啊啊啊啊啊啊啊啊啊啊啊 安咯哦哟哦只有行么额呃呃呃   体现一下喷雾器咯哦哦的啦 民工晚自习王一民敏敏公民踢腿吱吱吱嘻嘻嘻仔细找是呃呃呃   体现一下喷雾器咯哦哦的啦 民工晚自习王一民敏敏公民踢腿吱吱吱嘻嘻嘻仔细找是王一民敏敏公民踢腿吱吱吱嘻嘻嘻仔细找是";
    sttt = "www.baidu.com阿萨斯哦鞍山市绿卡单拉是打开拉萨的看着你呢，自行车，自行车，自行车，，自行车凯斯齐威王  圣安东尼奥四大纳斯的状况才能做出这些内存模组";
    std::string ret = dealExceed128Bytes( sttt );

    std::cout << "ret=" << ret << std::endl;
    std::cout << "ret.size=" << ret.size() << std::endl;

}

int64_t GetRandom64() {

//    int ittt = 123;
//    auto xxxx = ittt;
//    std::cout << "xxxx=" << xxxx << std::endl;
//    return 1;


    {
//        std::random_device  rd;
//        std::mt19937 mt(rd());
//        // 产生10个随机数
//        for(int i = 0; i < 10; i++) {
//            cout << mt() << endl;
//        }

    }

    {
//        std::random_device rd; // 種子產生器
//        std::mt19937 gen = std::mt19937(rd()); //使用mt19937引擎
//        std::uniform_real_distribution<> dis(-1, 1); //使用平均分佈

//        auto randfun = std::bind(dis, gen); //將引擎和分佈綁在一起變成新函數 randfun

//        cout<<randfun()<<' '<<randfun()<<' '<<randfun()<<endl; //可以直接重複呼叫
    }

    {
        independent_bits_engine<default_random_engine,64,unsigned long long int> engine;
        engine.seed(time(NULL));//设定随机数种子

        for(int i=0;i<100;i++)   //获得100个随机数
        {
            std::cout<<engine()<<std::endl;
        }
    }

//    //engine.discard(8);     //跳过8次，相当于执行8次engine()而不输出结果
//    //engine.max()           //获得最大值
//    //engine.min()           //获得最小值

    return 0;
}

void tst_string_1() {

    {
        std::string strDigit = "123467";
        int tmp = convert<int>(strDigit);
        std::cout << "tmp=" << tmp << std::endl;
        return ;
    }
    {
        std::string strTest = "/data/linkdood/im/vrvfile (1)";

        strTest = strTest.substr(strTest.find("("));
        std::cout << "strTest=" << strTest << std::endl;

        // '('
        if (strTest.at(0) == '(' && strTest.at(strTest.length() - 1) == ')') {
            std::cout << "ok find it" << std::endl;

            strTest = strTest.substr(1, strTest.length() - 2);
            std::cout << "strTest=" << strTest << std::endl;

        }
        return ;
    }


    {
        uint64_t newFileCode = 7755548965449811111222;

        std::string str_newCode = convert<std::string>(newFileCode+1);
        std::cout << "str_newCode=" << str_newCode <<  std::endl;
        return ;
    }

    std::string sonFileCode = "77555489654498";

    if (sonFileCode.length() > 4) {
        sonFileCode = sonFileCode.substr(sonFileCode.length() - 4);
        std::cout << "sonFileCode.length() > 4, sonFileCode=" << sonFileCode << std::endl;
    } else {
        std::cout << "sonFileCode.length() < 4 " << std::endl;
    }

}

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "en_US.utf8");
    Logger::setLogLevel(Logger::DEBUG);
//    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_string_1(); return 1;

    GetRandom64();
    return 1;

    std::string sttt = "12345";
    LOG_INFO << "sttt=" << sttt.substr(1);
    return 1;

    AtomicInt64 tmp64;
    LOG_INFO << "tmp64=" << tmp64.get();
    tmp64.add(1);
    LOG_INFO << "tmp64=" << tmp64.getAndAdd(2);
    LOG_INFO << "tmp64=" << tmp64.get();
    return 1;

    tst_shared_Ptr_1(); return 1;

    tst_convert_1(); return 1;

//    tst_weakPtr_1(); return 1;

#ifdef SO_REUSEPORT
    printf( "define SO_REUSEPORT\n" );
#else
    printf( "no define SO_REUSEPORT\n" );
#endif

    tst_Tcp_server1(); return 1;

}












