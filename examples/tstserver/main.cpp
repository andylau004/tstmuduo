


#include <iostream>

#include <sstream>

#include <random>

#include <set>


#include <boost/bind.hpp>

#include <stdio.h>

#include <time.h>
#include <sys/time.h>
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


#include "thirdpart/rapidjson/document.h"
#include "thirdpart/rapidjson/writer.h"
#include "thirdpart/rapidjson/stringbuffer.h"
#include "thirdpart/rapidjson/filereadstream.h"

#include "muduo/base/common.h"


#include <boost/algorithm/string.hpp>
#include <boost/tuple/tuple.hpp>

using namespace rapidjson;


#include "stockfactory.h"
#include "booststockfactory.h"





// template classes for retrieving values from RapidJson document
template <typename T, typename P>
struct safeGetter {
    static bool get(T& ret, const P& doc, const char* key) { return false; }
};

template <typename P>
struct safeGetter<std::string, P> {
    static bool get(std::string& ret, const P& doc, const char* key) {
        if (doc.HasMember(key)) {
            if (doc[key].GetType() == rapidjson::kObjectType) {
                StringBuffer sb;
                Writer<StringBuffer> writer(sb);
                doc[key].Accept(writer);
                ret = sb.GetString();
                return true;
            }

            ret = doc[key].GetString();
            return true;
        }
        return false;
    }
};

template <typename P>
struct safeGetter<int, P> {
    static bool get(int& ret, const P& doc, const char* key) {
        if (doc.HasMember(key)) {
            ret = doc[key].GetInt();
            return true;
        }
        return false;
    }
};

template <typename P>
struct safeGetter<int64_t, P> {
    static bool get(int64_t& ret, const P& doc, const char* key) {
        if (doc.HasMember(key)) {
            ret = doc[key].GetInt64();
            return true;
        }
        return false;
    }
};





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

//int64_t GetRandom64() {

////    int ittt = 123;
////    auto xxxx = ittt;
////    std::cout << "xxxx=" << xxxx << std::endl;
////    return 1;


//    {
////        std::random_device  rd;
////        std::mt19937 mt(rd());
////        // 产生10个随机数
////        for(int i = 0; i < 10; i++) {
////            cout << mt() << endl;
////        }

//    }

//    {
////        std::random_device rd; // 種子產生器
////        std::mt19937 gen = std::mt19937(rd()); //使用mt19937引擎
////        std::uniform_real_distribution<> dis(-1, 1); //使用平均分佈

////        auto randfun = std::bind(dis, gen); //將引擎和分佈綁在一起變成新函數 randfun

////        cout<<randfun()<<' '<<randfun()<<' '<<randfun()<<endl; //可以直接重複呼叫
//    }

//    {
//        independent_bits_engine<default_random_engine,64,unsigned long long int> engine;
//        engine.seed(time(NULL));//设定随机数种子

//        for(int i=0;i<100;i++)   //获得100个随机数
//        {
//            std::cout<<engine()<<std::endl;
//        }
//    }

////    //engine.discard(8);     //跳过8次，相当于执行8次engine()而不输出结果
////    //engine.max()           //获得最大值
////    //engine.min()           //获得最小值

//    return 0;
//}


std::string  constructUpdateSql( const std::string& inCode ) {
    std::string retSql;

    std::string strCurMisec = GetCurMilliSecond_string();

    retSql = "update @table set ";

    retSql += " validity=1, ";
    retSql += " pid=-1, ";

    retSql += (std::string(" updatedAt=") + strCurMisec + ", ");
    retSql += (std::string(" deletedAt=") + strCurMisec + " ");

    retSql += " where 1 = 1 and fileId not in(1,2) ";
    retSql += " and fileCode like CONCAT(";
    retSql += inCode;
    retSql += ",'%')";

    return retSql;
}


void tst_bindFunction_1() {

}

// 定义回调函数类型
typedef function<int (int x,int y)>  CallBackFunction;
//typedef int (*CallBackFunction)(int x, int y);             // 不能用于bind函数返回类型

// 类定义
class CMethod2
{
    typedef CMethod2 this_type;
public:
    CMethod2() {}
    ~CMethod2() {}
public:
    int Add(int x,int y)                 // 1.类非静态成员函数作为回调函数
    {
        return x + y;
    }
    static int AddS(int x,int y)         // 2.类静态成员函数作为回调函数
    {
        return x + y;
    }
};

int Add(int x,int y)                     // 3.非类成员函数作为回调函数
{
    return x + y;
}

// 测试接口
void Test()
{
    int x = 10;
    int y = 10;
    CMethod2 mtd1;
    CMethod2 &mtd2 = mtd1;
    CMethod2 *mtd3 = &mtd1;
    // 1.对象、对象引用、对象指针都可用于bind绑定类非静态成员函数绑定的实例
    CallBackFunction callBack1 = bind(&CMethod2::Add,&mtd1,_1,_2);
    CallBackFunction callBack2 = bind(&CMethod2::Add,&mtd2,_1,_2);
    CallBackFunction callBack3 = bind(&CMethod2::Add,mtd3,_1,_2);
    cout << "callBack1 Result:" << callBack1(x,y) << endl;
    cout << "callBack2 Result:" << callBack2(x,y) << endl;
    cout << "callBack3 Result:" << callBack3(x,y) << endl;
//    return ;

    // 2.静态成员函数作为回调
    //CallBackFunction callBack4 = CMethod2::AddS;
    CallBackFunction callBack4 = bind(CMethod2::AddS,_1,_2);
    cout << "callBack4 Result:" << callBack4(x,y) << endl;
    // 3.非成员函数作为回调
    //CallBackFunction callBack5 = Add;
    CallBackFunction callBack5 = bind(Add,_1,_2);
    cout << "callBack5 Result:" << callBack5(x,y) << endl;
}

typedef struct _tagTest {

    int i1;

    int i2;

    int i3;

    std::string str1;
    std::string str2;

    _tagTest() {
        i1 = i2 = -1;
    }

} TestSt;


void tst_rightVal( bool bInput ) {
    bInput = true;
    std::cout << "bInput=" << bInput << std::endl;
}
class demo {
public:
    demo() {
        std::cout << "demo cst" << std::endl;
    }
    ~demo() {
        std::cout << "demo dst" << std::endl;
    }
    void show() {
        std::cout << "call show" << std::endl;
    }
};
///weak_ptr -> shared_ptr
void my_weak_ptr() {
    {
        std::weak_ptr<demo> wkPtr;
        {
    //        auto sptr = std::make_shared<demo>();
            std::shared_ptr<demo> sptr(new demo());
            wkPtr = sptr;

            std::cout << "share user_cout=" << sptr.use_count() << std::endl;
            std::cout << "weak user_cout="  << wkPtr.use_count() << std::endl;
            std::cout << "--------------------" << std::endl;

            auto pTmp = wkPtr.lock();
            std::cout << "--------------------" << std::endl;
            std::cout << "share user_cout=" << sptr.use_count() << std::endl;
            std::cout << "weak user_cout="  << wkPtr.use_count() << std::endl;
            std::cout << "--------------------" << std::endl;

            if (!wkPtr.expired()) {
                printf( "shared_ptr is ok\n" );
                std::cout << "share user_cout=" << sptr.use_count() << std::endl;
                std::cout << "weak user_cout="  << wkPtr.use_count() << std::endl;
                pTmp->show();
            }
        }
        if (wkPtr.expired()) {
            std::cout << "sharedptr_Obj is destroyed" <<std::endl;
        }
    }


//    {
//        auto sptr = std::make_shared<demo>();
//        wptr = sptr;
//        auto sptr2 = wptr.lock();
//        if (!wptr.expired()){///等价于sptr2 != nullptr
//            printf("shared_ptr ok\n");
//            sptr2->show();
//        }
//    }
//    if (wptr.expired()){
//        printf("shared_ptr deleted\n");
//    }
}

void tst_string_1() {
    {
        tst_rightVal( false );
        return ;
    }

    {
        std::string strName = "123.txt";
        int dotPos = strName.find(".");
        if (dotPos != std::string::npos) {
            std::string strJustName = strName.substr(0, dotPos);
            std::cout << "justName=" << strJustName << std::endl;

            std::string strExt = strName.substr(dotPos+1);
            std::cout << "strExt=" << strExt << std::endl;
        }
        return;
    }
    {
        TestSt ttt1;
        TestSt ttt2;

        ttt1.str1 = "11123";
        ttt1.str2 = "334455";

        ttt2 = ttt1;
        std::cout << "i1=" << ttt2.i1 << std::endl;
        std::cout << "i2=" << ttt2.i2 << std::endl;
        std::cout << "i3=" << ttt2.i3 << std::endl;
        std::cout << "str1=" << ttt2.str1 << std::endl;
        std::cout << "str2=" << ttt2.str2 << std::endl;

//        TestSt ttt1;
//        std::cout << "i1=" << ttt1.i1 << std::endl;
//        std::cout << "i2=" << ttt1.i2 << std::endl;
//        std::cout << "i3=" << ttt1.i3 << std::endl;
        return ;
    }
//    Test(); return ;

    {

        std::cout << "1=" << constructUpdateSql("123456") << std::endl;


//        std::set<int64_t> setDel;
//        setDel.insert( 777 );setDel.insert( 222 );setDel.insert( 888 );

//        for ( auto& oneId : setDel ) {
//            std::cout << "oneId=" << oneId << std::endl;
//        }
return;
    }
    {
        //    sql_DeleFiles = "delete from @table where (fileId = 13811196572 or fileId = 15321920256);";

        std::set<std::string> setDel;
        setDel.insert( "123" );setDel.insert( "888" );setDel.insert( "777" );

        int icout = 0;
        std::string strTmpSql = "( ";
        for (auto& oneId : setDel) {
            strTmpSql += "fileId = ";
            strTmpSql += oneId;

            if (++icout < setDel.size()) {
                strTmpSql += " or ";
            }
        }

        strTmpSql += " )";

        std::cout << "strTmpSql=" << strTmpSql << std::endl;

        std::string sql_DeleFiles = "delete from @table where ";
        sql_DeleFiles += strTmpSql;
        std::cout << "sql_DeleFiles=" << sql_DeleFiles << std::endl;

        return ;
    }

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


inline std::string doc2string(const rapidjson::Document& doc) {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}
std::string makeResultJson(int code, const std::string& message, const std::string& result) {
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    doc.AddMember("code", code, allocator);
    doc.AddMember("msg", rapidjson::Value().SetString(message.c_str(), message.length(), allocator), allocator);
    doc.AddMember("result", rapidjson::Value().SetString(result.c_str(), result.length(), allocator), allocator);
    return doc2string(doc);
}

void tst_str_2() {

    {
        std::vector<int64_t> ownerIds;
        ownerIds.push_back( 776988766 );ownerIds.push_back( 887623446 );ownerIds.push_back( 123498765 );

        std::string tmpIds = " and ownerId in (";
        if (ownerIds.size()) {
            int icout = 0;
            for ( auto& oneId : ownerIds) {
                tmpIds += convert<std::string>(oneId);

                if (++icout < ownerIds.size()) {
                    tmpIds += ", ";
                }
            }

            tmpIds += ")";
        }
        std::cout << "tmpIds=" << tmpIds << std::endl;

        return ;
    }

    std::string tn111 = "IM_FILE_TABLE";
    std::string tmpSql = "123___fileNamexxx ttt";
    tmpSql.replace(tmpSql.find("fileName"), strlen("fileName"), tn111);

    std::cout << "tmpSql=" << tmpSql << std::endl;
    return ;

    std::cout << "result=" << makeResultJson( 123, "testkey", "tstvalue111" ) << std::endl;
    return ;

    std::string sql_queryTreeFolder;

    sql_queryTreeFolder = "SELECT * FROM \
( \
 SELECT * FROM @table WHERE ownerId = (?) AND fileType = 2 AND pid = (?) \
 UNION \
 SELECT * FROM @table WHERE fileType= 4 AND pid = (?) \
 ) AS T3 WHERE T3.deletedAt IS NULL";

            const static std::string kCloudFileTableName = "IM_FILE";
            std::string tn = kCloudFileTableName;

            std::string tmpUpdateSql = sql_queryTreeFolder;
    tmpUpdateSql.replace(tmpUpdateSql.find("@table"), strlen("@table"), tn);
    tmpUpdateSql.replace(tmpUpdateSql.find("@table"), strlen("@table"), tn);
    std::cout << "tmpUpdateSql=" << tmpUpdateSql << std::endl;
}


// 处理只有一对括号，且需要后面加入大量 (?,?....) 场景
bool ConstructInsertSql( std::string& in_sql ) {
    std::string str_ret;

    std::string cpy_sql = in_sql;

    size_t leftPos = cpy_sql.find('(');
    if (leftPos == std::string::npos) {
//        LOG_WARN("construct left failed!, insql=" << in_sql);
        return false;
    }

    std::string strLeft = cpy_sql.substr(0, leftPos+1);
//    LOG_INFO("construct right strLeft=" << strLeft);

    size_t rightPos = cpy_sql.find(')');
    if (rightPos == std::string::npos) {
//        LOG_WARN("construct right failed!, insql=" << in_sql);
        return false;
    }

    std::string strMid = cpy_sql.substr(leftPos+1, rightPos-leftPos-1);
//    LOG_INFO << "construct right strRight=" << strMid;

    int num = count(strMid.begin(), strMid.end(), ',');
    if (!num) {
//        LOG_WARN("construct count failed!, insql=" << in_sql);
        return false;
    }

    std::string strInsertLeft  = " values ( ";
    std::string strInsertRight = ")";

    for (int i = 0; i <= num; i++ ) {
        if (i == num)
            strInsertLeft += "? ";
        else
            strInsertLeft += "?, ";
    }

    std::string lastRet;
    lastRet = strLeft + strMid + ")" + strInsertLeft + strInsertRight;
//    LOG_INFO << "lastRet=" << lastRet;
    in_sql = lastRet;
    return true;
}


inline void setSqlTable(std::string& sql, const std::string& tableName) {
    boost::algorithm::replace_all(sql, "@table", tableName);
}

void tst_json_parse() {
    {
    std::string strStmt = "INSERT INTO @table (name, permission, uploadSize, totalSize, createdAt, orderNum, isShow, roleType, status) @table VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?) @table @table";

    setSqlTable(strStmt, "IM_FILE");
    std::cout << "strStmt=" << strStmt << std::endl;
        return ;
    }
    {
        std::string sql_addOneUser_ = "insert into @table \
     (userId, username, headImg, createdAt, updatedAt) ";

        bool bCheck = ConstructInsertSql( sql_addOneUser_ );
        if (!bCheck) {
//            LOG_WARN( "addOneUser_ sql construct failed! sql=" << sql_addOneUser_ );
        }
        std::cout << "sql_addOneUser_=" << sql_addOneUser_ << std::endl;
        return ;
    }

    char* pfile = GetSmallFileContent("./tmp.json", NULL);
    const std::string json = pfile;

    rapidjson::Document doc;
    doc.Parse(json.c_str());
    if (doc.HasParseError())
        return ;

    std::string get_string;
    safeGetter<std::string, Document>::get(get_string, doc, "tststring");

    int64_t get_i64 = -1;
    safeGetter<int64_t, Document>::get(get_i64, doc, "tstin64");

    int get_i = -1;
    safeGetter<int, Document>::get(get_i, doc, "tstint");

    std::cout << "get_string=" << get_string << std::endl;
    std::cout << "get_i64=" << get_i64 << std::endl;
    std::cout << "get_i=" << get_i << std::endl;
}


std::tuple<std::string, int> giveName(void)
{
    std::string cw("Caroline");
    int a(2013);
    std::tuple<std::string, int> t = std::make_tuple(cw, a);
    return t;
}

void SetOneValue(int64_t& pVal, int64_t newVal) {
    pVal = newVal;
}
void SetTuple( std::tuple< int64_t, int64_t, int64_t >& onetuple ) {
//    onetuple = std::make_tuple( 987654290, 13123428866, 98755123);

//    std::get<0>(onetuple) = 987654290;
    SetOneValue( (std::get<0>(onetuple)), 987654290);
//    std::get<1>(onetuple) = 13123428866;
    SetOneValue( (std::get<1>(onetuple)), 13123428866);
    std::get<2>(onetuple) = 98755123;

}
void tst_tuple_1() {

    std::tuple< int64_t, int64_t, int64_t > permissionT;
    SetTuple(permissionT);
    std::cout << "index1=" << std::get<0>(permissionT) << std::endl;
    std::cout << "index2=" << std::get<1>(permissionT) << std::endl;
    std::cout << "index3=" << std::get<2>(permissionT) << std::endl;

    return ;
    std::tuple< int, double, std::string > t1(64, 128.001, "name111");
    std::tuple<std::string, std::string, int > t2 = std::make_tuple("v1", "v2", 256);

    size_t num = std::tuple_size<decltype(t1)>::value;
    std::cout << "num=" << num << std::endl;

    //获取第1个值的元素类型
    std::tuple_element<1, decltype(t1)>::type cnt = std::get<1>(t1);
    std::cout << "cnt = " << cnt << std::endl;

    //比较
    std::tuple<int, int> ti(24, 48);
    std::tuple<double, double> td(28.0, 56.0);
    bool b = (ti < td);
    std::cout << "b = " << b << std::endl;

    //tuple作为返回值
    auto a = giveName();
    std::cout << "name: " << get<0>(a)
            << " years: " << get<1>(a) << std::endl;
}

template <typename R, typename... T>
class AbstractSqlHelper {
public:
    explicit AbstractSqlHelper(const std::tuple<T...> params) : params_(params) {}
    typedef R ReturnType;

    virtual void config(sql::PreparedStatement* stmt) = 0; // MUST
    virtual void getValues(sql::ResultSet* rs) { UNUSED(rs); } // OPTIONAL

    std::vector<R> results() const { return results_; }

protected:
    /*const */std::tuple<T...>/*&*/ params_;
    std::vector<R> results_;
};
template <class T>
void test_size(T t)
{
    int a[std::tuple_size<T>::value]; // 可在编译时
    std::cout << "test_size=" << std::tuple_size<T>::value << '\n'; // 或在运行时使用
}

class CQuerySystemHelp_SqlHelper : public AbstractSqlHelper<int, std::map<std::string, std::string> > {
public:
    explicit CQuerySystemHelp_SqlHelper(const std::tuple< std::map<std::string, std::string> >& params) : AbstractSqlHelper(params) {

        Print1();

//        std::cout << "get map beg"  << std::endl;
//        std::map<std::string, std::string> mapInfo = std::get<0>(params_);
//        std::cout << "mapInfo=" << mapInfo.size() << std::endl;
//        std::cout << "get map end"  << std::endl;
    }
    ~CQuerySystemHelp_SqlHelper() {
    }
    void Print1() {
        std::cout << "Print1 beg"  << std::endl;
//        std::cout << "tuplesiz="  << test_size(params_) <<  std::endl;

        std::cout << "get map beg"  << std::endl;
        const std::map<std::string, std::string>& mapInfo = std::get<0>(params_);
        std::cout << "get map end"  << std::endl;

        std::cout << "mapInfo=" << mapInfo.size() << std::endl;
        test_size(params_);

        for (std::map<std::string, std::string>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); it ++) {
            std::cout << "key=" << it->first << ", val=" << it->second << std::endl;
        }
        std::cout << "Print1 end"  << std::endl;
    }
    virtual void config(sql::PreparedStatement* stmt) {

    }
    virtual void getValues(sql::ResultSet* rs) { UNUSED(rs); }
private:
//    const std::tuple<T...>& params_;
};


class BaseClass {
public:
    BaseClass() { std::cout << "BaseClass const" << std::endl; }
    virtual ~BaseClass() { std::cout << "BaseClass dest" << std::endl; }
public:
    virtual void PrintVal() { std::cout << "BaseClass PrintVal function" << std::endl; }
    virtual void Log() { std::cout << "BaseClass Log function" << std::endl; }
};
class DeriveA : public BaseClass {
public:
    DeriveA() { std::cout << "DeriveA const" << std::endl; }
    virtual ~DeriveA() { std::cout << "DeriveA dest" << std::endl; }

public:
//    virtual void PrintVal() { std::cout << "DeriveA PrintVal function" << std::endl; }
//    virtual void Log() { std::cout << "DeriveA Log function" << std::endl; }
    void PrintVal() { std::cout << "DeriveA PrintVal function" << std::endl; }
    void Log() { std::cout << "DeriveA Log function" << std::endl; }

};


#include <iostream>
#include <utility>

void tst_tuple_2() {

    DeriveA  aObj;
    aObj.PrintVal();

    return;
//    auto p1111 = std::make_pair(1, 3.14);
//    std::cout << '(' << std::get<0>(p1111) << ", " << std::get<1>(p1111) << ")\n";
//    std::cout << '(' << std::get<int>(p1111) << ", " << std::get<double>(p1111) << ")\n";
//    return ;

    std::map<std::string, std::string> map_Param;
    map_Param["111"] = "1aaa";
    map_Param["222"] = "2bbb";

    CQuerySystemHelp_SqlHelper helper(std::make_tuple(map_Param));
    helper.Print1();

    std::cout << "last map_Param=" << map_Param.size() << std::endl;

}

void genRandom64() {
    AtomicInt64 s_seed;
    int64_t itmp = s_seed.addAndGet(1);
    std::cout << "tmp=" << itmp <<std::endl;
}

void string_substr_1() {

    std::string fileReMark = "1234-777";
    if (fileReMark.size()) {
        size_t rPos = fileReMark.rfind("-");
        if (rPos != std::string::npos) {
            fileReMark = fileReMark.substr(0, rPos);
            std::cout << "fileReMark=" << fileReMark << std::endl;
//            retFileBean.fileReMark = (fileReMark + "-" + retFileBean.fileName);
        } else {
//            retFileBean.fileReMark = retFileBean.fileName;
        }
    }

}

void tst_xx( int iIdx, int iCpyIdx ) {
    std::cout << "iIdx=" << iIdx << " iCpyIdx=" << iCpyIdx << std::endl;
}


void tst_sprintf() {
    std::vector < int > v_ints;
    v_ints.push_back( 23 ); v_ints.push_back( 21 ); v_ints.push_back( 3 ); v_ints.push_back( 34 );
    std::cout << "begin=" << *(v_ints.begin()) << std::endl;
    std::cout << "front=" << (v_ints.front()) << std::endl;

    return ;

    std::string tst_string;
//    std::cout << tst_string.size() ? "no null string" : "null string" << std::endl;

    int v1 = 123;
    int v2 = 456;
    std::cout << (tst_string.size() == 0 ? v1 : v2) << std::endl;
    std::cout << std::endl;

#define PUSH_MESSAGE_SUCCESS 1
#define PUSH_MESSAGE_ERROR   22
    bool bExec = false;
    std::cout << (bExec ? PUSH_MESSAGE_SUCCESS : PUSH_MESSAGE_ERROR) << std::endl;
    std::cout << std::endl;
    return ;

    char tmpbuff[2048] = {0};
    sprintf( tmpbuff, "1235=%s", "" );
    std::cout << "tmpbuffer=" << tmpbuff << std::endl;
}


/*
description：
输入两个整数n和sum，从数列1，2，3.......n 中随意取几个数，使其和等于sum，要求将其中所有的可能组合列出来。

方法1：
注意到取n，和不取n个区别即可，考虑是否取第n个数的策略，可以转化为一个只和前n-1个数相关的问题。
如果取第n个数，那么问题就转化为"取前n-1个数使得它们的和为sum-n"，对应的代码语句就是sumOfkNumber(sum - n, n - 1)；
如果不取第n个数，那么问题就转化为"取前n-1个数使得他们的和为sum"，对应的代码语句为sumOfkNumber(sum, n - 1)。

<<编程之法>>
参考：https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/02.03.md
author: JasonZhou
date：  2016-03-15
*/

#include<iostream>
#include <list>
using namespace std;

list<int>list1;
void SumOfkNumber(int sum, int n) {
    // 递归出口
    if (n <= 0 || sum <= 0) return;

    // 输出找到的结果
    if (sum == n) {
        // 反转list
        list1.reverse();
        for (list<int>::iterator iter = list1.begin(); iter != list1.end(); iter++)
            cout << *iter << " + ";
        cout << n <<endl;
        list1.reverse();//此处还需反转回来
    }

    list1.push_front(n);      //典型的01背包问题
    SumOfkNumber(sum - n, n - 1);   //“放”n，前n-1个数“填满”sum-n
    list1.pop_front();
    SumOfkNumber(sum, n - 1);     //不“放”n，n-1个数“填满”sum
}
void tst_sum_1() {
    int n=9, sum=10;
    SumOfkNumber(sum, n);
}

int main(int argc, char *argv[])
{
    std::setlocale(LC_ALL, "en_US.utf8");
    Logger::setLogLevel(Logger::DEBUG);
//    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    tst_tuple_2(); return 1;

    tst_sum_1(); return 1;

    tst_sprintf();
//    int tmpIn = 1;
//    tst_xx(tmpIn++, tmpIn);
//    tst_xx(tmpIn++, tmpIn);
    return 1;

    string_substr_1(); return 1;

    int64_t tmpVal = -1;
    if (tmpVal > 0) {
        std::cout << "tmpVal > 0, tmpVal=" << tmpVal << std::endl;
    } else {
        std::cout << "tmpVal <= 0, tmpVal=" << tmpVal << std::endl;
    }
    return 1;


    OneFile tstfile;
    tst_typeid(tstfile); return 1;

    tst_wkPtr_use(); return 1;

//    tst_sf_1(); return 1;

    tst_stock_f_1(); return 1;


    tst_tuple_1(); return 1;

    tst_json_parse(); return 1;


    tst_str_2(); return 1;

    my_weak_ptr(); return 1;

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












