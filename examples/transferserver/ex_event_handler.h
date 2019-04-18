

#pragma once


#include <time.h>
#include <thrift/TProcessor.h>
#include <thrift/server/TServer.h>

namespace thriftex {
namespace server {

using apache::thrift::protocol::TProtocol;
using apache::thrift::transport::TTransport;

/**
 * 不修改thrift源码，使用注入方式获取thrift客户端IP
 * Work in connection context
 */
class ExServerEventHandler: public apache::thrift::server::TServerEventHandler
{
public:
    ExServerEventHandler();
    virtual ~ExServerEventHandler();

public:
    /**
     * Generate inner struct to hold info about one connection
     *
     * @param input Input protocol
     * @param output Output protocol
     *
     * @return
     */
    virtual void* createContext(boost::shared_ptr<TProtocol> input,
                                boost::shared_ptr<TProtocol> output);

    /**
     * Get remote host / address / port
     *
     * @param serverContext Generate by #ExServerEventHandler::createContext
     * @param transport Transport underlying
     */
    virtual void processContext(void* serverContext,
                                boost::shared_ptr<TTransport> transport);

    /**
     * Free the inner struct generate by #ExServerEventHandler::createContext
     *
     * @param serverContext
     * @param input
     * @param output
     */
    virtual void deleteContext(void* serverContext,
                               boost::shared_ptr<TProtocol> input,
                               boost::shared_ptr<TProtocol> output);
};

/**
 * Work in process(...) context
 */
class ExProcessorEventHandler: public apache::thrift::TProcessorEventHandler
{
public:
    ExProcessorEventHandler();
    virtual ~ExProcessorEventHandler();

public:
    /**
     * Generate inner struct to hold info about one RPC call
     * 调用其他回调函数之前调用，期望返回一些有序的上下文对象以便传递给其他回调函数使用
     * @param fn_name Function name of RPC call
     * @param serverContext Generate by ExServerEventHandler
     */
    virtual void* getContext(const char* fn_name, void* serverContext);

    /**
     * Free the struct generate above
     * 期望释放一个上下文有关的资源
     * @param ctx Generate by #ExProcessorEventHandler::getContext
     * @param fn_name Function name of RPC call
     */
    virtual void freeContext(void* ctx, const char* fn_name);

    /**
     * After write back result, print log info
     *
     * @param ctx Generate by #ExProcessorEventHandler::getContext
     * @param fn_name Function name of RPC call
     * @param bytes The lenght of result
     */
    virtual void postWrite(void* ctx, const char* fn_name, uint32_t bytes);

protected:
    ///< Slow execution
    const static int SLOW_TIME = 100;
};

}}




