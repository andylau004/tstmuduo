






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


//#include "poco/Bugcheck.h"
//#include "poco/Foundation.h"
//#include "poco/Alignment.h"

#include "muduo/base/common.h"

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


#include "tst_1.h"
#include "codeconvert.h"


#include "boost_use_1.h"
#include "trie_tree.h"
#include "classfun.h"
#include "tstBinaryTree.h"
#include "quicksort.h"

#include "tstList.h"

#include "complexdata.h"

#include "mergesort.h"

#include "heapsort.h"
#include "externMergSort.h"


#include "hashfun.h"

#include "linkedlistopr.h"
#include "stackopr.h"

#include "bitmapfun.h"


using namespace std;
using namespace muduo;
using namespace muduo::net;



// https://github.com/hehe520/Data-structure-and-algorithm
// https://github.com/xtaci/algorithms/blob/master/README.md



#define VCZH_CHECK_MEMORY_LEAKS 1

#ifdef VCZH_CHECK_MEMORY_LEAKS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
//#define VCZH_CHECK_MEMORY_LEAKS_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new VCZH_CHECK_MEMORY_LEAKS_NEW
#endif




/*

快速排序。考察的地方包括算法步骤、平均算法复杂度、最好和最坏的情形，有人说校招要把算法写出来，我这里是社招，所以一般描述一下算法过程即可。

二分查找法。一般面试官会要求面试者把算法写出来，或者先结合一个具体场景来提问，需要你联想到要使用该算法，比如求一个数的平方根，接着要你把它具体实现。

链表。常见的面试题有要求你写出从一个链表中删除一个节点的算法、单链表倒转、两个链表找相交的部分等，一般需要完全无误地写出来。

实现一些基础的函数。例如 strcpy、memcpy、memmov 与 atoi 这些函数，面试官会要求你用自己的理解和思路去实现。同样，这些一般也需要完全无误地写出来，并且要保证高效，比如你的实现中有动态分配堆内存，那么这道题目就算答错。

链表和实现基础函数的关键点一般在于考察你的代码风格、对边界条件的处理，比如判断指针是否为空，千万不要故意不考虑这种情形，即使你知道也不行，只要你不写，一般面试官就认为你的思路不周详，容错率低；再比如，单链表的倒转，最后的返回值肯定是倒转后的链表头结点，这样才能引用一个链表，这些都是面试官想考虑的重点。

哈希表。对哈希表的细节要求很高，比如哈希表的冲突检测、哈希函数常用实现、算法复杂度；比如百度二面就让我写一个哈希表插入元素算法，元素类型是任意类型。

AVL 树与 B 树。考察概念与细节，比如会问 MySQL 数据库索引的实现原理，基本上就等于在问你 B 树。

红黑树。这个基本上是必问的数据结构，包括红黑树的概念、平均算法复杂度、最好最坏情况下的算法复杂度、左右旋转与颜色变换。面试官常见的套路有：你熟悉 C++ 的 stl 吗？你说熟悉，ok，stl 的 map 用过吧？用过，ok，那 map 是如何实现的？红黑树，ok，那什么是红黑树？这样提问，红黑树就开始了。Java 的也类似。

二、考察操作系统原理等基础技术
这一种以饿了么、bilibli、喜马拉雅、360 与携程等为代表，主要兼顾算法与数据结构，但重点放在其它开发技术，比如操作系统原理、网络协议细节与具体的开发库源码等内容。

基础的 C++ 问题

以 C++ 语言为例，这一类常见的问题有：

C++ 的继承体系中 virtual 关键字的作用，如继承关系中析构函数为什么要申明成 virtual 函数，如果不申明为 virtual 会有什么影响、再涉及到父子类时构造与析构函数的执行顺序、多重继承时类的成员列表在地址空间的排列；

static 关键字的作用，static_cast、reinterpret_cast 与 dynamic_cast 等几个转换符的使用场景；

问得最多的就是虚表的布局，尤其是菱形继承，即 B 和 C 继承 A，D 继承 B 和 C，每个对象的空间结构分布，比如问 D 有几份虚表，D 中 B 和 C 的成员空间排布。

另外，如果你应聘的职位使用 C++ 开发，很多公司会问你一些 C++ 11 的东西，或者问 boost 库，基本上都一样。这个你用过就用过，没有用过就说没用过不要装 X，常见的 C++ 11 需要掌握的一些技术库我也列举一下吧：

auto 关键字

for-each 循环

右值及移动构造函数

std::forward、std::move

stl 容器新增的 emplace_back() 方法、std::thread 库、std::chrono 库

智能指针系列 std::shared_ptr、std::unique_ptr、std::weak_ptr，智能指针的实现原理一定要知道，最好是自己实现过

线程库 std::thread

线程同步技术库 std::mutex、std::condition_variable、std::lock_guard 等

lamda 表达式（Java 中现在也常常考察 lamda 表达式的作用）

std::bind、std::function 库

网络通信问题

考察对网络协议的理解和掌握程序，比如 TCP/IP 协议栈的层级关系，三次握手和四次挥手的细节，注意我说的是细节，比如 CLOSE_WAIT 和 TIME_WAIT 状态。bilibili 问了这样一个问题，你可以感受一下：

A 与 B 建立了正常连接后，从未相互发过数据，这个时候 B 突然机器重启，问 A 此时处于 TCP 什么状态？如何消除服务器程序中的这个状态？

再罗列其它一些常见问题：

万得问过流量拥塞和控制机制

腾讯问过 TCP 和 IP 包头常见字段有哪些

东方财富网问了阻塞和非阻塞 socket 在 send、recv 函数上的行为表现

异步 connect 函数的写法

select 函数的用法

epoll 与 select 的区别，基本上只要问到 epoll，必问 epoll 的水平模式和边缘模式的区别

一些 socket 选项的用法，nagle、keepalive、linger 等选项的区别

TCP 与 UDP 的区别和适用场景

通信协议如何设计避免粘包

HTTP 协议的 get 和 post 方法的区别，问得比较深的会让你画出 HTTP 协议的格式

可能会问到 Windows 完成端口模型(IOCP)

总之，网络通信问题能搞得多清楚就搞多清楚，最起码把 TCP 应用层各种 socket API 的用法细节搞清楚。

操作系统原理

上海黄金交易所和喜马拉雅听书都问了 Linux 下 elf 文件的节结构，映射到进程地址空间后，分别对应哪些段。相关的问题还有全局变量、静态存储在进程地址空间的哪里。饿了么二面问了操作系统的保护模式实模式、中断向量表、Linux 下的 CAS。Linux 系统下可能还会问什么是 daemon 进程，如何产生 daemo 进程，什么是僵尸进程，僵尸进程如何产生和消除。

堆和栈的区别，栈的结构、栈的细节一点要搞得特别清楚，因为一些对技术要求比较高的公司会问得比较深入，例如京东的一面是让我先写一个从 1 加到 100 的求和函数，然后让我写出这个函数的汇编代码，Java 开发的同学可能会让你试着去写一点 JVM 的指令。如果你对栈的结构，如函数参数入栈顺序、函数局部变量在栈中的布局、栈帧指针和栈顶指针位置不熟悉的话，这题目就无法答对了。

栈的问题，可能会以常见的函数调用方式来提问，常见的有 __cdecl、__stdcall、__thiscall、__fastcall 的区别，比如像 printf 这样具有不定参数的函数为什么不能使用 __stdcall。

此外还有进程和线程的联系与区别，问得最多的就是线程之间的一些同步技术，如互斥体、信号量、条件变量等，Windows 上还有事件、临界区等，这类问题还可以引申为什么是死锁、如何避免死锁。这些东西你必须熟悉到具体的 API 函数使用的层面上来。这是实际开发工作中编码最常用的东西，如果你连这个都不能熟练使用，那么你多半不是一个合格的开发者。

进程之间通信的常用技术也需要掌握，Linux 常用的通信方式有共享内存、匿名和具名管道、socket、消息队列等等，管道和 socket 是两个必须深入掌握的考察点。

使用过的开源技术

考察使用过的开源技术，比如 Redis 与 MySQL 等项目。开源技术的使用一般不做硬性要求，但是这里必须强调一下 Redis，熟练使用 Redis 甚至研究过 Redis 源码，现在一般是对做后台开发的技术硬性要求。

基于 Redis 的面试题既可以聊算法与数据结构，也可以聊网络框架等东西。我面试的公司中基本上百分之九十以上都问到了 Redis，只是深浅不一而已，比如喜马拉雅问了 Redis 的数据存储结构、rehash，bilibili 问了 Redis 的事务与集群。

三、考察项目经验
只问一些做过的业务或者项目经验，这类公司他们招人其实对技术要求不高，除非是资深及主管级开发。只要你过往的项目与当前应聘职位匹配，可以过来直接上手干活就可以了，当然薪资也就不会给很多。比如游戏公司会关心你是否有某某类型的游戏开发经验、股票类公司会关心你是否有过证券或者交易系统的开发经验等。

业务开发哪里都能找到，真正重视技术的公司，应该是广大做技术尤其是初中级开发的朋友更为关注的。

不靠谱型公司
面试过程中，其实还会发现一些不靠谱的公司，我遇到的大致有四类：

装 X 忽悠型公司

面试过程冗长繁琐，我面试的有一家公司就是这个样子，先是一轮长长的电话面试，然后是五轮技术面试，前三轮是刷 leetcode 上的原题，然后后几轮面试官从基本的操作系统中断、GDT、LDT、分表分页机制问到上层高并发海量数据的架构。从外太空聊到内子宫，最后问具体职位做什么时，要么遮遮掩掩要么原型毕露，或者讨论薪资时，要么面露难色要么各种画饼，但是实际就给不了多少薪水的。

佛性公司

全程面试官面带微笑，问的问题你回答之后面试官也表示很赞同，但最后你就是没通过。我猜测要么公司不是很缺人，想观望一下是否有合适的人才，要么招聘信息上开的薪资给不到。

老奶奶裹脚布型公司

其特点是面试周期长，往往第一轮面试通知你过了，让你回去等上十天半个月后，给你打电话通知你来第二轮面试。面试要求穿正装，带好各种证件，面试前必须先查验你的身份证、学历证学位证，甚至是四六级考试证等等，麻烦至极。即使你一路过关斩将过了终面，薪资也给不了多少。大家都是要养家糊口的，都是忙着找工作，谁有时间和你耗上十天半个月呢？

不尊重人的公司

我这里说的不尊重人，不是指的是面试过程中对你人身攻击，而是不根据你的工作年限和经验随意安排面试官。举个例子，比如你工作十年，你去面试一个技术总监的职位，对方公司安排一个工作不满两年的部门职员作为面试官，这个面试官如果是走过场可以理解，但是非要和你纠结一个如二进制位移、现代编译器要不要在子类析构函数前加 virtual 关键字这些技术细节就没必要了。还有一类就是故意问一些刁钻的问题，或者全场都心不在焉、玩手机、漫不经心的面试官，比如问你 TCP 协议头有多少个字段，每个字段是干啥的。遇到这一类面试官我的经验就是要么婉拒，要么直接怼回去。

技术之外的关注点
除了技术上的东西，面试中还有其它方面需要思考，这些东西有时候可能比你直接面试通过一家公司还重要，因为其中有一些是关系到你整个职业发展的：

第一，如果你的工作年限不长，尤其是渴望在技术方面有一定的造诣，那么你首先考虑的应该是新的单位是否有利于你技术上的成长，而不是把眼光放在三五千的工资差距上。另一方面，如果想转行的同学，比如从客户端转服务器，从 C++ 转 Java，那么不要因为薪资突然变低而拒绝这种阵痛，要把目光放长远一点。

第二，一些公司虽然招聘信息上写了最多能给到多少多少，但实际上即使你全程面试下来都很完美，可能最终也会因为在招聘信息范围内提出的薪资要求“太高”而不被录取。

第三，一些根本不想去的公司，如果你有时间的话，去面试积累下经验也不是什么坏事。

第四，面试的时候，同时也是你在考察面试官，一般面试官问你的问题，你能回答出来的在百分之八十左右，这样的公司可以考虑去入职，你进去的话可能才会在技术上有一些提升。如果你全场秒杀面试官的题目，那么一旦你入职，你的技术天花板可能也就在那里了。

第五，面试的时候聊清楚你将来的职位内容，避免进去客串一些不想做的工作。

第六，遇到不会的面试题，不要直接就否定自己，可以尝试着去和面试官沟通一下，或者要求给点提示或者思路。

第七，不要轻视笔试中的一些数学智力题目，认真作答，试问算法不也是数学智力题吗？

第八，自信一点，每个人的经历和经验都是独一无二的，面试的时候，一些特定领域的问题，回答不出来也不要太在意。

上边这些就是我经过与 30+ 家公司过招，引发的关于面试的思考，希望对阅读的朋友有所帮助。

因为个人经验能力有限，而且限于篇幅，很多地方也不便详细展开，读者如果有什么想法或者指教，欢迎交流探讨。



*/

class TestClient
{
public:
    TestClient(EventLoop *loop, const InetAddress &listenAddr)
        : loop_(loop),
          client_(loop, listenAddr, "TestClient"),
          stdinChannel_(loop, 0)
    {
        client_.setConnectionCallback(
                    boost::bind(&TestClient::onConnection, this, _1));
        client_.setMessageCallback(
                    boost::bind(&TestClient::onMessage, this, _1, _2, _3));
        //client_.enableRetry();
        // 标准输入缓冲区中有数据的时候，回调TestClient::handleRead
        stdinChannel_.setReadCallback(boost::bind(&TestClient::handleRead, this));
        stdinChannel_.enableReading();      // 关注可读事件
    }

    void connect() {
        client_.connect();
    }

private:
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
//            printf("onConnection(): new connection [%s] from %s\n",
//                   conn->name().c_str(),
//                   conn->peerAddress().toIpPort().c_str());
            LOG_INFO << "TestClient onConnection(): new connection [" << conn->name() << "] from " << conn->peerAddress().toIpPort();
            std::string strMsg = "abcdefg";
            conn->send( strMsg.c_str(), strMsg.size() );

            exit(-1);
            return;

            conn->shutdown();
        }
        else
        {
            LOG_INFO << "TestClient onConnection(): connection [" << conn->name() << " is down";
//            printf("onConnection(): connection [%s] is down\n", conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
    {
        muduo::string msg(buf->retrieveAllAsString());
        printf("onMessage(): recv a message [%s]\n", msg.c_str());
        LOG_INFO << conn->name() << " recv " << msg.size() << " bytes at " << time.toFormattedString();
    }

    // 标准输入缓冲区中有数据的时候，回调该函数
    void handleRead()
    {
        char buf[1024] = {0};
        fgets(buf, 1024, stdin);
        buf[strlen(buf) - 1] = '\0';        // 去除\n
        client_.connection()->send(buf);
    }

    EventLoop *loop_;
    TcpClient client_;
    Channel stdinChannel_;      // 标准输入Channel
};



/*
 * 如何对n个大小都小于100的整数进行排序，要求时间复杂度O(n)，空间复杂度O(1)。
 * */
#define N 100
#define RANGE 100

int* getRandomArray() {
    srand( unsigned(time(0)) );

    int* randomArray = new int[N];
    for (int i = 0; i < 100; i++ ) {
        randomArray[i] = rand()%99 + 1;
    }

    return randomArray;
}
void prinfArray(int* sortCount, int len) {
    for (int i = 0; i < len; ++i) {
//        for (int j = 0; j < sortCount[i]; ++j) {
//            cout << i << " ";
//        }
        if (sortCount[i])
            std::cout << /*"index=" <<*/ i /*<< ", cout=" << sortCount[i]*/ << " ";
        if ((i + 1) % 10 == 0) {
            cout << endl;
        }
    }
}
void tst_sort_1() {
    int* randArray = getRandomArray();
//    prinfArray(randArray, RANGE);
    printf("----------------------\n");

    //记录出现次数,再还原,RANGE为常数则，空间复杂度O(1)
    int* sortArray = new int[RANGE];
    memset(sortArray, 0, sizeof(int) * RANGE);

    for (int i = 0; i < N; ++i) {
        sortArray[ randArray[i] ]++;
    }

    prinfArray(sortArray, RANGE);
}


void SuperSort(int* randomArray, int n) {
    int hashTable[10] = {0};

    for (int i = 0; i < n; i ++ ) {
        hashTable[ randomArray[ i ] ] ++;
    }

    for (int j=0; j < 10; j ++) {
        if ( hashTable[j] ) {
            std::cout << hashTable[j] << " ";
        }
        if ((j+1)%10 == 0)
            std::cout << std::endl;
    }
}
void tst_sort_2() {
//    int n;
//    cin >> n;

//    int i = 0;
//    int *arr = new int[n];

//    for(i=0;i<n;i++)
//        cin>>arr[i];

    int* pret = getRandomArray();
    SuperSort(pret, 100);

    //前面几个实验都老是忘了delete 这个很不好，还是要铭记在心，尤其我是那么那么的珍惜内存啊
//    delete []arr;
}



//题目：实现一个排序算法，对0~n-1范围内的n个不重复的无序数组进行排序，时间复杂度为O(n)，空间复杂度为O(1)。
//思想：从头到尾扫描这个数组中的每个数字，当扫描到下标为i的数字是，首先比较这个数字（假设为m）是不是等于i，如果等于继续下去；如果不等于则就和第m个位置的数字交换，依次重复下去，直到循环结束。

void Mysort(vector<int> & arr) {
    int len = arr.size();
    if (len <= 1)
        return;
    for (int i = 0; i < len; i++) {
        while (arr[i] != i) {
            swap(arr[i], arr[arr[i]]);			//交换
        }
    }
}

void tst_sort_3() {
    std:vector<int> arr;
    arr.push_back(4);arr.push_back(6);arr.push_back(3);
    arr.push_back(1);arr.push_back(5);arr.push_back(2);
    arr.push_back(7);arr.push_back(0);

    Mysort(arr);
    for (int i = 0; i < arr.size(); i ++)
        cout << arr[i] << " ";
    std::cout << std::endl;
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




void tst_int64_1() {
    char c=255;
    cout << "c=" << c << endl;
    unsigned short b = c;
    cout <<b << endl;
    return ;

    int index = 0;
    int i = 0;
    for(i=0; i<10; i++){
      printf("%d, %d\n", index, index++);
    }

    return;

    std::map< std::string, std::string > map_1;
    std::map< std::string, std::string > map_2;

    map_1["1"] = "111";
    map_1["2"] = "211";
    map_1["3"] = "311";

    map_2["sss"] = "dddd";
    map_2.insert(map_1.begin(), map_1.end());

    for (std::map<std::string, std::string>::iterator it=map_2.begin(); it!=map_2.end(); it++) {
        std::cout << "key=" << it->first << " value=" << it->second << std::endl;
    }

    return ;

    char str1[]       = "abc";
    char str2[]       = "abc";
    const char str3[] = "abc";
    const char str4[] = "abc";
    const char* str5  = "abc";
    const char* str6  = "abc";
    cout << boolalpha << ( str1==str2 ) << endl; //输出什么？
    cout << boolalpha << ( str3==str4 ) << endl; //输出什么？
    cout << boolalpha << ( str5==str6 ) << endl; //输出什么？
    return;

    typedef struct _tagNull_Test
    {
        _tagNull_Test() {}
    }Null_Test;
    std::cout << "sizeof(Null_Test)=" << sizeof(Null_Test) << std::endl;
//    Test obj;
//    std::cout << "Test=" << sizeof(obj) << std::endl;

    typedef struct _tagTest
    {
        int m_val;
        _tagTest() {m_val=1;}
    }Test;
    std::cout << "sizeof(Test)=" << sizeof(Test) << std::endl;
    Test obj;
    std::cout << "sizeof(Test obj)=" << sizeof(obj) << std::endl;

    class SizeClass {

    };
    std::cout << "sizeof(SizeClass)=" << sizeof(SizeClass) << std::endl;
    SizeClass obj1;
    std::cout << "sizeof(SizeClass obj1)=" << sizeof(obj1) << std::endl;

//    char* str1[]="abc";
//    char* str2[]="abc";

//    std::cout << "cmpret=" << strcmp(str1, str2) << std::endl;

//    std::cout << "sizeof(str1)=" << sizeof(str1) << " strlen=" << strlen(str1) << std::endl;
//    std::cout << "sizeof(str2)=" << sizeof(str2) << " strlen=" << strlen(str2) << std::endl;
    return ;

    uint64_t ufileId = -123;

    if ( (int64_t)ufileId < 0 ) {
        std::cout << "ufileId < 0, ufileId=" << ufileId << std::endl;
    } else {
        std::cout << "ufileId >= 0, ufileId=" << ufileId << std::endl;
    }
}

// 最大连续子数组
int tst_MaxSubArray(int* array, int length) {
    int maxSum = 0;
    int curSum = 0;

    for (int i = 0; i < length; i++ ) {

        for (int j = i; j < i; j++ ) {

            for (int k = i; k < j; k++) {
                curSum += array[k];
            }

            if (curSum > maxSum)
                maxSum = curSum;

            curSum = 0;
        }

    }
    return maxSum;
}


// sizeof一个空类是多少，含有普通成员函数呢，含有虚函数呢，含有static函数或者static变量呢
class CTest1{
public:
    CTest1() {}
    ~CTest1() {}

    virtual void abc() {std::cout << "abc" << std::endl;}
    virtual void def() {std::cout << "def" << std::endl;}
    virtual void hij() {std::cout << "hij" << std::endl;}
private:
    static int s_ivalue;
};
int CTest1::s_ivalue = 1;

void tst_class_1() {
    std::cout << "sizeof(CTest1)=" << sizeof(CTest1) << std::endl;
}


#include <stdio.h>
#include <arpa/inet.h>

void tst_host_net_shunxu() {
    unsigned int ai = 0x12345678;
    unsigned long a = 0x12345678;
    unsigned char *p = (unsigned char *)(&a);

    std::cout << "sizeof(ai)=" << sizeof(ai) << std::endl;
    std::cout << "sizeof(a)=" << sizeof(a) << std::endl;
    printf("主机字节序:%0x    %0x   %0x   %0x\n",  p[0], p[1], p[2], p[3]);

    unsigned long b = htonl(a);  //将主机字节序转化成了网络字节序
    p = (unsigned char *)(&b);
    printf("网络字节序:%0x    %0x   %0x   %0x\n",  p[0], p[1], p[2], p[3]);
}




struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* getPartation(ListNode *start, ListNode *end)
    {
        if (start == end) return start ;

        ListNode *p1 = start ;
        ListNode *p2 = p1->next ;
        int key = start->val ;

        while(p2 != end)
        {
            if (p2->val < key)
            {
                p1 = p1->next ;
                if ( p1 != p2 )
                    swap(p1->val, p2->val) ; //找到一个比key小的数字，与p1到p2间的数交换，
            }										//这之间的数都大于等于key
            p2 = p2->next ;
        }
        swap(start->val, p1->val) ; //找到划分位置
        return p1 ;
    }

    void QuickSort(ListNode* start, ListNode *end)
    {
        if (start != end)
        {
            ListNode *pt = getPartation(start, end) ;
            QuickSort(start, pt) ;
            QuickSort(pt->next, end) ;
        }
    }

    ListNode *sortList(ListNode *head) {
        QuickSort(head, NULL) ;
        return head ;
    }
};

void insertBack(ListNode** head, ListNode** tail,  ListNode* n) //从尾部插入
{
    if (n)
    {
        if (*head == NULL)
        {
            *head = n ;
            *tail = n ;
        }
        else
        {
            (*tail)->next = n ;
            *tail = n ;
        }
    }
}

int tst_list_qs()
{
    ifstream in("data.txt") ;
    ListNode* head = NULL ;
    ListNode* tail = NULL ;
    int val ;

    Solution s ;
    while(in >> val)
    {
        ListNode*tmp = new ListNode(val) ;
        insertBack(&head, &tail, tmp) ;
    }
    head = s.sortList(head) ;
    while(head)
    {
        cout << head->val << " " ;
        head = head->next ;
    }
    cout << endl ;
    return 0 ;
}


void use_cpu() {
//    std::cout << "tid=" << muduo::CurrentThread::tid() << " is working..." << std::endl;
    printf( "tid=%d beg work!!!\n", muduo::CurrentThread::tid() );
    while (1) {

    }
}
void tst_cpu_Use() {
    std::vector < std::thread* > v_threads;
    std::thread tmpThread;
    for (int i = 0; i < 4; ++ i) {
        v_threads.push_back( new std::thread(&use_cpu) );
    }
    for (int i = 0; i < 4; ++ i) {
        v_threads[i]->join();
    }
    std::cout << "all thread work done" << std::endl;
}

void tst_list_print() {
    std::list< int > list1; list1.push_back(127);list1.push_back(257);list1.push_back(15);
    list1.reverse();
    for (list<int>::iterator iter = list1.begin(); iter != list1.end(); iter++)
        cout << *iter << " + ";
    std::cout << std::endl;

    std::cout << "-------------------------" << std::endl;

    list1.reverse();
    for (list<int>::iterator iter = list1.begin(); iter != list1.end(); iter++)
        cout << *iter << " + ";
    std::cout << std::endl;
}

void tst_pri_queue_1() {

    int i = 0;
    int len = 5;
    int a[/*len*/] = {3,5,9,6,2};

    priority_queue<int> qi;
    for(i = 0; i < len; i++)
        qi.push(a[i]);

    for(i = 0; i < len; i++)
    {
        std::cout << qi.top() << " ";
        qi.pop();
    }
    std::cout << std::endl;

    typedef struct  _tagNode
    {
        int m_val;
         _tagNode() : m_val(-1) {}
    }Node;

}

void tst_ConstructSql() {
    std::string strStmt("INSERT INTO @table (id,parentId,title,helpType,linkType,clientType,displayStatus,linkUrl,detailUrl,content,sortNum,createTime,updateTime,status)");
    bool bCheck = ConstructInsertSql(strStmt);
    if (!bCheck) {
//        LOG_WARN( "insert sql construct failed! sql=" << strStmt );
        std::cout << "failed!!! insert result=" << strStmt << std::endl;
    } else {
        std::cout << "successed!!! insert result=" << strStmt << std::endl;
    }
}

void ConstructUpdateSystemHelpSql(std::string& _returnSql, const SystemHelp& in_systemHelp) {

    std::string sql = " parentId=";
    SPLICESQL((in_systemHelp.parentId > 0), _returnSql, sql + convert<std::string>(in_systemHelp.parentId) + ",");

    sql = " title=\'";
    SPLICESQL((!in_systemHelp.title.empty()), _returnSql, sql + in_systemHelp.title + "\',");

    sql = " helpType=";
    SPLICESQL((in_systemHelp.helpType > 0), _returnSql, sql + convert<std::string>(in_systemHelp.helpType) + ",");

    sql = " linkType=";
    SPLICESQL((in_systemHelp.linkType > 0), _returnSql, sql + convert<std::string>(in_systemHelp.linkType) + ",");

    sql = " clientType=\'";
    SPLICESQL((!in_systemHelp.clientType.empty()), _returnSql, sql + in_systemHelp.clientType + "\',");

    sql = " displayStatus=";
    SPLICESQL((in_systemHelp.displayStatus > 0), _returnSql, sql + convert<std::string>(in_systemHelp.displayStatus) + ",");

    sql = " linkUrl=\'";
    SPLICESQL((!in_systemHelp.linkUrl.empty()), _returnSql, sql + in_systemHelp.linkUrl + "\',");

    sql = " detailUrl=\'";
    SPLICESQL((!in_systemHelp.detailUrl.empty()), _returnSql, sql + in_systemHelp.detailUrl + "\',");

    sql = " content=\'";
    SPLICESQL((!in_systemHelp.content.empty()), _returnSql, sql + in_systemHelp.content + "\',");

    sql = " sortNum=";
    SPLICESQL((in_systemHelp.sortNum > 0), _returnSql, sql + convert<std::string>(in_systemHelp.sortNum) + ",");

    sql = " updateTime=";
    SPLICESQL((in_systemHelp.updateTime > 0), _returnSql, sql + convert<std::string>(in_systemHelp.updateTime) + ",");

    sql = " status=";
    SPLICESQL((in_systemHelp.status > 0), _returnSql, sql + convert<std::string>(in_systemHelp.status) + ",");

    remove_last_comma(_returnSql);

    _returnSql += (std::string(" where id=") + convert<std::string>(in_systemHelp.id));

}

bool updateSystemHelp(const SystemHelp& in_systemHelp) {
    std::string strStmt("update @table set ");
    ConstructUpdateSystemHelpSql(strStmt, in_systemHelp);
    std::cout << "strStmt=" << strStmt << std::endl;
    return true;
}

void tst_systemHelp_use_1() {
    SystemHelp newObj;
    newObj.id = 800;
    newObj.parentId = 97876;
    newObj.clientType = "tstclient1_type";
    newObj.createTime = GetCurMilliSecond();
    updateSystemHelp(newObj);
}
void tst_systemHelp_use_2() {
    std::vector< SystemHelp > vec_Task;
    for ( int i = 1; i <= 10; ++i ) {
        SystemHelp newObj;
        newObj.id = (1024 + i);
        vec_Task.push_back(newObj);
    }
//    for ( int i = 0; i < vec_Task.size(); ++i ) {
//        vec_Task[i].id += 1000;
//    }
    for ( int i = 0; i < vec_Task.size(); ++i ) {
        std::cout << "i=" << 0 << ", val=" << vec_Task[i].id << std::endl;
    }
}

void tst_rand() {
    srand(time(NULL));

    int count = 20;
    for (int i = 0; i < count; ++i) {
        int r = rand()%(10);
        std::cout << "rand=" << r << std::endl;
    }
}
int tst_rand(int low, int high)
{
    return rand()%(high-low+1)+low;
}
void tst_randEx() {
    time_t t;
    srand(time(&t));
    for (int i = 0; i < 20; ++i) {
        std::cout << "out=" << tst_rand(100, 200) << std::endl;
    }
}

int foo(int in) { return 32; }

//extern static int s_val;
void tstMemoryLeak() {
    boost::function<int(int)> func;
    func = foo;
    std::cout << func.empty() << std::endl;

//    func = NULL;
    func.clear();
    std::cout << func.empty() << std::endl;

    func = boost::function<int(int)>();
    std::cout << func.empty() << std::endl;
    return ;

    char *p = (char*)malloc(1024);
    sprintf(p, "%s", "abcde");
    printf("%s\n", p);
    int *p1 = (int*)malloc(4);
    *p1 = 100;
    void *p2 = malloc(256);
//    free(p1);
    int a = 2;
    printf("%d\n", a);
//    free(p);
    void *p3 = malloc(124);
//    free(p2);
    void *p4 = malloc(23);
//    free(p4);
    //    getchar();
}

class CTestBind {
public:
    CTestBind() {

    }
    virtual ~CTestBind() {

    }
public:
    int TestPrint(int param1, const std::string strParam2, int ival3) {
        std::cout << "param1=" << param1 << ", strParam2=" << strParam2 << ", ival3=" << ival3 << std::endl;
    }
};

boost::function< int (int, const std::string, int) > g_tmpFunc;
void tryPrint() {
    if (g_tmpFunc) {
        g_tmpFunc(-1, "-777", 33333);
    }
}
void tst_boostBind_1() {
    CTestBind ctstBindObj;

//    boost::function< int (int, const std::string, int) > tmpFunc;
    g_tmpFunc = boost::bind( &CTestBind::TestPrint, &ctstBindObj, 777, "88990", _3 );

    tryPrint();
}

//==================================================================
// 《剑指Offer——名企面试官精讲典型编程题》代码
// 作者：何海涛
//==================================================================

void tst_catFile() {

    std::string strCmd, file1, file2, lstfile;
    file1 = "file1";
    file2 = "file2";
    lstfile = "lastfile";

    char tmpBuffer[2048] = {0};
    sprintf(tmpBuffer, "cat %s %s > %s",
            file1.c_str(), file2.c_str(), lstfile.c_str());

    int iret = system( tmpBuffer );
    std::cout << "tmpBuffer=" << tmpBuffer << std::endl;
    std::cout << "iret=" << iret << std::endl;

//    {
//        char tmpBuffer[2048] = {0};
//        sprintf(tmpBuffer, "mv %s %s", lstfile.c_str(), file1.c_str());

//        int iret = system( tmpBuffer );
//        std::cout << "tmpBuffer=" << tmpBuffer << std::endl;
//        std::cout << "iret=" << iret << std::endl;
//    }

}


int MergeTwoFile(const std::string& file1, const std::string& file2) {
    std::string c_1 = GetFileContent_string(file1);
    std::string c_2 = GetFileContent_string(file2);

    std::string strMerged = c_1 + c_2;

//    int ifind = file1.rfind('/');
//    if (ifind == std::string::npos) {
//        return -1;
//    }
//    std::cout << "i=" <<ifind <<std::endl;

//    return 0;
    std::string fileMerge = file1;//.substr(0, ifind+1);
    fileMerge += "123";
    FILE* pwrite = fopen(fileMerge.c_str(), "wb");
    if (!pwrite) {
//        LOG_ERROR(" open write file failed, file=" << fileMerge.c_str());
        return -1;
    }

    fwrite(strMerged.c_str(), 1, strMerged.size(), pwrite);
    fclose(pwrite);

    int iret = rename(fileMerge.c_str(), file1.c_str());
    std::cout << "iret=" << iret << std::endl;
    return 0;
}

void tst_catFileEx() {
//    std::string file1 = GetFileContent_string("/tmp/file1");
//    std::string file2 = GetFileContent_string("file2");

//    std::string strAllFile = file1+file2;
//    FILE* pwrite = fopen("exout", "wb");
//    fwrite(strAllFile.c_str(), 1, strAllFile.size(), pwrite);
//    fclose(pwrite);
//    ::rename("exout", "newexout");
    MergeTwoFile("/tmp/file1", "file2");
}

void tst_print_time() {
#define HOURS_CALC(x)   (x * 365 * 24)

    const char *wday[]={"星期天","星期一","星期二","星期三","星期四","星期五","星期六"};
    {
        boost::posix_time::ptime ttt;
        std::tm t = boost::posix_time::to_tm(ttt);
//        X509_VERIFY_PARAM_set_time(raw(), ::mktime(&t));
//        ::mktime(&t);
        time_t xti = (::mktime(&t));
        struct tm* p = localtime(&xti);
        struct tm* pp;
        time_t tts = timegm(&t);

        printf("1111 %d年%d月%d日 %d:%d:%d %s\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec, wday[p->tm_wday]);
        return ;
    }

    {
        time_t timeNow = time(NULL);

        struct tm* p = localtime(&timeNow);

        struct tm* pp;

        printf("1111 %d年%d月%d日 %d:%d:%d %s\n",p->tm_year+1900,p->tm_mon+1,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec, wday[p->tm_wday]);

        pp = gmtime(&timeNow);

        printf("1111 %d年%d月%d日 %d:%d:%d %s\n",pp->tm_year+1900,pp->tm_mon+1,pp->tm_mday,pp->tm_hour,pp->tm_min,pp->tm_sec, wday[pp->tm_wday]);
//        return ;
    }

    {
        time_t timep;

        time(&timep); /*获取time_t类型的当前时间*/
        /*用gmtime将time_t类型的时间转换为struct tm类型的时间按，／／没有经过时区转换的UTC时间
          然后再用asctime转换为我们常见的格式 Fri Jan 11 17:25:24 2008
        */
        printf("%s", asctime(gmtime(&timep)));
    }

    {
//        time_t timep;

//        time(&timep); /*获取time_t类型当前时间*/
//        /*转换为常见的字符串：Fri Jan 11 17:04:08 2008*/
//        printf("%s", ctime(&timep));
//        char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
        time_t timep;
        struct tm *p;

        time(&timep); /*获得time_t结构的时间，UTC时间*/
        p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
        //
        printf("GMT   timeFormat=%d-%d-%d ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
        printf("%d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
//        printf("%s %d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
    }

    {
        time_t tmp_time;
        struct tm *ptime;
        struct tm *tmp;
        tmp_time = time(NULL);//获取当前时间
        ptime = localtime(&tmp_time);//转换成 本地时间

        printf("Local timeFormat=%d-%d-%d %d:%d:%d\n",(1900+ptime->tm_year),(1+ptime->tm_mon),ptime->tm_mday,\
                    ptime->tm_hour,ptime->tm_min,ptime->tm_sec);

        return;

        tmp_time += 30*60;
        tmp = localtime(&tmp_time);
        printf("%d-%d-%d %d:%d:%d\n",(1900+tmp->tm_year),(1+tmp->tm_mon),tmp->tm_mday,\
                    tmp->tm_hour,tmp->tm_min,tmp->tm_sec);

        ptime->tm_min += 30;
        printf("%d-%d-%d %d:%d:%d\n",(1900+ptime->tm_year),(1+ptime->tm_mon),ptime->tm_mday,\
                    ptime->tm_hour,ptime->tm_min,ptime->tm_sec);
        return ;
    }

//    std::cout << "3 years=" << HOURS_CALC(3) << std::endl;
//    return;

//    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm *p;

    time(&timep); /*获得time_t结构的时间，UTC时间*/
    p = localtime(&timep); /*转换为struct tm结构的当地时间*/
    printf("%d%02d%02d ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
    printf("%02d%02d%02d\n", p->tm_hour, p->tm_min, p->tm_sec);
//    printf("%s %d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);

    std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;

    timep += 365 * 24 * 60 * 60;
    p = localtime(&timep); /*转换为struct tm结构的当地时间*/
    printf("%d%02d%02d ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
    printf("%02d%02d%02d\n", p->tm_hour, p->tm_min, p->tm_sec);
}


void GetInOut( const std::vector< int >& vstocks, int& inBuy, int& outBuy) {
    int min = 0;

    int buyin  = 0;
    int buyout = 0;

    int maxProfit = 0;
    int curProfit = 0;

    for ( int i = 1; i < vstocks.size(); ++i ) {

        curProfit = vstocks[i] - vstocks[min];

        if (maxProfit < curProfit) {
            maxProfit = curProfit;
            buyin  = min;
            buyout = i;
        }
        if (vstocks[i] < vstocks[min]) {
            min = i;
        }
    }

    inBuy  = buyin;
    outBuy = buyout;
}
// N天内,一次交易
// 获取股票买卖利润最大值
void tst_GetInout() {
    std::vector<int> vStocks;
    vStocks.push_back( 5 );  vStocks.push_back( 2 );
    vStocks.push_back( 12 ); vStocks.push_back( 6 );
    vStocks.push_back( 1 );  vStocks.push_back( 19 );

    int byIn, byOut = 0;
    LOG_INFO << "byIn=" << byIn << ", byOut=" << byOut;
    GetInOut( vStocks, byIn, byOut );
    LOG_INFO << "byIn=" << byIn << ", byOut=" << byOut;
}

void tst_ptr_vec () {

    boost::ptr_vector < CTest1 > ptr_v;

    CTest1* pObj1 = new CTest1;

    ptr_v.push_back( pObj1 );

    LOG_INFO << "obj addr=" << pObj1;

    for ( auto pIt = ptr_v.begin(); pIt != ptr_v.end(); ++pIt ) {

        CTest1* pTmp = &(*pIt);

        LOG_INFO << "pTmp addr=" << pTmp;
    }


}

int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

//    tst_ptr_vec(); return 1;
    tst_MergeSortEntry_(); return 1;

    tst_qs_new(); return 1;

    tst_HeapSortEntry_(); return 1;

    tst_GetInout(); return 1;

    tst_qs_111(); return 1;

    tst_ListEntry_(); return 1;

#ifdef __cplusplus//C++编译器定义了-cplusplus
std::cout << "c++" << std::endl;
#else
std::cout << "c" << std::endl;
#endif

std::cout << "argc=" << argc << std::endl;
/////////////////////
    if (argc >= 2) {
        if ( 0 == strcmp(argv[1], "heapsort"/*, strlen("heapsort")*/) ) {
            std::cout << "heapsort beg" << std::endl;
            tst_HeapSortEntry_();
            return 1;
        } else if ( 0 == strcmp(argv[1], "mergesort"/*, strlen("heapsort")*/) ) {
            std::cout << "mergsort beg" << std::endl;
            tst_MergeSortEntry_(); return 1;
            return 1;
        }

    }

    tst_bitmap_entry(); return 1;

    tst_stack_entry(); return 1;

    tst_linkedlist_entry();
    return 1;

    tst_hash_fun_entry();
//    return 1;
    tst_Extern_MergeSortEntry_();
    return 1;

//    CopyFileEx("template-openssl.cnf", "123.cnf");
//    return 1;

//    std::cout << std::endl;
    std::cout << std::endl;


    tst_ComplexDataEntry_(); return 1;

tst_print_time(); return 1;

    tst_struct_fun(); return 1;

    tst_vec_quchong();
    return 1;

//    tst_catFile();
    tst_catFileEx();
    return 1;

    tst_maxheap_sort(); return 1;

    tst_qs_111();  return 1;

    ts_PrintLines(); return 1;

    tst_boostBind_1(); return 1;
    tstMemoryLeak(); return 1;

    std::cout << "s_val=" << ++ s_val << std::endl;
    tst_randEx();
    return 1;

    tst_rand(); return 1;
    tst_cpy_data();  return 1;

    Print_yinyong(); return 1;

    print_continue_seq(); return 1;
    tst_systemHelp_use_2(); return 1;

    tst_class_a(); return 1;

    tst_systemHelp_use_1(); return 1;
    tst_ConstructSql(); return 1;

    yinyongBind();

//    std::string tmpPath = "";
//    if ( tmpPath.at(tmpPath.size()-1) == '/' ) {
//        std::cout << "find ///" << std::endl;
//        tmpPath = tmpPath.substr(0, tmpPath.size()-1);
//        std::cout << "aaa tmpPath=" << tmpPath << std::endl;
//    } else
//        std::cout << "bbb tmpPath=" << tmpPath << std::endl;

print_time ();

    return 1;

    tst_list_qs(); return 1;


//    trie_tree_1(); return 1;

//    int sum = 0, minSum = 0, maxSum = INT_MIN;
//    std::cout << "maxSum=" << maxSum << std::endl;
//    return 1;

    TestMemberFunction();
    TestBoostBind();

    return 1;

    tst_pri_queue_1(); return 1;
    tst_class_1(); return 1;
    return 1;


    tst_host_net_shunxu(); return 1;
//

    tst_int64_1(); return 1;


    std::string addFile_      = "insert into @table \
(fileId,pid,ownerId,fileName,fileType,fileSuffix,filePath,fileSize,encryptFileSize,secretKey,\
uploaderId,uploaderName,md5Hash,sha1Hash,createdAt,updatedAt,deletedAt,fileCode,orderNum,\
roleId,members,fileReMark,pdfPath,validity,fileClass) \
values (?,?,?,?,?,?,?)";
    std::cout << "addFile_=" <<  addFile_ << std::endl;
    ConstructInsertSql( addFile_ );
return 1;

    tst_sort_3(); return 1;

//    tst_sort_2();
//    return 1;
    tst_sort_1();
    return 1;
}













