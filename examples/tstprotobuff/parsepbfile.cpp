
#include "parsepbfile.h"



#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/compiler/importer.h>


#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "muduo/base/common.h"

#include "sylar.pb.h"
#include "query.pb.h"



//filename proto文件名
//classname proto文件里面的class， 格式package.Classname
//cb, 创建好proto文件里面的classname类之后执行的回调，可以在该回调里面操作数据
int DynamicParseFromPBFile(const std::string& filename,
                           const std::string& classname,
                           std::function<void(::google::protobuf::Message* msg)> cb);
//proto_string proto文件的内容
//classname proto文件里面的class， 格式package.Classname
//cb, 创建好proto文件里面的classname类之后执行的回调，可以在该回调里面操作数据
int DynamicParseFromPBString(const std::string& proto_string,
                             const std::string& classname,
                             std::function<void(::google::protobuf::Message* mgs)> cb);


int DynamicParseFromPBFile(const std::string& filename,
                           const std::string& classname,
                           std::function<void(::google::protobuf::Message* msg)> cb) {
    // TODO 检查文件名是否合法
    auto pos = filename.find_last_of('/');
    std::string path;
    std::string file;
    if(pos == std::string::npos) {
        file = filename;
    } else {
        path = filename.substr(0, pos);
        file = filename.substr(pos + 1);
    }
    LOG_INFO << "file=" << file << ", path=" << path;

    ::google::protobuf::compiler::DiskSourceTree sourceTree;
    sourceTree.MapPath("", path);
    ::google::protobuf::compiler::Importer importer(&sourceTree, NULL);
    importer.Import(file);

    // 先获得类型的Descriptor
    const ::google::protobuf::Descriptor* descriptor
            = importer.pool()->FindMessageTypeByName(classname);
    if (!descriptor) {
        LOG_INFO << "descriptor null";
        return -1;
    } else {
        LOG_INFO << "descriptor not null, by classname=" << classname;
    }

    // 利用Descriptor拿到类型注册的instance. 这个是不可修改的.
    ::google::protobuf::DynamicMessageFactory factory;
    const ::google::protobuf::Message *message = factory.GetPrototype(descriptor);
    if(!message) {
        LOG_INFO << "message null";
        return 2;
    } else {
        LOG_INFO << "message not null";
    }

    // 构造一个可用的消息.
    ::google::protobuf::Message* msg = message->New();
    if(!msg) {
        LOG_INFO << "msg null";
        return 3;
    } else {
        LOG_INFO << "msg not null";
    }
    cb(msg);
    delete_object(msg);
    return 0;
}
int DynamicParseFromPBString(const std::string& proto_string,
                             const std::string& classname,
                             std::function<void(::google::protobuf::Message* mgs)> cb) {
    std::stringstream ss;
    ss << "/tmp/dps_" << rand() << "_" << rand() << ".proto";

    std::ofstream ofs(ss.str());
    ofs << proto_string;
    ofs.close();

    return DynamicParseFromPBFile(ss.str(), classname, cb);
}
int tst_pbParse_entry() {

    {
        std::string protoDir  = "../examples/tstprotobuff/";
        std::string protoFile = "query.proto";
        std::string fileContent = GetFileContent_string(protoFile);

        ::google::protobuf::compiler::DiskSourceTree sourceTree;
        sourceTree.MapPath("", protoDir.c_str());

        ::google::protobuf::compiler::Importer importer(&sourceTree, NULL);
        const google::protobuf::FileDescriptor* fd = importer.Import(protoFile);
        LOG_INFO << "protoFile=" << protoFile << ", fd=" << fd;
//        importer.Import(protoDir + protoFile);
//        LOG_INFO << "proto dir+file=" << protoDir + protoFile;

        std::string classname = "muduo.Query";

        // 先获得类型的Descriptor
        const ::google::protobuf::Descriptor* descriptor
                = importer.pool()->FindMessageTypeByName(classname);
        if (!descriptor) {
            LOG_INFO << "descriptor null, classname=" << classname;
            return -1;
        } else {
            LOG_INFO << "descriptor not null, by classname=" << classname;
        }

        // 利用Descriptor拿到类型注册的instance. 这个是不可修改的.
        ::google::protobuf::DynamicMessageFactory factory;
        const ::google::protobuf::Message *message = factory.GetPrototype(descriptor);
        if (!message) {
            LOG_INFO << "message null";
            return 2;
        } else {
            LOG_INFO << "message not null";
        }

        // 构造一个可用的消息.
        ::google::protobuf::Message* msg = message->New();
        if(!msg) {
            LOG_INFO << "msg null";
            return 3;
        } else {
            LOG_INFO << "msg not null";

//            google::protobuf::Message* newQuery = createMessage("muduo.Query");

//            assert(newQuery != NULL);
//            assert(typeid(*newQuery) == typeid(muduo::Query::default_instance()));

//            cout << "typeid(*newQuery) = " << typeid(*newQuery).name() << endl;
//            cout << "typeid(muduo::Query::default_instance()) = " << typeid(muduo::Query::default_instance()).name() << endl;
//            cout << "createMessage(\"muduo.Query\") = " << newQuery << endl;


            google::protobuf::Message* newQuery = (msg);
            assert(newQuery != NULL);
            assert(typeid(*newQuery) == typeid(muduo::Query::default_instance()));
            LOG_INFO << "typeid(*newQuery) = " << typeid(*newQuery).name();
            LOG_INFO << "typeid(muduo::Query::default_instance()) = " << typeid(muduo::Query::default_instance()).name();
            LOG_INFO << "createMessage(\"muduo.Query\") = " << newQuery;

            // 直接调用message的具体接口
            // 其实这些接口是语法糖接口.所以并没有对应的反射机制来对应调用.
            // 反射机制实现了的Set/Get XXX系列接口，是属于Reflection的接口，接收Message作为参数.
//            pQueryObj->set_id(1);
//            pQueryObj->set_questioner("test fuck usa");
//            pQueryObj->add_question("fire in the hole...");
//            LOG_INFO << pQueryObj->Utf8DebugString();
//            delete message;
        }

        return 1;
    }

    sylar::Test test;
    test.set_name("test_name");
    test.set_age(100);
    test.add_phones("138xxxxxx");
    test.add_phones("139xxxxxx");

    for (int i = 0; i < 3; ++i) {
        ::sylar::AA* a = test.add_aa();
        a->set_name("a_name_" + std::to_string(i));
        a->set_age(100 + i);
    }

    std::string pb_str;
    //把测试类，序列化到string中，以供后续解析
    test.SerializeToString(&pb_str);
    //打印测试类的数据信息
    LOG_INFO << "test.debugstring=" << test.DebugString();
    LOG_INFO << "pb_str=" << pb_str;
//    LOG_INFO << "===============================" ;
    //从文件中sylar.proto中的sylar.XX类，解析上面测试类的序列化二进制
    //并输出序列化后的sylar.XX的信息
//    DynamicParseFromPBFile("sylar.proto", "sylar.XX"
//        , [pb_str](::google::protobuf::Message* msg) {
//        if(msg->ParseFromString(pb_str)) {
//            LOG_INFO << "sylar.XX debugstring=" << msg->DebugString() ;
//        }
//    });

//    LOG_INFO << "===============================";
    //从文件中sylar.proto中的sylar.Test，解析上面测试类的序列化二进制
    //并输出序列化后的sylar.Test的信息
//    DynamicParseFromPBFile("sylar.proto", "sylar.Test"
//        , [pb_str](::google::protobuf::Message* msg) {
//        if(msg->ParseFromString(pb_str)) {
//            LOG_INFO << "sylar.Test debugstring=" << msg->DebugString() ;
//        }
//    });
    //字符串pb文件内容
    std::string pbstr = "syntax = \"proto2\";\n package xx;\n"
        "message BB { \n"
        "    optional string name = 1; \n"
        "    optional int32 age = 2; \n"
        "} \n"
        "message TT { \n"
        "    optional string name = 1; \n"
        "    optional int32 age = 2; \n"
        "    repeated string phones = 3; \n"
        "    repeated BB aa = 3; \n"
        "}";

    LOG_INFO << "===============================";
    //pbstr的proto信息中的xx.TT，解析上面测试类的序列化二进制
    //并输出序列化后的xx.TT的信息
    DynamicParseFromPBString(pbstr, "xx.TT"
        , [&pb_str](::google::protobuf::Message* msg) {
        if(msg->ParseFromString(pb_str)) {
            LOG_INFO << "xx.TT debugstring=" << msg->DebugString();
        }
    });
    LOG_INFO << "===============================";
    //pbstr的proto信息中的xx.TT，解析上面测试类的序列化二进制
    //并输出序列化后的xx.TT的信息
    DynamicParseFromPBString(pbstr, "xx.BB"
        , [pb_str](::google::protobuf::Message* msg) {
        if(msg->ParseFromString(pb_str)) {
            LOG_INFO << "xx.BB debugstring=" << msg->DebugString();
        }
    });

    return 1;
}




int tst_pbParse_entry(int argc, char *argv[]) {
    return 1;
}
