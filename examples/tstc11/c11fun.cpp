

#include "c11fun.h"




#include "muduo/base/common.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>


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


#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TNonblockingServer.h>





using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;
//using namespace apache::thrift::async;



#include <cstdio>
#include <cstdlib>
#include <functional>


using namespace std;
using namespace muduo;
using namespace muduo::net;


#define __SCOPEGUARD_CONCATENATE_IMPL(s1, s2) s1##s2
#define __SCOPEGUARD_CONCATENATE(s1, s2) __SCOPEGUARD_CONCATENATE_IMPL(s1, s2)

#if defined(__cplusplus)

#include <type_traits>

// ScopeGuard for C++11
namespace clover {
    template <typename Fun>
    class ScopeGuard {
    public:
        ScopeGuard(Fun &&f) : _fun(std::forward<Fun>(f)), _active(true) {
        }

        ~ScopeGuard() {
            if (_active) {
                _fun();
            }
        }

        void dismiss() {
            _active = false;
        }

        ScopeGuard() = delete;
        ScopeGuard(const ScopeGuard &) = delete;
        ScopeGuard &operator=(const ScopeGuard &) = delete;

        ScopeGuard(ScopeGuard &&rhs) : _fun(std::move(rhs._fun)), _active(rhs._active) {
            rhs.dismiss();
        }

    private:
        Fun _fun;
        bool _active;
    };

    namespace detail {
        enum class ScopeGuardOnExit {};

        template <typename Fun>
        inline ScopeGuard<Fun> operator+(ScopeGuardOnExit, Fun &&fn) {
            return ScopeGuard<Fun>(std::forward<Fun>(fn));
        }
    } // namespace detail
} // namespace clover

// Helper macro
#define ON_SCOPE_EXIT \
    auto __SCOPEGUARD_CONCATENATE(ext_exitBlock_, __LINE__) = clover::detail::ScopeGuardOnExit() + [&]()

#else

// ScopeGuard for Objective-C
typedef void (^ext_cleanupBlock_t)(void);
static inline void ext_executeCleanupBlock(__strong ext_cleanupBlock_t *block) {
    (*block)();
}

#define ON_SCOPE_EXIT                                                              \
    __strong ext_cleanupBlock_t __SCOPEGUARD_CONCATENATE(ext_exitBlock_, __LINE__) \
        __attribute__((cleanup(ext_executeCleanupBlock), unused)) = ^

#endif

void test_cpp() {
    void *p = malloc(1000);
    printf ( "new memory addr=%p\n", p );
    ON_SCOPE_EXIT {
        printf ( "free memory addr=%p\n", p );
        free(p);
    };
}



class defer {
public:
    defer(const boost::function < void() >& deferFunc) :
        m_defer_func(deferFunc) {

    }
    ~defer() {
        m_defer_func();
//        m_defer_func = nullptr;
    }
    boost::function < void() > m_defer_func;
};


void tst_c11fun_entry() {
    OutputDbgInfo tmpOut( "tst_c11fun_entry begin", "tst_c11fun_entry end" );

    FILE* file1 = fopen( "file1.txt", "wb" );
    defer releaseFile = boost::function < void() >([&]()
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


