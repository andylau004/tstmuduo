





#include <iostream>

#include <sstream>



#include <iostream>
#include <queue>




#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <fstream>



#include <stdio.h>
#include <thread>


#include <boost/bind.hpp>


//#include "poco/Bugcheck.h"
//#include "poco/Foundation.h"
//#include "poco/Alignment.h"

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



using namespace std;
using namespace muduo;
using namespace muduo::net;





const int MAX_LENGTH_INSERT_SORT = 7;


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

// 处理　只有一对　左右括号，且需要后面加入大量(?,?....) 场景
bool ConstructInsertSql( std::string& in_sql ) {
    std::string str_ret;

    std::string cpy_sql = in_sql;

    size_t leftPos = cpy_sql.find('(');
    if (leftPos == std::string::npos) {
        LOG_INFO << "construct left failed!, insql=" << in_sql;
        return false;
    }

    std::string strLeft = cpy_sql.substr(0, leftPos+1);
    LOG_INFO << "construct right strLeft=" << strLeft;

    size_t rightPos = cpy_sql.find(')');
    if (rightPos == std::string::npos) {
        LOG_INFO << "construct right failed!, insql=" << in_sql;
        return false;
    }

    std::string strMid = cpy_sql.substr(leftPos+1, rightPos-leftPos-1);
    LOG_INFO << "construct right strRight=" << strMid;

    int num = count(strMid.begin(), strMid.end(), ',');
    if (!num) {
        LOG_INFO << "construct count failed!, insql=" << in_sql;
        return false;
    }

    std::string strInsertLeft  = " values ( ";
    std::string strInsertRight = " )";

    for (int i = 0; i <= num; i++ ) {
        if (i == num)
            strInsertLeft += "? ";
        else
            strInsertLeft += "?, ";
    }

    std::string lastRet;
    lastRet = strLeft + strMid + ")" + strInsertLeft + strInsertRight;
LOG_INFO << "lastRet=" << lastRet;

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

// 从数组的两端向中间扫描
/*
length=10
before sort----
12 45 748 15 56 3 89 4 48 2
111 ready swap a[0]=12, a[9]=2, pivot=12
2 45 748 15 56 3 89 4 48 12
swap end----

222 ready swap a[1]=45, a[9]=12, pivot=12
111 ready swap a[1]=12, a[7]=4, pivot=12
2 4 748 15 56 3 89 12 48 45
swap end----

222 ready swap a[2]=748, a[7]=12, pivot=12
111 ready swap a[2]=12, a[5]=3, pivot=12
2 4 3 15 56 12 89 748 48 45
swap end----

222 ready swap a[3]=15, a[5]=12, pivot=12
111 ready swap a[3]=12, a[3]=12, pivot=12
2 4 3 12 56 15 89 748 48 45
swap end----

222 ready swap a[3]=12, a[3]=12, pivot=12
111 ready swap a[0]=2, a[0]=2, pivot=2
2 4 3 12 56 15 89 748 48 45
swap end----

222 ready swap a[0]=2, a[0]=2, pivot=2
111 ready swap a[1]=4, a[2]=3, pivot=4
2 3 4 12 56 15 89 748 48 45
swap end----

222 ready swap a[2]=4, a[2]=4, pivot=4
111 ready swap a[4]=56, a[9]=45, pivot=56
2 3 4 12 45 15 89 748 48 56
swap end----

222 ready swap a[6]=89, a[9]=56, pivot=56
111 ready swap a[6]=56, a[8]=48, pivot=56
2 3 4 12 45 15 48 748 56 89
swap end----

222 ready swap a[7]=748, a[8]=56, pivot=56
111 ready swap a[7]=56, a[7]=56, pivot=56
2 3 4 12 45 15 48 56 748 89
swap end----

222 ready swap a[7]=56, a[7]=56, pivot=56
111 ready swap a[4]=45, a[5]=15, pivot=45
2 3 4 12 15 45 48 56 748 89
swap end----

222 ready swap a[5]=45, a[5]=45, pivot=45
111 ready swap a[8]=748, a[9]=89, pivot=748
2 3 4 12 15 45 48 56 89 748
swap end----

222 ready swap a[9]=748, a[9]=748, pivot=748

after sort----
2 3 4 12 15 45 48 56 89 748
 */
int partion_1(int a[], int left, int right, int length) {
    int pivot = a[left];

    while (left < right) {
        while (left < right && a[right] >= pivot)
            right --;
        if (left == right) {
            std::cout << "111 left==right break" << std::endl;
            break;
        }
        std::cout << "111 ready swap a[" << left << "]=" << a[left]
                  << ", a[" << right << "]=" << a[right] << ", pivot=" << pivot << std::endl;
        if (left < right) {
            std::swap(a[left], a[right]);
//            a[left++] = a[right];//将比key小的元素移到低端
        }

//        std::cout              << "swap begin----" << std::endl;
        for (size_t i = 0; i != 10; ++i)
            std::cout << a[i] << " ";
//        std::cout << std::endl << "swap end----" << std::endl << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;

//        std::cout << "left=" << left << ", right=" << right << std::endl;
        while (left < right && a[left] < pivot)
            left ++;
        if (left == right) {
            std::cout << "222 left==right break" << std::endl;
            break;
        }
        std::cout << "222 ready swap a[" << left << "]=" << a[left]
                  << ", a[" << right << "]=" << a[right] << ", pivot=" << pivot << std::endl;
        if (left < right) {
            std::swap(a[left], a[right]);
//            a[right--] = a[left];//将比key大的元素移到高端
        }

        for (size_t i = 0; i != 10; ++i)
            std::cout << a[i] << " ";
//        std::cout << std::endl << "swap end----" << std::endl << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
    a[left] = pivot;
    return left;
}
//
int partion(int a[], int left, int right, int length) {
    int flag = a[left];
    int j = left;

    for (int i = left + 1; i <= right; ++ i) {

        if (a[i] < flag) {
            j ++;
            if (j != i) {
                std::swap(a[i], a[j]);
            }
        }

    }
    std::swap(a[j], a[left]);
    return j;
}
void insertSort( int arr[], int left, int right, int length ) {// 对子序列arr[low...high]进行插入排序
    int i, j, tmp;
    for (i = left+1; i < right; i++) {// 从下标low+1开始遍历,因为下标为low的已经排好序
        if (arr[i] < arr[i-1]) {
            // 如果当前下标对应的记录小于前一位记录,则需要插入,否则不需要插入，直接将记录数增加1
            tmp = arr[i]; // 记录下标i对应的元素
            for (j = i-1; j >= left && arr[j] > tmp; j--) {
                arr[j+1] = arr[j];// 记录后移
            }
            arr[j+1] = tmp;
        }
    }
}
void quickSort( int a[], int left, int right, int length ) {
    if (length > MAX_LENGTH_INSERT_SORT) {// 待排序数组长度大于临界值，则进行快速排序
        int pivotLoc; // 记录枢轴(pivot)所在位置
        if (left < right) {// 2. 优化小数组时的排序方案，将快速排序改为插入排序
//            pivotLoc = partion(a, left, right, length);// 将arr[low...high]一分为二,并返回枢轴位置
            pivotLoc = partion_1(a, left, right, length);// 将arr[low...high]一分为二,并返回枢轴位置
            std::cout << "pivotLoc=" << pivotLoc << std::endl;

            quickSort(a, left, pivotLoc/*pivotLoc - 1*/, length);// 递归遍历arr[low...pivotLoc-1]
            quickSort(a, pivotLoc + 1, right, length);// 递归遍历arr[pivotLoc+1...high]
        }
    } else {
//        insertSort(a, left, right, length);
    }

}
void tst_qs_111() {
    int arr[10] = {12, 45, 748, 15, 56, 3, 89, 4, 48, 2};
//    int a[10] = {2, 3, 4, 12, 15, 45, 48, 56, 89, 748};
//    int a[] = {5, 1, 9, 3, 7, 4, 8, 6, 2};
    int length = sizeof(arr)/sizeof(int);
    std::cout << "length=" << length << std::endl;
    std::cout << std::endl;

    std::cout << "before sort----" << std::endl;
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    quickSort(arr, 0, length - 1, length);

//    std::cout << "------------------" << std::endl;
    std::cout << std::endl << "after sort----" << std::endl;
    for (size_t i = 0; i != length; ++i) {
        std::cout << arr[i] << " ";
    }
    cout << std::endl;
    cout << std::endl;
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

int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

#ifdef __cplusplus//C++编译器定义了-cplusplus
std::cout << "c++" << std::endl;
#else
std::cout << "c" << std::endl;
#endif

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

    tst_qs_111();  return 1;
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













