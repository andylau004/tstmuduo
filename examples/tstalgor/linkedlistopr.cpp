

#include "linkedlistopr.h"


#include <stdio.h>

#include <iostream>
#include <unordered_map>

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

struct TreeNode;
void PrintDoubleLinkedList(TreeNode* pHeadOfList);



int vec_partition( std::vector< int >& vec_data, int left, int right ) {
    int flag = vec_data[left];
    int i = left;
    int j = right+1;

    while (1) {
        while ( vec_data[--j] > flag ) {
        }
        while ( vec_data[++i] < flag ) {
            if (i == right) break;
        }
        if (i >= j)
            break;
        else
            std::swap( vec_data[i], vec_data[j] );
    }
    std::swap( vec_data[left], vec_data[j] );
    return j;
}
// vector形式 快排
void vec_qsort( std::vector< int >& v, int head, int tail ) {
//    int pivotLoc ;
//    if (left < right) {
//        pivotLoc = vec_partition( v, head, tail );

//        vec_qsort( v, head, pivotLoc - 1 );
//        vec_qsort( v, pivotLoc + 1, tail );
//    }
    if (head >= tail) return;
    int i = head;
    int j = tail;
    int piv = v[i];

    while (i < j) {
        while (i < j && v[j] > piv) j --;
        if (i < j) v[ i ++ ] = v[ j ];

        while (i < j && v[i] <= piv) i ++;
        if (i < j) v[ j -- ] = v[i];
    }
    v[i] = piv;
    vec_qsort( v, head, i - 1 );
    vec_qsort( v, j + 1, tail );
}

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};


ListNode* swapPairs(ListNode* head) {
    if(head == NULL)
        return NULL;
    ListNode* first = head;
    ListNode* second = head->next;
    while(first!=NULL && second!=NULL) {
        int temp = first->val;      // 交换val
        first->val = second->val;
        second->val = temp;
        if(first->next!=NULL)
            first = first->next->next;
        if(second->next!=NULL)
            second = second->next->next;
    }
    return head;
}

void insertBack_1(ListNode** head, ListNode** tail,  ListNode* n) //从尾部插入
{
    if (n)
    {
        if (*head == NULL)
        {
            *head = n ;
            *tail = n ;
        }
        else
        {
            (*tail)->next = n ;
            *tail = n ;
        }
    }
}
// 翻转list临近节点
void reverse_linjin_node() {
    ifstream in("data.txt") ;
    ListNode* head = NULL ;
    ListNode* tail = NULL ;
    int val ;

    while(in >> val)
    {
        ListNode*tmp = new ListNode(val) ;
        insertBack_1(&head, &tail, tmp) ;
    }

    std::cout << "first print " << std::endl;
    while(head)
    {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;

    std::cout << "second print " << std::endl;
    swapPairs( head );
    while(head)
    {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}








//二叉树的树结构
struct TreeNode{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int t) : val(t), left(NULL), right(NULL) {}
};

//有序数组转成平衡二叉树
//也是一个递归的过程，每次都把数组分为两个部分，
//作为左子树和右子树。
TreeNode* BST(std::vector<int>& nums, int s, int e) {
    {
        if (s > e) return NULL;
        int midPos = (s+e) / 2;
        TreeNode* newNode = new TreeNode( nums[midPos] );

        newNode->left = BST(nums, s, midPos - 1);
        newNode->right= BST(nums, midPos + 1, e);
        return newNode;
    }
    {
        if (s > e) return NULL;

        int mid = (s + e) / 2;
        TreeNode* pNewNode = new TreeNode(nums[mid]);
        pNewNode->left = BST(nums, s, mid - 1);
        pNewNode->right= BST(nums, mid + 1, e);
        return pNewNode;
    }
}
TreeNode* sortedArray2BST(vector<int>& nums) {
    return BST(nums, 0, nums.size() - 1);
}

void qianxu_traverse( TreeNode* pRoot ) {
    if (pRoot) {
        std::cout << pRoot->val << std::endl;
        if (pRoot->left)
            qianxu_traverse(pRoot->left);
        if (pRoot->right)
            qianxu_traverse(pRoot->right);
    }
}
void zhongxu_traverse( TreeNode* pRoot ) {
    if (pRoot) {
        if (pRoot->left)
            zhongxu_traverse(pRoot->left);
        std::cout << pRoot->val << std::endl;
        if (pRoot->right)
            zhongxu_traverse(pRoot->right);
    }
}

void reverseOrderTree( TreeNode* pRoot ) {
    if (pRoot) {
        if ( pRoot->right )
            reverseOrderTree( pRoot->right );
        std::cout << pRoot->val << std::endl;
        if ( pRoot->left )
            reverseOrderTree( pRoot->left );
    }
}



// ====================辅助测试代码====================
void ConvertNode(TreeNode* pNode, TreeNode** ppLastNodeInList);

TreeNode* CreateBinaryTreeNode_1(int iVal) {
    return new TreeNode(iVal);
}
void ConnectTreeNodes(TreeNode* pParent, TreeNode* pLeft, TreeNode* pRight) {
    if(pParent) {
        pParent->left  = pLeft;
        pParent->right = pRight;
    }
}
void PrintTreeNode(TreeNode* pNode) {
    if(pNode) {
        printf("value of this node is: %d\n", pNode->val);
        if(pNode->left)
            printf("value of its left child is: %d.\n", pNode->left->val);
        else
            printf("left child is null.\n");
        if(pNode->right)
            printf("value of its right child is: %d.\n", pNode->right->val);
        else
            printf("right child is null.\n");
    }
    else {
        printf("this node is null.\n");
    }
    printf("\n");
}
void PrintTree(TreeNode* pRoot) {
    PrintTreeNode(pRoot);
    if(pRoot) {
        if(pRoot->left)
            PrintTree(pRoot->left);
        if(pRoot->right)
            PrintTree(pRoot->right);
    }
}
TreeNode* Convert( TreeNode* pRoot ) {
    TreeNode* pLastNode = NULL;//指向双向链表的尾结点
    ConvertNode(pRoot, &pLastNode);//转换排序二叉树为双向链表

    TreeNode* pHeadOfList = pLastNode;
    while(pHeadOfList && pHeadOfList->left)
        pHeadOfList = pHeadOfList->left;
    return pHeadOfList;
}
void ConvertNode(TreeNode* pNode, TreeNode** pLastNodeInList) {
    if(pNode == nullptr)
        return;

    TreeNode *pCurrent = pNode;

    if (pCurrent->left != nullptr)
        ConvertNode(pCurrent->left, pLastNodeInList);

    pCurrent->left = *pLastNodeInList;

    if(*pLastNodeInList != nullptr)
        (*pLastNodeInList)->right = pCurrent;

    *pLastNodeInList = pCurrent;

    if (pCurrent->right != nullptr)
        ConvertNode(pCurrent->right, pLastNodeInList);
}
void Test(char* testName, TreeNode* pRootOfTree) {
    if(testName != nullptr)
        printf("%s begins:\n", testName);

    PrintTree(pRootOfTree);

    TreeNode* pHeadOfList = Convert(pRootOfTree);

    PrintDoubleLinkedList(pHeadOfList);
}
void PrintDoubleLinkedList(TreeNode* pHeadOfList)
{
    TreeNode* pNode = pHeadOfList;

    printf("The nodes from left to right are:\n");
    while(pNode != nullptr)
    {
        printf("%d\t", pNode->val);

        if(pNode->right == nullptr)
            break;
        pNode = pNode->right;
    }

    printf("\nThe nodes from right to left are:\n");
    while(pNode != nullptr)
    {
        printf("%d\t", pNode->val);

        if(pNode->left == nullptr)
            break;
        pNode = pNode->left;
    }

    printf("\n");
}

void insert_treenode( TreeNode* pTree, int iVal ) {
    TreeNode* pNew = new TreeNode(iVal);

    TreeNode* parent = NULL;
    TreeNode* temp   = pTree;

    while (temp) {
        parent = temp;
        if (pNew->val > temp->val) {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }


}
void tst_link_1() {

    // 先对数组容器排序，把数组转换为二叉树，二叉树转换为双向链表
    std::vector< int > vec_data = { 10922, 33, 79, 7654, 7890, 7, 21, 1986, 1, 2, 4, 654, 543 };
    vec_qsort( vec_data, 0, vec_data.size() - 1 );

    std::cout << "-----------------vec qsort beg" << std::endl;
    PrintInContainer( vec_data );
    std::cout << "-----------------vec qsort end" << std::endl;

    // 排序的数组转换为二进制查找树
    TreeNode* pBst = sortedArray2BST( vec_data );
    if (!pBst) { std::cout << "sortarray2bst failed!!!" << std::endl; return;}

    // 前序: 根左右; 中序: 左根右; 后序: 左右根;
    std::cout << "-----------------qianxu_traverse beg" << std::endl;
    qianxu_traverse( pBst );
    std::cout << "-----------------qianxu_traverse end" << std::endl;

    // 中序　左根右　遍历二叉树 升序
    std::cout << "-----------------zhongxu_traverse beg" << std::endl;
    zhongxu_traverse( pBst );
    std::cout << "-----------------zhongxu_traverse end" << std::endl;

    // 右根左　遍历二叉树 降序
    std::cout << "-----------------reverseOrderTree beg" << std::endl;
    reverseOrderTree( pBst );
    std::cout << "-----------------reverseOrderTree end" << std::endl;
    return;

    TreeNode* pNode10 = CreateBinaryTreeNode_1(10);
    TreeNode* pNode6  = CreateBinaryTreeNode_1(6);
    TreeNode* pNode14 = CreateBinaryTreeNode_1(14);
    TreeNode* pNode4  = CreateBinaryTreeNode_1(4);
    TreeNode* pNode8  = CreateBinaryTreeNode_1(8);
    TreeNode* pNode12 = CreateBinaryTreeNode_1(12);
    TreeNode* pNode16 = CreateBinaryTreeNode_1(16);

    ConnectTreeNodes(pNode10, pNode6, pNode14);
    ConnectTreeNodes(pNode6, pNode4, pNode8);
    ConnectTreeNodes(pNode14, pNode12, pNode16);

    Test("test", pNode10);
}

void tst_linkedlist_entry() {
    tst_link_1();
    return;

    reverse_linjin_node();
}


