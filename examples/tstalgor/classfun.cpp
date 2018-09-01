


#include "classfun.h"

#include "boost_use_1.h"


#include <stdio.h>


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
//#include <Windows.h>

#include "muduo/base/Timestamp.h"
//#include <muduo/base/Timestamp.h>

#include "muduo/base/common.h"


using namespace std;
using namespace muduo;
//using namespace muduo::net;



//#define MAX_LOOP (1000 /** 1000 * 1000*/ * 8000)
#define MAX_LOOP (10000 * 8000)

typedef boost::function1<void, int> Funtor;




class Base {
private:
    virtual void display() { std::cout<<"Base display()"<<std::endl; }
    void say(){ std::cout<<"Base say()"<<std::endl; }
public:
    void exec(){ display(); say(); }
    void f1(std::string a) { std::cout<<"Base f1(string)"<< std::endl; }
    void f1(int a) { std::cout<<"Base f1(int)"<<std::endl; } //overload，两个f1函数在Base类的内部被重载
};
class DeriveA : public Base{
public:
    void display() { std::cout<<"DeriveA display()"<<std::endl; } //override，基类中display为虚函数，故此处为重写
    void f1(int a,int b) { std::cout<<"DeriveA f1(int,int)"<<std::endl; }//redefining，f1函数在Base类中不为虚函数，故此处为重定义
    void say() { std::cout<<"DeriveA say()"<<std::endl; } //redefining，同上
};
class DeriveB : public Base
{
public:
    void f1(int a) { std::cout<<"DeriveB f1(int)"<<std::endl; } //redefining，重定义
};

int tst_class_a() {
    DeriveA a;
    Base *b = &a;

    std::cout << "1111111111111" << std::endl;
    b->exec(); //display():version of DeriveA call(polymorphism)

    std::cout << "22222222222222" << std::endl;
    //say():version of Base called(allways ) b里边的函数display被A类覆盖，但是say还是自己的。
    a.exec(); //same result as last statement

    std::cout << "33333333333333" << std::endl;
    a.say();

    std::cout << "444444444444444" << std::endl;
    DeriveB c;
    c.f1(1); //version of DeriveB called
    return 0;
}




//==================================================================
// 《剑指Offer——名企面试官精讲典型编程题》代码
// 作者：何海涛
//==================================================================

// 面试题57（二）：为s的连续正数序列
// 题目：输入一个正数s，打印出所有和为s的连续正数序列（至少含有两个数）。
// 例如输入15，由于1+2+3+4+5=4+5+6=7+8=15，所以结果打印出3个连续序列1～5、
// 4～6和7～8。

#include <cstdio>

void PrintContinuousSequence(int small, int big);


void PrintContinuousSequence(int small, int big)
{
    for(int i = small; i <= big; ++ i)
        printf("%d ", i);

    printf("\n");
}

void FindContinuousSequence(int sum)
{
    if(sum < 3)
        return;

    int small = 1;
    int big = 2;
    int middle = (1 + sum) / 2;
    int curSum = small + big;

//    while (small < middle) {
//        if (curSum == sum)
//            PrintContinuousSequence(small, big);
//        while (curSum > sum && small < big) {
//            curSum -= small;
//            small ++;
//            if (curSum == sum)
//                PrintContinuousSequence(small, big);
//        }
//        big ++;
//        curSum += big;
//    }

    while(small < middle)
    {
        if(curSum == sum)
            PrintContinuousSequence(small, big);

        while(curSum > sum && small < middle) {
            curSum -= small;
            small ++;

            if(curSum == sum)
                PrintContinuousSequence(small, big);
        }

        big ++;
        curSum += big;
    }
}
// ====================测试代码====================
void Test(const char* testName, int sum)
{
    if(testName != nullptr)
        printf("%s for %d begins: \n", testName, sum);

    FindContinuousSequence(sum);
}

int print_continue_seq()
{
    Test("test1", 1);
    Test("test2", 3);
    Test("test3", 4);
    Test("test4", 9);
    Test("test5", 15);
    Test("test6", 100);

    return 0;
}


class CYinYong {
public:
    CYinYong( std::vector< std::string >& in_vec, int& in_val ) : _queryRecords(in_vec), m_val(in_val) {
//        in_val = -1;
    }
    ~CYinYong() {

    }
    void PrintVal() {
        std::cout << "addr=" << &m_val << ", val=" << m_val << std::endl;
//        std::cout << "val=" << m_val << std::endl;

        _queryRecords.clear();
        for (int i = 1023; i < 1033; ++i) {
            _queryRecords.push_back(convert< std::string > (i));
        }

    }
    void IncreVal() {
        m_val++;
//        std::cout << "addr=" << &m_val << ", val=" << m_val << std::endl;
//        std::cout << "val=" << m_val << std::endl;
    }
    void PrintRecords () {
        for ( auto oneLine : _queryRecords ) {
            std::cout << "oneline=" << oneLine << std::endl;
        }
    }

private:
    int&    m_val;
    std::vector< std::string >& _queryRecords;
};


typedef struct _tag_CpyData {
    int iVal1;
    int iVal2;
    std::string str_1;

}CPY_DATA;

void tst_cpy_data() {
    float f_1 = 0.00;
    if (!f_1) {
        std::cout << "just equl" << std::endl;
    } else {
        std::cout << "not equl" << std::endl;
    }
    return ;

    std::vector < CPY_DATA > vec_datas;

    CPY_DATA data1;
    data1.iVal1 = 123;
    data1.iVal2 = 456;
    data1.str_1 = "aabbcc";

    vec_datas.push_back(data1);

    CPY_DATA tmp111;
    tmp111 = vec_datas.front();
    std::cout << "v1=" << tmp111.iVal1 << ", v2=" << tmp111.iVal2  << ", str_1=" << tmp111.str_1 << std::endl;
}

void Print_yinyong()
{
    std::cout << "intsize=" << sizeof(int) << ", int32=" << sizeof(int32_t) << std::endl;
    std::cout << "------------------------" << std::endl;
//    return;

    std::vector<std::string> tmpRecord;
    CYinYong* pYinyong = NULL;
    int* ptmp = NULL;
    {
        int iVal = 256;
        std::cout << "addr=" << &iVal << ", val=" << iVal << std::endl;

        pYinyong = new CYinYong(tmpRecord, iVal);
        ptmp = &iVal;
    }
    pYinyong->PrintVal();

//    *ptmp = 9987;

    pYinyong->IncreVal();
    pYinyong->IncreVal();
    pYinyong->IncreVal();
//    std::cout << "addr=" << &iVal << ", val=" << iVal << std::endl;
    std::cout << "------------------------" << std::endl;
    pYinyong->PrintVal();

    std::cout << "------------------------" << std::endl;
    pYinyong->PrintRecords();

    getchar();
}






