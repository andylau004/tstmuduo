





#include <iostream>
#include <sstream>
#include <stdio.h>

#include "tst_1.h"

#include "codeconvert.h"

#include "muduo/base/Logging.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/TcpServer.h"
#include "muduo/net/TcpClient.h"

#include <boost/bind.hpp>


#include <muduo/base/TimeZone.h>
#include <muduo/net/EventLoop.h>

#include <cthrift/cthrift_svr.h>
#include "Echo.h"

using namespace std;

using namespace echo;
using namespace muduo;
using namespace muduo::net;
using namespace meituan_cthrift;



//重载处理handler类
class EchoHandler : virtual public EchoIf {
public:
    EchoHandler() {
    }

    //业务真正的处理业务逻辑
    void echo(std::string &str_ret, const std::string &str_req) {
        printf("recv echo call, req=%s\n", str_req.c_str());
        str_ret.assign(str_req);
    }
};


int main(int argc, char *argv[])
{
    muduo::Logger::setLogLevel(muduo::Logger::INFO);
    LOG_INFO << "pid = " << getpid() << ", tid = " << muduo::CurrentThread::tid();

    muduo::string name("EchoServer");

    //初试化处理handler
    boost::shared_ptr<EchoHandler> handler(new EchoHandler());
    boost::shared_ptr<TProcessor> processor(new EchoProcessor(handler));

    try {
        CthriftSvr server(processor);

        //server初始化
        if (server.Init() != 0) {
            std::cout << "server init error" << endl;
            return -1;
        }

        //server对外服务，服务会陷入该函数，直到服务退出
        server.serve();

        //服务退出之后执行清理工作
        server.stop();

    } catch (TException &tx) {
        std::cout << tx.what() << endl;
        return -1;
    }

    return 1;
}



