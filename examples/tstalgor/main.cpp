






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



int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

#ifdef __cplusplus//C++编译器定义了-cplusplus
std::cout << "c++" << std::endl;
#else
std::cout << "c" << std::endl;
#endif

    tst_hash_fun_entry();
//    return 1;
    tst_Extern_MergeSortEntry_();
    return 1;

//    CopyFileEx("template-openssl.cnf", "123.cnf");
//    return 1;

//    std::cout << std::endl;
    std::cout << std::endl;
    tst_MergeSortEntry_(); return 1;

    tst_HeapSortEntry_();  return 1;
    tst_ComplexDataEntry_(); return 1;
    tst_ListEntry_(); return 1;

tst_print_time(); return 1;
tst_qs_111(); return 1;

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













