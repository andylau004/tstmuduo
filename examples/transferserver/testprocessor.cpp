
#include "testprocessor.h"


#include <iostream>

#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>



#include "muduo/base/common.h"
#include "muduo/base/Logging.h"



#include "clientctx.hpp"



#include "../gen-cpp/transfer_types.h"

#include "../gen-cpp/Photo.h"



std::atomic<int64_t>    g_clientSessionId;



const std::string kEmptyRequest = "SendEmptyRequest";

//extern com::vrv::im::ap::SessionManager _sm;
//extern log4cplus::Logger gLogger;


typedef void (TestProcessor::*Test_ProcessFunction)(int32_t,
                                                 protocol::TProtocol*,
                                                 protocol::TProtocol*,
                                                 void*);

typedef boost::unordered_map<std::string, Test_ProcessFunction> ProcessMap;

static const ProcessMap processmap = boost::assign::map_list_of \
    ("login_im", &TestProcessor::process_login_im)
    ("SendPhoto", &TestProcessor::process_SendPhoto)
    ("Add", &TestProcessor::process_Add);




//template <class T>
//void printXto_console(const char* lpszPrefix, T& x) {
//    std::ostringstream stream;
//    x.printTo(stream);
////    printf( "tostrings=%s\n", stream.str().c_str() );
//    LOG_INFO << lpszPrefix << " tostrings=" << stream.str().c_str();
//}



TestProcessor::TestProcessor(ClientCtx * ctx, TypeProtocol proc)
    : Processor(ctx, proc)//,
//    m_apservicehandler(NULL) {
{
}

TestProcessor::~TestProcessor() {
//    if (m_apservicehandler) {
//        delete m_apservicehandler;
//        m_apservicehandler = NULL;
//    }
}

bool TestProcessor::dispatchCall(protocol::TProtocol *iprot,
                                 protocol::TProtocol *oprot,
                                 const std::string &fname,
                                 int32_t seqid,
                                 void *callContext) {
    ProcessMap::const_iterator pfn;
    pfn = processmap.find(fname);
    if ( pfn == processmap.end() ) {
        LOG_ERROR << "Invalid method name:" << fname;

        iprot->skip(::apache::thrift::protocol::T_STRUCT);
        iprot->readMessageEnd();
        iprot->getTransport()->readEnd();

        ::apache::thrift::TApplicationException x(::apache::thrift::TApplicationException::UNKNOWN_METHOD,
                                                  "Invalid method name: '"+fname+"'");

        oprot->writeMessageBegin(fname, ::apache::thrift::protocol::T_EXCEPTION, seqid);
        x.write(oprot);
        oprot->writeMessageEnd();
        oprot->getTransport()->writeEnd();
        oprot->getTransport()->flush();
        set_transport(callContext);
        return true;
    }
    (this->*(pfn->second))(seqid, iprot, oprot, callContext);
    return true;
}

bool TestProcessor::process(boost::shared_ptr<TProtocol> in,
                            boost::shared_ptr<TProtocol> out,
                            void *connectionContext) {
    protocol::TProtocol* inRaw  = in.get();
    protocol::TProtocol* outRaw = out.get();

    std::string fname;
    protocol::TMessageType mtype;
    int32_t seqid;
    inRaw->readMessageBegin(fname, mtype, seqid);

    LOG_INFO <<", **read fname:" << fname;
    if (mtype != protocol::T_CALL && mtype != protocol::T_ONEWAY) {
        LOG_ERROR << ", mtype is not T_CALL or T_ONEWAY:" << mtype;
        return false;
    }

    if (fname == kEmptyRequest) {
        SendEmptyRequestReq args;
        args.read(inRaw);
        inRaw->readMessageEnd();

        uint32_t readsize = inRaw->getTransport()->readEnd();
        int64_t sessionID = args.sessionID;

        LOG_INFO << "kEmptyRequest readsize=" << readsize << ", sessionID=" << sessionID;
        printXto_console("recv kEmptyRequest args=", args);

        set_transport(NULL);
        return true;
    }

    return this->dispatchCall(inRaw, outRaw, fname, seqid, connectionContext);
}

bool TestProcessor::process(boost::shared_ptr<TProtocol> /*in*/,
                            boost::shared_ptr<TProtocol> out,
                            void *connectionContext,
                            int64_t flag) {
    UNUSED(connectionContext);
    if (flag != 0 && out.get() != NULL) {
        if (m_clientctx != NULL) {
//            bool success = _sm.GetOrWait(flag, m_clientctx->getClientTicket(),
//                                         m_clientctx->getClientNotifyID(), 0, 0,
//                                         out.get()->getTransport(), out.get(), m_clientctx, 2);
//            if (!success) {
//                APNotifyClientT_send_notifyError(out.get(),
//                                                 m_clientctx->getSessionID()
//                                                 /*conn->getSessionID()*/, 1);
//            }
            set_transport(NULL);
            return true;
        }
    }
    return false;
}

bool TestProcessor::init() {
    Processor::init();
//    if (m_apservicehandler == NULL) {
//        m_apservicehandler = new (std::nothrow) APServiceHandler(this);
//    }
//    if (m_apservicehandler == NULL || !m_apservicehandler->init()) {
//        return false;
//    }
    return true;
}

void TestProcessor::set_transport(void *ctx) {
//    m_apservicehandler->clear();
    Processor::set_transport(ctx);
}

void TestProcessor::process_SendPhoto(int32_t seqid,
                                      protocol::TProtocol* iprot,
                                      protocol::TProtocol* oprot,
                                      void* callContext) {
    OutputDbgInfo tmpOut("process_SendPhoto beg---------------", "process_SendPhoto end---------------");

    LOG_INFO << "m_clientctx=" << m_clientctx << ", callContext=" << callContext;
    UNUSED(callContext);
    Photo_SendPhoto_args args;
    read_helper_fun(args, iprot);

    m_resultCallback = boost::bind(&TestProcessor::return_SendPhoto, this, seqid, oprot, _3);
//    m_apservicehandler->login(args.param);

    LOG_INFO << "file_size=" << args.onefile.file_size << " file_size=" << args.onefile.file_hsh;
    bool bret = save_file("./mysavefile", (char*)args.onefile.file_buffer.data(), args.onefile.file_size);
//    return_result(&bret);
    UNUSED(bret);

    int tstret = 1024;
    return_result(&tstret);
}
void TestProcessor::return_SendPhoto(int32_t seqid, protocol::TProtocol * oprot, void * _return) {
    OutputDbgInfo tmpOut("return_SendPhoto beg---------------", "return_SendPhoto end---------------");
    Photo_SendPhoto_result result;
    LOG_INFO << "*_return=" << (*(int*)_return);
    if ((*(int*)_return) == 1024) {
//        result.success = *(reinterpret_cast<Photo_SendPhoto_result *>(_return));
        result.__isset.success = true;
        result.success = true;
    } else {
        result.__isset.success = false;
        result.success = false;
    }
    return_helper_fun(result, "SendPhoto", seqid, oprot);
}

void TestProcessor::process_Add(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext) {
    OutputDbgInfo tmpOut("process_Add beg---------------", "process_Add end---------------");

    LOG_INFO << "m_clientctx=" << m_clientctx << ", callContext=" << callContext;
    UNUSED(callContext);
    Photo_Add_args args;
    read_helper_fun(args, iprot);

    m_resultCallback = boost::bind(&TestProcessor::return_Add, this, seqid, oprot, _3);

    LOG_INFO << "add1=" << args.add1 << " add2=" << args.add2;

    int32_t retVal = args.add1 + args.add2;
    return_result(&retVal);
}
void TestProcessor::return_Add(int32_t seqid, protocol::TProtocol * oprot, void * _return) {
    OutputDbgInfo tmpOut("return_Add beg---------------", "return_Add end---------------");
    Photo_Add_result result;
    if (_return) {
        result.__isset.success = true;
        result.success = *(reinterpret_cast<int32_t *>(_return));
        result.success = -1;
    } else {
        result.__isset.success = false;
        result.success = -1;
    }
    return_helper_fun(result, "Add", seqid, oprot);
}


void TestProcessor::process_login_im(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext) {
    OutputDbgInfo tmpOut("process_login_im beg---------------", "process_login_im end---------------");

    LOG_INFO << "m_clientctx=" << m_clientctx << ", callContext=" << callContext;
    UNUSED(callContext);
    Photo_login_im_args args;
    read_helper_fun(args, iprot);

    m_resultCallback = boost::bind(&TestProcessor::return_login_im, this, seqid, oprot, _3);

    LoginResult login_result;

    // 给客户端分配一个唯一递增回话ID
    g_clientSessionId ++;
    int64_t itmp = std::atomic_load(&g_clientSessionId);
    login_result.__set_sessionID( itmp );
    //

    return_result(&login_result);
}
void TestProcessor::return_login_im(int32_t seqid, protocol::TProtocol * oprot, void * _return) {
    OutputDbgInfo tmpOut("return_login_im beg---------------", "return_login_im end---------------");

    Photo_login_im_result result;
    if (_return != NULL) {
        result.success = *(reinterpret_cast<LoginResult *>(_return));
        result.success.code = 0;
        LOG_INFO << "return_login_im success! clientInfo=" << (reinterpret_cast<LoginResult *>(_return))->ticket
                 << ", sessionID=" << (reinterpret_cast<LoginResult *>(_return))->sessionID;
        result.__isset.success = true;
    } else {
        LOG_INFO << "return_login_im failed! clientInfo=" << (reinterpret_cast<LoginResult *>(_return))->ticket
                 << ", sessionID=" << (reinterpret_cast<LoginResult *>(_return))->sessionID;
        result.success.code = 11102;
        result.__isset.success = true;
    }
    return_helper_fun(result, "login_im", seqid, oprot);
}

//void TestProcessor::process_SendEmptyRequest(int32_t seqid, TProtocol* iprot, TProtocol* oprot, void* callContext) {
//    OutputDbgInfo tmpOut("process_SendEmptyRequest beg---------------", "process_SendEmptyRequest end---------------");

//    LOG_INFO << "m_clientctx=" << m_clientctx << ", callContext=" << callContext;
//    UNUSED(callContext);
//    Photo_login_im_args args;
//    read_helper_fun(args, iprot);

//    m_resultCallback = boost::bind(&TestProcessor::return_SendEmptyRequest, this, seqid, oprot, _3);

////    return_result(&retVal);
//}
//void TestProcessor::return_SendEmptyRequest(int32_t seqid, protocol::TProtocol * oprot, void * _return) {
//    OutputDbgInfo tmpOut("return_SendEmptyRequest beg---------------", "return_SendEmptyRequest end---------------");
//    Photo_Add_result result;
//    if (_return) {
//        result.__isset.success = true;
//        result.success = *(reinterpret_cast<int32_t *>(_return));
//        result.success = -1;
//    } else {
//        result.__isset.success = false;
//        result.success = -1;
//    }
//    return_helper_fun(result, "SendEmptyRequest", seqid, oprot);
//}


