


#include "tstbucketwheel.h"


#include <stdio.h>
#include <unistd.h>
#include <sys/poll.h>


#include <list>


#include <fcntl.h>
#include <arpa/inet.h>
#include <assert.h>

#include <unordered_set>
#include <boost/circular_buffer.hpp>


#include "muduo/net/Callbacks.h"
#include "muduo/base/common.h"
#include "muduo/base/Logging.h"

#include "muduo/net/EventLoop.h"
#include "muduo/base/Timestamp.h"

#include "muduo/base/libevent.h"

#include "muduo/net/TcpServer.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>


using namespace std;
using namespace muduo;
using namespace muduo::net;


using  WeakTcpConnectionPtr = boost::weak_ptr<muduo::net::TcpConnection>;

// RFC 862
class EchoServerEx
{
public:
    EchoServerEx(muduo::net::EventLoop* loop, const muduo::net::InetAddress& listenAddr, int idleSeconds);
    void start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);

    void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);

    void onTimer();

    void dumpConnectionBuckets();

    struct Entry : public muduo::copyable
    {
        // 初始化时候获得muduo::net::TcpConnection类型对象的弱指针
        explicit Entry(const WeakTcpConnectionPtr& weakConn) : weakConn_(weakConn) {
        }
        ~Entry() {// 析构时候将弱指针提升为强指针,提升成功则关闭写端
            OutputDbgInfo tmp("--------------Entry Obj Destroying beg--------------",
                              "--------------Entry Obj Destroying beg--------------");
            muduo::net::TcpConnectionPtr conn = weakConn_.lock();
            if (conn) {
                conn->shutdown();
                LOG_INFO << "--------------shutdowning--------------";
            }
        }
        WeakTcpConnectionPtr weakConn_;
    };
    typedef std/*boost*/::shared_ptr<Entry> EntryPtr;
    typedef std/*boost*/::weak_ptr<Entry>   WeakEntryPtr;

    typedef std::unordered_set<EntryPtr>   Bucket;
    typedef boost::circular_buffer<Bucket> WeakConnectionList;

    muduo::net::TcpServer server_;
    WeakConnectionList connectionBuckets_;// 存放连接的强指针
    // connectionBuckets是个循环队列，当往队尾插入一个元素空的Bucket时候，
    // 队头就会被删掉，此时队头的Bucket中每个EntryPtr的引用计数均会-1的，当该EntryPtr的引用计数值为0时候就会调用Entry对象的析构函数
};

EchoServerEx::EchoServerEx(EventLoop* loop, const InetAddress& listenAddr, int idleSeconds)
    : server_(loop, listenAddr, "EchoServer"), connectionBuckets_(idleSeconds)
{
    server_.setConnectionCallback(boost::bind(&EchoServerEx::onConnection, this, _1));
    server_.setMessageCallback(boost::bind(&EchoServerEx::onMessage, this, _1, _2, _3));
    loop->runEvery(1.0, boost::bind(&EchoServerEx::onTimer, this));
    connectionBuckets_.resize(idleSeconds);
    dumpConnectionBuckets();
}

void EchoServerEx::start() { server_.start(); }

void EchoServerEx::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << "EchoServerEx - " << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected()) {
        // 当客户端连接上时候，客户的conn为TcpConnectionPtr；
        EntryPtr entry(new Entry(conn)); // 强引用计数+1
        connectionBuckets_.back().insert(entry);// 将该强引用指针放入Bucket的末尾的set中

        dumpConnectionBuckets();

        WeakEntryPtr weakEntry(entry); //将弱指针保存在conn中,如果将强指针存于conn中则永远删不掉
        conn->setContext(weakEntry);
    } else {
        assert(!conn->getContext().empty());
        WeakEntryPtr weakEntry(boost::any_cast<WeakEntryPtr>(conn->getContext()));
        LOG_DEBUG << "Entry use_count = " << weakEntry.use_count();
    }
}
void EchoServerEx::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
    string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes at " << time.toString();
    conn->send(msg);

    assert(!conn->getContext().empty());
    WeakEntryPtr weakEntry(boost::any_cast<WeakEntryPtr>(conn->getContext()));
    EntryPtr entry(weakEntry.lock());
    if (entry)
    {
        connectionBuckets_.back().insert(entry);
        dumpConnectionBuckets();
    }
}
void EchoServerEx::onTimer()
{
    // 这样会将队列的头部Bucket给弹出去的，即该Bucket中的每一个强引用计数-1，
    // 当减为0时候即调用~Entry()，这里面可以直接conn->shutdown();或者conn->handleClose();
    connectionBuckets_.push_back(Bucket());
    dumpConnectionBuckets();
}

void EchoServerEx::dumpConnectionBuckets()
{
    LOG_INFO << "";
    LOG_INFO << "dump Connection Buckets beg";
    LOG_INFO << "size = " << connectionBuckets_.size();
    int idx = 0;
    for (WeakConnectionList::const_iterator bucketI = connectionBuckets_.begin();
         bucketI != connectionBuckets_.end();
         ++bucketI, ++idx)
    {
        const Bucket& bucket = *bucketI;
        printf("[%d] len = %zd : ", idx, bucket.size());
        for (const auto& it : bucket)
        {
            bool connectionDead = it->weakConn_.expired();
            printf("%p(%ld)%s, ", boost::get_pointer(it), it.use_count(), connectionDead ? " DEAD" : "");
        }
//        puts("");
        printf("\n");
    }
    LOG_INFO << "dump Connection Buckets end";
    LOG_INFO << "";
}


class CTxxx {
public:
    CTxxx() {
        printf( "CTxxx cst\n" );
    }
    ~CTxxx() {
        printf( "CTxxx dst\n" );
    }

};
void tst_wk_shared_fun() {
    boost::shared_ptr<CTxxx> sp_ct(new CTxxx);
    boost::weak_ptr<CTxxx> wk_ct = sp_ct;

    boost::weak_ptr<CTxxx> wka1;
    {
        std::cout << "wk_ct.expired()=" << wk_ct.expired() << std::endl;

        boost::shared_ptr<CTxxx> tmpP = wk_ct.lock();
        if (tmpP) {
            std::cout << "tmpP usecount=" << tmpP.use_count() << std::endl;
        } else {
            std::cout << "tmpP invalid" << std::endl;
        }

        boost::shared_ptr<CTxxx> a1(new CTxxx);
        wka1 = (a1);
    }

    std::cout << "wka1.expired()=" << wka1.expired() << std::endl;
    std::cout << "wka1.lock()=" << wka1.lock() << std::endl;
    boost::shared_ptr<CTxxx> cpySp = wka1.lock();
    if (cpySp)
        std::cout << "cpySp is ok" << std::endl;
    else
        std::cout << "cpySp is destroyed" << std::endl;
}
int tst_bucket_wheel_entry(int argc, char *argv[]) {
    EventLoop loop;

    InetAddress listenAddr(2007);
    int idleSeconds = 10;
    if (argc > 1) {
        idleSeconds = atoi(argv[1]);
    }

    EchoServerEx server(&loop, listenAddr, idleSeconds);
    server.start();
    loop.loop();
    return 0;
}



