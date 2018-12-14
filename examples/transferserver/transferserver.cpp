

#include "transferserver.h"


#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/shared_ptr.hpp>


#include "muduo/base/common.h"

#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include "muduo/net/TcpClient.h"


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>

#include <thrift/concurrency/PosixThreadFactory.h>
#include <thrift/concurrency/Monitor.h>



#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>

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



#include "transferserver.h"

#include "../gen-cpp/transfer_types.h"

#include "../gen-cpp/Photo.h"

#include "muduo/base/thrift_helper.h"
#include "muduo/base/event_watcher.h"

#include "ex_event_handler.h"


#include "server.hpp"



using namespace std;
using namespace muduo;
using namespace muduo::net;

using boost::shared_ptr;


static boost::shared_ptr<transport::TTransport> g_CurrentTransport = nullptr;


// 接受传输文件客户端(transferclient)上传的二进制文件服务器程序
class PhotoHandler : virtual public PhotoIf {
public:
    PhotoHandler() {
        // Your initialization goes here
    }

    virtual void login_im(LoginResult& _return, const LoginParam& param) {

    }
    virtual bool SendPhoto(const OneFile& onefile) {
        // Your implementation goes here
        printf("SendPhoto Recv Onefile\n");
//        std::cout << "fileName=" << onefile.name << std::endl;
        std::cout << "file_size=" << onefile.file_size << " file_size=" << onefile.file_hsh << std::endl;
//        std::cout << "file_size=" << onefile.file_hsh << std::endl;

        save_file("./tmpSaveUploadFile", (char*)onefile.file_buffer.data(), onefile.file_size);
        return true;
    }
    virtual int32_t Add(const int32_t add1, const int32_t add2) {
        return (add1 + add2);
//        return 1;
    }
    void SendEmptyRequest(const SendEmptyRequestReq& req) {

    }
};

class MyTask : public Runnable {
    public:
        MyTask(Monitor &monitor, int &count) : monitor_(monitor), count_(count) {
        }
        void run() {
//            printf( "work tid=%d\n", CurrentThread::tid() );
            Synchronized s(monitor_);
            count_++;
            printf( "work tid=%d count_=%d\n", CurrentThread::tid(), count_ );
            if (count_ == 10) {
                monitor_.notify();
            }
        }
    private:
        int &count_;
        Monitor& monitor_;
};

void tst_thrift_threadmanager_fun() {
    int num_threads = 2;

    boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(num_threads);
    boost::shared_ptr<PosixThreadFactory> threadFactory(new PosixThreadFactory());
    threadManager->threadFactory(threadFactory);
    threadManager->start();

    Monitor monitor;
    int m = 0;

    std::cout << "11111111111111111" << std::endl;
    std::set < boost::shared_ptr<MyTask> > tasks;
    for (int i = 0; i < 20; i++) {
        tasks.insert( boost::shared_ptr<MyTask>(new MyTask(monitor, m)));
    }
    std::cout << "222222222222222" << std::endl;
    for ( auto it = tasks.begin(); it != tasks.end(); ++it ) {
        threadManager->add(*it, 50, 50);
    }
    std::cout << "3333333" << std::endl;
    {
        Synchronized s(monitor);
        monitor.wait();
    }
    std::cout << "4444444" << std::endl;

    threadManager->join();
    threadManager->stop();

}

class ClientIPHandler : virtual public TServerEventHandler {
public:
    ClientIPHandler() {
    }
    virtual ~ClientIPHandler() {
    }
    std::string GetThriftClientIp() {
//        lock::MutexLock g(&mutex);
//        return thrift_client_ip[pthread_self()];
        return "";
    }

    virtual void preServe() {
//        stdcxx::shared_ptr<int> pre;
        std::cout << " call preServe " << std::endl;
    }
    virtual void* createContext(boost::shared_ptr<TProtocol> input,
                                boost::shared_ptr<TProtocol> output) {
        UNUSED(output);
        std::cout << " call createContext " << std::endl;
        TFramedTransport* pframetrans = dynamic_cast<TFramedTransport *>(input->getTransport().get());
        if ( !pframetrans ) {
            std::cout << " pframetrans == null" << std::endl;
        } else {
            std::cout << " pframetrans != null" << std::endl;
            TSocket *sock = dynamic_cast<TSocket *>(pframetrans->getUnderlyingTransport().get());
            std::cout << " ip=" << sock->getPeerAddress() << std::endl;
        }
        {
            // insert when connection open
            TBufferedTransport* pbufftrans = dynamic_cast<TBufferedTransport *>(input->getTransport().get());
            if ( !pbufftrans ) {
                std::cout << " pbufftrans == null" << std::endl;
            } else {
                std::cout << " pbufftrans != null" << std::endl;
//                TSocket *sock = dynamic_cast<TSocket *>(tbuf->getUnderlyingTransport().get());
//                std::cout << " ip=" << sock->getPeerAddress() << std::endl;
            }
//        TSocket *sock = dynamic_cast<TSocket *>(tbuf->getUnderlyingTransport().get());
//        lock::MutexLock g(&mutex);
//        thrift_client_ip[pthread_self()] = sock->getPeerAddress();

        }
        return NULL;
    }
    virtual void deleteContext(void* serverContext,
                               boost::shared_ptr<TProtocol>input,
                               boost::shared_ptr<TProtocol>output) {
        std::cout << " call deleteContext " << std::endl;
        // erase when connection close
//        lock::MutexLock g(&mutex);
//        thrift_client_ip.erase(pthread_self());
    }
    virtual void processContext(void* serverContext, boost::shared_ptr<TTransport> transport) {
        std::cout << " call processContext " << std::endl;

        TSocket *tsocket = static_cast<TSocket*>(transport.get());
        if(socket){
            struct sockaddr* addrPtr;
            socklen_t addrLen;
            addrPtr = tsocket->getCachedAddress(&addrLen);
            if (addrPtr){
                getnameinfo((sockaddr*)addrPtr,addrLen,(char*)serverContext,32,nullptr,0,0);
                std::cout << "111111111---------------------------------------" << std::endl;
                std::cout << "serverContext=" << serverContext << std::endl;
                std::cout << "111111111---------------------------------------" << std::endl;
            }// test1

            {
                TSocket *sock = static_cast<TSocket *>(transport.get());
                if (sock)
                {
                    std::cout << "222222222---------------------------------------" << std::endl;
                    std::cout << "getPeerAddress=" << sock->getPeerAddress() << std::endl;
                    std::cout << "getSocketInfo=" << sock->getSocketInfo() << std::endl;
                    std::cout << "222222222---------------------------------------" << std::endl;
                }
            }// test2

        }

    }

private:
};


typedef struct _tag_KV {
    int64_t key;
    char value[8];
}st_kv;


struct demo
{
    int Add(int a,int b) {
        return a+b;
    }
    int operator()(int a,int b) {
        return a+b;
    }
};


//int g_reads, g_writes, g_fired;
//int g_total_reads = 0;

void ReadCallback(int idx) {
//    g_total_reads++;
//    g_reads++;
//    if (g_writes > 0) {
//        int widx = idx + 1;
//        if (widx >= numPipes) {
//            widx -= numPipes;
//        }
//        g_pipes[widx]->Notify();
//        g_writes--;
//        g_fired++;
//    }

//    if (g_fired == g_reads) {
//        g_loop->Stop();
//    }
}
//void tt1() {
//    auto f = std::bind(&ReadCallback, _1);
//    PipeEventWatcherPtr w(new PipeEventWatcher(nullptr, f));
//    w->Init();
//}

void disorderfun() {
    //    demo a;
    //    demo* pa = &a;
    //    typedef boost::function<void()> Handler;
    //    int iret = 0;

    //    Handler funptr = (boost::bind(&demo::Add, pa, 2, 1));
    //    std::cout << "add1=" << boost::bind( &demo::Add, pa, 10, _2 )( 12, 23 ) << std::endl;

    //    typedef boost::function<int(int, int)> HandlerEx;
    //    HandlerEx funcEx = (boost::bind(&demo::Add, pa, 2, 1));
    //    std::cout << "addex=" << funcEx( 12, 23 ) << std::endl;

    ////    Handler testPtr = (boost::bind(&demo::Add, pa, _1, _2));
    ////    std::cout << "add_aaa=" << testPtr(3, 5) << std::endl;

    ////    std::cout << "add=" << funptr() << std::endl;
    ////    int iret = funptr();
    //    std::cout << "add2=" << iret << std::endl;
    //    return;

    //    tst_thrift_threadmanager_fun();
    //    return ;

    //    setEventHandler
    //    CThriftServerHelper<PhotoHandler, PhotoProcessor> thrift_server_agent((new ClientIPHandler), false);
}

void my_thrift_srv() {
    printf( "my_thrift_srv beg-----------------------\n" );
    Server * s = new Server(P_TEST_PROCESS, PRO_BINARY, 9900, 2);
    if (!s->serve()) {
        printf("bad server\n");
        delete_object(s);
        return;
    }
    printf( "my_thrift_srv end-----------------------\n" );
}
void tst_transfer_server_entry() {
//    std::cout << "sizeof (st_kv)=" << sizeof (st_kv) << std::endl;
//    return ;
//    tt1(); return ;

    OutputDbgInfo tmpOut( "tst_transfer_server_entry begin", "tst_transfer_server_entry end" ) ;
    my_thrift_srv();
    return;

    boost::shared_ptr<thriftex::server::ExProcessorEventHandler>
            pProcessEventHandler(new thriftex::server::ExProcessorEventHandler);

    CThriftServerHelper<PhotoHandler, PhotoProcessor>
            thrift_server_agent((new thriftex::server::ExServerEventHandler), false, pProcessEventHandler);

    uint16_t srv_port = 9090;
    thrift_server_agent.serve(srv_port);
    return;

//    boost::shared_ptr<PhotoHandler>     handler(new PhotoHandler());
//    boost::shared_ptr<TProcessor>       processor(new PhotoProcessor(handler));
//    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

//    boost::shared_ptr<ThreadManager>      threadManager = ThreadManager::newSimpleThreadManager(4);
//    boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory> (new PosixThreadFactory()); //PosixThreadFactory可以自定义（继承于ThreadFactory）

//    int nStackSize = threadFactory->getStackSize();
//    printf("tst_transfer_server_entry stack size=%d\n", nStackSize); // default value is 1

//    threadManager->threadFactory(threadFactory);
//    threadManager->start();

//    TNonblockingServer server(processor, protocolFactory, srv_port, threadManager);
//    printf("BinaryServer beg\n");
//    try {
//        server.serve();
//    }
//    catch(TException e) {
//        printf("Server.serve() failed\n");
//        exit(-1);
//    }
//    printf("BinaryServer end\n");

}


