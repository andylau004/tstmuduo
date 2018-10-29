//#include "thriftex/server/ex_event_handler.h"
//#include "thriftex/base/logging.h"
#include "ex_event_handler.h"
//#include "thriftex/base/logging.h"

#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <sys/time.h>
#include <iostream>


#include "muduo/base/common.h"
#include "muduo/base/CurrentThread.h"



namespace thriftex {
namespace server {

using apache::thrift::transport::TSocket;
using apache::thrift::transport::TMemoryBuffer;
using apache::thrift::transport::TFramedTransport;

/**
 * Connection info
 */
struct ServerContext {
    std::string remote_host;
    std::string remote_address;
    int port;
};

/**
 * Process info
 */
struct ProcessContext {
    ServerContext *server_context;
    ::timeval timeval;
};

ExServerEventHandler::ExServerEventHandler()
{}

ExServerEventHandler::~ExServerEventHandler()
{}

void *
ExServerEventHandler::createContext(boost::shared_ptr<TProtocol> input,
                                    boost::shared_ptr<TProtocol> output) {
    ServerContext *context = new ServerContext();
    printf( "                                 tid=%d  createContext context=%p\n", CurTid(), context );
    printf( "                                 tid=%d  input=%p output=%p\n", CurTid(), input.get(), output.get() );

//    TBufferedTransport *tbuf = dynamic_cast<TBufferedTransport *>(input->getTransport().get());
//    TSocket *sock = dynamic_cast<TSocket*>(tbuf->getUnderlyingTransport().get());

    return context;
}

void
ExServerEventHandler::processContext(void* serverContext,
                                     boost::shared_ptr<TTransport> transport) {
    ServerContext *context = static_cast<ServerContext *>(serverContext);
    printf( "                                 tid=%d  processContext context=%p\n", CurTid(), context );

    TSocket *tSocket = static_cast<TSocket *>(transport.get());
    if (context != nullptr && tSocket != nullptr) {
        tSocket->getPeerHost();
        context->remote_host = tSocket->getPeerHost();
        context->remote_address = tSocket->getPeerAddress();
        context->port = tSocket->getPeerPort();

        printf( "                                 tid=%d  processContext beg\n", CurTid() );
        printf( "                                 tid=%d  remote_address=%s remote_port=%d\n",
                CurTid(), context->remote_address.c_str(), context->port );
        printf( "                                 tid=%d  processContext end\n", CurTid() );

    } else {
//        EX_ERROR << "Invalid server context or tSocket!\n";
        printf( "Invalid server context or tSocket!\n" );
    }
}


void
ExServerEventHandler::deleteContext(void* serverContext,
                                    boost::shared_ptr<TProtocol> input,
                                    boost::shared_ptr<TProtocol> output) {
    ServerContext *context = static_cast<ServerContext *>(serverContext);
//    std::cout << "deleteContext context=" << context << std::endl;
    printf( "                                 tid=%d  deleteContext context=%p\n", CurTid(), context );
    delete context;
}


ExProcessorEventHandler::ExProcessorEventHandler()
{}

ExProcessorEventHandler::~ExProcessorEventHandler()
{}

void *
ExProcessorEventHandler::getContext(const char* fn_name, void *serverContext) {
    ProcessContext *process_context = new ProcessContext();

    if (gettimeofday(&process_context->timeval, nullptr) != 0) {
        delete process_context;
        process_context = nullptr;
    } else {
        process_context->server_context = static_cast<ServerContext *>(serverContext);
    }

    return process_context;
}

void
ExProcessorEventHandler::freeContext(void *ctx, const char * fn_name) {
    delete static_cast<ProcessContext *>(ctx);
    ctx = nullptr;
}

void
ExProcessorEventHandler::postWrite(void *ctx, const char *fn_name, uint32_t bytes) {
    ::timeval timeval_current;
    ::timeval &timeval_before_ptr =
            static_cast<ProcessContext *>(ctx)->timeval;

    ServerContext *server_context =
            static_cast<ProcessContext *>(ctx)->server_context;

    if (gettimeofday(&timeval_current, nullptr) == 0) {
        int ms = 0;
        ms =  (timeval_current.tv_sec - timeval_before_ptr.tv_sec) * 1000;
        ms += (timeval_current.tv_usec - timeval_before_ptr.tv_usec) / 1000;

        if (ms >= this->SLOW_TIME) {
            std::cout << server_context->remote_host << "("
                    << server_context->remote_address << ":" << server_context->port
                    << ") " << fn_name << ".SLOW " << ms << "ms cost\n";
        } else {
            std::cout << server_context->remote_host << "("
                          << server_context->remote_address << ":"
                          << server_context->port << ") " << fn_name << " "
                          << ms << "ms cost\n";
        }
    }
}

}}
