﻿






#include <stdio.h>

#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>
#include <stack>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



//#include <numeric_limits.hpp>
#include <limits>
#include <iostream>

#include <unordered_map>


#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"

//#include "quicksort.h"
//#include "tst_signal.h"
#include "mysqlinsert.h"
//#include "cpp11fun.h"


//#include "tst_1.h"
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

#include "bst.h"
#include "List.h"
#include "tstdp.h"
#include "lc.h"



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



extern void tst_link_1();
extern void PrintInorder(BstNode* root);


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
ListNode* g_pListHead;

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
    std::vector<int> arr;
    arr.push_back(4);arr.push_back(6);arr.push_back(3);
    arr.push_back(1);arr.push_back(5);arr.push_back(2);
    arr.push_back(7);arr.push_back(0);

    Mysort(arr);
    for (size_t i = 0; i < arr.size(); i ++)
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
    for(i=0; i<10; i++) {
        index++;
        printf("%d, %d\n", index, index);
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


// 联机算法：联机算法是在任意时刻算法对要操作的数据只读入(扫描)一次，
// 一旦被读入并处理，它就不需要在被记忆了。而在此处理过程中算法能对它已经读入的数据立即给出相应子序列问题的正确答案。
int max_subarray( const std::vector<int>& v, int left, int right ) {
    int tmp    = 0;
    int maxSum = 0;
    for (int i = left; i < right; i ++) {
        tmp += v[i];
        if (tmp > maxSum)
            maxSum = tmp;
        if (tmp < 0) {
            tmp = 0;
        }
    }
    return maxSum;
}
// 给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。
void wrap_max_subarray() {
    std::vector < int > v {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    auto maxsum = max_subarray( v, 0, v.size() - 1);
    std::cout << "maxsum=" << maxsum << std::endl;
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




//struct ListNode {
//    int val;
//    ListNode *next;
//    ListNode(int x) : val(x), next(NULL) {}
//};

//class Solution {
//public:
//    ListNode* getPartation(ListNode *start, ListNode *end)
//    {
//        if (start == end) return start ;

//        ListNode *p1 = start ;
//        ListNode *p2 = p1->next ;
//        int key = start->val ;

//        while(p2 != end)
//        {
//            if (p2->val < key)
//            {
//                p1 = p1->next ;
//                if ( p1 != p2 )
//                    swap(p1->val, p2->val) ; //找到一个比key小的数字，与p1到p2间的数交换，
//            }										//这之间的数都大于等于key
//            p2 = p2->next ;
//        }
//        swap(start->val, p1->val) ; //找到划分位置
//        return p1 ;
//    }

//    void QuickSort(ListNode* start, ListNode *end)
//    {
//        if (start != end)
//        {
//            ListNode *pt = getPartation(start, end) ;
//            QuickSort(start, pt) ;
//            QuickSort(pt->next, end) ;
//        }
//    }

//    ListNode *sortList(ListNode *head) {
//        QuickSort(head, NULL) ;
//        return head ;
//    }
//};

//void insertBack(ListNode** head, ListNode** tail,  ListNode* n) //从尾部插入
//{
//    if (n)
//    {
//        if (*head == NULL)
//        {
//            *head = n ;
//            *tail = n ;
//        }
//        else
//        {
//            (*tail)->next = n ;
//            *tail = n ;
//        }
//    }
//}

//int tst_list_qs()
//{
//    ifstream in("data.txt") ;
//    ListNode* head = NULL ;
//    ListNode* tail = NULL ;
//    int val ;

//    Solution s ;
//    while(in >> val)
//    {
//        ListNode*tmp = new ListNode(val) ;
//        insertBack(&head, &tail, tmp) ;
//    }
//    head = s.sortList(head) ;
//    while(head)
//    {
//        cout << head->val << " " ;
//        head = head->next ;
//    }
//    cout << endl ;
//    return 0 ;
//}


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

    // typedef struct  _tagNode
    // {
    //     int m_val;
    //      _tagNode() : m_val(-1) {}
    // } Node;

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
    for ( size_t i = 0; i < vec_Task.size(); ++i ) {
        std::cout << "i=" << 0 << ", val=" << vec_Task[i].id << std::endl;
    }
}

void tst_rand() {
    // srand(time(NULL));
    time_t t;
    srand(time(&t));
    int count = 20;
    for (int i = 0; i < count; ++i) {
        int r = rand()%(10);
        std::cout << "rand=" << r << std::endl;
    }
}
int tst_rand(int low, int high)
{
    //     time_t t;
    // srand(time(&t));
    return rand()%(high-low+1)+low;
}
void tst_randEx() {
    for (int i = 0; i < 20; ++i) {
        std::cout << " " << tst_rand(1, 200);
    }
    std::cout << std::endl;
}


//extern static int s_val;
void tstMemoryLeak() {
//     boost::function<int(int)> func;
//     func = foo;
//     std::cout << func.empty() << std::endl;

// //    func = NULL;
//     func.clear();
//     std::cout << func.empty() << std::endl;

//     func = boost::function<int(int)>();
//     std::cout << func.empty() << std::endl;
//     return ;

//     char *p = (char*)malloc(1024);
//     sprintf(p, "%s", "abcde");
//     printf("%s\n", p);
//     int *p1 = (int*)malloc(4);
//     *p1 = 100;
//     void *p2 = malloc(256);
// //    free(p1);
//     int a = 2;
//     printf("%d\n", a);
// //    free(p);
//     void *p3 = malloc(124);
// //    free(p2);
//     void *p4 = malloc(23);
// //    free(p4);
//     //    getchar();
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
        return 1;
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
//        struct tm* pp;
//        time_t tts = timegm(&t);

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

double g_iBenJin = 1000 * 10000;

void tst_lixi() {
//    double yinhang = 0.04;
    double yuebao  = 0.028;

    double year_yinhang = g_iBenJin * 0.04;
    std::cout << "year_yinhang=" << year_yinhang << std::endl;

    double lixi = 0;
    double tmpBenjin = g_iBenJin;
    // double sum = 0;
    for ( int i = 1; i < 365; ++ i ) {
        lixi += (tmpBenjin * yuebao) / 365;
        tmpBenjin = tmpBenjin + lixi;
    }
    std::cout << "year_yuebao=" << lixi << std::endl;
}


//把一个数组最开始的若干个元素搬到数组的末尾，我们称之为数组的旋转。
//输入一个非减排序的数组的一个旋转，输出旋转数组的最小元素。
//例如数组{3,4,5,1,2}为{1,2,3,4,5}的一个旋转，该数组的最小值为1。
//NOTE：给出的所有元素都大于0，若数组大小为0，请返回0。
int findmin_inrotatearrry( std::vector<int> v ) {
    int left = 0;
    int right = v.size() - 1;
    int mid = left;

    while (v[left] >= v[right]) {
        if (right - left == 1) {
            mid = right;
            break;
        }

        mid = left + (right - left) / 2;
        std::cout << "while mid=" << mid << std::endl;

        if (v[mid] >= v[left]) {
            left = mid;
        } else if (v[mid <= v[right]]) {
            right= mid;
        }
    }
    std::cout << "mid=" << mid << std::endl;
    return v[mid];
}
void tst_findmin_inrotatearrry() {
    std::vector< int > v;
    v.push_back(3);v.push_back(4);v.push_back(5);
    v.push_back(1);v.push_back(2);
    std::cout << "findmin_inrotatearrry=" << findmin_inrotatearrry(v) << std::endl;
}

// 面试题15之链表中倒数第K个结点
ListNode* FindKthToTail(ListNode* pListHead, unsigned int k) {
    ListNode* pFront = pListHead;
    // 倒数第k个结点与倒数第一个结点相隔k-1个位置
    for (unsigned int i = 0; i < k - 1; i ++) {
        if (pFront->next)
            pFront = pFront->next;
        else
            return NULL;
    }
    ListNode* pBehind = pListHead;
    while (pFront->next) {
        pFront = pFront->next;
        pBehind = pBehind->next;
    }
    return pBehind;
}
// 寻找单链表的中间节点
// 当链表中有偶数个元素时，最中间的两个节点都可以算作中间节点；
// 但通过该方法查找到的是前面那个节点；如｛1，2，3，4，5｝，通过该方法查找到的中间节点是3。
// 查找中间节点，如果是奇数个队列元素，返回中间
ListNode* FindMidNode(ListNode* pHead) {
    ListNode* pMid  = NULL;
    ListNode* ptemp = pHead;

    while (pHead && pHead->next && pHead->next->next) {
        pHead = pHead->next->next;
        ptemp = ptemp->next;
        pMid = ptemp;
    }
    return pMid;
}
void call_rever_KthNode() {
    printf("=====Test1 starts:=====\n");
    ListNode* pNode1 = CreateListNode(1);
    ListNode* pNode2 = CreateListNode(2);
    ListNode* pNode3 = CreateListNode(3);
    ListNode* pNode4 = CreateListNode(4);
    ListNode* pNode5 = CreateListNode(5);

    ConnectListNodes(pNode1, pNode2);
    ConnectListNodes(pNode2, pNode3);
    ConnectListNodes(pNode3, pNode4);
    ConnectListNodes(pNode4, pNode5);

    printf("expected result: 4.\n");
    ListNode* pNode = FindKthToTail(pNode1, 2);

    PrintListNode(pNode);
    pNode = FindMidNode(pNode1);
    PrintListNode(pNode);

    DestroyList(pNode1);
}

//
int Max_array_Sum(int* arr, int length) {
    int max = arr[0];
    int sum = arr[0];
    for (int i = 1; i < length; i++) {
        if (sum < 0) {
            sum = arr[i];
        } else {
            sum += arr[i];
        }
        if (max < sum) {
            max = sum;
        }
    }
    return max;
}
// 求连续子数组的最大和
void call_Max_array_Sum() {
    int arr[]  = {6, -3, -2, 7, -15, 1, 2, 2};
    int length = array_size(arr);
    // last result value=8
    // set: "6, -3, -2, 7"
    std::cout << "sum=" << Max_array_Sum(arr, length) << std::endl;
}

// 给定数组a, 里面的元素先严格递增后严格递减, 求最大值元素的下标
int find_max_in_array(const std::vector<int>& v) {
//    if (nums.length == 1) {
//        return 0;
//    }
//    if (nums[0] > nums[1]) {//数组单调递减
//        return 0;
//    }
//    int index = nums.length-1;
//    if (nums[index] > nums[index-1]) {//数组单调递增
//        return index;
//    }
    int left = 0;
    int right = v.size() - 1;
    int mid = 0;
    while ( left < right ) {
        mid = (left + right) >> 1;
//        std::cout << "mid=" << mid << ", v[mid]=" << v[mid]
//                  << ", v[mid + 1]=" << v[mid + 1]
//                  << ", v[mid - 1]=" << v[mid - 1] << std::endl;

        if (v[mid] > v[mid - 1] && v[mid] > v[mid + 1]) {
            return mid;
        }
        else if (v[mid] > v[mid + 1]) {// 处于下坡段, 即递减段
            right = mid - 1;
        } else if (v[mid] > v[mid - 1]) {//处于上坡段, 即递增段
            left = mid + 1;
        }
    }
    return -1;
}
void call_find_max_in_array() {
    std::vector<int> v;
    for (int i = 0; i < 10; i+=2) {
        v.push_back(i);
    }
    for (int j = 15; j > 0; j-=3) {
        v.push_back(j);
    }
    std::cout << "orignal array: ";
    for (size_t k = 0; k < v.size(); k++) {
        std::cout << v[k] << " ";
    }
    std::cout << std::endl;

    int idx = find_max_in_array(v);
    if (idx != -1) {
        std::cout << "find maxVal Idx=" << idx << ", value=" << v[idx] << std::endl;
    } else {
        std::cout << "no found max val" << std::endl;
    }
}


void MakeX7() {
    int iVal = 8;
    int iLst = (iVal << 3)/* - 8*/;
    std::cout << "iLst= " << iLst <<std::endl;
}

char* reverse_string(char* s) {
    char* p = s;
    char* q = s;
    while (*q) {
        q ++ ;
    }
    q --;

    while (q > p) {
        char tmp = *p;
        *p++ = *q;
        *q-- = tmp;
    }
    return s;
}
void my_itoa( int iVal ) {

    bool bIsNegtaive = false;

    int  i          = 0;
    char all[ 256 ] = {0};

    if ( iVal < 0 ) {
        iVal = -iVal;
        bIsNegtaive = true;
    }

    do {
        all[ i++ ] = iVal % 10 + '0';
        iVal = iVal / 10;
    } while(iVal > 0);

    if (bIsNegtaive) {

        all [ i++ ] = '-';
    }
    reverse_string(all);
    std::cout << "last=" << all << std::endl;
}

// char* returnstr()
// {
    // char p[] = "hello";
    // printf("111=%d\n",sizeof(p));
    // printf("222=%d\n",strlen(p));
// }
void tst_some_one_1()
{
    class Base1
    {
    public:
        int base1_1;
        int base1_2;
    };
    std::cout << "sizeof(Base1)=" << sizeof(Base1) << std::endl;
    return;

//     char* s = returnstr();
//     printf("%s\n",s);
}

//#define f(a,b) a##b

//#define g(a) #a
//#define h(a) g(a)


//#define A(x) T_##x
//#define B(x) #@x
//#define C(x) #x


//#define WARN_IF(EXP) if(EXP) cerr << #EXP << endl;
//#define paster( n ) cout << "token" << #n << " = " << n << endl;
//#define _CONS(a, b) int(a##+##b)
//#define _STRI(s) #s

void tst_111()
{
//    std::cout << "a###b=" << f(1, 2) << std::endl;
//    printf( "a###b=%d\n", f(1,2) );

//    printf("%s\n", h(f(1, 2))); //输出:12
//    printf("%s\n", g(f(1, 2))); //输出:f(1,2)
//    printf("%s\n", h(A(1)));    // A(1)------〉T_1
//    printf("%d\n", B(1));       // B(1)------〉'1'
//    printf("%s\n", C(1));       // C(1)------〉"1"

//    int div = 0;
//    WARN_IF(div == 0);              //输出: div == 0
//    paster(9);                      //输出: token9 = 9
//    std::cout << _CONS(1+2, 2) << std::endl;  //输出: 3
//    std::cout << _STRI(INT_MAX) << std::endl; //输出: INT_MAX
//    std::cout << STRI(INT_MAX) << std::endl; // prints : 2147483647
}

class CSomeStatic {
public:
    CSomeStatic() {}
    ~CSomeStatic() {}

    static void PrintVal() {
        std::cout << "val=" << m_val << std::endl;
    }
public:
    static int m_val;
};
int CSomeStatic::m_val = 1123;


void StackHeapObj() {

    CSomeStatic::PrintVal();
return;
    char* pbuffer = new char[ 1024 ] ;
    memset(pbuffer, 0, 1024);
    free(pbuffer);
    return;

//    class CA {// only create in Heap
//    public:
//        CA() {
//            printf( "CA construct\n" );
//        }
//    private:
//        ~CA() {
//            printf( "CA destruct\n" );
//        }
//    };

//    class CB {// only create in stack
//    public:
//        CB() {
//            printf( "CB construct\n" );
//        }
//        ~CB() {
//            printf( "CB destruct\n" );
//        }
//    private:
//        void* operator new(size_t size) {}
//        void operator delete(void* pObj) {}
//    };

////    CA objA;
//    CA* pA = new CA;

//    CB objB;
//    CB* pB = new CB;

}


char* strReverse(char* str) {
    if (str == NULL || str == '\0')
        return str;

    char* beg = str;
    char* end = str;

    while (*end != '\0') {
        end ++;
    }
    end --;

    char tmp;
    while ( beg != end ) {
        tmp = *beg;

        *beg = *end;
        *end = tmp;

        beg ++;
        end --;
    }
    return str;
}


std::string strReverseEx(std::string str) {
    {
        std::string ret;
        std::reverse(str.begin(), str.end());

        size_t start = 0;
        size_t pos = str.find(' ', start);
        string word;

        while (pos != string::npos) {
            word = str.substr(start, pos - start);
            std::reverse(word.begin(), word.end());
            word.push_back(' ');

            ret.insert(ret.size(), word);
            start = pos + 1;

            pos = str.find( ' ', start );
        }
        word = str.substr(start);
        std::reverse(word.begin(), word.end());

        ret.insert(ret.size(), word);
        return ret;
    }

}

void tst_reverse_str() {
    char tmpBuffer[ 512 ] = {0};
    strcpy(tmpBuffer, "test one instance");

    std::string ret = strReverseEx((char*)tmpBuffer);
    std::cout << "plst=" << ret << std::endl;
}

typedef BSTNode < int > BstNodeInt;

class MyBst {
private:
    BstNodeInt* _Root;

public:
    MyBst() : _Root (nullptr) {
    }
    ~MyBst() {
    }

    void insert( int key )  {
        BstNodeInt* z = new BstNodeInt(key);
        insert( z );
    }
    void insert( BstNodeInt*& z ) {
        BstNodeInt* parent = nullptr;
        BstNodeInt* temp = _Root;

        if (temp == nullptr) {
            std::cout << "first tree root" << std::endl;
        }

        while (temp) {
            parent = temp;
            if ( z->_key > temp->_key )
                temp = temp->_rchild;
            else
                temp = temp->_lchild;
        }
        z->_parent = parent;

        if ( z->_parent == nullptr ) {//如果树本来就是空树，则直接把z节点插入根节点
            _Root = z;
        } else if ( z->_key > parent->_key ) {//如果z的值大于其双亲，则z为其双亲的右孩
            parent->_rchild = z;
        } else {
            parent->_lchild = z;
        }

    }

    void preOder(BstNodeInt*& tree) {
        if (tree) {
            std::cout << "node=" << tree->_key << std::endl;
            preOder( tree->_lchild );
            preOder( tree->_rchild );
        }
    }
    void inOder(BstNodeInt*& tree) {
        if (tree) {
            inOder( tree->_lchild );
            std::cout << "node=" << tree->_key << std::endl;
            inOder( tree->_rchild );
        }
    }

    void preOder() {
        preOder(_Root);
    }
    void inOder() {
        inOder(_Root);
    }
};


void Opr_Bst() {
    MyBst oneBst;
    oneBst.insert(321); oneBst.insert(12);
    oneBst.insert(978); oneBst.insert(77);
    oneBst.insert(65535); oneBst.insert(1023);

//    oneBst.preOder();
    oneBst.inOder();
}


void showArr(int ages[], size_t length) {
    for(size_t i=0;i<length;i++)
        std::cout << "i=" << ages[i]<<" "<<std::endl;
}

void SortAges(int ages[], size_t length) {

    const int OldAges = 99;
    int tempArr[ OldAges + 1 ] = {0};

    for (size_t i = 0; i < length; ++i ) {
        std::cout << "i=" << ages[i] << " " << std::endl;
        tempArr[ ages[i] ] ++;
    }

    for(int i=0;i<=OldAges;i++) {
        if (tempArr[i])
            std::cout << "i=" << i << ", val=" << tempArr[i]<<" " << std::endl;
    }
//    return;

    int i = 0, j = 0;
    while ( j <= OldAges )
    {
        if ( tempArr[j] != 0 )
        {
            ages[i] = j;
            tempArr[j]--;
            i++;
        } else {
            j++;
        }
    }

}
// 时间复杂度为O(n)的排序算法
void tst_O1_age() {

    int ages[ ] = {23,26,28,32,34,23,23,28,34,41, 21, 63,};
//    std::cout << "length=" << sizeof(ages) / sizeof(ages[0]) << std::endl;

    size_t length = sizeof(ages) / sizeof(ages[0]);

    SortAges(ages, length);
//    return ;
    showArr(ages, length);

}
void tst_lower_upper() {
    int num[6] = { 130, 130, 256, 130, 130, 130 };
    int ret = -1;
    for ( int i = 0; i < 6; ++ i ) {
        ret ^= num[i];
    }
    std::cout << "ret=" << ret << std::endl;
    return;

//    int num[6] = { 130, 69 , 256, 7128, 98, 67};
//    std::sort( num, num + 6 );

//    int pos1 = std::lower_bound( num , num + 6, 99 ) - num;
//    std::cout << "pos1=" << pos1 << " val=" << num[pos1] << std::endl;
}


void tst_class_x() {
    class CA {
    public:
        virtual void foo() {
            printf( "ca foo\n" );
        }
    };
    class CB : public CA {
    public:
//        virtual void foo() {
        void foo() {
            printf ( "cb foo\n" );
        }
    };

    CA* pObj = new CB;
    pObj->foo();
}

// 常规二分法查找
int binary_search( std::vector<int>& v, int target ) {
    int left = 0;
    int right= v.size() - 1;

    while (left <= right) {
        int mid = left + ((right - left) >> 1);

        std::cout << "mid=" << mid << std::endl;
        if ( v[mid] == target ) {
            return mid;
        }

        if ( v[mid] > target ) {
            right = mid - 1;
        }
        if ( v[mid] < target ) {
            left = mid + 1;
        }
    }
    return -1;
}

// 发现第一个等于待查找值，如果数组中存在多个等于待查找值的场景；
int bs_first( std::vector < int >& v, int target ) {
    int size = v.size();
    int left = 0;
    int right= size - 1;
    while ( left <= right ) {
        int mid = left + ((right - left) >> 1);

        std::cout << "mid=" << mid << std::endl;
        if ( v[mid] > target ) {
            right = mid - 1;
        }
        else if ( v[mid] < target ) {
            left = mid + 1;
        } else {
            if (mid == 0 || v[mid - 1] != target) {
                std::cout << "v[mid-1]=" << v[mid-1] << std::endl;
                return mid;
            }
            else {
                right = mid - 1;
            }
        }

    }
    return -1;
}

// 发现最后一个等于待查找值，如果数组中存在多个等于待查找值的场景；
int bs_last( std::vector < int >& v, int target ) {
    int size = v.size();
    int left = 0;
    int right= size - 1;
    while ( left <= right ) {
        int mid = left + ((right - left) >> 1);

        std::cout << "mid=" << mid << std::endl;
        if ( v[mid] > target ) {
            right = mid - 1;
        }
        else if ( v[mid] < target ) {
            left = mid + 1;
        } else {
            if (mid == size - 1 || v[mid + 1] != target) {
                std::cout << "v[mid + 1]=" << v[mid + 1] << std::endl;
                return mid;
            }
            else {
                left = mid + 1;
            }
        }
    }
    return -1;
}

// 查找第一个大于等于给定值的元素
int bs_first_greater_equal( std::vector < int >& v, int target ) {
    int size = v.size();
    int left = 0;
    int right= size - 1;

    while ( left <= right ) {
        int mid = left + ((right - left) >> 1);
//        std::cout << mid << std::endl;

        if ( v[ mid ] >= target ) {
            if (mid == 0 || v[ mid - 1 ] < target) return mid;
            else {
                right = mid - 1;
            }
        } else {
            left = mid + 1;
        }
    }
    return -1;
}

// 查找最后一个小于等于给定值的元素
int bs_last_lesser_equal( std::vector < int >& v, int target ) {
    int size = v.size();
    int left = 0;
    int right= size - 1;

    while ( left <= right ) {
        int mid = left + ((right - left) >> 1);
//        std::cout << mid << std::endl;

        if ( v[ mid ] <= target ) {
            if (mid == (size - 1) || v[ mid + 1 ] > target) return mid;
            else {
                left = mid + 1;
            }
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// 二分算法封装
void binary_search_wrap() {

    // chang gui binary search
    {
//        std::vector <int> v;
//        {
//            v.push_back( 10 );v.push_back( 30 );v.push_back( 980 );
//            v.push_back( 1108 );v.push_back( 1187 );
//            v.push_back( 119900 );
//        }
//        std::cout << binary_search( v, 1187 ) << std::endl;
    }

    // find first == val
    {
//        std::vector <int> v;
//        {
//            v.push_back( 10 ); v.push_back( 30 ); v.push_back( 980 );
//            v.push_back( 1108 ); v.push_back( 1109 );
//            v.push_back( 1187 ); v.push_back( 1187 ); v.push_back( 1187 );
//            v.push_back( 119900 );
//        }
//        std::cout << bs_first( v, 1187 ) << std::endl;
    }

    // find last == val
    {
//        std::vector <int> v;
//        {
//            v.push_back( 10 ); v.push_back( 30 ); v.push_back( 980 );
//            v.push_back( 1108 ); v.push_back( 1109 );
//            v.push_back( 1187 ); v.push_back( 1187 ); v.push_back( 1187 );
//            v.push_back( 119900 );
//        }
//        std::cout << bs_last( v, 1187 ) << std::endl;
    }

    // find first >= val
    {
//        std::vector <int> v;
//        {
//            v.push_back( 10 ); v.push_back( 30 ); v.push_back( 980 );
//            v.push_back( 1108 ); v.push_back( 1109 );
//            v.push_back( 1187 ); v.push_back( 1187 ); v.push_back( 1187 );
//            v.push_back( 119900 );
//        }
//        std::cout << bs_first_greater_equal( v, 1187 ) << std::endl;
    }
    // find last <= val
    {
        std::vector <int> v;
        {
            v.push_back( 10 ); v.push_back( 30 ); v.push_back( 980 );
            v.push_back( 1108 ); v.push_back( 1109 );
            v.push_back( 1187 ); v.push_back( 1187 ); v.push_back( 1187 );
            v.push_back( 119900 );
        }
        std::cout << bs_last_lesser_equal( v, 1187 ) << std::endl;
    }
}



BstNode* g_pBstTree = nullptr;

void mockAddBstNode(BstNode*& root, BstNode* pNewNode) {

    {
        BstNode* parent = nullptr;
        BstNode* tmp = root;

        while ( tmp ) {
            parent = tmp;
            if ( pNewNode->val > tmp->val ) {
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }
        pNewNode->parent_ = parent;
        if (!parent) {
            root = pNewNode;
        } else if (pNewNode->val > parent->val) {
            parent->right = pNewNode;
        } else {
            parent->left = pNewNode;
        }
        return ;
    }
}

// 递归中序遍历二叉树；
void inOrder(BstNode* root) {
    if (root->left) {
        inOrder(root->left);
    }
    std::cout << " " << root->val;
    if (root->right) {
        inOrder(root->right);
    }
}

void preOrder(BstNode* root) {
    if( !root ) return;
    std::cout << " " << root->val;
    if (root->left) {
        preOrder(root->left);
    }
    if (root->right) {
        preOrder(root->right);
    }
}


// 中序遍历获取第K大节点；
BstNode* GetKthNode(BstNode* root, int& K) {
    BstNode* target = nullptr;

    if (root->left) {
        target = GetKthNode(root->left, K);
    }

    if (target == nullptr) {
        if (K == 1)
            target = root;
        K --;
    }

    if (target == nullptr && root->right) {
        target = GetKthNode(root->right, K);
    }
    return target;
}

// 二叉搜索树 最接近值 查找
int ClosestValueInBst_1( BstNode* root, const int& target ) {
    int res = root->val;
    while (root) {
        if (abs(res - target) >= abs(root->val - target)) {
            res = root->val;
        }
        root = target < root->val ? root->left : root->right;
    }
    return res;
}
int ClosestValueInBst_2( BstNode* root, const int& target ) {
    int ret = root->val;

    if (target < root->val && root->left) {

        int lVal = ClosestValueInBst_2(root->left, target);
        if (abs(ret - target) >= abs(lVal - target))
            ret = lVal;

    }
    if (target > root->val && root->right) {

        int rVal = ClosestValueInBst_2( root->right, target );
        if (abs(ret - target) >= abs(rVal - target))
            ret = rVal;

    }
    return ret;
}

// FYTODO: 剑指offer（C++）——二叉树的下一个结点
/*思路：分两种情况考虑：
（1）当前结点有右子树时：只要从当前结点的右子结点开始，沿着左指针一直寻找下去，最终结点就是当前结点的下一个结点
（2）当前结点没有右子树：
    a、如果当前结点是其父节点的左子结点，则下一个结点就是其父节点；
    b、如果当前结点是其父节点的右子结点，就沿着指向父节点的指针一直向上遍历，
       直到找到一个结点是其父节点的左子结点为止，这个父节点就是下一个结点
*/
BstNode* GetNextBstNode(BstNode* pNode) {
    if ( !pNode ) return nullptr;

    BstNode* pNext = nullptr;

    // 如果当前结点有右子树，那么当前结点的下一个结点为其右子树中最左结点
    if ( pNode->right ) {
        BstNode* pRight = pNode->right;
        while (pRight->left) { //沿右结点的指向左子结点指针一直找下去
            pRight = pRight->left;
        }
        pNext = pRight;
    } else if ( pNode->parent_ ) {//节点没有右子树

        BstNode* pCurrent = pNode;                       //始终指向当前遍历的结点
        BstNode* pParent  = pNode->parent_;              //始终指向父节点

        while ( pParent && pParent->right == pCurrent )//沿着父指针一直向上遍历
        {
            pCurrent = pParent;
            pParent = pParent->parent_;
        }

        pNext = pParent;//父节点就是下一个结点（或者父节点为空，说明没有下一个结点）
    }
    return pNext;
}

// 剑指Offer——面试题47：不用加减乘除做加法
// 题目：写一个函数，求两个整数之和，要求在函数体内不得使用+、-、×、÷四则运算符号
// 思路：我们分解一下两个数字相加，1、各个位数上的数字相加，但不考虑进位；2、考虑进位；3、将前面两步的结果相加。
// 对于不能使用四则运算，我们能用的就是位运算，而位运算是针对二进制的。所以，我们根据十进制数的相加方式来考虑二进制。
// 对于二进制
// 1、各个位数数字相加，不考虑进位，那么0与1、1与0相加都是1，而0与0、1与1相加都是0，看到这个方式，想到的是“异或”运算。
// 2、而考虑进位，0与0、0与1、1与0都是不会发生进位的，而1与1是会发生进位。那么可以考虑“位与”运算，“位与”运算后，然后将位与后的结果进行左移一位即可。
// 3、将前面两步结果相加，可以把1产生的结果看成一个数字，2产生的结果看成另一个数字，然后继续计算即可。直到不产生进位即可。
int tst_add_two_num() {
    int num1 = 12;
    int num2 = 24;
    int temp;
    do {
        temp = (num1 ^ num2);
        num2 = (num1 & num2) << 1;
        num1 = temp;
    } while (num2 != 0);
    return num1;
}


void printInorderImpl(BstNode* root) {
    if (!root) return;
    {
        if (root->left)
            printInorderImpl(root->left);
        std::cout << " " << root->val;
        if (root->right)
            printInorderImpl(root->right);
    }
}
void printInorder(BstNode* root) {
    std::cout << std::endl;
    std::cout << "-------------------------------inOrder beg-------------------------------" << std::endl;
    printInorderImpl(root);
    std::cout << std::endl;
    std::cout << "-------------------------------inOrder end-------------------------------" << std::endl;
    std::cout << std::endl;
}

/**
 * 给定一个二叉树，返回该二叉树层序遍历的结果，（从左到右，一层一层地遍历）
 * @return
 */
std::vector< std::vector<int> > levelOrder_new(BstNode* root) {
    {
        std::vector< std::vector<int> > res;

        std::vector< BstNode* > vecNodes;
        vecNodes.push_back( root );

        while ( !vecNodes.empty() ) {
            std::vector<int> vline;
            std::vector<BstNode*> nextLine;

            for ( auto node : vecNodes ) {
                vline.push_back(node->val);

                if ( node->left ) nextLine.push_back( node->left );
                if ( node->right )nextLine.push_back( node->right );
            }

            vecNodes = nextLine;
            res.push_back(vline);
        }
        return res;
    }

    std::vector< std::vector<int> > result;
    if (!root) return result;

    std::vector < BstNode* > vecNodes;
    vecNodes.push_back( root );

    while (vecNodes.size()) {
        std::vector<int> values;
        std::vector< BstNode* > nextLevel;

        for ( auto node : vecNodes ) {
            values.push_back(node->val);
        }

        for ( auto node : vecNodes ) {
            if (node->left) nextLevel.push_back(node->left);
            if (node->right) nextLevel.push_back(node->right);
        }

        vecNodes = nextLevel;
        result.push_back(values);
    }
    return result;
}



void zhiZiPrintBstTree(BstNode* root) {

    std::vector< std::vector<int> > result;
    if (!root) return;

    bool flag = true; //从左向右打印为true，从右向左打印为false
    std::deque < BstNode* > q;
    q.push_back( root );

    while ( !q.empty() ) {
        
        int              n = q.size();
        std::vector<int> out;
        BstNode*         node;

        while ( n > 0 ) {
            if (flag) { // 前取后放: 从左向右打印，所以从前边取，后边放入
                node = q.front();
                q.pop_front();
                if ( node->left )  q.push_back( node->left );
                if ( node->right ) q.push_back( node->right );
            } else { // 后取前放: 从右向左，从后边取，前边放入
                node = q.back();
                q.pop_back();
                if ( node->right )  q.push_back( node->right );
                if ( node->left )   q.push_back( node->left );
            }
            out.push_back(node->val);
            n --;
        }

        flag = !flag;
        result.push_back(out);
    }// while end ----

    // return result;
}
void Test_zhiZiPrintBstTree() {

    zhiZiPrintBstTree(g_pBstTree);

}


void tst_my_bst_tree() {

    auto ret = levelOrder_new(g_pBstTree);
    for (std::vector< std::vector<int> >::iterator itLine = ret.begin(); itLine != ret.end(); itLine ++) {

        for (std::vector<int>::iterator itVal = (*itLine).begin(); itVal != (*itLine).end(); itVal ++) {
            std::cout << " " << *itVal;
        }
        std::cout << std::endl;
    }
    return ;

    std::cout << "ver1 find k=120 closest value= "
              << ClosestValueInBst_1(g_pBstTree, 120) << std::endl;
    std::cout << "ver2 find k=120 closest value= "
              << ClosestValueInBst_2(g_pBstTree, 120) << std::endl;

    std::cout << "isBalanced=" << isBalanced(g_pBstTree) << std::endl;

    // 查找第K大节点；
    int kVal = 3;
    BstNode* retNode = GetKthNode(g_pBstTree, kVal);
    if (retNode)
        std::cout << "GetKthNode ret val = " << retNode->val << std::endl;
}

void tst_my_bst_1() {
    //5，3，7，2，4，6，8
    int tmpInts[ ] = {5, 3, 7, 2, 4, 6, 8};
    int size = sizeof(tmpInts)/sizeof(tmpInts[0]);
    std::cout << "size=" << size << std::endl;
}

void sortArrayOddEvenImpl( std::vector < int >& vecInts ) {

    std::cout << " before sort -------------" << std::endl;
    for (auto& it : vecInts) {
        std::cout << " " << it /*<< std::endl*/;
    }
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    int i = 0;
    int j = vecInts.size() - 1;

    while (i < j) {

        bool a = (vecInts[i] % 2 == 1);
        bool b = (vecInts[j] % 2 == 1);

        if ( a && !b ) { // i: ji; j: ou;
            ::swap( vecInts[i++], vecInts[j--] );
        }  else if ( !a && !b ) { // both ou
            i ++;
        } else if ( a && b ) { // both jishu
            j --;
        } else if ( !a && b ) { // i: ou; j: ji;
            i ++; j --;
        }

    }

    std::cout << " " << std::endl;
    std::cout << " after sort -------------" << std::endl;
    for (auto& it : vecInts) {
        std::cout << " " << it /*<< std::endl*/;
    }
    std::cout << " " << std::endl;

}

// 奇偶排序数组
// 前半段，偶数；后半段， 奇数；
void sortArrayOddEven() {
    std::vector < int > vecInts;
    vecInts.push_back( 119 ); vecInts.push_back( 24 );
    vecInts.push_back( 13 ); vecInts.push_back( 7171 );
    vecInts.push_back( 256 ); vecInts.push_back( 1024 );

    sortArrayOddEvenImpl( vecInts );
}




// 二叉树叶子节点个数
/*
（1）如果二叉树为空，返回0
（2）如果二叉树不为空且左右子树为空，返回1
（3）如果二叉树不为空，且左右子树不同时为空，返回左子树中叶子节点个数加上右子树中叶子节点个数
*/
int GetLeafNodeCountImpl(BstNode* root) {
    if (root == nullptr)
        return 0;
    if (nullptr == root->left && nullptr == root->right)
        return 1;

    int leftCount  = GetLeafNodeCountImpl(root->left);
    int rightCount = GetLeafNodeCountImpl(root->right);

    int ret = leftCount + rightCount;
    return ret;
}

int GetBstTreeNodeCount(BstNode* root) {// 二叉树节点个数
    if (root == nullptr)
        return 0;
    return 1 + GetBstTreeNodeCount(root->left) + GetBstTreeNodeCount(root->right);
}
/*
    求二叉树的深度
    递归解法：
    1：如果二叉树为空，则二叉树的深度为0
    2：如果二叉树不为空，二叉树的深度 = MAX(左子数深度，右子树深度)+1；
*/
int GetBstTreeDepth(BstNode* root) {
    if (!root) return 0;
    int leftDepth = GetBstTreeDepth(root->left);
    int rightDepth = GetBstTreeDepth(root->right);
    return (1 + std::max(leftDepth, rightDepth));
}
void LevelTraverseBstTree(BstNode* root) {
    if (!root) return;

    std::queue<BstNode*> q;
    q.push(root);

    while (!q.empty()) {
        BstNode* pNode = q.front();
        q.pop();
        std::cout << " " << pNode->val;

        if (pNode->left) {
            LevelTraverseBstTree(pNode->left);
        }
        if (pNode->right)
            LevelTraverseBstTree(pNode->right);
    }
    std::cout << " " << std::endl;
}

vector<vector<int>> levelOrder(BstNode* root)
{
    vector<vector<int>> result;
    if (!root) return result;

    queue<BstNode*> Q;
    Q.push(root);
    while ( !Q.empty() ) {
        vector<int> tmp_v;
        size_t sizeLen = Q.size();
        for (size_t i = 0; i < sizeLen; i++) {
            BstNode* tmp_node = Q.front(); Q.pop();
            tmp_v.push_back(tmp_node->val);
            if (tmp_node->left) Q.push(tmp_node->left);
            if (tmp_node->right)Q.push(tmp_node->right);
        }
//        for (auto it : tmp_v) {
//            std::cout << " " << it;
//        }
//        std::cout << " " << std::endl;
        result.push_back(tmp_v);
    }
    return result;
}
std::vector < std::vector <int> > levelPrintBst(BstNode* root) {

    std::vector < std::vector <int> > vvInt;
    if (!root) return vvInt;

    {
        std::queue < BstNode* > q;
        q.push(root);

        while (!q.empty()) {
            int curSize = q.size();

            std::vector < int > v_level;
            for (int i = 0; i < curSize; i++) {
                BstNode* tmp = q.front(); q.pop();
                v_level.push_back(tmp->val);

                if (tmp->left) q.push(tmp->left);
                if (tmp->right) q.push(tmp->right);
            }
            vvInt.push_back(v_level);
        }// end --- for
        return vvInt;
    }

    std::queue < BstNode* > q;
    q.push(root);
    while (!q.empty()) {
        int cur = q.size();   //当前层数的节点数量

        std::vector < int > v_level;
        for (int i = 0; i < cur; i ++) {
            BstNode* tmp = q.front();
            q.pop();

            // 若当前层节点有孩子，则孩子进队
            if (tmp->left) q.push(tmp->left);
            if (tmp->right) q.push(tmp->right);

            v_level.push_back(tmp->val);
        }

        vvInt.push_back(v_level);
    }

    return vvInt;
}

void mirrorBst(BstNode* root) {
    if (!root) return;
    if (!root->left && !root->right) return;

    BstNode* tmp;
    tmp = root->left;
    root->left = root->right;
    root->right = tmp;

    mirrorBst(root->left);
    mirrorBst(root->right);
}

// 题目：从扑克牌中随机抽5张牌，判断是不是一个顺子，即这5张牌是不是连续的。
// 2~10为数字本身，A为1，J为11，Q为12，K为13，而 大小王可以看成 任意数字
bool IsContinuous( std::vector<int>& vPoker ) {
    std::sort( vPoker.begin(), vPoker.end() );

    // 统计有多少0
    int numZero = 0;
    for ( const auto& it : vPoker ) {
        if ( 0 == it ) {
            numZero ++;
        }
    }

    int gap = 0;
    int n   = vPoker.size();
    for ( int i = numZero, j = numZero + 1; j < n; i ++, j ++ ) {
        if (vPoker[i] == vPoker[j]) return false;
        gap += vPoker[j] - vPoker[i] - 1;
    }
    std::cout << " numZero=" << numZero << " gap=" << gap << std::endl;
    return (numZero >= gap) ? true : false;
}
void wrap_IsContinuous() {
    std::vector<int> vecPokers { 0, 3, 7, 9, 0, 10 };
    IsContinuous( vecPokers );
}


void tst_copy_backward() {
    std::vector<int> myvector;
    std::vector<int>::iterator iter;

    //为容器myvector赋初始值:10 20 30 40 50
    for ( int i = 1; i <= 5; ++i )
        myvector.push_back( i*10 );

    //将myvector容器的大小增加3个单元
    myvector.resize( myvector.size()+3 );

    //将容器元素20、10拷贝到第八、第七个单元中：10 20 30 40 50 0 10 20
    //注意copy_backward是反向复制，先将20拷贝到第八个单元，再将10拷贝到第七个单元
    std::copy_backward( myvector.begin(), myvector.begin()+2, myvector.end() );

    for ( iter = myvector.begin(); iter != myvector.end(); ++iter ) {
        std::cout << " " << *iter;
    }

    std::cout << std::endl;
}


typedef list<int> IList;

void print(const IList& list) {
    IList::const_iterator ite = list.begin();
    for (; ite != list.end(); ++ite)
        cout << *ite << "  ";
    cout << endl;
}

int tst_list_1() {
    IList s;
    s.push_back(7);
    s.push_back(6);
    s.push_back(5);
    s.push_back(4);
    s.push_back(3);
    s.push_back(2);
    s.push_back(1);
    s.push_back(0);

    IList carry;
    IList counter[64];
    int fill = 0;
    int num = 0;
    while (!s.empty())
    {
        std::cout << "取第" << num << "个数据: fill = " << fill << std::endl;
        carry.splice(carry.begin(), s, s.begin());
//        for ( auto it : carry ) {
//            std::cout << "carry it=" << it << std::endl;
//        }
//        std::cout << std::endl;
//        for ( auto it : s ) {
//            std::cout << "s it=" << it << std::endl;
//        }
//        return 1;

        int i = 0;
        std::cout << " couter[" << i <<  "] size=" << counter[i].size() << std::endl;
        while (i < fill && !counter[i].empty())
        {
            counter[i].merge(carry);
            carry.swap(counter[i++]);
        }
        carry.swap(counter[i]);
        if (i == fill)
            ++fill;
        //我自己加的计数
        num++;
        //打印每次完的结果
        for (int i = 0; i < fill; ++i) {
            std::cout << i << "==";
            print(counter[i]);
        }
    }

    for (int i = 1; i < fill; ++i)
        counter[i].merge(counter[i - 1]);
    s.swap(counter[fill - 1]);

    getchar();
    return 0;
}

int SubMax(vector<int> num, int length) {
    if (num.empty() || length <= 0)
        return -1;

    int curMax = 0;
    int nextMax = 0;
    for (int i = 0; i < length; ++i) {
        nextMax += num[i];
        if (nextMax > curMax) {
            curMax = nextMax;
        } else if (nextMax < 0) {
            curMax = 0;
            nextMax = 0;
        }
    }
    return curMax;
}

// 剑指offer 面试题：连续子数组最大和
int FindGreatestSumOfSubArray(vector<int> array) {
    if (array.empty()) return 0;

    {
        return SubMax( array, array.size() );
//        int res = array[ 0 ];
//        int mx = array[ 0 ];
//        for ( int i = 1; i < array.size(); ++ i ) {
//            mx = std::max( array[ i ], mx + array[i] );
//            res = max ( res, mx );
//        }
//        return res;
    }

    int res = array[0], mx = array[0];
    for (size_t i=1; i < array.size(); ++i)
    {
        mx = max(array[i],mx + array[i]);
        res = max(res, mx);
    }
    return res;
}
void tst_FindGreatestSumOfSubArray() {
    std::vector < int > vints = { -1, 100, -200, 201, -20, 61, -104, 103, 1 };
//    vector<int>  a5 = { -1, 100, -200, 201, -20, -20, -20, 61, -104, 103, 1};
    std::cout << "res=" << FindGreatestSumOfSubArray(vints) << std::endl;
}

void tst_multiset() {
    std::multiset < int, std::greater<int> > setInts;
    setInts.insert( 117 );setInts.insert( 117 );
    setInts.insert( 37 );setInts.insert( 2098 );
    for (auto it : setInts) {
        std::cout << " " << it;
    }
    std::cout << std::endl;
}

void tst_heap_fun( ) {
    std::vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    for (auto x : v1) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    v1.push_back(4);
    make_heap(v1.begin(), v1.end());//将某区间内的元素转化为heap

    std::cout << "after push_heap(): ";
    for (auto x : v1)
        std::cout << x << " ";
    std::cout << std::endl;

    pop_heap(v1.begin(), v1.end());           //对heap减少一个元素
    cout << "after pop_heap() : ";
    for (auto x : v1)
        cout << x << " ";
    cout << endl;

    sort_heap(v1.begin(), v1.end()-1);       //对[beg,end-1)区间内的元素进行排序，之后就不再是heap
    cout << "after sort_heap(): ";
    for (auto x : v1)
           cout << x << " ";
    cout << endl;
}


void serializeImpl(BstNode *root, std::string& str) {
    if (!root) {
        str += "$,";
        return;
    }
    str += to_string(root->val);
    str += ",";
    serializeImpl(root->left, str);
    serializeImpl(root->right, str);
}
/*=========================二叉树序列化==============================*/
char* Serialize(BstNode *root) {
    if (root == nullptr) return nullptr;

    string str;
    serializeImpl(root, str);

    char* buffer = new char[str.size() + 1];
    strcpy(buffer, str.c_str());                     //将string类型的字符串转换成C_style类型并复制到一个char类型数组
    return buffer;
}

// 输入某二叉树的前序遍历和中序遍历的结果，请重建出该二叉树。
// 假设输入的前序遍历和中序遍历的结果中都不含重复的数字。
// 例如输入前序遍历序列{1,2,4,7,3,5,6,8}和中序遍历序列{4,7,2,1,5,3,8,6}，则重建二叉树并返回。
//BstNode* reConstructBinaryTree(vector<int> pre, vector<int> vin) {
    /*
解题思路：
1.  前序遍历的第一个数字总是树的根节点的值。
    但在中序遍历中，根节点的值在序列的中间，其中左子树的节点的值位于根节点的值的左边，
    右子树的节点的值位于根节点的值的右边。要扫描中序遍历序列，才能找到根节点的值。
2.  又因为前序遍历总是在根节点后先遍历完左子树，才会遍历右子树，
    所以由后续遍历推断出左子树中节点的数目之后，前序遍历中根节点之后的相同数目的值都是左子树的节点的值。剩下的就是右子树的值。这样就分别找到了左右子树序列。
3.  分别确定了根节点和左右子树的前序、中序遍历，我们可以用同样的方法构建左右子树，剩下的可以用递归来完成。
    */
//    int vinlen = vin.size();

//    vector<int> leftpre, rightpre, leftvin, rightvin;
//    BstNode* newNode = new BstNode(pre[0], nullptr, nullptr, nullptr);

//    int root = pre[0];
//    auto posi = std::find(vin.begin(), vin.end(), root);
//    if (posi == vin.end()) {
//        printf("cant't found root=%d\n", root);
//        return nullptr;
//    }
//    int leftSize = posi - vin.begin();
//    int rightSize= vin.end() - posi - 1;

//    //递归求解
//    //这里取前序和后序遍历的左右子树可能有点绕，可以好好思考一下
//    newNode->left = reConstructBinaryTree(vector<int>(pre.begin() + 1, pre.begin() + 1 + leftSize),
//                                             vector<int>(vin.begin(), vin.begin() + leftSize));
//    newNode->right = reConstructBinaryTree(vector<int>(pre.begin() + 1 + leftSize, pre.end()),
//                                             vector<int>(vin.begin() + 1 + leftSize, vin.end()));
//    return nullptr;
//}


template <typename ...Args>
inline std::string format_string(const char* format, Args... args) {
    std::cout << "BUFSIZE=" << BUFSIZ << std::endl;
    constexpr size_t oldlen = BUFSIZ;
    char buffer[oldlen];  // 默认栈上的缓冲区

    size_t newlen = snprintf(&buffer[0], oldlen, format, args...);
    newlen++;  // 算上终止符'\0'

    if (newlen > oldlen) {  // 默认缓冲区不够大，从堆上分配
        std::vector<char> newbuffer(newlen);
        snprintf(newbuffer.data(), newlen, format, args...);
        return std::string(newbuffer.data());
    }

    return buffer;
}
template <typename ...Args>
void errExit(const char* format, Args... args) {
    auto errmsg = format_string(format, std::forward<Args>(args)...);
    errmsg = errmsg + ": " + strerror(errno) + "\n";
    fputs(errmsg.c_str(), stderr);
//    exit(1);
}

void tst_format_str() {
    const char* s = "hello world!";
    int fd = -1;
    if (write(fd, s, strlen(s)) == -1)
        errExit("write \"%s\" to file descriptor(%d) failed", s, fd);
}

void tst_bit_opr() {
    int readTag  = 0X01;
    int writeTag = 0X02;
    std::cout << "readTag=" << readTag << std::endl;
    std::cout << "writeTag=" << writeTag << std::endl;
    std::cout << "rwTag=" << (readTag|writeTag) << std::endl;
}

//【C++】字符串中的括号是否匹配
void tst_kuohao_pipei() {
    // “()” and “()[]{}” are all valid but “(]” and “([)]”
    std::string str = "{}[]()";
    str = "{)";

    std::map < char, char > map_s { {'(', ')'}, {'{', '}'}, {'[',']'} };
    std::stack < char > temp;

    for (size_t i = 0; i < str.size(); i++) {

        if ( map_s.find(str[i]) != map_s.end() ) {
            temp.push( map_s[str[i]] );//将s[i]在map中的值，放到栈中
        } else {
            if (temp.empty()) {
                std::cout << "no found anything-------" << std::endl;
                return;
            }

            if (str[i] == temp.top()) {
                temp.pop();
            } else {
                std::cout << "no -------str[" << i << "]=" << str[i] << ", pop=" << temp.top() << std::endl;
                return;
            }

        }

    }// for str traverse

    std::cout << " complete check match" << std::endl;
}

// 剑指offer——滑动窗口的最大值（c++）
/*
     * 给定数组，和滑动窗口的大小，请找出所有滑动窗口内的最大值。
     * 例如，数组{2,3,4,2,6,2,5,1}，滑窗大小为3，那么结果为{4,4,6,6,6,5}。
     * 思路:
     * 1. 使用双向队列，用来保存有可能是滑动窗口最大值的数组的下标。
     * 2. 在存入一个数字的下标之前，首先要判断队列里已有数字是否小于待存入的数字。
     *    如果已有的数字小于待存入的数字，那么这些数字已经不可能是滑动窗口的最大值，因此需要在队列尾部删除（pop_back）。
     * 3. 同时，如果队列头部的数字已经从窗口里滑出，那么滑出的数字也需要从队列的头部删除（pop_front）。
     * 数组{2,3,4,2,6,2,5,1}的长度为3的滑动窗口最大值求解步骤如下
*/
std::vector<int> maxInWindow(std::vector<int>& nums, unsigned int size) {
    UNUSED(nums);
    UNUSED(size);
    std::vector<int> vret;
    std::deque<int> dq;

//    for (int i = 0; i < size; i ++) {

//        while (!dq.empty() && nums[i] >= dq) {
//            dq.pop_back();
//        }

//        dq.push_back(i);
//    }

    return vret;
}
void tst_maxinwindows() {
    std::vector<int> vNums{2,3,4,2,6,2,5,1};
    auto retV = maxInWindow(vNums, 3);
    for (size_t i = 0; i < retV.size(); i ++) {
        std::cout << " " << retV[i] ;
    }
    std::cout << std::endl;
}

void tst_multimap( ) {
    {
        multimap<string,int> m_map;
        string s("中国"), s1("美国");
        m_map.insert(make_pair(s,50));
        m_map.insert(make_pair(s,55));
        m_map.insert(make_pair(s,60));
        m_map.insert(make_pair(s1,30));
        m_map.insert(make_pair(s1,20));
        m_map.insert(make_pair(s1,10));
        m_map.insert(make_pair(s1,40));
        m_map.insert(make_pair(s1,50));

        //方式1
        // int k = 0;
        multimap<string,int>::iterator m, beg, end;
//        m = m_map.find(s1);

        int  findVal = 30;
        bool bfind = false;
//        for(k = 0; k != m_map.count(s1); k++, m++) {
//            if (m->second == findVal) bfind = true;
//        }
//        if (bfind) {
//            std::cout << "find it " << findVal << std::endl;
//        } else {
//            std::cout << "no find it " << findVal << std::endl;
//        }

        {
            beg = m_map.lower_bound(s1);
            end = m_map.upper_bound(s1);
            for(m = beg; m != end; m++) {
                if (m->second == findVal) bfind = true;
                cout<<m->first<<"--"<<m->second<<endl;
            }
            if (bfind) {
                std::cout << "find it " << findVal << std::endl;
            } else {
                std::cout << "no find it " << findVal << std::endl;
            }
            return;
        }

        //方式2
        {
//            std::string findKey = "美国";
//            multimap<string,int>::iterator beg,end;
//            beg = m_map.lower_bound(s1);
//            end = m_map.upper_bound(s1);
//            if (beg == end) {
//                std::cout << " beg == end " << std::endl;
//            }
//            for(m = beg; m != end; m++) {
////                cout<<m->first<<"--"<<m->second<<endl;
//                if (m->second == findVal) bfind = true;
//            }
//            if (bfind) {
//                std::cout << "find it " << findVal << std::endl;
//            } else {
//                std::cout << "no find it " << findVal << std::endl;
//            }
        }

//        beg = m_map.lower_bound(s1);
//        end = m_map.upper_bound(s1);
//        for(m = beg; m != end; m++)
//            cout<<m->first<<"--"<<m->second<<endl;
        //方式3
//        beg = m_map.equal_range(s).first;
//        end = m_map.equal_range(s).second;
//        for(m = beg; m != end; m++)
//            cout<<m->first<<"--"<<m->second<<endl;
        return;
    }
    {
        multimap<int,int> a;
        a.insert(pair<int,int>(1,11));
        a.insert(pair<int,int>(1,12));
        a.insert(pair<int,int>(1,13));
        a.insert(pair<int,int>(2,21));
        a.insert(pair<int,int>(2,22));
        a.insert(pair<int,int>(3,31));
        a.insert(pair<int,int>(3,32));

//        if (a.find(pair<int,int>(3,32)) != a.end()) {
//            std::cout << "find pair(3, 32) exist" << std::endl;
//        }
        {
            auto pr = a.equal_range(1);
            if (pr.first != std::end(a))
            {
                for (auto iter = pr.first; iter != pr.second; ++iter)
                    std::cout << iter->first << " is " << iter->second << std::endl;
            }
            return;
        }

        multimap<int,int>::iterator p_map;
        std::pair<multimap<int,int>::iterator, multimap<int,int>::iterator> ret;

        for(p_map = a.begin(); p_map != a.end();)
        {
            std::cout<<p_map->first<<" =>";

            ret = a.equal_range(p_map->first);
            for(p_map = ret.first; p_map != ret.second; ++p_map)
                std::cout << " " << (*p_map).second;

            std::cout << std::endl;
        }

        return;
    }
    std::multimap < int, int > mltIds;

    mltIds.insert( std::make_pair(2099, 1024) );

    mltIds.insert( std::make_pair(3018, 1024) );
    mltIds.insert( std::make_pair(3018, 998) );
    mltIds.insert( std::make_pair(3018, 998) );
    mltIds.insert( std::make_pair(3018, 998) );


    for (auto it = mltIds.begin(); it != mltIds.end(); ++it){
        std::cout << "it->first=" << it->first << " second=" << it->second << std::endl;
    }

    // auto find = mltIds.find ( 3018 );
}

class Base1 {
public:
    Base1() {
        std::cout << "construct Base1" << std::endl;
    }
    /*virtual */~Base1() {
        std::cout << "destroy   Base1" << std::endl;
    }
    virtual void release() {
        std::cout << "release   Base1" << std::endl;
    }
};
class BaseA : public Base1{
public:
    BaseA() {
        std::cout << "construct BaseA" << std::endl;
    }
   virtual ~BaseA() {
        std::cout << "destroy   BaseA" << std::endl;
    }
    virtual void release() {
        std::cout << "release   BaseA" << std::endl;
        delete this;
    }
};
class BaseB : public Base1{
public:
    BaseB() {
        std::cout << "construct BaseB" << std::endl;
    }
    virtual ~BaseB() {
        std::cout << "destroy   BaseB" << std::endl;
    }
    virtual void release() {
        std::cout << "release   BaseB" << std::endl;
        delete this;
    }
};
void tst_base_derive() {
//     BaseA* aobj = new BaseA;
//     BaseB* bobj = new BaseB;
//     void* v_aaa = aobj;
//     void* v_bbb = bobj;

//     Base1* pBase = aobj;
//     aobj->release();

//     pBase = bobj;
//     bobj->release();
// //    delete v_aaa;
// //    delete v_bbb;
}

void tst_tuple_1() {

    { // std::tuple::swap: Exchanges the content of the tuple object by the content of tpl,
      // which is another tuple of the same type (containing objects of the same types in the same order)
        std::tuple<int, char> a(10, 'x');
        std::tuple<int, char> b(20, 'y');

//        a.swap(b);
        std::cout << "a contains: " << std::get<0>(a);
        std::cout << " and " << std::get<1>(a) << '\n';

//        std::swap(a, b);
//        std::cout << "a contains: " << std::get<0>(a);
//        std::cout << " and " << std::get<1>(a) << '\n';
    }

}

std::vector<int> GenerateDiffNumber(int min,int max,int num)
{
    int rnd;
    std::vector<int> diff;
    std::vector<int> tmp;//存储剩余的数
    //初始化
    for ( int i = min;i < max+1; i++ ) {
        tmp.push_back(i);
    }
    srand((unsigned int)time(0));//初始化随机数种子
    for(int i = 0; i < num; i++)
    {
        do{
            rnd = min+rand()%(max-min+1);
        } while(tmp.at(rnd-min)==-1);
        diff.push_back(rnd);
        tmp.at(rnd-min) = -1;
        printf("rnd-min=%d\n", rnd-min);
    }
    return diff;
}

void tstDiffNumber() {
    std::vector <int> retVec = GenerateDiffNumber(1, 10, 10);
    std::cout << "size(ret)=" << retVec.size() << std::endl;
    for (auto i : retVec) {
        printf("i=%d\n", i);
    }
}


void tstCmpare() {
#define ngx_str7_cmp(m, c0, c1, c2, c3, c4, c5, c6, c7)                       \
    *(uint32_t *) m == ((c3 << 24) | (c2 << 16) | (c1 << 8) | c0)             \
        && ((uint32_t *) m)[1] == ((c7 << 24) | (c6 << 16) | (c5 << 8) | c4)

    const char* lpszConn = "CONNECT ";
    char* ptr = (char*)lpszConn;

    if (ngx_str7_cmp(ptr, 'C', 'O', 'N', 'N', 'E', 'C', 'T', ' ')) {
//        r->method = NGX_HTTP_CONNECT;
        std::cout << " ngx http connect method " << std::endl;
    }

}

// instance = (uintptr_t)c & 1;

typedef struct MyConn_st {
    int ival;
}MyConn;

typedef struct Bit_st {
    unsigned         instance;
}Bit_ST;


// 给定一个二叉搜索树，kthSmallest 查找其中第 k 个最小的元素。
int kthSmallest(BstNode* root, int k) {
    int cnt = 0;

    stack<BstNode*> s;
    BstNode *p = root;

    while ( p || !s.empty() ) {
        while (p) {
            s.push(p);
            p = p->left;
        }
        p = s.top();
        s.pop();
        ++ cnt;
        if (cnt == k) return p->val;
        p = p->right;
    }
    return 1;
}


void tstC11() {

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    printf(" __GXX_EXPERIMENTAL_CXX0X__  been define\n");
#else
    printf(" __GXX_EXPERIMENTAL_CXX0X__  no been define\n");
#endif

}



void tst1() {
//    std::vector<int> vecArr{ 10,-3,0,5,9};
//    TreeNode* root = sortedArrayToBst(vecArr);
//    std::cout << std::endl;
//    someInOrder(root);
//    std::cout << std::endl;
}

/**
     * @Author: ZwZ
     * @Description: 输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4。
     * @Param: [arr, k, length]
     * @return: 
     * @Date: 2020/1/30-21:55
*/
int quick_st_aesc( std::vector<int>& a, int start, int end, int k) {
    UNUSED(k);
    int pivot = a[start];
    int i = start;
    int j = end;
    int res = 0;

    while (i < j) {
        while (i < j && a[j] >= pivot) -- j;
        while (i < j && a[i] <= pivot) ++ i;
        if (i < j) swap(a[i], a[j]);
    }
    swap(a[start], a[i]);

//    if ( i+1 == k ) return a[i];
//    else if ( i+1 > k ) res = quick_st(a, start, i-1, k);
//    else if ( i+1 < k ) res = quick_st(a, i+1, end, k);
    return res;
}
int quick_st_desc( std::vector<int>& a, int start, int end, int k) {
    int pivot = a[start];
    int i = start;
    int j = end;
    int res = 0;

    while (i < j) {
        while (i < j && a[j] <= pivot) -- j;
        while (i < j && a[i] >= pivot) ++ i;
        if (i < j) swap(a[i], a[j]);
    }
    swap(a[start], a[i]);

    if ( i+1 == k ) return a[i];
    else if ( i+1 > k ) res = quick_st_desc(a, start, i-1, k);
    else if ( i+1 < k ) res = quick_st_desc(a, i+1, end, k);
    return res;
}
/**
 * @Description: 有一个整数数组，请你根据快速排序的思路，找出数组中第K大的数。
 * @Param: [arr, k, length]
 * @return: 
 * @Date: 2020/1/30-21:55
 */
int findKth() {
    std::vector<int> abc{ 4, 5, 1, 6, 2, 7, 8, 123, 456 };

    auto res = quick_st_desc(abc, 0, abc.size() - 1, 3);
    std::cout << "res=" << res << std::endl;

    quick_st_aesc(abc, 0, abc.size() - 1, 4);
    return 1;
}

// 输入n个整数，找出其中最小的K个数。例如输入4,5,1,6,2,7,3,8这8个数字，则最小的4个数字是1,2,3,4。
std::vector<int> GetLeastNumbers(std::vector<int> a, size_t k) {
    std::vector<int> result;
    if (!a.size() || k <= 0) return result;

    std::priority_queue<int, std::vector<int>, less<int>> pa;
    for ( size_t i = 0 ; i < a.size(); i ++ ) {
        if (pa.size() < k) {
            pa.push(a[i]);
        } else if (a[i] < pa.top()){
            pa.pop();
            pa.push(a[i]);
        }
    }
    while (!pa.empty()) {
        result.push_back(pa.top());
        pa.pop();
    }
    std::sort(result.begin(), result.end());
    return result;
}
void Test_GetLeastNumbers() {
    std::vector<int> vecInts{ 4,5,1,6,2,7,3,8 };
    auto retVector = GetLeastNumbers(vecInts, 4);
    PrintInContainer(retVector);
}


/**
 * @Description: 一只青蛙一次可以跳上1级台阶，也可以跳上2级。
 *               求该青蛙跳上一个n级的台阶总共有多少种跳法（先后次序不同算不同的结果）。
 * @param number int整型
 * @return int整型
 */
int jumpFloor(int number) {
    // write code here
    int a[number + 1];
    a[1] = 1;
    a[2] = 2;
    for(int i = 3; i <= number; i++){
        a[i] = a[i - 1] + a[i - 2];
    }
    return a[number];
}
void Test_JumpFloor() {

}

// 用两个栈来实现一个队列，完成队列的Push和Pop操作。 队列中的元素为int类型。
class CStackToQueue {
public:
    void push( int val ) {
        stack1.push(val);
    }
    int pop() {


        {
            if ( stack1.empty() ) return -1;

            while ( !stack1.empty() ) {
                stack2.push( stack1.top() );
                stack1.pop();
            }

            auto ret = stack2.top();
            stack2.pop();

            while ( !stack2.empty() ) {
                stack1.push( stack2.top() );
                stack2.pop();
            }
            return ret;
        }
    }

private:
    std::stack<int> stack1;
    std::stack<int> stack2;
};

void Test_stackToQueue() {

}


/*
    给定一棵二叉树以及这棵树上的两个节点 o1 和 o2，请找到 o1 和 o2 的最近公共祖先节点。
    当我们用递归去做这个题时不要被题目误导，应该要明确一点
    这个函数的功能有三个：给定两个节点 p 和 q

    如果 p 和 q 都存在，则返回它们的公共祖先；
    如果只存在一个，则返回存在的一个；
    如果 p 和 q 都不存在，则返回NULL

    思路：
    （1） 如果当前结点 root 等于 NULL，则直接返回 NULL
    （2） 如果 root 等于 p 或者 q ，那这棵树一定返回 p 或者 q
    （3） 然后递归左右子树，因为是递归，使用函数后可认为左右子树已经算出结果，用 left 和 right 表示
    （4） 此时若 left 为空，那最终结果只要看 right; 若 right 为空，那最终结果只要看 left
    （5） 如果 left 和 right 都非空，因为只给了 p 和 q 两个结点，都非空，说明一边一个，因此 root 是他们的最近公共祖先
    （6） 如果 left 和 right 都为空，则返回空（其实已经包含在前面的情况中了）

    时间复杂度是 O(n)：每个结点最多遍历一次或用主定理
    空间复杂度是 O(n)：需要系统栈空间
*/
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (!root || root == p || root == q) return root;

    auto leftNode  = lowestCommonAncestor(root->left, p, q);
    auto rightNode = lowestCommonAncestor(root->right, p, q);
    if (leftNode && rightNode) { return root; }
    return leftNode ? leftNode : rightNode;
}

// 二叉搜索树 版本，跟二叉树版本实现逻辑不太一样；
BstNode* lowestCommonAncestor_Bst(BstNode* root, BstNode* p, BstNode* q) {

{
    auto tmp = root;
    while ( 1 ) {
        if ( p->val < tmp->val && q->val < tmp->val ) {
            tmp = tmp->left;
        } else if ( p->val > tmp->val && q->val > tmp->val ) {
            tmp = tmp->right;
        } else {
            break;
        }
    }
    return tmp;
}

    if ( root == nullptr ) return nullptr;

    if ( p->val > root->val && q->val > root->val ) {
        return lowestCommonAncestor_Bst(root->right, p, q);
    }
    else if ( p->val < root->val && q->val < root->val ) {
        return lowestCommonAncestor_Bst(root->left, p, q);
    }
    else {
        return root;
    }
        
}

void tst_lowestCommonAncestor() {
}



// 对于一个给定的链表，返回环的入口节点，如果没有环，返回null. 拓展：你能给出不利用额外空间的解法么？
// 破坏链表节点连接 方式
ListNode* DetectListCycle(ListNode* head) {
    ListNode* marknode = new ListNode(-1024);
    ListNode* p = head;
    ListNode* temp = nullptr;

    while (p) {
//<<<<<<< HEAD
//        if (p->next_ == marknode)
//            return p;
//        temp = p->next_;
//        p->next_ = marknode;
//=======
        if (p->next == marknode)
            return p;
        temp = p->next;
        p->next = marknode;
//>>>>>>> b76a882d3a34dee237d198232f59bc789372a704
        p = temp;
    }
    return nullptr;
}
void Test_DetectListCycle() {
    std::vector <int> vecInts{ 10, 97, 21, 16, 2, 0};
    auto listHead = ConstructList(vecInts);
    PrintList(listHead);

    // make list cycle --- --- ---
    ListNode* tmp = listHead;
    while (1) {
        if ( tmp->val == 16 ) {
            tmp->next = listHead;
            break;
        }
        tmp = tmp->next;
    }
//    PrintList(listHead);
    //  --- --- --- --- --- ---

    auto retNode = DetectListCycle(listHead);
    std::cout << "retnod=" << retNode->val << std::endl;
}


// 给出两个有序整数数组 A和B，请将数组 B合并到数组 A中，变成一个有序的数组
// 注意：假设 A数组有足够的空间存放 B数组的元素， A和 B中初始的元素数目分别为 m和n
void merge(int* A, int m, int* B, int n) {

    int i = m - 1, j = n - 1, k = m + n - 1;
    while (i >= 0 && j >= 0) {

        if (A[i] > B[j]) {
            A[k--] = A[i--];
        }
        else {
            A[k--] = B[j--];
        }

    }
    while(j >= 0) {
        A[k--] = B[j--];
    }
}
void Test_mergeTwoArray() {
    int A[128] = { 11, 12, 13, 16 };
    int B[6]   = { 1, 2, 3, 6 };

    std::cout << "len1=" << array_size(A) << std::endl;
    std::cout << "len2=" << array_size(B) << std::endl;

    merge( A, 4, B, 4 );

    std::cout << std::endl;
    for ( int i = 0; i < 128; i ++ ) {
        std::cout << " " << A[i];
    }
    std::cout << std::endl;
}

// 给定一个数组arr，返回子数组的最大累加和
// 例如，arr = [1, -2, 3, 5, -2, 6, -1]，所有子数组中，[3, 5, -2, 6]可以累加出最大的和12，所以返回12.
// [要求] 时间复杂度为O(n)，空间复杂度为O(1)
int maxsumofSubarray(vector<int>& arr) {
    int max = 0;
    int sum = 0;

    for ( size_t i = 0; i < arr.size(); i ++ ) {
        if (arr[i] >= 0) {
            sum += arr[i];
        } else {
            sum = 0;
        }
        if (sum > max) {
            max = sum;
        }
    }
    return max;
}
void Test_maxsumofSubarray() {
    std::vector<int> arr{ 0, -1, -9, 126, 1, 0, 5 };
    std::cout << "maxsum=" << maxsumofSubarray(arr) << std::endl;
}

void Test_ReverseString() {
    std::string str = "12345";

    size_t i = 0;
    int nsize = str.size();
    for ( size_t j = nsize - 1; i <= j; j --, i ++ ) {
        std::swap(str[i], str[j]);
    }
    std::cout << "str=" << str << std::endl;
}




/*
    实现 int sqrt(int x) 函数。
    计算并返回 x 的平方根，其中 x 是非负整数。
    由于返回类型是整数，结果只保留整数的部分，小数部分将被舍去。
    示例 1:
    输入: 4
    输出: 2

    示例 2:
    输入: 8
    输出: 2
    说明: 8 的平方根是 2.82842..., 由于返回类型是整数，小数部分将被舍去。
 * */
int Sqrt__1() {
    int x = 144;
    if ( x <= 1 ) return x;
    int l = 0, r = x;

    while ( l <= r ) {
        int mid = l + ((r-l) >> 1);
        if  ( mid == x / mid ) {
            return mid;
        }
        if ( x / mid  > mid ) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return r;
}
void Test_Sqrt() {
    std::cout << "ret=" << Sqrt__1() << std::endl;
}




/**
 * @Description: 给定一个只包括 '('，')'，'{'，'}'，'['，']' 的字符串，判断字符串是否有效。

                有效字符串需满足：
                左括号必须用相同类型的右括号闭合。
                左括号必须以正确的顺序闭合。
                注意空字符串可被认为是有效字符串。
 * @param
 * @return
 */
bool CheckBracket() {
    bool isTrue = true;
    std::stack < char > st;

    boost::unordered_map < char, int > m { {'(', 1}, {'{', 2}, {'[', 3}, {')', 4}, {'}', 5}, {']', 6} };
    std::string str = "()[]{}";
    str = "([)]";
    str = "{[abcdefg]}";

    for ( char ch : str ) {
        // 如果为开括号,将其入栈
        int flag = m[ ch ];

        if ( flag <= 3 && flag >= 1) st.push( ch );
        else if ( !st.empty() && m[ st.top() ] == (flag - 3) ) st.pop();
        else {
            isTrue = false;
            break;
        }
    }
    if (!st.empty()) isTrue = false;

    std::cout << "check kuohao=" << isTrue << std::endl;
    return isTrue;
}

// 一个整型数组里除了两个数字之外，其他的数字都出现了两次。请写程序找出这两个只出现一次的数字。
// void Test_FindNumsAppearOnce(const vector<int>& data,int* num1,int *num2) {
void FindNumsAppearOnce(vector<int> data,int* num1,int *num2) {

    int x = 0;//x保存所有元素异或的结果

    for ( auto iVal : data ) {
        x ^= iVal;
    }
    // std::cout << "s=" << s << std::endl;

    int n = 1;
    while ( (x & n)  == 0 ) {//找出最右边第1个不为0的位置
        n = n << 1;
    }

    for ( auto itmp : data ) {
        if (itmp & n) {//根据第一个不为0的位置重新将数组进行划分
            *num1 ^=  itmp;
        } else {
            *num2 ^=  itmp;
        }
    }
}
void Test_FindNumsAppearOnce() {
    int n1 = 0 , n2 = 0;

    std::vector<int> arr { -333, 1, 1, 7, 19, 123, -333, 72, 72, 7, 19, 9981 };

    FindNumsAppearOnce( arr, &n1, &n2 );

    std::cout << "n1=" << n1 << " n2=" << n2 << std::endl;
}


/*
请完成一个函数，输入一个二叉树，该函数输出它的镜像。
例如输入：
     4
   /   \
  2     7
 / \   / \
1   3 6   9
    
   镜像输出：
     4
   /   \
  7     2
 / \   / \
9   6 3   1

示例 1：
输入：root = [4,2,7,1,3,6,9]
输出：[4,7,2,9,6,3,1]
*/
BstNode* mirrorTree(BstNode* root) {
    std::stack<BstNode*> s;
    s.push(root);
    while (!s.empty()) {
        BstNode* node = s.top();
        s.pop();
        if (!node) continue;
        std::swap(node->left, node->right);
        s.push(node->left);
        s.push(node->right);
    }
    return nullptr;
}



/*
    定义栈的数据结构，请在该类型中实现一个能够得到栈的最小元素的 min 函数在该栈中，
    调用 min、push 及 pop 的时间复杂度都是 O(1)。
    示例:
    MinStack minStack = new MinStack();
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    minStack.min();   --> 返回 -3.
    minStack.pop();
    minStack.top();      --> 返回 0.
    minStack.min();   --> 返回 -2.
*/
class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {

    }
    
    void push(int x) {
        UNUSED(x);
    }
    
    void pop() {

    }
    
    int top() {

        return 1;
    }
    
    int min() {

        return 1;
    }
};


/*
给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
此外，你可以假设该网格的四条边均被水包围。

示例 1：
输入：grid = [
  ["1","1","1","1","0"],
  ["1","1","0","1","0"],
  ["1","1","0","0","0"],
  ["0","0","0","0","0"]
]
输出：1

示例 2：
输入：grid = [
  ["1","1","0","0","0"],
  ["1","1","0","0","0"],
  ["0","0","1","0","0"],
  ["0","0","0","1","1"]
]
输出：3
 
提示：
m == grid.length
n == grid[i].length
1 <= m, n <= 300
grid[i][j] 的值为 '0' 或 '1'
*/
int numIslands(vector<vector<char>>& grid) {
    UNUSED(grid);
    return 1;
}

// 盛最多水的容器

/* 
    输入一棵二叉树的根节点，求该树的深度。
    从根节点到叶节点依次经过的节点（含根、叶节点）形成树的一条路径，最长路径的长度为树的深度。

    例如：
    给定二叉树 [3,9,20,null,null,15,7]，
         3
        / \
        9  20
          /  \
         15   7
    返回它的最大深度=3
*/
int maxDepth_Recur(BstNode* root) {
    if ( nullptr == root ) return 0;
    int lDepth = maxDepth_Recur(root->left);
    int rDepth = maxDepth_Recur(root->right);
    int ret = 1 + std::max(lDepth, rDepth);
    return ret;
}
int maxDepth_NoRecur(BstNode* root) {
    {
        if (!root) return 0;
        int depth = 0;
        std::queue<BstNode*> que;
        que.push(root);

        while (!que.empty()) {
            size_t sz = que.size();
            while (sz > 0) {
                BstNode* tmp = que.front(); que.pop();
                if (tmp->left)  que.push(tmp->left);
                if (tmp->right) que.push(tmp->right);
                sz --;
            }
            depth ++;
        }
        return depth;
    }
    if (nullptr == root) return 0;
    
    int ans = 0;
    std::queue < BstNode* > que;
    que.push(root);
    
    while (!que.empty()) {
        int sz = que.size();
        while ( sz > 0 ) {
            BstNode* tmp = que.front(); que.pop();
            if (tmp->left) que.push(tmp->left);
            if (tmp->right) que.push(tmp->right);
            sz -= 1;
        }
        ans += 1;
    }
    return ans;
}
void Test_maxDepth() {
        // std::vector<int> vecData{4, 2, 7, 1, 3, 6, 9};
        // CreateBstTree(vecData);
        CreateBstTree();
    
std::cout << "before handle ..." << std::endl;
        printInorder(g_pBstTree);
std::cout << std::endl;

std::cout << "maxDepth_Recur=" << maxDepth_Recur(g_pBstTree) << std::endl;
std::cout << "maxDepth_NoRecur=" << maxDepth_NoRecur(g_pBstTree) << std::endl;
}


/*
    给定一棵二叉搜索树，请找出其中第k大的节点。
    示例 1:
    输入: root = [3,1,4,null,2], k = 1
         3
        / \
       1   4
        \
         2
        输出: 4
        示例 2:

    输入: root = [5,3,6,2,4,null,null,1], k = 3
         5
        / \
        3   6
       / \
      2   4
     /
    1
    输出: 4
*/
class CKthLargest_FromBstTree {
public:
    int ans;
    CKthLargest_FromBstTree() : ans(-1) {}
public:
    void dfs(BstNode* root, int& k) {
        if (!root) return;
        dfs(root->right, k);
        k --;
        if (k == 0) ans = root->val;
        if (k > 0) dfs(root->left, k);
    }
    int KthLargest_FromBstTree() {

        CreateBstTree();
        printInorder(g_pBstTree);
        std::cout << std::endl;
        int k = 3;
        dfs(g_pBstTree, k);

        std::cout << "kthLargest=" << ans << std::endl;
        return 1;
    }
};

/*
    给定一棵二叉搜索树，请找出其中第k小的结点。
*/
class CKthLeaest_FromBstTree {
public:
    int idx;
    CKthLeaest_FromBstTree() : idx(0) {
    }
    BstNode* KthNode(BstNode* root, int k) {
        if (!root) return nullptr;

        auto node = KthNode(root->left, k);
        if (node) return node;

        idx ++;
        if (idx == k) {
            return root;
        }
        node = KthNode(root->right, k);
        if (node) return node;
        return nullptr;
    }
    void Test_KthNode() {
        CreateBstTree();

        printInorder(g_pBstTree);
        std::cout << std::endl;

        auto ret = KthNode(g_pBstTree, 3);
        std::cout << "Kth least node=" << ret->val << std::endl;
        std::cout << std::endl;
    }
};

// 序列化二叉树
class CEncDecBstTree {
public:
    void TestSerial() {
        CreateBstTree();

        printInorder(g_pBstTree);
        std::cout << std::endl;

        std::cout << "serial res=" << SerialTree(g_pBstTree) << std::endl;
        std::cout << std::endl;
    }
    std::string SerialTree(BstNode* root) {
        UNUSED(root);
        // if (!root) return "";

        // std::ostringstream out;
        // std::queue<BstNode*> qNodes;
        
        // while (!qNodes.empty()) {
        //     BstNode* temp = qNodes.front();
        //     qNodes.pop();
        //     if (temp) {
        //         out << temp->val << " ";
        //         qNodes.push(temp->left);
        //         qNodes.push(temp->right);
        //     } else {
        //         out << "null ";
        //     }
        //     std::cout << "out=" << out.str().c_str() << std::endl;
        // }
        // return out.str();
        return "";
    }
};


/*
    二叉树中和为某一值的路径
    输入一棵二叉树和一个整数，打印出二叉树中节点值的和为输入整数的所有路径。
    从树的根节点开始往下一直到叶节点所经过的节点形成一条路径。
示例:
给定如下二叉树，以及目标和 sum = 22，
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
返回:
[
   [5,4,11,2],
   [5,8,4,5]
]
*/
class CPathSum {
public:
    std::vector< std::vector<int> > res;
    std::vector< int > path;

    void dfs(BstNode* root, int sum) {
        if (!root) return;

        path.push_back(root->val);
        sum -= root->val;

        if (sum == 0 && !root->left && !root->right) {
            res.push_back(path);
        }
        if (root->left)  dfs(root->left, sum);
        if (root->right) dfs(root->right, sum);
        path.pop_back(); // 递归回溯退回父节点，在路径上要删除当前节点
    }
//    std::vector< std::vector<int> > pathSum(BstNode* root, int sum) {
//    }
};




/*
输入一棵二叉树的根节点，判断该树是不是平衡二叉树。
如果某二叉树中任意节点的左右子树的深度相差不超过1，那么它就是一棵平衡二叉树。

示例 1:
给定二叉树 [3,9,20,null,null,15,7]

    3
   / \
  9  20
    /  \
   15   7
返回 true 。

示例 2:
给定二叉树 [1,2,2,3,3,null,null,4,4]

       1
      / \
     2   2
    / \
   3   3
  / \
 4   4
返回 false 。
*/
int maxDepthEx(BstNode* root) {
    if (!root) return 0;
    return std::max(maxDepthEx(root->left), maxDepthEx(root->right)) + 1;
}
bool isBalancedEx(BstNode* root) {
    if ( !root ) return true;
    int leftDepth = maxDepthEx(root->left);
    int rightDepth = maxDepthEx(root->right);
    std::cout << "depth gap=" << std::abs(leftDepth - rightDepth) << std::endl;
    if ( std::abs( leftDepth - rightDepth) > 1 ) return false;

    return isBalancedEx(root->left) && isBalancedEx(root->right);
}
void Test_isBalanced() {
    CreateBstTree();
    
    printInorder(g_pBstTree);
    std::cout << std::endl;

std::cout << std::endl;
std::cout << "level print beg" <<  std::endl;
    // LevelTraverseBstTree(g_pBstTree);

    // auto ret = levelOrder_new(g_pBstTree);
    // for (std::vector< std::vector<int> >::iterator itLine = ret.begin(); itLine != ret.end(); itLine ++) {
    //     for (std::vector<int>::iterator itVal = (*itLine).begin(); itVal != (*itLine).end(); itVal ++) {
    //         std::cout << " " << *itVal;
    //     }
    //     std::cout << std::endl;
    // }
std::cout << "level print end" <<  std::endl;
std::cout << std::endl;
    std::cout << "is balance=" << isBalanced(g_pBstTree) << std::endl;
}

/*
输入某二叉树的前序遍历和中序遍历的结果，请重建该二叉树。
例如:
    前序遍历 preorder = [3,9,20,15,7]
    中序遍历 inorder  = [9,3,15,20,7]
返回如下的二叉树：
    3
   / \
  9  20
    /  \
   15   7
*/
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    UNUSED(preorder);
    UNUSED(inorder);
    return nullptr;
}


/*
    二叉树中的最大路径和
    给定一个非空二叉树，返回其最大路径和。

本题中，路径被定义为一条从树中任意节点出发，沿父节点-子节点连接，达到任意节点的序列。
该路径至少包含一个节点，且不一定经过根节点。

示例 1：
输入：[1,2,3]
       1
      / \
     2   3
输出：6

示例 2：
输入：[-10,9,20,null,null,15,7]
   -10
   / \
  9  20
    /  \
   15   7
输出：42
*/
//【二叉树中的最大路径和】递归，条理清晰


/*
剑指 Offer 33. 二叉搜索树的后序遍历序列
输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历结果。
如果是则返回 true，否则返回 false。假设输入的数组的任意两个数字都互不相同。

参考以下这颗二叉搜索树：
     5
    / \
   2   6
  / \
 1   3
示例 1：
输入: [1,6,3,2,5]
输出: false

示例 2：
输入: [1,3,2,6,5]
输出: true
*/



// 链表-奇偶重排
ListNode* oddEvenList(ListNode* head) {
    if ( !head ) return nullptr;    
    ListNode* odd = head, *even = odd->next, *evenStart = even;
    while (even && even->next) {
        odd->next = even->next;
        odd = odd->next;
        even->next = odd->next;
        even = even->next;
    }
    odd->next = evenStart;
    return head;
}
void Test_oddEvenList() {
    std::vector<int> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto head = ConstructList(arr);
    auto ret = oddEvenList(head);
    PrintList(ret);
}

void impl_printInorder(BstNode *root)
{
    if (!root)
        return;
    if (root->left)
        impl_printInorder(root->left);
    std::cout << " " << root->val;
    if (root->right)
        impl_printInorder(root->right);
}
/*
-----------------------------------------------------------------------
    复杂度分析

    时间复杂度： O(n)，其中 n 为二叉树节点的个数。二叉树的遍历中每个节点会被访问一次且只会被访问一次。
    空间复杂度： O(n)。空间复杂度取决于递归的栈深度，而栈深度在二叉树为一条链的情况下会达到 O(n) 的级别。
-----------------------------------------------------------------------
*/
void PrintInorder(BstNode *root)
{
    impl_printInorder(root);
    std::cout << std::endl;
}
void extractNodeInorder(BstNode *root, std::vector<BstNode *> &retVec)
{
    if (!root)
        return;
    if (root->left)
        extractNodeInorder(root->left, retVec);
    retVec.push_back(root);
    if (root->right)
        extractNodeInorder(root->right, retVec);
}

/*
    二叉树中和为某一值的路径
    输入一棵二叉树和一个整数，打印出二叉树中节点值的和为输入整数的所有路径。
    从树的根节点开始往下一直到叶节点所经过的节点形成一条路径。
示例:
给定如下二叉树，以及目标和 sum = 22，
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
返回:
[
   [5,4,11,2],class CKthLeaest_FromBstTree
   [5,8,4,5]
]
*/
//class CPathSum {
//public:
//    std::vector< std::vector<int> > res;
//    std::vector< int > path;

//    void dfs(BstNode* root, int sum) {
//        if (!root) return;

//        path.push_back(root->val);
//        sum -= root->val;

//        if (sum == 0 && !root->left && !root->right) {
//            res.push_back(path);
//        }
//        if (root->left)  dfs(root->left, sum);
//        if (root->right) dfs(root->right, sum);
//        path.pop_back(); // 递归回溯退回父节点，在路径上要删除当前节点
//    }
//    std::vector< std::vector<int> > pathSum(BstNode* root, int sum) {
//        std::vector< std::vector<int> > a;
//        return a;
//    }
//};


//bool isBalancedEx(BstNode* root) {
//    if ( !root ) return true;
//    int leftDepth = maxDepthEx(root->left);
//    int rightDepth = maxDepthEx(root->right);
//    std::cout << "depth gap=" << std::abs(leftDepth - rightDepth) << std::endl;
//    if ( std::abs( leftDepth - rightDepth) > 1 ) return false;

//    return isBalancedEx(root->left) && isBalancedEx(root->right);
//}
//void Test_isBalanced() {
//    PrintInorder(g_pBstTree);
//    std::cout << std::endl;

//std::cout << std::endl;
//std::cout << "level print beg" <<  std::endl;
//    // LevelTraverseBstTree(g_pBstTree);

//    // auto ret = levelOrder_new(g_pBstTree);
//    // for (std::vector< std::vector<int> >::iterator itLine = ret.begin(); itLine != ret.end(); itLine ++) {
//    //     for (std::vector<int>::iterator itVal = (*itLine).begin(); itVal != (*itLine).end(); itVal ++) {
//    //         std::cout << " " << *itVal;
//    //     }
//    //     std::cout << std::endl;
//    // }
//std::cout << "level print end" <<  std::endl;
//std::cout << std::endl;
//    std::cout << "is balance=" << isBalanced(g_pBstTree) << std::endl;
//}


/*
    二叉树中的最大路径和
    给定一个非空二叉树，返回其最大路径和。

本题中，路径被定义为一条从树中任意节点出发，沿父节点-子节点连接，达到任意节点的序列。
该路径至少包含一个节点，且不一定经过根节点。

示例 1：
输入：[1,2,3]
       1
      / \
     2   3
输出：6

示例 2：
输入：[-10,9,20,null,null,15,7]
   -10
   / \
  9  20
    /  \
   15   7
输出：42
*/
//【二叉树中的最大路径和】递归，条理清晰


/*
    剑指 Offer 33. 二叉搜索树的后序遍历序列
输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历结果。
如果是则返回 true，否则返回 false。假设输入的数组的任意两个数字都互不相同。

参考以下-二叉搜索树：
     5
    / \
   2   6
  / \
 1   3
示例 1：
输入: [1,6,3,2,5]
输出: false

示例 2：
输入: [1,3,2,6,5]
输出: true

解题思路
1.最后一个节点为根节点
2.左边的节点全部要小于根，右边的节点全部要大于根，因此数组可以分成两个区间，前半部分全部小于根，后半部分全部大于根
3.找到两个区间的分割点，判断分割后的两个区间是否也符合
*/
class CheckPostOrder {
    bool Dfs(int start, int end, vector<int>& postorder) {
        if (start >= end) return true;
        
        int rootval = postorder[end];//最后一个为根节点，前半段小于根，后半段大于根
        int pos = start;
        while (postorder[pos] < rootval) pos ++;

        for ( ; pos < end; pos ++ ) {// 右子树内，所有节点必须大于根节点值
            if ( postorder[pos] <= rootval ) return false;
        }

        return Dfs(start, pos - 1, postorder) && Dfs(pos, end - 1, postorder);
    }
public:
    bool verifyPostorder(vector<int>& postorder) {
        if (postorder.size() < 2) return true;
        return Dfs(0, postorder.size() - 1, postorder);      
    }
};

/*
    将二叉搜索树转化为双向循环链表。
    链表中的每    都有环链个节点的前驱是最后一个节点，最后一个节点的后继是第一个节点。
*/
class CBstTreeToDoubleList {
    BstNode* head, *tail;
    CBstTreeToDoubleList() : head(nullptr), tail(nullptr) {
    }
    void inorder(BstNode* cur) {
        if (!cur) return;

        inorder(cur->left);// 构造出链表的所有结构，除了头连尾和尾连头的两个指针

        if (!head) {//当前前驱节点为空，说明这是双向链表的头节点（树中最左节点）
            head = cur;// 当tail还不存在，也就是cur此时在整个BST的最左边的节点，这个节点就是head
        } else { //此时已有前驱，说明这是链表中的某个中间节点，将前驱的右指针指向cur
            tail->right = cur;
            cur->left = tail;
        }
        tail = cur;// 将前一个节点更新为当前节点（所以到最后，tail就会挪到整个BST的最右边的节点，这个节点就是链表的尾节点）

        inorder(cur->right);
    }
public:
    void TreeToDoubleList(BstNode* root) {
        inorder(root);
        head->left = tail;// 补上头连尾
        tail->right = head;// 补上尾连头
    }
};

class CKthSmallest_FromBstTree {
public:
    int ans;
    CKthSmallest_FromBstTree() : ans(-1) {}
public:
    void dfs(BstNode* root, int& k) {
        if (!root) return;
        {
            dfs(root->left, k);
            k --;
            if (k == 0) ans = root->val;
            if (k > 0) dfs(root->right, k);
            return ;
        }
    }
    int CKthSmallest() {
        PrintInorder(g_pBstTree);
        std::cout << std::endl;
        
        int k = 3;
        dfs(g_pBstTree, k);

        std::cout << "CKthSmallest=" << ans << std::endl;
        return 1;
    }
};
void Test_CKthSmallest() {
    CKthSmallest_FromBstTree cskt;
    cskt.CKthSmallest();
    // return;
    {
        CKthLeaest_FromBstTree klnode;
        klnode.Test_KthNode();
    }
}


/*
    938. 二叉搜索树的范围和
    给定二叉搜索树的根结点 root，返回值位于范围 [low, high] 之间的所有结点的值的和。

    示例 1：
    输入：root = [10,5,15,3,7,null,18], low = 7, high = 15
    输出：32

    示例 2：
    输入：root = [10,5,15,3,7,13,18,1,null,6], low = 6, high = 10
    输出：23
*/
int rangeSumBST(BstNode* root, int low, int high) {
    if (!root) return 0;
    if (root->val < low) {
        return rangeSumBST(root->right, low, high);
    } else if ( root->val > high ) {
        return rangeSumBST(root->left, low, high);
    }
    return root->val + rangeSumBST(root->left, low, high) + rangeSumBST(root->right, low, high);
}
void Test_rangeSumBST() {    
    std::cout << "before handle ..." << std::endl;
    PrintInorder(g_pBstTree);
    std::cout << std::endl;
}


/*
给定二叉搜索树（BST）的根节点和一个值。
你需要在BST中找到节点值等于给定值的节点, 返回以该节点为根的子树。
如果节点不存在，则返回 NULL。

例如: 给定二叉搜索树:
        4
       / \
      2   7
     / \
    1   3
和值: 2

你应该返回如下子树:
      2     
     / \   
    1   3
在上述示例中，如果要找的值是 5，但因为没有节点值为 5，我们应该返回 NULL。
*/
class CSearchBst {
    int kthVal;

    void getKthVal(BstNode* root, int& k) {
        if (!root) return;
        if (root->left) getKthVal(root->left, k);
        k --;
        if (k == 0) {
            kthVal = root->val; return ;
        }
        if (root->right) getKthVal(root->right, k);
    }

    BstNode* searchBST(BstNode* root, int val) {
        if (!root || root->val == val) return root;
        if (root->val > val) return searchBST(root->left, val);
        if (root->val < val) return searchBST(root->right, val);
        return nullptr;
    }

public:
    void Test_searchBst() {
        std::cout << "before handle ..." << std::endl;
        PrintInorder(g_pBstTree);

        int idx = 0;
        while (idx == 0) {
            idx = rand() % 10;
        }
        std::cout << "idx=" << idx << std::endl;
        getKthVal(g_pBstTree, idx);

        std::cout << std::endl;
        auto node = searchBST(g_pBstTree, kthVal);
        std::cout << node->val << std::endl;
    }
};

void levelOrderPrint(BstNode* root) {
    if (!root) return;
    
    std::vector< std::vector<int> > res;

    std::queue< BstNode* > qNodes;
    qNodes.push(root);

    while (qNodes.size()) {
        
        std::vector<int> vecLevel;
        size_t sz = qNodes.size();

        for ( size_t i = 0; i < sz; i ++ ) {
            BstNode* t = qNodes.front(); qNodes.pop();
            if (!t) continue;

            vecLevel.push_back( t->val );
            if (t->left)  vecLevel.push_back( t->left->val );
            if (t->right) vecLevel.push_back( t->right->val );
        }

        res.push_back(vecLevel);
    }

    // res is result ...
}

/*
    剑指 Offer 52. 两个链表的第一个公共节点
    给定两个（单向）链表，判定它们是否相交并返回交点。
    请注意相交的定义基于节点的引用，而不是基于节点的值。
    换句话说，如果一个链表的第k个节点与另一个链表的第j个节点是同一节点（引用完全相同），则这两个链表相交。

    示例 1：
    输入：intersectVal = 8, listA = [4,1,8,4,5], listB = [5,0,1,8,4,5], skipA = 2, skipB = 3
    输出：Reference of the node with value = 8
    输入解释：相交节点的值为 8 （注意，如果两个列表相交则不能为 0）。从各自的表头开始算起，链表 A 为 [4,1,8,4,5]，链表 B 为 [5,0,1,8,4,5]。在 A 中，相交节点前有 2 个节点；在 B 中，相交节点前有 3 个节点。

    示例 2：
    输入：intersectVal = 2, listA = [0,9,1,2,4], listB = [3,2,4], skipA = 3, skipB = 1
    输出：Reference of the node with value = 2
    输入解释：相交节点的值为 2 （注意，如果两个列表相交则不能为 0）。从各自的表头开始算起，链表 A 为 [0,9,1,2,4]，链表 B 为 [3,2,4]。在 A 中，相交节点前有 3 个节点；在 B 中，相交节点前有 1 个节点。

    示例 3：
    输入：intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2
    输出：null
    输入解释：从各自的表头开始算起，链表 A 为 [2,6,4]，链表 B 为 [1,5]。
    由于这两个链表不相交，所以 intersectVal 必须为 0，而 skipA 和 skipB 可以是任意值。
    解释：这两个链表不相交，因此返回 null。

    创建两个指针 pA 和 pB，分别初始化为链表 A 和 B 的头结点。然后让它们向后逐结点遍历。
当 pApA 到达链表的尾部时，将它重定位到链表 B 的头结点 (你没看错，就是链表 B); 
当 pBpB 到达链表的尾部时，将它重定位到链表 A 的头结点。
若在某一时刻 pApA 和 pBpB 相遇，则 pApA/pBpB 为相交结点。
想弄清楚为什么这样可行, 可以考虑以下两个链表: A={1,3,5,7,9,11} 和 B={2,4,9,11}，相交于结点 9。
由于 B.length (=4) < A.length (=6)，pBpB 比 pApA 少经过 2 个结点，会先到达尾部。
将 pB 重定向到 A 的头结点，pA 重定向到 B 的头结点后，pB 要比 pA 多走 2 个结点。因此，它们会同时到达交点。
如果两个链表存在相交，它们末尾的结点必然相同。因此当 pApA/pBpB 到达链表结尾时，记录下链表 A/B 对应的元素。
若最后元素不相同，则两个链表不相交。

复杂度分析
时间复杂度 : O(m+n)
空间复杂度 : O(1)
*/
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    if ( headA == nullptr || headA == nullptr ) return nullptr;

    ListNode* p1 = headA;
    ListNode* p2 = headB;

    while ( p1 != p2 ) {
        if (p1) {
            p1 = p1->next;
        } else {
            p1 = headB;
        }
        if (p2) {
            p2 = p2->next;
        } else {
            p2 = headA;
        }
    }
    return p1;
}


/*
    给定一个二叉树，计算整个树的坡度

    节点坡度 定义: 该节点 左子树节点之和 和 右子树节点之和 的差的绝对值。

    如果没有左子树，左子树的节点之和为0；
    没有右子树也是一样。空结点的坡度是0。

    整个树的坡度: 是其所有节点的坡度之和。
*/
int findTilt(BstNode* root) {
    UNUSED(root);
    return 1;
}


/*
    剑指 Offer 28. 对称的二叉树
请实现一个函数，用来判断一棵二叉树是不是对称的。如果一棵二叉树和它的镜像一样，那么它是对称的。

例如: 二叉树 [1,2,2,3,4,4,3] 是对称的。
    1
   / \
  2   2
 / \ / \
3  4 4  3

但是下面这个 [1,2,2,null,3,null,3] 则不是镜像对称的:
    1
   / \
  2   2
   \   \
   3    3

示例 1：
输入：root = [1,2,2,3,4,4,3]
输出：true

示例 2：
输入：root = [1,2,2,null,3,null,3]
输出：false
*/
bool check_isSysmmetric(BstNode* p, BstNode* q) {
    if ( !p && !q )          return true;
    if ( p == nullptr && q ) return false;
    if ( p && q == nullptr)  return false;
    return (p->val == q->val) && check_isSysmmetric(p->left, q->right) && check_isSysmmetric(p->right, q->left);
}
void isSymmetric() {

std::cout << "before check ..." << std::endl;
        PrintInorder(g_pBstTree);
std::cout << std::endl;

    std::cout << "check sysmmetric=" << check_isSysmmetric(g_pBstTree, g_pBstTree) << std::endl;
}

/*
    543. 二叉树的直径
    给定一棵二叉树，你需要计算它的直径长度。
    一棵二叉树的直径长度是任意两个结点路径长度中的最大值。这条路径可能穿过，也可能不穿过根结点。

示例: 给定二叉树
          1
         / \
        2   3
       / \     
      4   5    
返回 3, 它的长度是路径 [4,2,1,3] 或者 [5,2,1,3]。

注意: 两结点之间的路径长度 是以它们之间边的数目表示。
-----------------------------------------------------------------------

-----------------------------------------------------------------------
*/
int diameterOfBinaryTree(BstNode *root) {
    UNUSED(root);
    return 1;
}
void Test_diameterOfBinaryTree() {

}

/*
    1367. 二叉树中的列表
给你一棵以 root 为根的二叉树和一个 head 为第一个节点的链表。

如果在二叉树中，存在一条一直向下的路径，且每个点的数值恰好一一对应以 head 为首的链表中每个节点的值，那么请你返回 True，否则返回 False 。

一直向下的路径的意思是：从树中某个节点开始，一直连续向下的路径。
*/
class CCheckTreeSubList {
    bool dfs(ListNode* head, TreeNode* root) {
        if (!head) return true;
        if (!root) return false;
        if (root->val != head->val) return false;
        return (dfs(head->next, root->left) || dfs(head->next, root->right));
    }
public:
    bool isSubPath(ListNode *head, TreeNode *root) {
        std::queue<TreeNode *> que;
        que.push(root);
        while (que.size()) {
            TreeNode *tmp = que.front();
            que.pop();

            if (tmp->val == head->val)
            {
                if (dfs(head, tmp))
                    return true;
            }
            if (tmp->left)
                que.push(tmp->left);
            if (tmp->right)
                que.push(tmp->right);
        }
        return false;
    }
};
void Test_CheckTreeSubList() {
    CCheckTreeSubList cts;
    cts.isSubPath(nullptr, nullptr);
}


/*
    654. 最大二叉树
    给定一个不含重复元素的整数数组。
    一个以此数组构建的最大二叉树定义如下：

    二叉树的根是数组中的最大元素。
    左子树是通过数    最大值左边部分构造出的最大二叉树。
    右子树是通过数组中最大值右边部分构造出的最大二叉树。
    通过给定的数组构建最大二叉树，并且输出这个树的根节点。

    示例：
    输入：[3,2,1,6,0,5]
    输出：返回下面这棵树的根节点：
      6
    /   \
   3     5
    \    / 
     2  0   
       \
        1
*/
class CConstructMaxBst {
    TreeNode* dfs(vector<int>& nums, int l, int r) {
        if (l > r) return nullptr;

        int idx = l;
        for (int i = l + 1; i < r; i ++) {
            if (nums[i] > nums[idx]) idx = i;
        }

        TreeNode* node = new TreeNode( nums[idx] );
        node->left  = dfs(nums, l, idx - 1);
        node->right = dfs(nums, idx + 1, r);
        return node;
    }
public:
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        return dfs(nums, 0, nums.size() - 1);
    }
};
void Test_ConstuctMaxBst() {
        
}




/*
    783. 二叉搜索树节点最小距离
给定一个二叉搜索树的根节点 root，返回树中任意两节点的差的最小值。

示例：
输入: root = [4,2,6,1,3,null,null]
输出: 1
解释: 注意，root是树节点对象(TreeNode object)，而不是数组。

给定的树 [4,2,6,1,3,null,null] 可表示为下图:
         4
       /   \
      2     6
     / \
    1   3
最小的差值是 1, 它是节点1和节点2的差值, 也是节点3和节点2的差值。
*/
class CMinDiffInBst {
    BstNode* pre;
    int      minVal;

    void minDiff(BstNode* root) {
        if (!root) return;
        if (root->left) {
            minDiff(root->left);
        }
        if (pre) {
            minVal = std::min(minVal, std::abs(pre->val - root->val));
        }
        pre = root;
        if (root->right)
            minDiff(root->right);
    }
public:
    CMinDiffInBst() : pre(nullptr), minVal(INT_MAX) {}

    void minDiffInBST() {

std::cout << "before handle ..." << std::endl;
        PrintInorder(g_pBstTree);
std::cout << std::endl;
    minDiff(g_pBstTree);
std::cout << "res=" << minVal << std::endl;
    }
};
void Test_minDiffInBST() {
    CMinDiffInBst cm;
    cm.minDiffInBST();
}

/*
    501. 二叉搜索树中的众数
给定一个有相同值的二叉搜索树（BST），找出 BST 中的所有众数（出现频率最高的元素）。

例如： 给定 BST [1,null,2,2],
   1
    \
     2
    /
   2
返回[2].

提示：如果众数超过1个，不需考虑输出顺序

进阶：你可以不使用额外的空间吗？（假设由递归产生的隐式调用栈的开销不被计算在内）
*/
class CFindModeNum {
    std::vector<int> res;
    int maxTimes = 0;
    int curTimes = 0;
    BstNode *pre = nullptr; // 指向当前节点的前一个节点
public:
    void inorder(BstNode* cur) {
        if (!cur) return ;
        inorder(cur->left);

        if (pre) {
            curTimes = (cur->val == pre->val) ? curTimes + 1 : 1;
        }
        if (curTimes == maxTimes) {
            res.push_back(cur->val);
        } else if (curTimes > maxTimes) {
            res.clear(); res.push_back(cur->val);
            maxTimes = curTimes;
        }
        pre = cur;// 将前一个节点更新为当前节点
        inorder(cur->right);
    }
    void findMode() {
        inorder(g_pBstTree);
        
        PrintInContainer(res);
    }
};
void Test_CFindModeNum() {
    CFindModeNum cf;
    cf.findMode();
}

// 递归计算，数组和
int arrSum(const vector<int>& v1, size_t len) {
    if (len == v1.size()) return 0;
    return v1[len] + arrSum(v1, len + 1);
}
void Test_arraySum() {
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::cout << "last ret=" << arrSum(v1, 0) << std::endl;
}

/*
    1038. 把二叉搜索树转换为累加树
    给出二叉 搜索 树的根节点，该树的节点值各不相同，请你将其转换为累加树（Greater Sum Tree），
    使每个节点 node 的新值等于原树中大于或等于 node.val 的值之和。

示例 1：
输入：[4,1,6,0,2,5,7,null,null,null,3,null,null,null,8]
输出：[30,36,21,36,35,26,15,null,null,null,33,null,null,null,8]

示例 2：
输入：root = [0,null,1]
输出：[1,null,1]

示例 3：
输入：root = [1,0,2]
输出：[3,3,2]

示例 4：
输入：root = [3,2,4,1]
输出：[7,9,4,10]
*/

/*
    109. 有序链表转换二叉搜索树
给定一个单链表，其中的元素按升序排序，将其转换为高度平衡的二叉搜索树。

本题中，一个高度平衡二叉树是指一个二叉树每个节点 的左右两个子树的高度差的绝对值不超过 1。

示例: 给定有序链表： [-10, -3, 0, 5, 9]

一个可能的答案是：[0, -3, 9, -10, null, 5], 它可以表示下面这个高度平衡二叉搜索树：
      0
     / \
   -3   9
   /   /
 -10  5
*/
class CSortedListToBst {
private:
    ListNode* getMidNode(ListNode* left, ListNode* right) {
        ListNode* fast = left;
        ListNode* slow = left;
        while (fast != right && fast->next != right) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    }
    BstNode* buildBstTree(ListNode* l, ListNode* r) {
        if (l == r) return nullptr;

        ListNode* mid = getMidNode(l, r);

        BstNode* root = new BstNode(mid->val);
        if (!root) return nullptr;

        root->left  = buildBstTree(l, mid);
        root->right = buildBstTree(mid->next, r);
        return root;
    }
public:
    CSortedListToBst() {
    }
    void sortedListToBST() {
        g_pBstTree = buildBstTree(g_pListHead, nullptr);
    }
};

void initList() {
    std::vector<int> l{ -11, -3, 0, 16, 23, 89, 110, 312, 5277, 77612};
    g_pListHead = ConstructList(l);
}


/*
    876. 链表的中间结点
    给定一个头结点为 head 的非空单链表，返回链表的中间结点。
    如果有两个中间结点，则返回第二个中间结点。

    示例 1：
    输入：[1,2,3,4,5]
    输出：此列表中的结点 3 (序列化形式：[3,4,5])
    返回的结点值为 3 。 (测评系统对该结点序列化表述是 [3,4,5])。
    注意，我们返回了一个 ListNode 类型的对象 ans，这样：
    ans.val = 3, ans.next.val = 4, ans.next.next.val = 5, 以及 ans.next.next.next = NULL.

    示例 2：
    输入：[1,2,3,4,5,6]
    输出：此列表中的结点 4 (序列化形式：[4,5,6])
    由于该列表有两个中间结点，值分别为 3 和 4，我们返回第二个结点。
*/
ListNode* middleNode(ListNode* head) {

    ListNode* fast = head, *slow = head;
    
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    return slow;
}
void Test_middleNode() {
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8};
    auto l1 = ConstructList(v1);
    PrintList(l1);

    std::cout << "val=" << middleNode(l1)->val << std::endl;
}


/*
    100. 相同的树
    给定两个二叉树，编写一个函数来检验它们是否相同。
    如果两个树在结构上相同，并且节点具有相同的值，则认为它们是相同的。

    示例 1:
    输入:       1         1
              / \       / \
             2   3     2   3

            [1,2,3],   [1,2,3]
    输出: true

    示例 2:
    输入:      1          1
              /           \
             2             2

            [1,2],     [1,null,2]
    输出: false

    示例 3:
    输入:       1         1
              / \       / \
             2   1     1   2

            [1,2,1],   [1,1,2]
    输出: false
-----------------------------------------------------------------------
时间复杂度： O(min(m,n))，其中 m 和 n 分别是两个二叉树的节点数。
对两个二叉树同时进行深度优先搜索，只有当两个二叉树中的对应节点都不为空时才会访问到该节点，
因此被访问到的节点数不会超过较小的二叉树的节点数。

空间复杂度： O(min(m,n))，其中 m 和 n 分别是两个二叉树的节点数。
空间复杂度取决于递归调用的层数，递归调用的层数不会超过较小的二叉树的最大高度，最坏情况下，二叉树的高度等于节点数。
-----------------------------------------------------------------------
*/
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr)
        return true;
    else if (!p || !q)
        return false;
    else if (p->val != q->val)
        return false;
    else 
        return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

void simple_multi_derive() {
    class A {
    public:
        int a;
        virtual void v();
    };
    class B {
    public:
        int b;
        virtual void w();
    };
    class C : public A, public B {
    public:
        int c;
    };

    // size C=32
    std::cout << "size C=" << sizeof(C) << std::endl;
}
void some_derive() {
    {
        class Base {
        public:
            virtual void func() {
                cout << "base::func" << endl;
            }
            virtual void fun1() {
                cout << "base::fun1" << endl;
            }
        };
        class Derived : public Base{
        public:
            /*virtual*/ void func() /*override*/ { // 确保func被重写
                cout << "derived::func" << endl;
            }
            void fun1() {
                cout << "Derived::fun1 not virtual function" << endl;
            }
        };

        Derived dd;
        Base& b = dd;

        dd.fun1();
        b.fun1();
//        Derived dd;
//        dd.func();
        return;

        Base* pBase = &dd;
        pBase->func();
        return;
    }

    class XXXX
    {
    public:
        XXXX()  { std::cout << " cst XXXX obj" << std::endl; }
        XXXX(int val) : a(val)  {
            std::cout << " cst XXXX obj, by val" << ", a=" << a << std::endl;
        }
        ~XXXX() { std::cout << " dst XXXX obj" << std::endl; }
        inline void innerFun() {
            std::cout << " inner fun" << std::endl;
        }
    public:
        int a;
    };
    class AAAA
    {
        public:
            int a;
        public:
            virtual void v();
    };
    class BBBB : public AAAA {
    public:
        int b;
    };
//    std::shared_ptr<XXXX> x1 = std::make_shared<XXXX>(1123);
//    return;

    std::cout << "size XXXX=" << sizeof(XXXX) << std::endl; // 4 bytes
    std::cout << "size AAAA=" << sizeof(AAAA) << std::endl; // 16 bytes
    std::cout << "size BBBB=" << sizeof(BBBB) << std::endl; // 16 bytes
}
    int lower_Bound(vector<int> &nums, int target) {
        size_t left = 0;
        size_t right = nums.size();

        while ( left < right ) {
            size_t mid = left + ((right - left) >> 1);

                std::cout << "000_mid=" << mid << std::endl;
            if ( nums[ mid ] >= target ) {
                right = mid;
                std::cout << "111_mid=" << mid << std::endl;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }

void tst_lower_bound() {
    std::vector<int> nums{ 1, 2, 3,  3, 3, 3,  3, 3,   4, 4, 5, 8 };
    std::cout << "lower_bound=" << lower_Bound(nums, 4) << std::endl;
}




void longa() 
{ 
  int i,j; 
  for(i = 0; i < 10000000; i++) 
    j=i; //am I silly or crazy? I feel boring and desperate. 
}

void funcA() 
{
    int i;
    for(i=0 ; i < 1000; i++) {
        longa();
    }
}

void funcB() 
{ 
  int i;

  for(i = 0; i< 10000; i++) {
      longa();
  }
}

void tst_perf() {
  for(auto i = 0; i< 10000; i++) {
      sleep(2);
  }
    // funcA();
    // funcB();
}

int main(int argc, char *argv[])
{
    initList();
    CreateBstTree();

    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid=" << CurrentThread::tid();

//    return 1;
//tst_perf();
//return 1;

    // --------------------function entry--------------------
//    tst_qs_new(); return 1;

    {
        // tst_lower_bound(); return 1;
        //        simple_multi_derive();
        //        some_derive(); return 1;
    }

    if ( argc > 1 ) {
//        std::cout << "argv[ 0 ]=" << argv[ 0 ] << std::endl;
//        std::cout << "argv[ 1 ]=" << argv[ 1 ] << std::endl;

        // tst_my_bst_tree();
        // std::cout << "res=" << kthSmallest(g_pBstTree, 3) << std::endl;
        // return 1;


        if ( strcmp( argv[1], "lc" ) == 0 ) {
            std::cout << "tst leet code ---" << std::endl;
            std::cout << std::endl;

            lc_Entry(); return 1;
        } else if (strcmp( argv[1], "l" ) == 0 ) {
            std::cout << "tst list code ---" << std::endl;
            std::cout << std::endl;

            Test_ListEntry(); return 1;
        } else if ( strcmp( argv[1], "dp" ) == 0 ) {
            tst_dpfun_entry(); return 1;
        }
    } else {
        std::cout << " argc < 1, argc=" << argc;
        return -1;
    }


    Test_middleNode(); return 1;

    // PrintInorder(g_pBstTree);
    // std::cout << std::endl;

    isSymmetric();      return 1;


{
    CSortedListToBst c1;
    c1.sortedListToBST();
    return 1;
}

    Test_CFindModeNum(); return 1;

    std::cout << "INT_MIN=" << INT_MIN << std::endl;

    Test_arraySum();   return 1; 
//    CreateBstTreeEx(); return 1;

    Test_minDiffInBST(); return 1;

{
    CSearchBst csb;
    csb.Test_searchBst(); return 1;
}
    Test_rangeSumBST(); return 1;
    Test_maxDepth(); return 1;

    Test_CKthSmallest(); return 1;


    // Test_isBalanced(); return 1;

    CPathSum pathsum; return 1;

    // CEncDecBstTree cEnDeTree;
    // cEnDeTree.TestSerial();
    // return 1;

    CKthLargest_FromBstTree ctt1;
    ctt1.KthLargest_FromBstTree();
    return 1;

    Test_oddEvenList(); return 1;
//  tst_randEx();    return 1;

    findKth(); return 1;

    Test_FindNumsAppearOnce(); return 1;

    CheckBracket(); return 1;
    
    Test_Sqrt(); return 1;

    Test_ReverseString(); return 1;

    Test_maxsumofSubarray(); return 1;

    Test_mergeTwoArray(); return 1;

    Test_DetectListCycle(); return 1;

    tst_lowestCommonAncestor(); return 1;

    Test_stackToQueue(); return 1;

    Test_GetLeastNumbers(); return 1;

    // tst_RemoveNthListNode(); return 1;

//    std::cout << "serial ret=" << Serialize(g_pBstTree) << std::endl;
//    return 1;




    tstC11();  return 1;


//    int data[16] = {1,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0}; //明文
//    std::cout << "sizeof(data)=" << sizeof(data) << std::endl;
//    return 1;

    uint tmpval = 0x10;
    LOG_INFO << "tmpval=" << tmpval;
    LOG_INFO << "!tmpval=" << (!tmpval);
    LOG_INFO << "!!tmpval=" << (!!tmpval);
//    LOG_INFO << "tmpval&1=" << (tmpval&1);

    tmpval = 17;
    uint newval = ((uint) tmpval & (uint) ~1);
    LOG_INFO << "newval=" << newval;
    return 1;

    Bit_ST obj1;
    LOG_INFO << "obj1.instanceq=" << obj1.instance;
    LOG_INFO << "sizeof(Bit_ST)=" << sizeof(obj1);
    return 1;

    std::vector< MyConn* > vObjs;
    for ( int i = 0; i < 10; i ++ ) {
        MyConn* tmpobj = new(MyConn);

//        void *ptr = (void*)((uintptr_t)c | ev->instance);
//        LOG_INFO << "a=" << ((uintptr_t)tmpobj & 1) << " obj=" << tmpobj;

        int* tmpIdx = new int(123);
        LOG_INFO << " tmpIdx=" << tmpIdx;

        void* changPtr = (void *) ((uintptr_t) tmpIdx | 1);
        LOG_INFO << " newPtr=" << changPtr;

        void* recoverPtr = (void *) ((uintptr_t) tmpIdx & (uintptr_t) ~1);
        LOG_INFO << " oldPtr=" << recoverPtr;

        vObjs.push_back(tmpobj);
    }
    return 1;

    tstCmpare(); return 1;

    tstDiffNumber(); return 1;
    tst_tuple_1(); return 1;

    tst_base_derive();  return 1;
    tst_multimap();     return 1;

    tst_maxinwindows(); return 1;

    tst_kuohao_pipei(); return 1;

    tst_bit_opr();      return 1;

    tst_format_str();   return 1;

//    vector<vector<int>> asd = levelPrintBst/*levelOrder*/(g_pBstTree);
//    for ( std::vector<vector<int>>::iterator itV = asd.begin(); itV != asd.end(); itV ++ ) {
//        for ( auto it : *itV ) {
//            std::cout << " " << it;
//        }
//        std::cout << std::endl;
//    }

    // 遍历二叉树；
    std::cout << "inorder print bst 1----" << std::endl;
    inOrder(g_pBstTree);
    std::cout << std::endl;
    std::cout << "inorder print bst 1----" << std::endl;

    std::cout << "preorder print bst 1----" << std::endl;
    preOrder(g_pBstTree);
    std::cout << std::endl;
    std::cout << "preorder print bst 1----" << std::endl;
    return 1;

    LevelTraverseBstTree(g_pBstTree); return 1;

    std::cout << " tree depth = " << GetBstTreeDepth(g_pBstTree) << std::endl; // return 1;
    std::cout << " tree count = " << GetBstTreeNodeCount(g_pBstTree) << std::endl;// return 1;
    std::cout << " leaft count = " << GetLeafNodeCountImpl(g_pBstTree) << std::endl; return 1;

    tst_heap_fun(); return 1;

    tst_multiset(); return 1;

    tst_FindGreatestSumOfSubArray(); return 1;

    tst_qs_111(); return 1;

    tst_list_1(); return 1;

    tst_copy_backward(); return 1;

//    wrap_longestsubstring(); return 1;

//    wrap_IsContinuous(); return 1;

//    wrap_max_subarray(); return 1;

//    tst_bst_tree(); return 1;
//    sortArrayOddEven(); return 1;

//    tst_my_bst_1(); return 1;

    binary_search_wrap(); return 1;


    tst_class_x(); return 1;

    tst_lower_upper(); return 1;

    tst_O1_age(); return 1;

    tst_reverse_str(); return 1;

    StackHeapObj(); return 1;

    tst_111(); return 1;

    tst_some_one_1(); return 1;

    my_itoa(10089);
    my_itoa(-32652300); return 1;

    MakeX7(); return 1;

    tst_hash_fun_entry(); return 1;

    tst_HeapSortEntry_(); return 1;


    call_find_max_in_array(); return 1;
    call_Max_array_Sum(); return 1;

    call_rever_KthNode(); return 1;

    tst_findmin_inrotatearrry();return 1;

    tst_link_1(); return 1;

    tst_lixi(); return 1;

//    tst_ptr_vec(); return 1;
    tst_MergeSortEntry_(); return 1;


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

    tst_linkedlist_entry(); return 1;

    tst_Extern_MergeSortEntry_(); return 1;

//    CopyFileEx("template-openssl.cnf", "123.cnf");
//    return 1;

    tst_ComplexDataEntry_(); return 1;

    tst_print_time(); return 1;

    tst_struct_fun(); return 1;

    tst_vec_quchong(); return 1;

//    tst_catFile();
    tst_catFileEx(); return 1;

    tst_maxheap_sort(); return 1;

    tst_qs_111(); return 1;

    ts_PrintLines(); return 1;

    tst_boostBind_1(); return 1;
    tstMemoryLeak(); return 1;

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

    print_time(); return 1;

//    tst_list_qs(); return 1;


//    trie_tree_1(); return 1;

//    int sum = 0, minSum = 0, maxSum = INT_MIN;
//    std::cout << "maxSum=" << maxSum << std::endl;
//    return 1;

    TestMemberFunction();
    TestBoostBind();

    return 1;

    tst_pri_queue_1(); return 1;
    tst_class_1(); return 1;

    tst_host_net_shunxu(); return 1;

    tst_int64_1(); return 1;

//     std::string addFile_      = "insert into @table 
// (fileId,pid,ownerId,fileName,fileType,fileSuffix,filePath,fileSize,encryptFileSize,secretKey,
// uploaderId,uploaderName,md5Hash,sha1Hash,createdAt,updatedAt,deletedAt,fileCode,orderNum,
// roleId,members,fileReMark,pdfPath,validity,fileClass) 
// values (?,?,?,?,?,?,?)";
//     std::cout << "addFile_=" <<  addFile_ << std::endl;
//     ConstructInsertSql( addFile_ );
// return 1;

    tst_sort_3(); return 1;
//    tst_sort_2(); return 1;
    tst_sort_1(); return 1;
}
