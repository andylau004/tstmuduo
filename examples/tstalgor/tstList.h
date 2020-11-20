

#pragma once



#include <stdio.h>
#include <iostream>

#include <cstdio>

#include <set>
#include <queue>
#include <stack>

#include <sys/time.h>
#include <unistd.h>

#include <boost/bind.hpp>


#include <iterator>     // ostream_iterator
#include <algorithm>    // unique，distance

#include "List.h"

#include "muduo/base/common.h"

#include "muduo/base/BinaryTree.h"

using namespace std;




struct ListNodeEx
{
    int val;
    ListNodeEx* next;
    ListNodeEx() : val(-1), next(nullptr) {
    }
};

extern void tst_reverse_list_1();

extern void tst_RGetKthNode_fun();


extern int tst_ListEntry_();





