

#include "c11fun.h"



#include <unordered_set>
#include <boost/circular_buffer.hpp>


#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield


#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

#include "muduo/base/ThreadPool.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>




///////////////////////////////////


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;



#include <cstdio>
#include <cstdlib>
#include <functional>

#include <atomic>

using namespace std;
using namespace muduo;
using namespace muduo::net;

using namespace boost;


enum enum_Minganci {
    E_UNKNOWN = 0,
    E_MINGANCI_AAA,
    E_MINGANCI_BBB,
    E_MINGANCI_CHANGE,
    E_MINGANCI_SWITCH

};

// 模拟golang的defer实现

void test_cpp() {
    void *p = malloc(1000);
    printf ( "new memory addr=%p\n", p );
    ON_SCOPE_EXIT {
        printf( "free memory addr=%p\n", p);
        free(p);
    };
}



class defer {
public:
    defer(const boost::function < void() >& deferFunc) :
        m_defer_func(deferFunc) {

    }
    ~defer() {
        m_defer_func();
//        m_defer_func = nullptr;
    }
    boost::function < void() > m_defer_func;
};



void tst_stl_1() {
    std::cout << "true=" << true << "&false=" << false << std::endl;

    std::list<int> mylist;
    for (int i=0; i<10; i++) mylist.push_back (i*10);

    PrintInContainer(mylist);

    std::list<int>::iterator it = mylist.begin();

    std::advance (it,5);
    std::cout << "The sixth element in mylist is: " << *it << '\n';

    std::advance (it,-1);
    std::cout << "The fifth element in mylist is: " << *it << '\n';
}

// 测试std::list splice 拼接功能
void tst_list_splice() {
    std::list<int> mylist1, mylist2;
    std::list<int>::iterator it;

    // set some initial values:
    for (int i=1; i<=4; ++i)
       mylist1.push_back(i);      // mylist1: 1 2 3 4

    for (int i=1; i<=3; ++i)
       mylist2.push_back(i*10);   // mylist2: 10 20 30

    {
        auto tmp_it = mylist1.begin();// 1 2 3 4
        std::advance(tmp_it, 2);
        std::cout << "tmp_it=" << *tmp_it << std::endl;// tmp_it: 3
    }

    it = mylist1.begin();
    ++it;                         // points to 2

//    mylist1.splice(it, mylist2);
//    PrintInContainer(mylist1);// mylist1: 1 10 20 30 2 3 4

    mylist1.splice(mylist1.end(), mylist1, it);// 1 3 4 2
    PrintInContainer(mylist1);// 1 3 4 2

}

class Connect {
public:
    Connect() {
        printf( "connect cst\n" );
    }
    ~Connect() {
        printf( "connect dst\n" );
    }
    void outputConnectInfo() {
        printf( "out put connection info\n" );
    }
private:
};
class ConnectEntry {
public:
    ConnectEntry(const boost::weak_ptr<Connect>& inweakConn)
        : weak_Conn_(inweakConn) {
        printf( "ConnectEntry cst\n" );
    }
    ~ConnectEntry() {
        printf( "ConnectEntry dst\n" );
    }
    void output() {
        boost::shared_ptr<Connect> usePtr = weak_Conn_.lock();
        if (usePtr) {
            printf( "usePtr is valid ptr\n" );
            usePtr->outputConnectInfo();
        } else {
            printf( "usePtr is invalid ptr\n" );
        }
    }
private:
    boost::weak_ptr<Connect> weak_Conn_;
};

// weak_ptr 负责观察 后者 shared_ptr，如果后者被析构，则前者能通过lock的返回发现是否被析构
void tst_shared_weak_ptr() {
    ConnectEntry* tstObj;
    {
        boost::shared_ptr<Connect> newConnect(new Connect);
        tstObj = new ConnectEntry(newConnect);
        tstObj->output();
    }
    printf ( "----------------------------------\n" );
    tstObj->output();

}

void tst_set_fun() {
#if 0
{
//    typedef std::unordered_set<Connect> Bucket;
//    Connect cNewConn;

//    std::unordered_set<Connect> setbuket;
}
#endif

//    typedef std::unordered_set<Connect*> Bucket;
//    Connect* pNewConn = new Connect;

//    Bucket onebt;
//    onebt.insert(pNewConn);

//    {
//        onebt.insert(pNewConn);
//    }

//    std::cout << "size=" << onebt.size() << std::endl;
}

void tst_113( int inval ) {
    std::cout << "inval=" << inval << std::endl;
    inval = 1923;
    std::cout << "new inval=" << inval << std::endl;
}

Connect GetOneConnect() {
    Connect newConnObj;
    return newConnObj;
}
void tst_Ret_cpy_construct() {
    Connect recvConn = GetOneConnect();
}


class CB;
class CA {
public:
    ~CA() { printf( "CA destroy\n" ); }
public:
    boost::shared_ptr<CB> m_ptrB;
};

class CB {
public:
    ~CB() { printf( "CB destroy\n" ); }
public:
    boost::shared_ptr<CA> m_ptrA;
};

void tst_shared_ptr_1() {
    boost::shared_ptr<CA> a_obj(new CA);
    boost::shared_ptr<CB> b_obj(new CB);

//    a_obj->m_ptrB = b_obj;
//    b_obj->m_ptrA = a_obj;
//    std::cout << "all done" << std::endl;

    std::cout << "a_obj->m_ptrB count=" << a_obj.use_count() << std::endl;
    std::cout << "b_obj->m_ptrA count=" << b_obj.use_count() << std::endl;
    a_obj = NULL;
    b_obj = NULL;
}

typedef std::shared_ptr<Connect>            SP_ConnectPtr;
typedef std::unordered_set<SP_ConnectPtr>   setBucket;

setBucket setObjs;

void InsertObj( const SP_ConnectPtr inObj ) {
    setObjs.insert(inObj);
}

void tst_ref_count() {

    SP_ConnectPtr p1(new Connect);

    printf( "p1 usecount=%d\n", p1.use_count() );

    InsertObj(p1);

    printf( "p1 usecount=%d\n", p1.use_count() );

}
// 测试 weak_ptr 作为 shared_ptr 对象的观察者
// weak_ptr  shared_ptr 互相引用，使用
void tst_ref_count_1() {
    auto sp1 = boost::make_shared<Connect>();
    printf( "sp1 usecount=%d\n", sp1.use_count() );
    boost::weak_ptr<Connect> wp1(sp1);
    printf( "wp1 usecount=%d\n", wp1.use_count() );

    sp1 = nullptr;
    if (wp1.expired())
        std::cout << "wp1 is expired" << std::endl;
}
class self_shared: public boost::enable_shared_from_this<self_shared>
{
public:
    self_shared(int n):x(n){
        printf( "construct self_shared obj, this=%p\n", this );
    }
    ~self_shared() {
        printf( "destroy self_shared obj, this=%p\n", this );
    }

    int x;
    void print()
    {std::cout << " self_shared:" << x << std::endl;}
};
typedef boost::shared_ptr<self_shared> self_sharedLPtr;


boost::shared_ptr<Connect> getone_ConnectObj() {
    boost::shared_ptr<Connect> tmpPtr(new Connect());
    std::cout << "tmpPtr count=" << tmpPtr.use_count() << std::endl;
    return tmpPtr;
}

self_sharedLPtr getone_selstObj() {
    self_sharedLPtr oneObjPtr(new self_shared(9527));
    return oneObjPtr;
}

void tst_ref_count_2() {
    boost::shared_ptr<self_shared> sp1 = boost::make_shared<self_shared>(313);
    sp1->print();

    boost::shared_ptr<self_shared> p_cpy = sp1->shared_from_this();
    std::cout << "sp1 count=" << sp1.use_count() << std::endl;
    std::cout << "p_cpy count=" << p_cpy.use_count() << std::endl;
    p_cpy->print();
}
void tst_ref_count_3() {
//    self_sharedLPtr returnPtr = getone_selstObj();
//    std::cout << "returnPtr count=" << returnPtr.use_count() << std::endl;

    boost::shared_ptr<Connect> oneConnPtr = getone_ConnectObj();
    std::cout << "oneConnPtr count=" << oneConnPtr.use_count() << std::endl;
}

boost::unordered_map < uint64_t, boost::shared_ptr< Connect > > id_ctx_map_;

void insert_ctx( const boost::shared_ptr<Connect>& inPtr ) {
//    Connect* pfake = nullptr;
//    id_ctx_map_.insert( std::make_pair(1, pfake) );
    id_ctx_map_.insert( std::make_pair(1, inPtr) );
//    tst_map.insert(std::make_pair(1, 1));
}
void tst_ref_count_4() {
    Connect* newConnPtr = new Connect();

    boost::shared_ptr<Connect> tmp_sp1(newConnPtr);
    std::cout << "before tmp_sp1 usercount=" << tmp_sp1.use_count() << std::endl;
    insert_ctx( tmp_sp1 );
    std::cout << "after tmp_sp1 usercount=" << tmp_sp1.use_count() << std::endl;

    getchar();
}

#include <boost/atomic.hpp>
#include <atomic>

using namespace std;

atomic_uint64_t  g_tstatomic;// 此处 用的 其实是 boost 内的 原子 类型
std::atomic<std::uint64_t> g_ucount;// 此处 用的 才是真正c11内置 atmoic 类型

void impl_add() {
    for ( auto i = 0 ; i < 100 ; i ++ )
        g_ucount ++ ;
}
void tst_atomic( ) {
    std::vector < std::thread* > v_threads;
    for (int i = 0; i < 4; ++ i) {
        v_threads.push_back( new std::thread(&impl_add) );
    }
    for (int i = 0; i < 4; ++ i) {
        v_threads[i]->join();
    }
    std::cout << "all thread work done" << std::endl;
    std::cout << "g_ucount = " << g_ucount << std::endl;
}

void tst_c11fun_entry() {
    OutputDbgInfo tmpOut( "tst_c11fun_entry begin", "tst_c11fun_entry end" );

    tst_atomic(); return;
    tst_ref_count_4(); return;
    tst_ref_count_3(); return;
    tst_ref_count_2(); return;
    tst_ref_count_1(); return;
    tst_ref_count(); return;

    tst_shared_weak_ptr(); return;

    tst_shared_ptr_1(); return;

    tst_Ret_cpy_construct(); return;

    tst_113(1023); return;

    tst_set_fun(); return ;


    tst_list_splice();
    return ;


    std::cout << " E_MINGANCI_AAA=" << E_MINGANCI_AAA << std::endl
              << " E_MINGANCI_BBB=" << E_MINGANCI_BBB << std::endl
              << " E_MINGANCI_CHANGE=" << E_MINGANCI_CHANGE << std::endl
              << " E_MINGANCI_SWITCH=" << E_MINGANCI_SWITCH << std::endl;
return;

    boost::shared_ptr<TMemoryBuffer> buf(new TMemoryBuffer(4*1024*1024));
    buf->getWritePtr(2);
    buf->wroteBytes(2);

    boost::shared_ptr<TProtocol> bp(new TBinaryProtocol(buf));
    buf->getWritePtr(4);
    buf->wroteBytes(4);

    uint32_t    cur_body_len  = 1;
    uint8_t*    pData     = NULL;
    uint32_t    tmpsize   = 0 ;
    buf->getBuffer( &pData, &tmpsize );

    std::cout << "pData=" << pData << ", tmpsize=" << tmpsize << std::endl;

return ;

    tst_stl_1();
    return;


    FILE* file1 = fopen( "file1.txt", "wb" );
    defer releaseFile = boost::function < void() >([&]()
    {
        fclose(file1);
        printf("closed file1\n");
//        getchar();
    });
    if (file1) {
        fprintf(file1, "write data file1 for test\n");
    } else {
        printf( "openfile file1 failed!!!\n" );
        exit(1);
    }

//    for ( auto i = 1; i < 10 ; ++ i ) {
        test_cpp();
        test_cpp();
//    }


        FILE* file2 = fopen( "file2.txt", "wb" );
        ON_SCOPE_EXIT {
            fclose(file2);
            printf("closed file2\n");
//            getchar();
        };
        if (file2) {
            fprintf(file2, "file2 for test\n");
        } else {
            printf( "openfile file2 failed!!!\n" );
            exit(1);
        }
}


