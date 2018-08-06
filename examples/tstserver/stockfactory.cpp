



#include "muduo/base/Mutex.h"
#include "muduo/base/Thread.h"


#include <boost/noncopyable.hpp>

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
//        printf("curtid=%d Stock[%p] %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
        LOG_INFO << " cst stock=" << this;
    }

    ~Stock()
    {
//        printf("curtid=%d ~Stock[%p] %s\n", muduo::CurrentThread::tid(), this, name_.c_str());
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
            pStock.reset(new Stock(key),
                         [this] (Stock* stock) { deleteStock(stock); });
            wkStock = pStock;

            LOG_INFO << "get stock=" << pStock.get() << " usecout=" << pStock.use_count();
        }
        return pStock;
    }
private:
/*
    Race condition 发生在 StockFactory::deleteStock() 这个成员函数里，如果进入 deleteStock 之后，在 lock 之前，有别的线程调用了相同 key 的 StockFactory::get()，会造成此 key 被从 stocks_ 哈希表中错误地删除，因此会重复创建 Stock 对象。程序不会 crash 也不会有 memory leak，但是程序中存在两个相同 key 的 Stock 对象，违背了对象池应有的语意。下图描绘了 race condition 的发生过程。
*/
    void deleteStock(Stock* delStockObj)
    {
        LOG_INFO << " deleteStock in delObj stock=" << delStockObj;
        if (delStockObj)
        {
            sleepMs(500);
            muduo::MutexLockGuard lock(mutex_);
#ifdef REPRODUCE_BUG
//            printf("tid=%d erase %zd\n", muduo::CurrentThread::tid(), stocks_.erase(stock->key()));
            LOG_INFO << " ************* delkey=" << delStockObj->key();
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
        delete delStockObj;  // sorry, I lied
    }

    mutable muduo::MutexLock mutex_;
    std::unordered_map<string, std::weak_ptr<Stock>> stocks_;
};

void threadB(StockFactory* factory)
{
    sleepMs(250);
    auto stock = factory->get("MS");
//    printf("tid=%d stock=%p usecout=%d\n", muduo::CurrentThread::tid(), stock.get(), stock.use_count());
    LOG_INFO << " stock=" << stock.get() << " usecount=" << stock.use_count();

    sleepMs(500);
    auto stock2 = factory->get("MS");
//    printf("tid=%d stock2=%p usecout=%d\n", muduo::CurrentThread::tid(), stock2.get(), stock2.use_count());
    LOG_INFO << " stock2=" << stock2.get() << " usecount=" << stock2.use_count();
    if (stock != stock2)
    {
        LOG_INFO << ("WARNING: stock != stock2");
    }
}

void tst_stock_f_1()
{
    LOG_INFO << "main tid=" << muduo::CurrentThread::tid() /*<< std::endl << std::endl*/;
//    printf( "main tid=%d\n", muduo::CurrentThread::tid() );

    StockFactory factory;
    muduo::Thread thr( [&factory] { threadB(&factory); }, "thrB" );
    thr.start();

    LOG_INFO << ( "(((((((((((((((((((((((((((((" );
    {
        auto stock = factory.get("MS");
//        printf("tid=%d stock=%p\n", muduo::CurrentThread::tid(), stock.get());
        LOG_INFO << ", stock=" << stock.get() << " usecout=" << stock.use_count();
    }
    LOG_INFO << "))))))))))))))))))))))))))))))";

    thr.join();
}


