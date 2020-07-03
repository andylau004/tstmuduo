


#include "examples/tstprotobuf/rpc/sudoku.pb.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/protorpc/RpcChannel.h"
#include "muduo/net/protorpc/RpcServer.h"

#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

namespace sudoku {

class SudokuServiceImpl : public sudoku::SudokuService {

public:
    virtual void Solve(::google::protobuf::RpcController *controller,
                       const sudoku::SudokuRequest *request,
                       sudoku::SudokuResponse *response,
                       google::protobuf::Closure *done)
    {
        LOG_INFO << "SudokuServiceImpl::Solve";
        response->set_solved(true);
        response->set_checkerboard("1234567");
        done->Run();
    }

};

}  // namespace sudoku

int main() {
    LOG_INFO << "pid = " << getpid();
    EventLoop loop;
    InetAddress listenAddr(9981);
    sudoku::SudokuServiceImpl impl;

    RpcServer server(&loop, listenAddr);
    server.registerService(&impl);
    server.start();
    loop.loop();
    google::protobuf::ShutdownProtobufLibrary();
    return 1;
}
