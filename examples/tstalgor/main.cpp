





#include <iostream>

#include <sstream>





#include "tst_1.h"

#include "codeconvert.h"


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


#include <boost/bind.hpp>

#include <stdio.h>



using namespace std;
using namespace muduo;
using namespace muduo::net;





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

int main(int argc, char *argv[])
{
    Logger::setLogLevel(Logger::DEBUG);
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

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

//int main()
//{
//    //    Logger::setLogLevel(Logger::DEBUG);
//    //    std::setlocale(LC_ALL, "en_US.utf8");

//    return 1;
//}













