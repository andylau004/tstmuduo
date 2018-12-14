

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



int  clientAddImpl() {
    OutputDbgInfo tmpOut(" clientAddImpl beg---------", /*strTid +*/ " clientAddImpl end---------" );

    uint32_t uSrvIp = ErasureUtils::str2ip("172.17.0.2");
    boost::shared_ptr<one_sendFile_ThriftConn> newClient = g_clientPool_sendfile->get_connection( uSrvIp, 9900 );

    {
        OneFile oneFile;
        const char* lpszFile = "./tstalgo";

        unsigned int ulFileSize = GetFileSize(lpszFile);
//        std::cout << "ulFileSize=" << ulFileSize << std::endl;

        oneFile.__set_name(lpszFile);
        oneFile.__set_file_buffer(GetFileContent_string(lpszFile));
        oneFile.__set_file_size(ulFileSize);
        oneFile.__set_file_hsh("12345");

        bool bSend = newClient->get_client()->SendPhoto(oneFile);
        getchar();
//        int iSum = newClient->get_client()->Add(99, 10);
//        LOG_INFO << "sum=" << iSum;
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

// 测试单个客户端登录功能
void tst_single_client_fun() {
    boost::shared_ptr<TSocket> clientSock(new TSocket("172.17.0.2", 9900));

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
    const char* lpszFile = "./CMakeCache.txt";

    unsigned int ulFileSize = GetFileSize(lpszFile);
    LOG_INFO << "ulFileSize=" << ulFileSize;
//    std::cout << "GetFileContent_string=" << GetFileContent_string(lpszFile) << std::endl;

    oneFile.__set_name(lpszFile);
    oneFile.__set_file_buffer(GetFileContent_string(lpszFile));
    oneFile.__set_file_size(ulFileSize);
    oneFile.__set_file_hsh("12345");

    bool bSend = client.SendPhoto(oneFile);
    LOG_INFO << "bsend=" << bSend;

    int addret = client.Add(444, 777);
    LOG_INFO << "addret=" << addret;

    for ( int i = 0 ; i < 3; ++i ) {
        LoginParam inparam_login;
        LoginResult ret_login;

        client.login_im(ret_login, inparam_login);
        LOG_INFO << "logcount=" << i+1 << ", ret_login sessionID=" << ret_login.sessionID;
    }
//    client.send_SendEmptyRequest();

    SendEmptyRequestReq reqEmptyRequest;
    reqEmptyRequest.__set_sessionID( 3 );

    LOG_INFO << "before SendEmptyRequest";
    client.SendEmptyRequest(reqEmptyRequest);
    printXto_console("send kEmptyRequest args=", reqEmptyRequest);
    LOG_INFO << "after SendEmptyRequest";

//    getchar();
    transport->close();
}

void tst_transfer_client_entry() {
    OutputDbgInfo tmpOut( "tst_transfer_client_entry begin", "tst_transfer_client_entry end" );

    tst_single_client_fun();


//    std::cout << std::endl;
//    std::cout << __FILE__ << ":" << __LINE__ << "  createContext" << std::endl;
//    return ;
//    std::cout << __FILE__ << ":" << __LINE__ << "  main threadid=" << muduo::CurrentThread::tid() << std::endl;

//    const int max_thread_count = 1;
//    g_pLatch.reset( new CountDownLatch(max_thread_count) );

//    g_clientPool_sendfile.reset(new sendFile_ThriftConnectionPool(FLAGS_conn_max,
//                                                                  FLAGS_conn_maxperhost,
//                                                                  FLAGS_conn_timeout_ms,
//                                                                  FLAGS_recv_timeout_ms,
//                                                                  FLAGS_send_timeout_ms,
//                                                                  FLAGS_idle_timeout_sec,
//                                                                  FLAGS_block_wait_ms));
//    muduo::ThreadPool  workpool;
//    workpool.start(max_thread_count);

////    workpool.run(clientAddImpl);
//    for (int i = 0; i < max_thread_count; ++i) {
//        workpool.run( boost::bind(clientAddImpl_2, 12) );
//    }
//    g_pLatch->wait();
//    std::cout << "all work Done!!!" << std::endl;
//    return;

}


