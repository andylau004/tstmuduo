

#include "transferclient.h"



//#include <stdio.h>

//#include <iostream>
//#include <sstream>

//#include <iostream>
//#include <string>
//#include <algorithm>
//#include <stack>
//#include <vector>
//#include <queue>

//#include <fstream>
//#include <thread>

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



#include "gen-cpp/transfer_types.h"

#include "gen-cpp/Photo.h"


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

void tst_transfer_client_entry() {
    OutputDbgInfo tmpOut( "tst_transfer_client_entry begin", "tst_transfer_client_entry end" );

//    std::cout << std::endl;
//    std::cout << __FILE__ << ":" << __LINE__ << "  createContext" << std::endl;
//    return ;

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


