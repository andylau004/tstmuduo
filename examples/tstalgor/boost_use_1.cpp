

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



