

#pragma once





#include <stdio.h>
#include <iostream>

#include <cstdio>

#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>

#include <sys/time.h>
#include <unistd.h>

#include <boost/bind.hpp>


#include <iterator>     // ostream_iterator
#include <algorithm>    // unique，distance
#include <memory>


#include "muduo/base/BinaryTree.h"
#include "muduo/base/common.h"

#include "List.h"


using namespace std;






struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
    }
};



extern bool isBalanced(BstNode* root);

extern void lc_Entry();

extern void CreateBstTree();
extern void CreateBstTree(const std::vector<int>& inVec);
extern void inOrderNonRecru(BstNode* root);

extern ListNode* ConstructTestList(const std::vector<int>& vecInts);
extern std::vector< ListNode* > ConstructTestListByParam(int start, int end, int numCount, int listCount);



extern TreeNode* generateBST(int left, int right, std::vector<int>& vec);

extern void TstList1();


