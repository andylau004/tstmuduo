

#include "boostany.h"

#include <iostream>       // std::cout
#include <atomic>         // std::atomic
#include <thread>         // std::thread, std::this_thread::yield

#include <iostream>
#include <string>
#include <thread>
#include <mutex>

#include <cstdio>
#include <cstdlib>
#include <functional>

#include <boost/circular_buffer.hpp>

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
#include <boost/format.hpp>

#include "muduo/base/common.h"
#include "muduo/base/Logging.h"


using namespace std;
using namespace muduo;

using namespace boost;


using BoostListAny = std::list<boost::any>;

#define PRINT(xxx) ( std::cout  << boost::format("%-20s = ") % (#xxx) << (xxx) << std::endl )
//#define PRINTEX(xxx) ( LOG_INFO << boost::format("%-20s = ") % (#xxx) << (xxx) )


//检测一个any是否能转换成某种类型，RTTI
template <typename T>
bool can_cast_to(const boost::any& param) {
    return typeid(T) == param.type();
}

void show_boost_any(BoostListAny& listAny) {

    BoostListAny::iterator it;
    boost::any anyone;
    for (it = listAny.begin(); it != listAny.end(); it ++) {
        anyone = *it;

        if (anyone.type() == typeid(int)) {
            LOG_INFO << "int val=" << boost::any_cast<int>(anyone);
        }/* else if (anyone.type() == typeid(std::string)) {
            LOG_INFO << "string val=" << boost::any_cast<std::string>(anyone);
        } */else {
            LOG_INFO << "unknown val=" << boost::any_cast<std::string>(anyone).c_str();
        }
    }

}

void tst_boost_any() {
    puts("base usage...");

    boost::any a(1);
    PRINT(boost::any_cast<int>(a));
    PRINT(a.type().name());
    return;

    BoostListAny blistAny;
    blistAny.push_back(10);
    blistAny.push_back("greate grandma ...");
    show_boost_any(blistAny);
}



