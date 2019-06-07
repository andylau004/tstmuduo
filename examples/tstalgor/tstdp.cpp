



#include "tstdp.h"




#include <iostream>
#include <sstream>

#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>

#include <fstream>
#include <thread>

#include <boost/bind.hpp>

#include "muduo/base/common.h"


using namespace std;
using namespace muduo;
//using namespace muduo::net;


/*
    动态规划（英语：Dynamic programming，简称DP）
    是一种在数学、管理科学、计算机科学、经济学和生物信息学中使用的，通过把原问题分解为相对简单的子问题的方式求解复杂问题的方法。
    动态规划常常适用于有重叠子问题[1]和最优子结构性质的问题，动态规划方法所耗时间往往远少于朴素解法。
*/





/*
    最优方法，时间复杂度O（n）
    和最大的子序列的第一个元素肯定是正数
    因为元素有正有负，因此子序列的最大和一定大于0
*/

int GetMax(int a, int b) {  //得到两个数的最大值
    return (a) > (b) ? (a) : (b);
}

int GetMaxAddOfArray(int* arr, int sz) {
    if (!arr || sz <= 0) {
        return 0;
    }
    int sum = arr[0];
    int max = arr[0];
    for (int i = 1; i < sz; ++i) {
        sum = GetMax(sum + arr[i], arr[i]);
        if (sum > max)
            max = sum;
    }
    return max;
}

void tst_dpfun_entry() {
    int array[] = { 2, 3, -6, 4, 6, 2, -2, 5, -9 };
    int sz = sizeof(array) / sizeof(array[0]);

    int MAX = GetMaxAddOfArray(array, sz);
    std::cout << "dpfun MAX=" << MAX << std::endl;
}
