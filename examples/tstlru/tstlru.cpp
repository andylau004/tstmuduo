

#include "tstlru.h"

#include <stdio.h>
#include <iostream>
#include <string>


#include <list>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>


#include "muduo/base/color.h"
#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"


#include "LRUCache.h"


using namespace std;
using namespace muduo;
//using namespace muduo::net;



struct Student {
    int id;
    const char* name;
};

void tst_lru_11() {

//        if(argc != 2)
//        {
//            printf(L_RED"usage:"YELLOW" input capacity\n"NONE);
//            return 0;
//        }
        unsigned int capacity = /*atoi*/(128);
//        if(capacity > MAXSIZE)
//        {
//            printf(L_RED"errMsg:"YELLOW"capacity should less than 1048576\n"NONE);
//            return 0;
//        }

        LRUCache<Student> cache(capacity);
        printf(L_RED "create cache: capacity=%u\n", capacity);
        printf(L_RED "usage:" YELLOW " input capacity\n" NONE);

        std::string key = "zhb1";

        struct Student stu1 = {1, key.c_str()};
        cache.put(key, stu1);
        printf("set cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(), stu1.id, stu1.name);

        key = "zhb2";
        struct Student stu2 = {2, key.c_str()};
        cache.put(key, stu2);
        printf("set cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(), stu2.id, stu2.name);

        struct Student stu;
        int ret = cache.get(key, stu);
        if(ret == 0) {
            printf("hit cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(), stu.id, stu.name);
        }
        else {
            printf("miss cache, key=%s\n", key.c_str());
        }

        key = "zhb2";
        struct Student stu3 = {3, key.c_str()};
        cache.put(key, stu3);
        printf("set cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(),stu3.id, stu3.name);

        ret = cache.get(key,stu);
        if(ret == 0)
        {
            printf("hit cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(), stu.id, stu.name);
        }
        else
        {
            printf("miss cache, key=%s\n", key.c_str());
        }

        key = "zhb3";
        ret = cache.get(key,stu);
        if(ret == 0)
        {
            printf("hit cache: key=%s, stu.id=%d, stu.name=%s\n", key.c_str(), stu.id, stu.name);
        }
        else
        {
            printf("miss cache, key=%s\n", key.c_str());
        }


}


void tst_lru_entry() {

//    std::list< std::pair < int, int > > listNode;

    std::list < int > listInts;
    int i = 0;
    for ( i ; i < 10; i ++ ) {
        listInts.push_back(i);
    }

    std::list<int>::iterator itfind;
    for ( itfind = listInts.begin(); itfind != listInts.end(); itfind ++ ) {
        if ( *itfind == 3 ) {
            listInts.erase(itfind);
            break;
        }
    }
    std::cout << "*itfind=" << *itfind << std::endl;
    return;

    tst_lru_11();
    return ;


    {
        list<int> mylist1, mylist2;
        list<int>::iterator it;

        // set some initial values:
        for (int i=1; i<=4; i++)
            mylist1.push_back(i);      // mylist1: 1 2 3 4
        for (int i=1; i<=3; i++)
            mylist2.push_back(i*10);   // mylist2: 10 20 30

        it = mylist1.begin();
        ++it;

        mylist1.splice(it, mylist2);// mylist1: 1 10 20 30 2 3 4
                                    // mylist2 (empty)
        std::cout << "mylist1-------------beg" << std::endl;
        PrintInContainer(mylist1);
        std::cout << std::endl;

        std::cout << "mylist2-------------beg" << std::endl;
        PrintInContainer(mylist2);
        std::cout << std::endl;

        mylist2.splice (mylist2.begin(), mylist1, it);
                                      // mylist1: 1 10 20 30 3 4
                                      // mylist2: 2
                                      // "it" is now invalid.

        std::cout << "mylist2-------------beg" << std::endl;
        PrintInContainer(mylist2);
        std::cout << std::endl;

        std::cout << "mylist1-------------beg" << std::endl;
        PrintInContainer(mylist1);
        std::cout << std::endl;

        return;
    }

    std::list<std::string> dictionary, bword;
    dictionary.push_back("any");
    dictionary.push_back("angle");
    dictionary.push_back("ajust");
    dictionary.push_back("common");
    dictionary.push_back("cannon");
    dictionary.push_back("company");
    bword.push_back("blue");
    bword.push_back("banana");
    bword.push_back("break");
    std::list<std::string>::iterator its = dictionary.begin();
    for (int i = 0; i < 3; i++)
        its++;
    dictionary.splice(its, bword);
//    copy(bword.begin(), bword.end(), std::ostream_iterator<std::string>(cout, "/n"));

    PrintInContainer(dictionary);
    std::cout << std::endl;

//       for (it=mylist1.begin(); it!=mylist1.end(); it++)
//         std::cout << " " << *it;
}
