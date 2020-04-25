


#include "pbwork.h"

//#include <stdio.h>
//#include <iostream>
//#include <string>

//#include <list>

//#include <boost/bind.hpp>
//#include <boost/function.hpp>
//#include <boost/scoped_ptr.hpp>


#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"
//#include "muduo/base/ThreadPool.h"
#include "muduo/base/Logging.h"


#include <google/protobuf/stubs/common.h>


#include "addressbook.pb.h"
#include "query.pb.h"
#include "codec.h"

#include "parsepbfile.h"



using namespace std;
using namespace muduo;


/* -------------------------------------------
 *
 *  protoc --cpp_out=./ addressbook.proto
 *
 * -------------------------------------------*/





// 测试protbuff

int saveAddInfo() {
    ContactInfo::AddressBook addbook;
    addbook.set_owner("xxx");

    // first person
    ContactInfo::Person* pperson = addbook.add_personinfo();
    pperson->set_curname("aaa");
    pperson->set_curid(25);
    pperson->set_curemail("aaa@126.com");

    ContactInfo::Person_PhoneNumber* pPhoneNum = pperson->add_phone();
    pPhoneNum->set_number("111111111");
    pPhoneNum->set_type(ContactInfo::Person::HOME);

    pPhoneNum = pperson->add_phone();
    pPhoneNum->set_number("222222222");
    pPhoneNum->set_type(ContactInfo::Person::MOBILE);

    // second person
    pperson = addbook.add_personinfo();
    pperson->set_curname("bbb");
    pperson->set_curid(30);
    // pperson->set_curemail("bbb@126.com");

    pPhoneNum = pperson->add_phone();
    pPhoneNum->set_number("333333333");
    // pPhoneNum->set_type(ContactInfo::Person::HOME);

    pPhoneNum = pperson->add_phone();
    pPhoneNum->set_number("444444444");
    pPhoneNum->set_type(ContactInfo::Person::MOBILE);

    // int length = addbook.ByteSize();
    // char* buf = new char[length];     // serialize to char*, and transmit by net or others
    // addbook.SerializeToArray(buf,length);

    std::fstream output("pbinfo.log", ios::out|ios::trunc|ios::binary);
    if(!addbook.SerializeToOstream(&output)) {
        std::cerr << "fail to write msg" << std::endl;
        //delete[] buf;
        return -1;
    }
    //delete[] buf;
    return 1;
}
void ShowMsg( const ContactInfo::AddressBook& addbook ) {
    OutputDbgInfo tmpOut( "ShowMsg beg", "ShowMsg end" );

    std::cout << "owner=" << addbook.owner() << std::endl;
    std::cout << "personinfo_size=" << addbook.personinfo_size() << std::endl;
    for (int i = 0; i < addbook.personinfo_size(); ++i)
    {
        cout << addbook.personinfo(i).curname() << endl;
        cout << addbook.personinfo(i).curid() << endl;

        if(addbook.personinfo(i).has_curemail())
            cout << addbook.personinfo(i).curemail() << endl;
        else
            cout << "no email" << endl;

        for (int j = 0; j < addbook.personinfo(i).phone_size(); ++j)
        {
            cout << addbook.personinfo(i).phone(j).number() << endl;

            if (addbook.personinfo(i).phone(j).has_type())
                cout << addbook.personinfo(i).phone(j).type() << endl;
            else
                cout << "no phone type" << endl;
        }
    }

}

void showMsgbyAuto(ContactInfo::AddressBook addbook)
{
    OutputDbgInfo tmpOut( "showMsgbyAuto beg", "showMsgbyAuto end" );
    cout << "addbook.owner=" << addbook.owner() << endl;

    auto pperson = addbook.mutable_personinfo();
    for (auto it = pperson->begin(); it != pperson->end(); ++it)
    {
        cout << it->curname() << endl;
        cout << it->curid() << endl;
        if(it->has_curemail())
            cout << it->curemail() << endl;
        else
            cout << "no email" << endl;

        auto pPhoneNum = it->mutable_phone();
        for (auto ij = pPhoneNum->begin(); ij != pPhoneNum->end(); ++ij)
        {
            cout << ij->number() << endl;
            if(ij->has_type())
                cout << ij->type() << endl;
            else
                cout << "no phone type" << endl;
        }
    }
}

int loadAddInfo() {
    ContactInfo::AddressBook addbook;

    fstream input("pbinfo.log", ios::in|ios::binary);
    if(!addbook.ParseFromIstream(&input))
    {
        cout << "fail to write msg" << endl;
        return -1;
    }
    cout << "now show msg" << endl;
    ShowMsg(addbook);
    cout << endl;
    showMsgbyAuto(addbook);
    return 0;
}


template < typename T >
void testDescriptor() {
    DeferFunctor PrintLine = boost::function<void()> ([]() {
        LOG_INFO << "---------------------------------------";
    });

    std::string type_name = T::descriptor()->full_name();
    LOG_INFO << "type_name=" << type_name;

    const google::protobuf::Descriptor* descriptor =
      google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
    assert(descriptor == T::descriptor());
    LOG_INFO << "FindMessageTypeByName() = " << descriptor;
    LOG_INFO << "T::descriptor()         = " << T::descriptor();
//    LOG_INFO << endl;

    const google::protobuf::Message* prototype =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    assert(prototype == &T::default_instance());
    LOG_INFO << "GetPrototype()        = " << prototype;
    LOG_INFO << "T::default_instance() = " << &T::default_instance();
//    LOG_INFO << std::endl;

    T* new_obj = dynamic_cast<T*>(prototype->New());
    assert(new_obj != NULL);
    assert(new_obj != prototype);
    assert(typeid(*new_obj) == typeid(T::default_instance()));
    LOG_INFO << "prototype->New() = " << new_obj;
    LOG_INFO << "typeid(*new_obj) = " << typeid(*new_obj).name();
//    LOG_INFO << std::endl;
    delete new_obj;
}

void tst_proto_reflection() {
    testDescriptor<muduo::Query>();
    testDescriptor<muduo::Answer>();
    google::protobuf::Message* newQuery = createMessage("muduo.Query");

    assert(newQuery != NULL);
    assert(typeid(*newQuery) == typeid(muduo::Query::default_instance()));

    LOG_INFO << "typeid(*newQuery) = " << typeid(*newQuery).name();
    LOG_INFO << "typeid(muduo::Query::default_instance()) = " << typeid(muduo::Query::default_instance()).name();
    LOG_INFO << "createMessage(\"muduo.Query\") = " << newQuery;

    google::protobuf::Message* newAnswer = createMessage("muduo.Answer");
    assert(newAnswer != NULL);
    assert(typeid(*newAnswer) == typeid(muduo::Answer::default_instance()));
    LOG_INFO << "createMessage(\"muduo.Answer\") = " << newAnswer;

    delete newQuery;
    delete newAnswer;

    puts("All pass!!!");

    google::protobuf::ShutdownProtobufLibrary();
}

void tst_copy_fun() {
    int  source [ 3 ] = { 1, 3, 2 };
    int  dest [ 5 ] = {0};
    std::copy( source, source + 3, dest );
    for ( int j = 0; j < 3; ++j ) {
        std::cout << "j=" << dest[j] << std::endl;
    }

    std::vector < int > vec_intval(3);
    std::copy( source, source + 3, &vec_intval.front() );
    for ( int j = 0; j < 3; ++j ) {
        std::cout << "vec_intval[j]=" << vec_intval[j] << std::endl;
    }
}


class CTestSharedPtr
{
public:
    CTestSharedPtr() { m_val = -1; std::cout <<"construct CTestSharedPtr\n"; }
    ~CTestSharedPtr() { std::cout <<"destruct CTestSharedPtr\n"; }
    void do_something() { std::cout << "do something\n"; }
private:
    int  m_val;
public:
    int  GetVal() {
        return m_val;
    }
    void SetVal( int inVal ) {
        m_val = inVal;
    }
};


void use_const_shared_ptr( const boost::shared_ptr < CTestSharedPtr >& in_ptr ) {
    in_ptr->GetVal();
    std::cout << "use_const_shared_ptr usecount=" << in_ptr.use_count() << std::endl;
}
void use_shared_ptr( boost::shared_ptr < CTestSharedPtr > in_ptr ) {
    in_ptr->GetVal();
    std::cout << "use_shared_ptr usecount=" << in_ptr.use_count() << std::endl;
}




int tst_protobuff_entry(int argc, char *argv[]) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    tst_codec_test(); return 1;

    tst_proto_reflection(); return 1;

    tst_pbParse_entry(); return 1;

//    std::cout << "proto version=" << GOOGLE_PROTOBUF_VERIFY_VERSION;

//    boost::shared_ptr< CTestSharedPtr > pInstance( new CTestSharedPtr );
//    use_shared_ptr( pInstance );
//    std::cout << "aaaaaaaaaaa" << std::endl;
//    std::cout << "usecount=" << pInstance.use_count() << std::endl;

//    use_const_shared_ptr( pInstance );
//    std::cout << "bbbbbbbbbbb" << std::endl;
//    std::cout << "usecount=" << pInstance.use_count() << std::endl;

//    return 1;



//    tst_copy_fun(); return 1;



    int choice;
    cout << "input choice: 1 for save, 2 for load" << endl;
    cin >> choice;

    if(1 == choice)
    {
        saveAddInfo();
    }
    else if (2 == choice)
    {
        loadAddInfo();
    }

    return 0;
}




