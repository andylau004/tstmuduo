

#include "transferclient.h"




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



using namespace std;
using namespace muduo;
using namespace muduo::net;



#include "../gen-cpp/transfer_types.h"

#include "../gen-cpp/Photo.h"


#include "muduo/base/thrift_helper.h"
#include "muduo/base/thrift_connection_pool.h"


const uint32_t FLAGS_conn_max  = 10;
const uint32_t FLAGS_conn_maxperhost  = 10;
const uint32_t FLAGS_conn_timeout_ms  = 1 * 1000;
const uint32_t FLAGS_recv_timeout_ms  = 1 * 1000;
const uint32_t FLAGS_send_timeout_ms  = 1 * 1000;
const uint32_t FLAGS_idle_timeout_sec = 1 * 1000;
const uint32_t FLAGS_block_wait_ms    = 1 * 1000;


boost::shared_ptr< CountDownLatch>    g_pLatch = nullptr;



typedef ThriftConnectionPool<PhotoIf, PhotoClient> sendFile_ThriftConnectionPool;
boost::shared_ptr < sendFile_ThriftConnectionPool > g_clientPool_sendfile;

typedef  sendFile_ThriftConnectionPool::TThriftConnection  one_sendFile_ThriftConn;


extern int  clientAddImpl();


// 传输二进制文件给服务器(transferserver) 客户端程序
class PhotoHandler : virtual public PhotoIf {
public:
    PhotoHandler() {
        // Your initialization goes here
    }

    bool SendPhoto(const OneFile& onefile) {
        // Your implementation goes here
        printf("SendPhoto Recv Onefile\n");
        std::cout << "fileName=" << onefile.name << std::endl;
        std::cout << "file_size=" << onefile.file_size << std::endl;

        return true;
    }

};



int  clientAddImpl() {
    OutputDbgInfo tmpOut( /*strTid +*/ " clientAddImpl beg---------", /*strTid +*/ " clientAddImpl end---------" );

    uint32_t uSrvIp = ErasureUtils::str2ip("172.17.0.2");
    boost::shared_ptr<one_sendFile_ThriftConn> newClient = g_clientPool_sendfile->get_connection( uSrvIp, 9090 );

    {
        OneFile oneFile;
        const char* lpszFile = "./tstalgo";

        unsigned int ulFileSize = GetFileSize(lpszFile);
//        std::cout << "ulFileSize=" << ulFileSize << std::endl;

        oneFile.__set_name(lpszFile);
        oneFile.__set_file_buffer(GetFileContent_string(lpszFile));
        oneFile.__set_file_size(ulFileSize);
        oneFile.__set_file_hsh("12345");

//        bool bSend = oneConn->get_client()->SendPhoto(oneFile);
        int iSum = newClient->get_client()->Add(99, 10);
        LOG_INFO << "sum=" << iSum;
    }

    g_pLatch->countDown();
    return 1;
}
int  clientAddImpl_2( int ival ) {
//    LOG_INFO << "hehe";
//    g_pLatch->countDown();
//    return 1;

    UNUSED(ival);
    return clientAddImpl();
}
void tst_transfer_client_entry() {
    OutputDbgInfo tmpOut( "tst_transfer_client_entry begin", "tst_transfer_client_entry end" );

//    std::cout << std::endl;
//    std::cout << __FILE__ << ":" << __LINE__ << "  createContext" << std::endl;
//    return ;
//    std::cout << __FILE__ << ":" << __LINE__ << "  main threadid=" << muduo::CurrentThread::tid() << std::endl;

    const int max_thread_count = 3;
    g_pLatch.reset( new CountDownLatch(max_thread_count) );

    g_clientPool_sendfile.reset(new sendFile_ThriftConnectionPool(FLAGS_conn_max,
                                                                  FLAGS_conn_maxperhost,
                                                                  FLAGS_conn_timeout_ms,
                                                                  FLAGS_recv_timeout_ms,
                                                                  FLAGS_send_timeout_ms,
                                                                  FLAGS_idle_timeout_sec,
                                                                  FLAGS_block_wait_ms));
    muduo::ThreadPool  workpool;
    workpool.start(max_thread_count);

//    workpool.run(clientAddImpl);
    for (int i = 0; i < max_thread_count; ++i) {
        workpool.run( boost::bind(clientAddImpl_2, 12) );
    }
    g_pLatch->wait();
    std::cout << "all work Done!!!" << std::endl;
    return;


    boost::shared_ptr<TSocket> clientSock(new TSocket("127.0.0.1", 9090));

    //设置发送、接收、连接超时
    clientSock->setConnTimeout(2000 * 100000);
    clientSock->setRecvTimeout(2000 * 100000);
    clientSock->setSendTimeout(2000 * 100000);

/*
    // 对接nonblockingServer时必须的，
    // 对普通server端时用boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
//    boost::shared_ptr<TTransport> transport(new THttpClient(clientSock, "192.168.32.19", ""));
//    boost::shared_ptr<TTransport>  t(new TBufferedTransport(clientSock, 10240, 10240));
    boost::shared_ptr<TTransport>  transport(new TFramedTransport(clientSock, 10240, 10240));
//    boost::shared_ptr<TTransport>  transport(new TBufferedTransport(clientSock, 10240, 10240));
    boost::shared_ptr<THttpClient> http(new THttpClient(transport, "www.zzzyyy.com", "/"));
    //
    boost::shared_ptr<TProtocol>   send_prot(new TBinaryProtocol(http));
*/
    boost::shared_ptr<TTransport>  transport(new TFramedTransport(clientSock, 10240, 10240));
    boost::shared_ptr<TProtocol>   send_prot(new TBinaryProtocol(transport));
    transport->open();

    PhotoClient client(send_prot);

    OneFile oneFile;
    const char* lpszFile = "./tstalgo";

    unsigned int ulFileSize = GetFileSize(lpszFile);
    std::cout << "ulFileSize=" << ulFileSize << std::endl;
//    std::cout << "GetFileContent_string=" << GetFileContent_string(lpszFile) << std::endl;

    oneFile.__set_name(lpszFile);
    oneFile.__set_file_buffer(GetFileContent_string(lpszFile));
    oneFile.__set_file_size(ulFileSize);
    oneFile.__set_file_hsh("12345");

    bool bSend = client.SendPhoto(oneFile);

    std::cout << "bsend=" << bSend << std::endl;

    transport->close();

}


