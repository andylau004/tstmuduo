

#include "c11fun.h"



#include <unordered_set>


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
#include <functional>
#include <unordered_map>

#include <memory>

#include <boost/atomic.hpp>
#include <boost/circular_buffer.hpp>

#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <boost/circular_buffer.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/any.hpp>

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


#include <thrift/async/TAsyncChannel.h>
#include <thrift/async/TEvhttpClientChannel.h>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>


#include "tstbacktrace.h"
#include "branch_predict.h"
#include "muduo/base/BlockingQueue.h"
#include "muduo/base/BoundedBlockingQueue.h"


#include "boostany.h"
#include "threadsepcific.h"



///////////////////////////////////


using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;


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

//    class Connect : public std::enable_shared_from_this<Connect> {
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
    std::shared_ptr < Connect > GetPtr() {
        return std::shared_ptr < Connect > ( this );
//        return shared_from_this();
    }
    std::shared_ptr < Connect > BadGetPtr() {
        return std::shared_ptr < Connect > ( this );
//        return shared_from_this();
    }
private:
};
using ConnectPtr = std::shared_ptr< Connect >;

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


std::shared_ptr<CA> getItem() {
    std::shared_ptr<CA> item(new CA);
    return item;
}


class Bad : public std::enable_shared_from_this < Bad > {
public:
    Bad() { std::cout << "Bad()" << std::endl; }
    ~Bad() { std::cout << "~Bad()" << std::endl; }
    std::shared_ptr<Bad> getPtr() {
//        return std::shared_ptr<Bad>(this);
        return shared_from_this();
    }
};

int xx1() {
    std::shared_ptr<Bad> bp1(new Bad);
    std::shared_ptr<Bad> bp2 = bp1->getPtr();
    std::cout << "bp2.use_count: " << bp2.use_count() << std::endl;
    return 0;
}

void tst_shared_ptr_2() {

    {
        ConnectPtr cObj = std::make_shared<Connect>();
        std::cout << "cObj usecount=" << cObj.use_count() << std::endl;
        cObj.reset();
        std::cout << "cObj usecount=" << cObj.use_count() << std::endl;
    }
    return;

    xx1();return ;
    {
        ConnectPtr  objC( new Connect );
        ConnectPtr  objD = objC->GetPtr();
        std::cout << "objD usecount=" << objD.use_count() << std::endl;
    }

//    std::shared_ptr < Connect > objC( new Connect );
//    std::shared_ptr < Connect > p23 = objC;
//    std::cout << "objC usecount=" << objC.use_count() << std::endl;

    return;

    int ival = 10;

    std::shared_ptr< int > pobj = std::make_shared< int > (ival);
    std::shared_ptr< int > p2 = pobj;
    std::cout << "pobj usecount=" << pobj.use_count() << std::endl;
    return;

    std::shared_ptr<CA> item = getItem();
    std::cout << "item usecount=" << item.use_count() << std::endl;
}

typedef std::shared_ptr<Connect>            SP_ConnectPtr;
typedef std::unordered_set<SP_ConnectPtr>   setBucket;

setBucket setObjs;

void InsertObj( const SP_ConnectPtr inObj ) {
    setObjs.insert(inObj);
}

void tst_ref_count() {

    SP_ConnectPtr p1(new Connect);

    printf( "p1 usecount=%lu\n", p1.use_count() );

    InsertObj(p1);

    printf( "p1 usecount=%lu\n", p1.use_count() );

}
// 测试 weak_ptr 作为 shared_ptr 对象的观察者
// weak_ptr  shared_ptr 互相引用，使用
void tst_ref_count_1() {
    auto sp1 = boost::make_shared<Connect>();
    printf( "sp1 usecount=%lu\n", sp1.use_count() );
    boost::weak_ptr<Connect> wp1(sp1);
    printf( "wp1 usecount=%lu\n", wp1.use_count() );

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

// --------------------------------------------------------
// 测试左值 右值
void process_val(int& in_val) {
    std::cout << "leftVal Process=" << in_val << std::endl;
}
void process_val(int&& in_val) {
    std::cout << "rightVal Process=" << in_val << std::endl;
}
void tst_rval_lval() {
    int a = 0;
    process_val(a);
    process_val(1);
}
// --------------------------------------------------------

// --------------------------------------------------------
// 测试移动语义
const int max_arry_size = 500;
class CTestMove {
public:
    CTestMove& operator=(const CTestMove& other) {
        LOG_INFO << "assign constructor";
        if (i_array==nullptr) {
            i_array = new int[max_arry_size];
        }
//        LOG_INFO << "this.i_array=" << this->i_array << ", other.i_array=" << other.i_array;
        memset(i_array, 0, max_arry_size*sizeof(int));
        memcpy(i_array, other.i_array, max_arry_size*sizeof(int));
        return *this;
    }
public:
    int* i_array = nullptr;
public:
    CTestMove() : i_array( new int[max_arry_size] {1, 2, 3, 4} ) {
        LOG_INFO << "default constructor, this=" << this << ", i_array=" << i_array;
    }
    CTestMove(const CTestMove& other) {
        LOG_INFO << "copy constructor, this=" << this << ", other=" << &other;
        if (i_array==nullptr) {
            i_array = new int[max_arry_size];
        }
//        LOG_INFO << "this.i_array=" << this->i_array << ", other.i_array=" << other.i_array;
        memset(i_array, 0, max_arry_size*sizeof(int));
        memcpy(i_array, other.i_array, max_arry_size*sizeof(int));
    }
    CTestMove(CTestMove&& t) : i_array(t.i_array) {
        this->i_array = t.i_array;
        LOG_INFO << "move    constructor, this=" << this << ", i_array=" << this->i_array;
        t.i_array = nullptr;
    }
    ~CTestMove() {
        LOG_INFO << "~destructor, this=" << this;
        if (i_array) {
            LOG_INFO << "delete i_arrry=" << this->i_array;
            delete []i_array;
        }
    }
    void PrintInfo() {
    }
};
void tst_mov_yuyi() {
//    CTestMove t1;
//    CTestMove t2 = t1;

    CTestMove t1;
    CTestMove duplicated( std::move(t1) );
    LOG_INFO << "t1.i_array=" << t1.i_array;
    LOG_INFO << "duplicated.i_array=" << duplicated.i_array;
}

CTestMove CreateNewTest() {
    OutputDbgInfo tmpout( "----------------CreateNewTest beg----------------", "----------------CreateNewTest end----------------" );
    CTestMove onetm;
    return onetm;
}

// --------------------------------------------------------

class A
{
public:
    A() : array(new int[3]{1, 2, 3})
    {
    }
    ~A()
    {
        if(nullptr != array)
        {
            delete [] array;
        }
    }
    A(const A& a)
    {
        std::cout << "Copy Construct" << endl;
    }
public:
    int *array{nullptr};
};

void tst_move_1() {
    std::string str = "hello";
    std::vector < std::string > v;
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";

    v.push_back(std::move(str));
    std::cout << "After copy, str is \"" << str << "\"\n";
    std::cout << "The contents of the vector are \"" << v[0] << "\", \"" << v[1] << "\"\n";
}

void recvCall(const char* pfnName) {
    std::cout << "fname=" << pfnName << std::endl;
}

#define NOTIFY_PROCESS(fname, customParam) \
    recvCall(#fname);



// 设计一个不能被继承的类
class Base_Uninherit {
private:
    // 简单构造函数
    Base_Uninherit() {
    }
    // 拷贝构造函数
    Base_Uninherit(const Base_Uninherit& rhs) {

    }
    Base_Uninherit& operator=(const Base_Uninherit& rhs) {
        return *this;
    }

public:
    ~Base_Uninherit() {
        printf( "~Base_Uninherit dst\n" );
    }
    static Base_Uninherit* constructObj() {
        Base_Uninherit* newObj = new Base_Uninherit();
        return newObj;
    }
};

//class DeriveA : public Base_Uninherit {
//public:
//    DeriveA() {
//        printf( "DeriveA cst\n" );
//    }
//    ~DeriveA() {
//        printf( "~DeriveA dst\n" );
//    }
//};

//Base_Uninherit objfactory() {
//    return Base_Uninherit();
//}

void tst_youzhi() {
//    std::cout << "before copy constructor..." << std::endl;
//    Base_Uninherit oneObj = objfactory();
//    std::cout << "after copy constructor..." << std::endl << std::endl;

//    Base_Uninherit&& obj2 = objfactory();
//    std::cout << "life time ends!" << std::endl << std::endl;

    {
        Base_Uninherit* pObj = Base_Uninherit::constructObj();
    }
}


class TestThread {
public:
    TestThread() : m_val(2024)/*, cThrdObj()*/ {
//    TestThread() : m_val(1024), cThrdObj(print, this) {

        m_pfnCb = std::bind(&TestThread::print, this);
    }
    void start() {
        std::thread cThrdObj(m_pfnCb);
        cThrdObj.join();
//        ::sleep(3);
    }
    ~TestThread() {
        printf( "xigou testthread\n" );
    }
    static void print(void* arg) {
        TestThread* pThis = (TestThread*)arg;

        printf( "111 hello everybody!!m_val=%d\n", pThis->m_val );
    }
private:
    typedef std::function<void()> ThreadCb;
    ThreadCb    m_pfnCb;

//    std::thread cThrdObj;
    int m_val;
};
void tstThrdWork() {
    TestThread ct;
    ct.start();
    sleep( 2 );
}

void GetFileNameFromDir(const char* rootPath)
{
    boost::filesystem::path dir(rootPath);
    if (boost::filesystem::exists(dir)) // 判断路径是否存在
    {
        boost::filesystem::directory_iterator itEnd;
        boost::filesystem::directory_iterator itDir(dir);
        std::string fileName("");
        for (; itDir != itEnd; itDir++) // 遍历路径下所有文件
        {
            fileName = itDir->path().string();
            if (boost::filesystem::is_directory(fileName.c_str()))
                std::cout << "is dir, val=" << fileName.c_str() << std::endl;
            else {
                std::cout << "is file, val=" << fileName.c_str() << std::endl;
            }
        }
    }
}



void GetCurSecond() {
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        printf("second:%ld\n",tv.tv_sec);  //秒
        printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
        printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒

        sleep(3); // 为方便观看，让程序睡三秒后对比
        std::cout << "3s later:" << std::endl;

        gettimeofday(&tv,NULL);
        printf("second:%ld\n",tv.tv_sec);  //秒
        printf("millisecond:%ld\n",tv.tv_sec*1000 + tv.tv_usec/1000);  //毫秒
        printf("microsecond:%ld\n",tv.tv_sec*1000000 + tv.tv_usec);  //微秒

    }
    struct timeval tv;
    char buf[1024] = {0};

    gettimeofday(&tv, NULL);
    strftime(buf, sizeof(buf)-1, "%Y-%m-%d %H:%M:%S", localtime(&tv.tv_sec));
//    printf( "buf=%s\n", buf );
    fprintf(stderr, "%s.%03d\n", buf, (int)(tv.tv_usec / 1000));
}


void tst_getline() {

    FILE* fp = nullptr;
    char* line = nullptr;
    ssize_t read = 0;
    size_t len = 0;

    fp = fopen("tstc11", "r");
    if (!fp) {
        return;
    }

    while ( (read = getline(&line, &len, fp)) != -1 ) {
//        printf("read=%d line=%s\n", len, line);
//        free(line);
    }

    free(line);
    fclose(fp);
}


class Test {
public:
    Test(int a){
        p = new int(a);
        std::cout << "default ctor: " << *p << std::endl;
    }
    Test(const Test &other) {
        std::cout << "this is left-ref copy ctor: " << *other.p << std::endl;
        p = new int(*other.p);
    }
    Test(Test &&other) {
        std::cout << "this is move copy ctor: " << std::endl;
        p = other.p;
        other.p = nullptr;
    }
    ~Test(){
        std::cout << "this is dstor" << std::endl;
        if (p) {
            delete p;
            p = nullptr;
        }
    }
    void print() const {
        if(p == nullptr)
        {
            std::cout<<"null"<<std::endl;
            return;
        }
        std::cout<<p<<":"<<*p<<std::endl;
        return;
    }
protected:
    int *p;
};

class President
{
public:
    std::string name;
    std::string country;
    int year;

    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year) {
        std::cout << "I am being constructed.\n";
    }
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year) {
        std::cout << "I am being moved.\n";
    }
    President& operator=(const President& other) = default;
};

void tstLeftRightValue_2() {
//    ::sleep(10);
    std::cout << "beg work\n";
    std::vector <President > v1;
    std::cout << "\nemplace_back:\n";
    v1.emplace_back("xijinping", "china", 66);

    std::vector<President> v2;
    std::cout << "\npush_back:\n";
    v2.push_back(President("sbtrump", "usa", 73));

    std::cout << "\nContents:\n";
    for (auto const& president: v1) {
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
    for (auto const& president: v2) {
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
}
void tstLeftRightValue_1() {
    Test t1(123);
    Test t2(456);
    Test t3(789);

    std::list<Test> v;
    v.push_back(std::move(t1));
    v.push_back(std::move(t2));
    v.push_back(std::move(t3));

    for(auto it = v.begin(); it != v.end(); ++it) {
        std::cout<<"address:"<<&(*it)<<std::endl;
        it->print();
    }
}

void threadfunc(void) {
    while (1) {
        std::cout << "i am a thread\n";
        sleep(3);
    }
}

int tstC11Thrd(void) {
    std::thread t1(threadfunc);
    t1.join();
    return 0;
}


//#define STR(s)     #s
//#define CONS(a,b)  int(a##e##b)

//int tstJing()
//{
//    printf(STR(vck));           // 输出字符串"vck"
//    printf("%d/n", CONS(2,3));  // 2e3 输出:2000
//    return 0;
//}



#define   FUNC2(x,y)   x##y
#define   FUNC1(x,y)   FUNC2(x,y)
#define   FUNC(x)      FUNC1(x, __COUNTER__)

//相当于int my_unique_prefix0;
//int   FUNC(my_unique_prefix);
//  //相当于int my_unique_prefix1;
//int   FUNC(my_unique_prefix);
// //相当于int my_unique_prefix2;
//int   FUNC(my_unique_prefix);

//void notgood() {
//    my_unique_prefix0 = 0;
//    my_unique_prefix1 = 10;
//    printf("\n%d\n",my_unique_prefix0);
//    printf("%d\n",my_unique_prefix1);
//    my_unique_prefix0++;
//    printf("%d\n",my_unique_prefix0);
//    printf("%d\n", __COUNTER__);
////    printf("my_unique_prefix2=%d\n",my_unique_prefix2);
//}

void call_over(bool bval) {
    printf("call_over bval=%d\n", bval);
}
void one_way(bool bval) {
    printf("one_way bval=%d\n", bval);
}

void tstDefer() {
    typedef boost::shared_ptr<President>        PresidentPtr;
    typedef boost::unordered_set<PresidentPtr>  Bucket;
    {
        tst_boost_any();
        return;
    }
//    {
//        President* obj1 = new President("name1", "usa", 72);
//        boost::shared_ptr<President> p1(obj1);
//        {
//            Bucket obj_bucket;
//            obj_bucket.insert(p1);
//            LOG_INFO << "a p1 usecount=" << p1.use_count();
//        }
//        LOG_INFO << "b p1 usecount=" << p1.use_count();
//        return;
//    }
    {
        President* obj1 = new President("name1", "usa", 72);
        boost::shared_ptr<President> p1(obj1);
        boost::shared_ptr<President> p2(obj1);
        if (p1 == p2) {// 两个 shared_ptr 封装了一个内存对象，这两个shared_ptr对象比较时，认为相同；
            LOG_INFO << "p1 == p2";
        } else {
            LOG_INFO << "p1 != p2";
        }

        Bucket obj_bucket;
        obj_bucket.insert(p1);
        obj_bucket.insert(p2);

        LOG_INFO << "bkt size=" << obj_bucket.size();
        getchar();
    // output:
    // 20200704 07:47:54.115630Z 38579 INFO  p1 == p2 - c11fun.cpp:887
    // 20200704 07:47:54.115696Z 38579 INFO  bkt size=1 - c11fun.cpp:898
        return;
    }

    int val = EV_READ|EV_PERSIST;
    LOG_INFO << val;
    return;

    bool bval = false;
    DeferFunctor pfnExit = boost::function < void() >([&]() {
        call_over(bval);
        one_way(bval);
    });

    if (0) {
        bval = true;
    } else {
        bval = false;
    }

    printf("exec some code\n");
    return ;

//    notgood();
//    return;
//    CHECK_TIME1(tstinterval);

    deferTime([&]() {
        LOG_INFO << "tst output 111";
    });

//    LOG_INFO << " __COUNTER__=" << __COUNTER__;

//    LOG_INFO << CONCATENATE(__FUNCTION__, __LINE__);
    LOG_INFO << " before out";

    {
        deferTime([&]() {
            LOG_INFO << "tst output 222";
        });

        for ( int i = 0; i < 10 ; i ++ ) {
            ::usleep( 1000 );
        }
//        deferTime([&]() {
//            LOG_INFO << "tst output 333";
//        });
    }
    LOG_INFO << "after out";
}


void tst_align() {

//    struct S1 {short f[3];};
//    struct S2 {short f[3];} __attribute__((aligned(64)));
//    struct S5 {short f[40];} __attribute__((aligned(64)));
//    fprintf(stdout, "S1 size: %d, S2 size: %d, S5 size: %d\n",
//        sizeof(struct S1), sizeof(struct S2), sizeof(struct S5)); // 6, 64, 128
//    return;

//    typedef int more_aligned_int __attribute__((aligned(16)));
//    fprintf(stdout, "aligned: %d, %d\n", alignof(int), alignof(more_aligned_int)); // 4, 16

//    struct S3 {more_aligned_int f;};
//    struct S4 {int f;};
//    fprintf(stdout, "S3 size: %d, S4 size: %d\n", sizeof(struct S3), sizeof(struct S4)); // 16, 4

//    int arr[2] __attribute__((aligned(16))) = {1, 2};
//    fprintf(stdout, "arr size: %d, arr aligned: %d\n", sizeof(arr), alignof(arr)); // 8, 16

//    struct S6 {more_aligned_int f;} __attribute__((packed));
//    fprintf(stdout, "S6 size: %d\n", sizeof(struct S6)); // 4

//    char c __attribute__((aligned(16))) = 'a';
//    fprintf(stdout, "c size: %d, aligned: %d\n", sizeof(c), alignof(c)); // 1, 16

//    struct S7 {double f;} __attribute__((aligned(4)));
//    fprintf(stdout, "S7 size: %d, algined: %d\n", sizeof(struct S7), alignof(struct S7)); // 8, 8

//    struct S8 {double f;} __attribute__((__aligned__(32)));
//    fprintf(stdout, "S8 size: %d, algined: %d\n", sizeof(struct S8), alignof(struct S8)); // 32, 32

}

//__thread unsigned int some_count = 1;
thread_local unsigned int some_count = 1;
std::mutex count_mtx;

void thread_func(const std::string& thread_name) {
    ++some_count;
    std::lock_guard<std::mutex> lock(count_mtx);
    size_t findPos = thread_name.find("aaa");
    if (findPos == std::string::npos) {
        some_count = 1024;
    } else {
        some_count = 4096;
    }
    LOG_INFO << thread_name << ", addr=" << &some_count << ", val=" << some_count;
}
void tst_thrd_local() {
    DeferFunctor pfnExit = boost::function < void() >([&]() {
        LOG_INFO << ("tst_thrd_local function done -----------------");
    });

    std::thread t1(thread_func, " thrd aaa"), t2(thread_func, " thrd bbb");
    {
        std::lock_guard< std::mutex> lock(count_mtx);
        LOG_INFO << "main thrd, addr=" << &some_count << ", val=" << some_count;
    }
    t1.join();t2.join();
}

void func(std::function<void(const std::string& src)> cob, std::string& str) {
    std::cout << "str1111=" << str << "\n";
    cob(str);
}

std::function<void(const std::string& src)> pfff1;

void tst_stdmove() {
    std::string echo_message = "123";
    int i = 1;

//    func([&i, &echo_message] (const std::string& srcmsg) {
////    func([&i, echo_message = std::move(echo_message)] (const std::string& srcmsg) {
//        std::cout << "msg=" << echo_message.c_str() << "\n";
//        std::cout << "srcmsg=" << srcmsg.c_str() << "\n";
//    }, echo_message);
//    return;

    pfff1 = [&i, &echo_message] (const std::string& srcmsg) {
//        auto pfn1 = [&i, echo_message = std::move(echo_message)] (std::string& srcmsg) {
        std::cout << "msg=" << echo_message.c_str() << "\n";
        std::cout << "srcmsg=" << srcmsg.c_str() << "\n";
    };
//    pfn1(echo_message);

}

class CShare {
public:
    CShare(int val) : val_(val) {
        LOG_INFO << "cst share val, this=" << this;
    }
    CShare() : val_(0) {
        LOG_INFO << "cst share, this=" << this;
    }
    ~CShare() {
        LOG_INFO << "dst share, this=" << this << "val=" << val_;
    }
    void showit() {
        LOG_INFO << "val=" << val_;
    }
private:
    int val_;
};
using CSharePtr     = std::shared_ptr<CShare>;
using CShareWeakPtr = std::weak_ptr<CShare>;

class DeleterCShare
{
public:
    void operator() (CShare* del) {
        std::cout << "Deleter function will Del=" << del << std::endl;
//        delete[] x;
    }
};


class CUser {
public:
    CUser(CSharePtr shareObj) : object_(shareObj) {
        std::cout << "CUser cst, share count=" << object_.use_count() << " object_=" << object_.get() << "\n";
    }
    ~CUser() {
        std::cout << "CUser dst, object_=" << object_.get() << std::endl;
        CShareWeakPtr wkptr(object_);
        if (wkptr.expired()) {
            std::cout << "CUser dst, expire..." << std::endl;
        } else {
            std::cout << "CUser dst, object_ count=" << object_.use_count() << "\n";
            auto lockobj = wkptr.lock();
            std::cout << "CUser dst, lockobj count=" << lockobj.use_count() << "\n";
        }
    }
private:
    CSharePtr object_;
};

void ShowCount(CSharePtr obj) {
    std::shared_ptr<CShare> underlying_;
    underlying_ = obj;
    std::cout << "count=" << underlying_.use_count() << std::endl;
}
CSharePtr GetCShare() {
    return std::make_shared<CShare>();
//    CSharePtr obj = std::make_shared<CShare>();
//    std::cout << "obj count=" << obj.use_count() << std::endl;
}

void tst_share_1() {

    {
        CSharePtr p1 = std::make_shared<CShare>(9981);
        CSharePtr p2 = p1;

        LOG_INFO << "sleep ----------------1" << ", count2=" << p2.use_count() << ", count1=" << p1.use_count();

        CSharePtr p3 = std::make_shared<CShare>(100);
        p2 = p3;

//        LOG_INFO << "count lst=" << ;

        ::getchar();
        return;
    }

    {
        CSharePtr p1 = std::make_shared<CShare>(9981);
        CSharePtr p2 = p1;
        CSharePtr p3 = p2;
        p2.reset();
        LOG_INFO << "sleep ----------------1" << ", count2=" << p2.use_count() << ", count1=" << p1.use_count();
        ::sleep(1);
        ::getchar();
        return;
    }

    CSharePtr obj1 = std::make_shared<CShare>(1998);
    CSharePtr obj2 = obj1;
//    CSharePtr obj3 = obj2;

    ::sleep(1);
    LOG_INFO << "sleep ----------------1" << ", count2=" << obj2.use_count() << ", count1=" << obj1.use_count();
    obj1.reset(new CShare(1999));
    LOG_INFO << "sleep ----------------1.5" << ", count2=" << obj2.use_count() << ", count1=" << obj1.use_count();
//    LOG_INFO << "sleep ----------------2";
//    ::sleep(1);
//    LOG_INFO << "sleep ----------------3";

    return;

#if 0
    CSharePtr obj1(NULL, DeleterCShare());
    if (obj1) {
        std::cout << "obj1 count=" << obj1.use_count() << std::endl;
    }
    return;
#endif

#if 0
    std::vector <int> temp(3);
    int a[3] = {1, 2, 3};
    std::copy(a, a+3, &temp.front());
    for(int j=0; j<3; j++)
        cout<< temp[j] << endl;
#endif
#if 0
//    CSharePtr tmpobj = GetCShare();
    CUser userA(GetCShare());
    return;
#endif

#if 0
    CSharePtr obj1(GetCShare());
    std::cout << "obj1 count=" << obj1.use_count() << std::endl;
    return;
#endif

#if 0
    CSharePtr obj = std::make_shared<CShare>();
    ShowCount(obj);
#endif
}


std::atomic<int> a(0);

void thread1() {
    int x = a.fetch_add(1, std::memory_order_relaxed);
    printf("111 x=%d\n", x);
}
void thread2() {
    int x = a.fetch_add(1, std::memory_order_relaxed);
    printf("222 x=%d\n", x);
}

void print_x_val() {
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    printf("last x=%d\n", a.fetch_add(0, std::memory_order_relaxed));
}



volatile bool start = 0;

void* thread_run( void* ) {
    while (1) {
        if (start) {
            cout << "Thread malloc" << endl;
            char *buf = new char[1024];
            start = 0;
        }
        sleep(1);
    }
    return nullptr;
}

void f(unsigned char v) {
    char c = v;
    unsigned char uc = v;
    unsigned int a = c, b = uc;

    int i = c, j = uc;

    printf("----------------\n");
    printf("%%c: %c, %c\n", c, uc);
    printf("%%X: %X, %X\n", c, uc);
    printf("%%u: %u, %u\n", a, b);
    printf("%%d: %d, %d\n", i, j);
}

void tst_run() {

    {
        f(0x80);
        f(0x7F);
        return ;
    }

    {
        std::string str1 = "%5";
        auto tmp1 = str1.substr(1);
        std::cout << "tmp1=" << tmp1 << std::endl;
        return;
    }
    {
// jdbc:mysql://127.0.0.1:11306/IM_CUSTOMER?useUnicode=true&characterEncoding=utf8
        std::string jdbcUrl = "jdbc:mysql://127.0.0.1:11306/IM_CUSTOMER?useUnicode=true&characterEncoding=utf8";
        std::cout << "jdbcUrl=" << jdbcUrl.c_str() << std::endl;

        size_t lPos = jdbcUrl.find("mysql");
        if (std::string::npos == lPos) {
            LOG_ERROR << " not find \"mysql\"" << " jdbcUrl: " << jdbcUrl;
            return ;
        }

        size_t rPos = jdbcUrl.find_first_of('?');
        if (std::string::npos == rPos) {
            LOG_ERROR << " not find \"?\"" << " jdbcUrl: " << jdbcUrl;
            return ;
        }

        std::string url = "tcp";
        size_t shiftLen = strlen("mysql");
        url += jdbcUrl.substr(lPos + shiftLen, rPos - lPos - shiftLen);
//        info->sqlAddr = url;
        std::cout << "url=" << url.c_str() << std::endl;
        return;
    }
    int a[3] = {11, 21, 23};
    int b[3] = {0};
    std::copy(a, a+3, b);
    for(int j=0; j<3; j++)
        std::cout << b[j] << std::endl;


#if 0
    for ( int x = 0; x < 110 ; x ++ ) {

        int a[1024][1024] = {0};
        for (int i = 0; i < 1024; ++i){
            for(int j = 0; j < 1024;j++){
//                a[i][j] += j;
                a[j][i] +=j;
            }
        }

    }
#endif

#if 0
    pthread_t th;
    ::getchar();
    ::getchar();
    ::pthread_create(&th, 0, thread_run, 0);
    while(getchar()) {
        start = 1;
    }
#endif
}

class CObj {
public:
    CObj(int val) : val_(val) { 
        printf("cst CObj, val_=%d addr=%p\n", val_, this);
    }
    ~CObj() {
        printf("dst CObj, val_=%d addr=%p\n", val_, this);
    }
    int val_;
};

class CBase {
    using CObjPtr = std::shared_ptr<CObj>;

public:
    CBase() { std::cout << "cst cbase" << std::endl; }
    CBase(int val) {
        std::cout << "cst cbase" << std::endl;
        m_val = val;
    }
    ~CBase() {
        std::cout << "dst cbase" << std::endl;
    }
    CObjPtr TestObjPtr() {
        auto obj = new CObj(111);

        CObjPtr resptr(obj, [](CObj * res) {
            if (res) {
                printf("del res, addr=%p\n", res);
                delete res;
            }
        });
        objPtr_.swap(resptr);
        // return nullptr;
        return objPtr_;
    }

    virtual void Print() {
        printf("print base val\n");
    }
private:
    CObjPtr objPtr_;
    int m_val;
};

class Derive1 : public CBase {
public:
    Derive1() { std::cout << "cst derive1" << std::endl; }
    ~Derive1() {
        std::cout << "dst derive1" << std::endl;
    }

    virtual void Print() {
        printf("print derive1 val\n");
    }
    void PrintVal() {
    }
};

void tstPrintVal() {
    Derive1* d1 = new Derive1;
    {
        CBase* pbase = (CBase*)d1;
        pbase->Print();
    }
   d1->Print();
}

void prepare(void)
{
    printf("pid = %d prepare ...\n", static_cast<int>(getpid()));
}

void parent(void)
{
    printf("pid = %d parent ...\n", static_cast<int>(getpid()));
}

void child(void)
{
    printf("pid = %d child ...\n", static_cast<int>(getpid()));
}

void tstAtFork() {
    printf("pid = %d Entering main ...\n", static_cast<int>(getpid()));

    ::pthread_atfork(prepare, parent, child);
    //父进程调用parent函数，子进程调用child函数；
    //这两个函数的调用顺序不一定
    fork();

    printf("pid = %d Exiting main ...\n",static_cast<int>(getpid()));
}

class CSomeThing {
private:
    int64_t microSecondsSinceEpoch_;
};

class CNull {
};

BOOST_STATIC_ASSERT(sizeof(CSomeThing) == sizeof(int64_t));


using Functor = boost::function<void()>;//任务T
muduo::BoundedBlockingQueue<Functor> gTaskQueue(10);

class TaskObj {
public:
    TaskObj(int val)
        : val_(val)
    {
    }
    void showval() {
        LOG_INFO << "val=" << val_;
    }
private:
    int val_;
};

bool running = true;
void/***/ Producer(/*void* param*/) {
    LOG_INFO << "producer start, param=";

    int idx = 0;
    Timestamp begTime;

    while (running) {
        TaskObj obj(idx++);

        begTime = Timestamp::now();

        gTaskQueue.put(boost::bind(&TaskObj::showval, &obj));

        double cost = timeDifference(Timestamp::now(), begTime);
        LOG_INFO << "put cost=" << cost;
    }
}
void/***/ Consumer(/*void* param*/) {
    while(running){
        Functor task = gTaskQueue.take();
        task();
    }
}

void tstProducerConsumer() {
//    pthread_t pid0;
//    pthread_t pid2[2];
//    ::pthread_create(&pid0, nullptr, Producer, nullptr);
//    for (int ii = 0; ii < 2; ii ++) {
//        ::pthread_create(&pid2[i], nullptr, Consumer, nullptr);
//    }
//    ::sleep(1);

    muduo::Thread producer1(Producer);
    producer1.start();

//    muduo::Thread producer2(Producer);
//    producer2.start();

//    muduo::Thread producer3(Producer);
//    producer3.start();

//    muduo::Thread producer4(Producer);
//    producer4.start();

//    producer2.join();
//    producer3.join();
//    producer4.join();

    muduo::Thread c1(Consumer);
    c1.start();
    muduo::Thread c2(Consumer);
    c2.start();
    c1.join();
    c2.join();
    producer1.join();

    ::sleep(1);
}

int tstopenfd(int argc, char* argv[]) {
    // 第一次打开
    int a = open("demo.txt", O_RDONLY);
    // 第二次打开
    int b = open("demo.txt", O_RDONLY);

    printf("a = %d, b = %d \n", a, b);
    // 关闭a文件描述符
    close(a);

    // 第三次打开
    int c = open("demo.txt", O_RDONLY);
    printf("b = %d, c = %d \n", b, c);
    return EXIT_SUCCESS;
    // output:
    // a = 3, b = 4
    // b = 4, c = 3
}


class CTvvv {
public:
    CTvvv() {
        LOG_INFO << "this=" << this << ", default cst CTvvv" << ", selfval=" << selfVal_;
    }
    CTvvv(int val)
        :
          selfVal_(val)
    {
        g_val = val;
        LOG_INFO << "this=" << this << ", param cst CTvvv" << ", selfval=" << selfVal_;
    }
    ~CTvvv() {
        LOG_INFO << "this=" << this << ", dst CTvvv" << ", selfval=" << selfVal_;
    }
    void ShowVal() {
        LOG_INFO << "this=" << this << ", show val selfval=" << selfVal_ << ", addr=" << &selfVal_;
    }
private:
    int                 selfVal_;
    static __thread int g_val;
};

__thread int CTvvv::g_val;

typedef boost::shared_ptr<CTvvv> CTvvvPtr;
typedef boost::weak_ptr<CTvvv>   CTvvvWkPtr;


void tst__thread() {
    CTvvv o1(33);
    CTvvv o2(44);
    CTvvv o3(77);

    muduo::Thread t1(boost::bind(&CTvvv::ShowVal, &o1));
    muduo::Thread t2(boost::bind(&CTvvv::ShowVal, &o2));
    muduo::Thread t3(boost::bind(&CTvvv::ShowVal, &o3));
    t1.start();
    t2.start();
    t3.start();

    t1.join();
    t2.join();
    t3.join();
}

void tst_unique() {
    boost::shared_ptr<CTvvv> obj(new CTvvv(123));

    LOG_INFO << "obj use count=" << obj.use_count() << ", unique=" << obj.unique();
    {
        boost::shared_ptr<CTvvv> cpy = obj;
        LOG_INFO << "cpy use count=" << cpy.use_count() << ", unique=" << cpy.unique();
    }
    LOG_INFO << "obj use count=" << obj.use_count() << ", unique=" << obj.unique();
}

void tst_htonl_fun() {
    int len = 1024;
    LOG_INFO << "htonl(len-4)=" << htonl(len-4);
    LOG_INFO << "htonl(len)-4=" << htonl(len)-4;
}

void tst_trim() {
    std::string tmp = " adfasdf 111 ";
    boost::trim(tmp);
    LOG_INFO << "tmp=" << tmp;
}


//muduo::MutexLock gMtx;

CTvvv GetCTvvObj(int val) {
    return CTvvv(val);
}


class CUseTv {
public:
    CUseTv()
        : data_(new CTvvv) {

    }

    CTvvvPtr GetData() {
        return data_;
    }

public:
    CTvvvPtr data_;
};


void SetVal(int& val) {
    std::cout << "&val=" << val << std::endl;
}
void SetVal(int&& val) {
    std::cout << "&&val=" << val << std::endl;
}


void testHash()
{


    {
#if 0
        int val = 13;
        SetVal(val);
        SetVal(std::move(val));
        std::cout << "lst val=" << val << std::endl;
//       output:
//        &val=13
//        &&val=13
//        lst val=13
        return;
#endif
    }

    {
        CUseTv useObj;
        CTvvvPtr sp_obj = useObj.GetData();
        LOG_INFO << "sp_obj1 use count=" << sp_obj.use_count();

        CTvvvWkPtr wktmp(sp_obj);
        sp_obj = nullptr;

        LOG_INFO << "sp_obj2 use count=" << sp_obj.use_count();

        if (wktmp.expired()) {
            LOG_INFO << "wktmp expired";
        } else {
            LOG_INFO << "wktmp not expired";
        }

        {
            CTvvvPtr obj = wktmp.lock();
            LOG_INFO << "new obj use count=" << obj.use_count();
        }
        return;
    }
    {
        CUseTv useObj;
        CTvvvPtr tmptv = useObj.GetData();
        LOG_INFO << "tmptv use count=" << tmptv.use_count();
        return;
    }
    {
        auto p1 = new CTvvv(1122);
        std::unique_ptr<CTvvv> obj1(p1);
        LOG_INFO << "before obj1 = " << obj1.get();
        std::unique_ptr<CTvvv> obj2 = std::move(obj1);
        LOG_INFO << "after obj1 = " << obj1.get();
        LOG_INFO << "after obj2 = " << obj2.get();
        return ;
    }
    {
        std::vector< std::unique_ptr<CTvvv> > vecCtvv_;
//        vecCtvv_.push_back(/*std::move*/(new CTvvv(3311)));

        std::vector< std::unique_ptr<CTvvv> >::value_type someobj(new CTvvv(3311));
        vecCtvv_.push_back(std::move(someobj));
        return;
    }
    {
        std::vector < std::string > vecStrings;
        std::vector < std::string >::value_type strTmp = "test value type";
        std::cout << "value type = " << strTmp << std::endl;
        return;
    }
    {
        GetCTvvObj(9981);
        GetCTvvObj(8864);
        return;
    }
    {
        auto pfnPrint = [&](const char* msg, int len) {// defaultOutput(const char* msg, int len)
            size_t n = fwrite(msg, 1, len, stdout);
            //FIXME check n
            (void)n;
        };

        std::string log1 = "tst msg1\n";
        pfnPrint(log1.c_str(), log1.size());

        std::string log2 = "tst msg2\n";
        pfnPrint(log2.c_str(), log2.size());
        return;
    }

    {
        boost::shared_ptr< Derive1 > d1 = boost::make_shared<Derive1>();
        boost::shared_ptr< CBase > d2 = d1;
        d1.reset();
        return ;
    }

    boost::hash< std::shared_ptr<int> > h;
    std::shared_ptr<int> x1(new int(10));
    std::shared_ptr<int> x2(new int(10));
    h(x1);
    LOG_INFO << "h(x1)=" << h(x1);
    LOG_INFO << "h(x2)=" << h(x2);

    {
        boost::hash < int > hnew;
        LOG_INFO << "hnew(1)=" << hnew(3);
        LOG_INFO << "hnew(2)=" << hnew(11);
    }
    return;

    assert(h(x1) != h(x2));
    x1 = x2;
    assert(h(x1) == h(x2));
    x1.reset();
    assert(h(x1) != h(x2));
    x2.reset();
    assert(h(x1) == h(x2));
}


boost::shared_ptr < std::vector<int> > sp_obj_;
muduo::MutexLock gRwMtx;

void readFunc() {
    boost::shared_ptr < std::vector<int> > rd_obj;
    {
        LOG_INFO << "--------------------- read wait";
        muduo::MutexLockGuard lock(gRwMtx); // 临界区足够小，适合用mutex
        rd_obj = sp_obj_; // 利用局部的shared_ptr来增加数据的引用计数，告诉写线程不要修改这份数据
        LOG_INFO << "read sleep 1" << ", rd use_count=" << rd_obj.use_count();
        sleep(1); // 为了阻塞其它线程
    }
    LOG_INFO << "read size " << rd_obj->size() << ", sleep 1";
    sleep(1);
}
void writeFunc(int val) {
    {
        LOG_INFO << "------------------------------ write wait";
        muduo::MutexLockGuard lock(gRwMtx);// 写线程的临界区比较大
        if (!sp_obj_.unique())
        {
            // 如果sp_data引用计数大于1，说明有其他线程在读（通过read_data()提升了引用计数）。
            // 此时将数据复制一份，再利用 reset 或 swap 让 sp_obj_ 指向新数据，老数据让读线程继续读。
            // 这个写线程现在独占了 sp_obj_ ，而之前那份数据在所有读线程读完之后，引用计数会变成0，被最后一个读线程自动析构。
            sp_obj_.reset(new vector<int>(*sp_obj_));
            LOG_INFO << "-------------------------------- copy on write";
        }
        sp_obj_->push_back(val);
    }
    LOG_INFO << "------------------------------ sleep write" << ", wr use_count=" << sp_obj_.use_count();
    sleep(2);
}
void read_thread() {
    while (1) {
        readFunc();
    }
}
void write_thread() {
    int val = 1;
    while (1) {
        writeFunc(val++);
    }
}
void tstCowFun() {
    sp_obj_ = boost::make_shared< std::vector<int> >();

    muduo::Thread t1(read_thread);
    muduo::Thread t2(write_thread);
    t1.start();
    t2.start();
    t1.join();
    t2.join();
}

void tst_set() {
    typedef std::set<int> IntSet;

    IntSet iSets;
    iSets.insert(189);
    iSets.insert(99);
    iSets.insert(67);

    for (auto& it : iSets) {
        LOG_INFO << it << " ";
    }

    int findval = 68;
    IntSet::iterator findIter = iSets.lower_bound(findval);
    LOG_INFO << "findIter =" << *findIter;
}

void tst_array_zero() {

class KVSharedLock {
    // std::shared_mutex m_mutex;
    std::unordered_map<std::string, int> m_map;

public:
    void put(const std::string &key, int value) {
        // std::lock_guard lock(m_mutex);
        // m_map.emplace(key, value);
    }

    int get(const std::string &key) {
        return 0;
        // std::shared_lock lock(m_mutex);
        // auto it = m_map.find(key);
        // if (it != m_map.end())
        //     return it->second;
        // return {};
    }

    bool remove(const std::string &key) {
        return true;
        // std::lock_guard lock(m_mutex);
        // auto n = m_map.erase(key);
        // return n;
    }
};



    /*
        零长数组在有固定头部的可变对象上非常有用。
    优点:
        1. 不需要使用指针来分配内存，节约一个指针变量所占内存大小，也使内存申请方式更加便捷；
        2. 分配的内存连续，管理与释放简单，只需要一次操作。
    缺点
        1. 零长数组是GNU C的实现，非标准，因此并不是所有的编译器都支持，有移植风险。
        2. 在结构体中，数组为0的数组必须在最后声明，使 用上有一定限制。
    */
    struct line{
        int length;
        char data[0];
    };
    std::cout << "size = " << sizeof(line) << std::endl;

struct line_1{
	int length;
	// char *data;
};
std::cout << "size = " << sizeof(line_1) << std::endl;

    char a[0];
    printf("sizeof(a)=%ld\n", sizeof(a));

}


std::string random_string(int len) {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static std::mt19937 gen(123);
    static std::uniform_int_distribution<int> dist(0, sizeof(alphanum) - 1);
    std::string s;
    for (int i = 0; i < len; ++i)
        s += alphanum[dist(gen)];
    return s;
}
#include <atomic>
// bool g_bInit = false;
std::atomic<bool> g_bInit(false);

void fun1(void)
{
    // Thread 1
    // ready was initialized to false
    auto pfnInit = [&] {
        printf("i am init\n");
    };
    pfnInit();
    // g_bInit = true;
    g_bInit.store(true, std::memory_order_release);
}
void fun2(void) {
    auto pfnLook = [&] {
        printf("look exec ...\n");
    };
    // if (g_bInit) {
    if (g_bInit.load(std::memory_order_acquire)) {
        pfnLook();
    } else {
        printf("no found init ...\n");
    }
}

void tst_memory_fence() {

    std::thread t1(fun1);
    std::thread t2(fun2);
    t1.join();
    t2.join();

    sleep(1);
}

const int loop_count = 100000000;

void tstmakesharedptr() {

    class Foo
    {
    public:
        typedef std::shared_ptr<Foo> Ptr;

        Foo()
            : a(42)
            , b(false)
            , c(12.234)
            , d("FooBarBaz")
        {}

    private:
        int a;
        bool b;
        float c;
        std::string d;
    };
#ifdef USE_MAKE_SHARED
    printf("make shared ,,,,\n");

#else
    printf("no make shared ,,,,\n");
#endif

    deferTime([&]() {
        for (int i = 0; i < loop_count; i++)
        {
            // #ifdef USE_MAKE_SHARED
            //             Foo::Ptr p = std::make_shared<Foo>();
            // #else
            Foo::Ptr p = Foo::Ptr(new Foo);
            // #endif
        }
    });

    {
        deferTime([&]() {
            for (int i = 0; i < loop_count; i++)
            {
                Foo::Ptr p = std::make_shared<Foo>();
            }
        });

    }

}
void task_lambda() {

    {
//        std::packaged_task<int()> task(std::bind(std::pow, 2, 11));
//        std::future<int> result = task.get_future();
//        task();
//        std::cout << "task_bind:\t" << result.get() << '\n';
//        return;
    }
    {
//        std::packaged_task<int()> task(std::bind([](int x, int y) { return std::pow(x, y); }, 2, 3));
        std::packaged_task<int()> task(std::bind([](int a, int b) {return a+b;}, 3, 9 ));
        std::cout << "valid=" << task.valid() << std::endl;
        std::future<int> result = task.get_future();
        task();
        std::cout << "task_bind:\t" << result.get() << '\n';
        return;
    }
    {
        std::packaged_task<int(int, int)> task([](int a, int b) {
            return a+b;
        });
        std::cout << "valid=" << task.valid() << std::endl;
        std::future<int> result = task.get_future();
        task(3, 12);
        std::cout << "task_bind:\t" << result.get() << '\n';
        return;
    }
    {
        std::packaged_task<int(int,int)> task([](int a, int b) {
            return a+b;
        });
        std::future<int> res = task.get_future();
        task(2, 9);
        std::cout << "task_lambda res=" << res.get() << std::endl;
    }
}
void tstFuture() {
    task_lambda();

}



#include "async_thrift_client.h"

// 2020-6-20
// add new 测试分支预测
void tst_c11fun_entry(int argc, char *argv[]) {

    tst_async_thrift_client(); return;
    
    tst_shared_ptr_2();
    return;

    {
        tstmakesharedptr();
        return;
    }

    tstFuture();
    return ;
    {
        // std::unique_ptr<int> pint(new int(1023));
        // std::cout << "pit=" << *pint << std::endl;

        std::unique_ptr<CBase> cbase(new CBase(111));
        auto tmpObj = cbase->TestObjPtr();
        std::cout << "count=" << tmpObj.use_count() << std::endl;
        return;
    }
    tst_memory_fence();
    return;

    tst_array_zero();
    return;

    tst_boost_specfic_fun(); return;

    tst_set(); return;

    tst_share_1(); return;

    tstCowFun(); return;

    testHash(); return;

    tst_trim();      return;

    tst_htonl_fun(); return;

    tst_thrd_local(); return;

//    tst_unique(); return;

    tst__thread(); return;

    tstopenfd(argc, argv); return;

//    tstProducerConsumer(); return;
    tstDefer(); return;

    std::cout << "sizeof(CSomeThing)=" << sizeof(CSomeThing) << std::endl;
    std::cout << "sizeof(CNull)=" << sizeof(CNull) << std::endl;
    return;

    tstAtFork(); return;

    tstLeftRightValue_2(); return;
    tstLeftRightValue_1(); return;

    tstPrintVal(); return;

    tstBranchPredict(argc, argv); return;

    tst_run(); return;

    print_x_val(); return;

    tst_stdmove();
    pfff1("xxv");
    return;
    tst_align(); return;

    tstC11Thrd(); return;
//    OutputDbgInfo tmpOut( "tst_c11fun_entry begin", "tst_c11fun_entry end" );
    tst_getline(); return;

    GetCurSecond();  return;

    int &&rval1 = 1;    // 正确，1是右值，可以赋值给右值引用
    GetFileNameFromDir("include");
    GetFileNameFromDir("CMakeFiles");
    GetFileNameFromDir("compile_commands.json");
    return ;

    tstThrdWork(); return;

    tst_die(); return;

    tst_youzhi(); return;

    NOTIFY_PROCESS(tstrecvfname__call, 123); return;

    tst_mov_yuyi();  return ;

    tst_move_1(); return;

    tst_rval_lval(); return ;

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

//    uint32_t    cur_body_len  = 1;
    uint8_t*    pData     = NULL;
    uint32_t    tmpsize   = 0 ;
    buf->getBuffer( &pData, &tmpsize );

    std::cout << "pData=" << pData << ", tmpsize=" << tmpsize << std::endl;

return ;

    tst_stl_1();
    return;


    FILE* file1 = fopen( "file1.txt", "wb" );
    DeferFunctor releaseFile = boost::function < void() >([&]()
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


