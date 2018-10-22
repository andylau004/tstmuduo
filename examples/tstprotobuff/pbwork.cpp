


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





#include "addressbook.pb.h"


using namespace std;
using namespace muduo;


/* -------------------------------------------
 *
 *  protoc --cpp_out=./ addressbook.proto
 *
 * -------------------------------------------*/

// 测试protbuff

int saveAddInfo()
{
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

int loadAddInfo()
{
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



int tst_protobuff_Work_entry(int argc, char *argv[]) {
    /*std::cout << */GOOGLE_PROTOBUF_VERIFY_VERSION;

    return 1;

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




