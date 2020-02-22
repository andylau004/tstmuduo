

#include "leetcode.h"





// Convert Sorted Array to Binary Search Tree
// Given an array where elements are sorted in ascending order, convert it to a height balanced BST.
// 思路：
// 给定一个区间[left, right]，取其中值mid=(left+right)/2对应的元素作为二叉树的根。二叉树的左子树根的值为对[left, mid-1]继续操作的结果，右子树类似。

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
    }
};

//class Solution {
//public:
    TreeNode* generateBST(int left, int right, std::vector<int>& vec) {
        if (left > right) {
            return nullptr;
        } else if (left == right) {
            return new TreeNode(vec[left]);
        } else {
            int mid1 = (left + right) >> 1;
            printf("mid1=%d\n", mid1);

//            int mid2 = left + (right - left) >> 1;
//            printf("mid2=%d\n", mid2);

            TreeNode* newnode = new TreeNode(vec[mid1]);
            newnode->left  = generateBST(0, mid1 - 1, vec);
            newnode->right = generateBST(mid1 + 1, right, vec);
            return newnode;
        }
    }
    TreeNode* sortedArrayToBst(std::vector<int>& vec) {
        generateBST(0, vec.size() - 1, vec);
    }
    void someInOrder(TreeNode* root) {
        if (!root) {
            printf("root is null\n");
            return;
        }
        if (root->left)
            someInOrder(root->left);
        std::cout << root->val << " ";
        if (root->right)
            someInOrder(root->right);
    }
//};


// Convert Sorted List to Binary Search Tree -- LeetCode 将有序链表转为二叉搜索树






















    void tst_1_lc() {
        std::vector<int> vecArr{ -10,-3,0,5,9};
        TreeNode* root = sortedArrayToBst(vecArr);
        std::cout << std::endl;
        someInOrder(root);
        std::cout << std::endl;
    }




void LeetCodeEntry() {


    tst_1_lc();

}
