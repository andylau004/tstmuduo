

#include "boost_use_1.h"


#include <stdio.h>


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
//#include <Windows.h>

#include "muduo/base/Timestamp.h"
//#include <muduo/base/Timestamp.h>


using namespace std;
using namespace muduo;
//using namespace muduo::net;



//#define MAX_LOOP (1000 /** 1000 * 1000*/ * 8000)
#define MAX_LOOP (10000 * 8000)

typedef boost::function1<void, int> Funtor;




//void swap(SystemHelp &a, SystemHelp &b) {
//    using ::std::swap;
//    swap(a.id, b.id);
//    swap(a.title, b.title);
//    swap(a.parentId, b.parentId);
//    swap(a.helpType, b.helpType);
//    swap(a.linkType, b.linkType);
//    swap(a.clientType, b.clientType);
//    swap(a.displayStatus, b.displayStatus);
//    swap(a.linkUrl, b.linkUrl);
//    swap(a.detailUrl, b.detailUrl);
//    swap(a.content, b.content);
//    swap(a.sortNum, b.sortNum);
//    swap(a.createTime, b.createTime);
//    swap(a.updateTime, b.updateTime);
//    swap(a.status, b.status);
////    swap(a.__isset, b.__isset);
//}

SystemHelp::~SystemHelp() throw() {
}

SystemHelp::SystemHelp(const SystemHelp& other762) {
    id = other762.id;
    title = other762.title;
    parentId = other762.parentId;
    helpType = other762.helpType;
    linkType = other762.linkType;
    clientType = other762.clientType;
    displayStatus = other762.displayStatus;
    linkUrl = other762.linkUrl;
    detailUrl = other762.detailUrl;
    content = other762.content;
    sortNum = other762.sortNum;
    createTime = other762.createTime;
    updateTime = other762.updateTime;
    status = other762.status;
//    __isset = other762.__isset;
}
SystemHelp::SystemHelp( SystemHelp&& other763) {
    id = std::move(other763.id);
    title = std::move(other763.title);
    parentId = std::move(other763.parentId);
    helpType = std::move(other763.helpType);
    linkType = std::move(other763.linkType);
    clientType = std::move(other763.clientType);
    displayStatus = std::move(other763.displayStatus);
    linkUrl = std::move(other763.linkUrl);
    detailUrl = std::move(other763.detailUrl);
    content = std::move(other763.content);
    sortNum = std::move(other763.sortNum);
    createTime = std::move(other763.createTime);
    updateTime = std::move(other763.updateTime);
    status = std::move(other763.status);
//    __isset = std::move(other763.__isset);
}
SystemHelp& SystemHelp::operator=(const SystemHelp& other764) {
    id = other764.id;
    title = other764.title;
    parentId = other764.parentId;
    helpType = other764.helpType;
    linkType = other764.linkType;
    clientType = other764.clientType;
    displayStatus = other764.displayStatus;
    linkUrl = other764.linkUrl;
    detailUrl = other764.detailUrl;
    content = other764.content;
    sortNum = other764.sortNum;
    createTime = other764.createTime;
    updateTime = other764.updateTime;
    status = other764.status;
//    __isset = other764.__isset;
    return *this;
}
SystemHelp& SystemHelp::operator=(SystemHelp&& other765) {
    id = std::move(other765.id);
    title = std::move(other765.title);
    parentId = std::move(other765.parentId);
    helpType = std::move(other765.helpType);
    linkType = std::move(other765.linkType);
    clientType = std::move(other765.clientType);
    displayStatus = std::move(other765.displayStatus);
    linkUrl = std::move(other765.linkUrl);
    detailUrl = std::move(other765.detailUrl);
    content = std::move(other765.content);
    sortNum = std::move(other765.sortNum);
    createTime = std::move(other765.createTime);
    updateTime = std::move(other765.updateTime);
    status = std::move(other765.status);
//    __isset = std::move(other765.__isset);
    return *this;
}
//void SystemHelp::printTo(std::ostream& out) const {
//    using ::apache::thrift::to_string;
//    out << "SystemHelp(";
//    out << "id=" << to_string(id);
//    out << ", " << "title="; (__isset.title ? (out << to_string(title)) : (out << "<null>"));
//    out << ", " << "parentId=" << to_string(parentId);
//    out << ", " << "helpType=" << to_string(helpType);
//    out << ", " << "linkType=" << to_string(linkType);
//    out << ", " << "clientType="; (__isset.clientType ? (out << to_string(clientType)) : (out << "<null>"));
//    out << ", " << "displayStatus=" << to_string(displayStatus);
//    out << ", " << "linkUrl="; (__isset.linkUrl ? (out << to_string(linkUrl)) : (out << "<null>"));
//    out << ", " << "detailUrl="; (__isset.detailUrl ? (out << to_string(detailUrl)) : (out << "<null>"));
//    out << ", " << "content="; (__isset.content ? (out << to_string(content)) : (out << "<null>"));
//    out << ", " << "sortNum=" << to_string(sortNum);
//    out << ", " << "createTime=" << to_string(createTime);
//    out << ", " << "updateTime=" << to_string(updateTime);
//    out << ", " << "status=" << to_string(status);
//    out << ")";
//}




struct  TBase
{
    virtual int DoSomething( int i ) = 0;
};

struct TChiled
{
    TChiled() {}
    int DoSomething(int i){
        Count(i);
        return 0;
    }
    int DoSomethingNotVitrual(int i){
        Count(i);
        return 0;
    }
    int Count(int i) {
        i ++;
        i *= 2;
        i /= 4;
        return i;
    }

};

void TestMemberFunction() {
    std::cout << "TestMemberFunction begin" << std::endl;

    TChiled* pChiled = new TChiled();
    Timestamp begTs(Timestamp::now());
    for (int i=0; i<MAX_LOOP; i++)
    {
        //        printf( "-----------------\n" );
        pChiled->DoSomethingNotVitrual(i);
        //        std::cout << "cout=" << i << std::endl;
        //        printf( "cout=%d\n", i );
    }
    std::cout << "TestMemberFunction spend time=" << (timeDifference(Timestamp::now(), begTs)) << "(s)" << std::endl;
}
void TestBoostBind() {
    std::cout << "TestBoostBind begin" << std::endl;

    TChiled* pChiled = new TChiled();
    Timestamp begTs(Timestamp::now());

    boost::function< int ( int ) > funPtr;
    funPtr = boost::bind(&TChiled::DoSomethingNotVitrual, pChiled, _1);

    for (int i=0; i<MAX_LOOP; i++)
    {
        //        printf( "-----------------\n" );
        funPtr(i);
        //        std::cout << "cout=" << i << std::endl;
        //        printf( "cout=%d\n", i );
    }
    std::cout << "TestBoostBind spend time=" << (timeDifference(Timestamp::now(), begTs)) << "(s)" << std::endl;

}



