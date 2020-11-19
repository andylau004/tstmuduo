

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



#include "muduo/base/BinaryTree.h"
#include "muduo/base/common.h"




using namespace std;


struct ListNode;


extern bool isBalanced(BstNode* root);

extern void LeetCodeEntry();


extern ListNode* ConstructTestList();

extern ListNode* ConstructTestList(const std::vector<int>& vecInts);
extern std::vector< ListNode* > ConstructTestListByParam(int start, int end, int numCount, int listCount);

extern void Test_mergeKLists();


struct TreeNode;
extern TreeNode* generateBST(int left, int right, std::vector<int>& vec);





