

#include "bitmapfun.h"




#include <stdio.h>

#include <iostream>
#include <unordered_map>
#include <stack>
#include <vector>
#include <bitset>


#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>
//#include <Windows.h>

#include "boost_use_1.h"

#include "muduo/base/Timestamp.h"
//#include <muduo/base/Timestamp.h>

#include "muduo/base/common.h"



using namespace std;
using namespace muduo;
//using namespace muduo::net;



void tst_bitmap_1() {
    const int max_cout = 20;

    int arr[ 10 ] = {2,9,3,5,4,0,18,11,12,15};
    std::bitset< max_cout > bit_map;
    bit_map.reset();  //所有位置0

    size_t i = 0;
    for (i = 0; i < array_size(arr); ++i ) {
        bit_map.set( arr[i] );
    }
    for (i = 0; i < max_cout; ++i ) {
        if ( bit_map.test(i) )
            std::cout << " " << i << std::endl;
    }
    std::cout << std::endl;
}


void   tst_bitmap_entry() {

    tst_bitmap_1();
}
