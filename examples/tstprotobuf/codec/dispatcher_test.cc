
#include "examples/tstprotobuf/codec/dispatcher.h"

#include "examples/tstprotobuf/codec/query.pb.h"

#include <iostream>

using std::cout;
using std::endl;


typedef boost::shared_ptr<muduo::Query> QueryPtr;
typedef boost::shared_ptr<muduo::Answer> AnswerPtr;

void test_down_pointer_cast() {
//    boost::shared_ptr< google::protobuf::Message > msg(new muduo::Query);
//    boost::shared_ptr< muduo::Query > query(muduo::down_pointer_cast<muduo::Query>(msg));
//    if (msg == query) {
//        LOG_INFO  << "msg == query" << ", querycount=" << query.use_count() << ", msgcount=" << msg.use_count();
//    } else {
//        LOG_INFO  << "msg != query";
//    }
}

int main() {
    test_down_pointer_cast();
    return 1;
}
