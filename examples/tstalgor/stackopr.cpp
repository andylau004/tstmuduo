



#include "stackopr.h"


#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <stack>
#include <vector>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/scoped_ptr.hpp>

#include "boost_use_1.h"

#include "muduo/base/common.h"
#include "muduo/base/Timestamp.h"


using namespace std;
using namespace muduo;



//剑指offer——栈的压入弹出顺序

//输入两个整数序列，第一个序列表示栈的压入顺序，请判断第二个序列是否为该栈的弹出顺序。
//假设压入栈的所有数字均不相等。例如序列1,2,3,4,5是某栈的压入顺序，
//序列4,5,3,2,1是该压栈序列对应的一个弹出序列，但4,3,5,1,2就不可能是该压栈序列的弹出序列。
//（注意：这两个序列的长度是相等的）

bool IsPopOrder(vector<int> pushV,vector<int> popV) {
    vector<int> stack;
    for(size_t i = 0, j = 0 ;i < pushV.size();i++){
        stack.push_back(pushV[i]);

        //根据出栈顺序模拟出栈
        while(j < popV.size() && stack.back() == popV[j]){
            std::cout << "stack.back=" << stack.back() << ", popV[" << j << "]=" << popV[j] << std::endl;
            stack.pop_back();
            j++;
        }
    }
    //全出栈就输出true，否则输出false.
    return stack.empty();
}

void tst_shunxu() {
    std::vector< int > vecPush = {1, 2, 3, 4, 5};
    std::vector< int > vecPop  = {4, 5, 3, 2, 1};
    std::cout << "ret=" << IsPopOrder(vecPush, vecPop) << std::endl;

}
void tst_stack_entry() {
    tst_shunxu();

}
