

#include "c11fun.h"



#include <unordered_set>
#include <boost/circular_buffer.hpp>


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


using namespace std;
using namespace muduo;
using namespace muduo::net;



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
class TestEntry {
public:
    TestEntry(const boost::weak_ptr<Connect>& inweakConn)
        : weak_Conn_(inweakConn) {
        printf( "TestEntry cst\n" );

    }
    ~TestEntry() {
        printf( "TestEntry dst\n" );
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

    TestEntry* tstObj;
    {
        boost::shared_ptr<Connect> newConnect(new Connect);

        tstObj = new TestEntry(newConnect);

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
void tst_c11fun_entry() {
    OutputDbgInfo tmpOut( "tst_c11fun_entry begin", "tst_c11fun_entry end" );

    tst_set_fun(); return ;

    tst_shared_weak_ptr();
    return ;

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


