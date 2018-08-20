

#include "stockfactory.h"

#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"


#include <boost/noncopyable.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

#include <memory>
#include <unordered_map>

#include <iostream>
#include <sstream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

using std::string;


#include "muduo/base/Logging.h"


#define REPRODUCE_BUG


void sleepMs(int ms)
{
    usleep(ms * 1000);
}

class Stock : boost::noncopyable
{
public:
    Stock(const string& name)
        : name_(name)
    {
        LOG_INFO << " cst stock=" << this;
    }

    ~Stock()
    {
        LOG_INFO << " dst stock=" << this;
    }

    const string& key() const { return name_; }

private:
    string name_;
};


class StockFactory : boost::noncopyable
{
public:

    std::shared_ptr<Stock> get(const string& key)
    {
        std::shared_ptr<Stock> pStock;

        muduo::MutexLockGuard lock(mutex_);
        std::weak_ptr<Stock>& wkStock = stocks_[key];
        pStock = wkStock.lock();
        if (!pStock)
        {
            LOG_INFO << "no found stock=" << key /*<< " mapsize=" << stocks_.size() << " firstkey=" << stocks_.begin()->first*/;
            pStock.reset(new Stock(key),
                         [this] (Stock* stock) { deleteStock(stock); });
            wkStock = pStock;

            LOG_INFO << "Create new stock, stock=" << pStock.get();
//            LOG_INFO << "get stock=" << pStock.get() << " usecout=" << pStock.use_count();
        }
        return pStock;
    }
private:
/*
    Race condition 发生在 StockFactory::deleteStock() 这个成员函数里，如果进入 deleteStock 之后，在 lock 之前，有别的线程调用了相同 key 的 StockFactory::get()，会造成此 key 被从 stocks_ 哈希表中错误地删除，因此会重复创建 Stock 对象。程序不会 crash 也不会有 memory leak，但是程序中存在两个相同 key 的 Stock 对象，违背了对象池应有的语意。下图描绘了 race condition 的发生过程。
*/
    void deleteStock(Stock* delStockObj)
    {
        LOG_INFO << " deleteStock in delObj stock=" << delStockObj << " key=" << delStockObj->key();
        if (delStockObj)
        {
            sleepMs(500);
            muduo::MutexLockGuard lock(mutex_);
#ifdef REPRODUCE_BUG
            LOG_INFO << " ************* delkey=" << delStockObj->key() << " mapsize=" << stocks_.size();
#else
            auto it = stocks_.find(delStockObj->key());
            assert(it != stocks_.end());
            if (it->second.expired()) {
                stocks_.erase(it);
            }
            else {
                LOG_INFO << delStockObj->key().c_str() << " is not expired";
            }
#endif
        }
        LOG_INFO << " mapsize=" << stocks_.size() << " firstkey=" << stocks_.begin()->first;
        delete delStockObj;  // sorry, I lied
    }

private:
    mutable muduo::MutexLock mutex_;
    std::unordered_map< string, std::weak_ptr<Stock> > stocks_;
};

void threadB(StockFactory* factory)
{
    sleepMs(250);
    auto stock = factory->get("MS");
    LOG_INFO << " stock=" << stock.get() << " usecount=" << stock.use_count();

    sleepMs(500);
    auto stock2 = factory->get("MS");
    LOG_INFO << " stock2=" << stock2.get() << " usecount=" << stock2.use_count();
    if (stock != stock2)
    {
        LOG_INFO << ("WARNING: stock != stock2");
    }
}

void tst_stock_f_1()
{
    LOG_INFO << "main tid=" << muduo::CurrentThread::tid() /*<< std::endl << std::endl*/;

    StockFactory factory;
    muduo::Thread thr( [&factory] { threadB(&factory); }, "thrB" );
    thr.start();

    LOG_INFO << ( "(((((((((((((((((((((((((((((" );
    {
        auto stock = factory.get("MS");
        LOG_INFO << ", stock=" << stock.get() << " usecout=" << stock.use_count();
    }
    LOG_INFO << "))))))))))))))))))))))))))))))";

    thr.join();
}


class CTestUse {
public:
    CTestUse(){
        m_val = 123;
        std::cout << "CTestUser cst" << std::endl;
    }
    virtual ~CTestUse() {
        m_val = -1;
        std::cout << "~CTestUser dst" << std::endl;
    }
    void print_val(){
        std::cout << "val=" << m_val << std::endl;
    }
private:
    int m_val;
};



void threadFunc(boost::weak_ptr<CTestUse> gpPtr) {
//    std::cout << "work thread=" << muduo::CurrentThread::tid() << std::endl;
    LOG_INFO << "threadFunc start";
    LOG_INFO << "usecount=" << gpPtr.use_count();

    if (gpPtr.expired()) {
        LOG_INFO << "Ptr is expired";
    } else {
        LOG_INFO << "Ptr is work";
    }
}


void tst_userId(OneUser& inUserBean) {
    LOG_INFO << "user name=" << typeid(inUserBean).name();
}
void tst_typeid(OneFile& inFile) {
    LOG_INFO << "file name=" << typeid(inFile).name();

    OneUser inUser;
    tst_userId(inUser);
}

void tst_wkPtr_use() {

    LOG_INFO << "wkPtr use start";

    boost::weak_ptr<CTestUse> wkPtr;
    {
        boost::shared_ptr<CTestUse> share_Ptr(new CTestUse);
        wkPtr = (share_Ptr);
    }
    boost::thread workThread( boost::bind(&threadFunc, wkPtr) );

    getchar();
}

