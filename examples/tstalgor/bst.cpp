

#include "bst.h"

#include <stdio.h>
#include <iostream>

#include <string>
#include <string.h>

#include <vector>

using namespace std;


#include "muduo/base/common.h"

#define N 63

typedef struct node
{
    struct node *leftChild;
    struct node *rightChild;
    int data;
    node() :
        data(0), leftChild(nullptr), rightChild(nullptr) {

    }
    node(int ival) :
        data(ival), leftChild(nullptr), rightChild(nullptr) {

    }
}BiTreeNode, *BiTree;

BiTreeNode* g_newRoot = nullptr;


//生成一个结点
BiTreeNode *createNode(int i)
{
    BiTreeNode * q = new BiTreeNode;
    q->leftChild  = NULL;
    q->rightChild = NULL;
    q->data = i;
    return q;
}
void insertNode(BiTreeNode*& tree, BiTreeNode* z) {
    BiTreeNode* parent = NULL;
    BiTreeNode* temp = tree;
    while (temp) {
        parent = temp;
        if (z->data > temp->data)
            temp = temp->rightChild;
        else
            temp = temp->leftChild;
    }
    if (parent == nullptr) {
        tree = z;
    } else if (z->data > parent->data) {
        parent->rightChild = z;
    } else {
        parent->leftChild = z;
    }
}
BiTreeNode* createBiTree1()
{
//    BiTreeNode *p[N] = {NULL};
    std::vector<int> tmpV;
    tmpV.push_back(87); tmpV.push_back(13); tmpV.push_back(6);
    tmpV.push_back(99); tmpV.push_back(27); tmpV.push_back(35); tmpV.push_back(12);

    int i = 0;
    for (i = 0; i < tmpV.size(); ++i) {
        insertNode( g_newRoot, createNode(tmpV[i]/*i + 10*/) );
    }
    return g_newRoot;
//    for (i = 0; i < N/2; ++i) {
//        p[i]->leftChild = p[i*2 + 1];
//        p[i]->leftChild = p[i*2 + 2];
//    }
//    return p[0];
}
int getBiggestValue(BiTree T) {
    if (!T) {
        return 0;
    }
    int maxleft = getBiggestValue(T->leftChild);
    int maxright= getBiggestValue(T->rightChild);
    std::cout << "maxleft=" << maxleft << ", maxright=" << maxright << std::endl;
    int max = maxleft > maxright ? maxleft : maxright;
    std::cout << "max=" << max << std::endl;
    return max > T->data ? max : T->data;
}
void inorderPrint(BiTree root) {
    if (!root) return;
    if (root->leftChild)
        inorderPrint(root->leftChild);
    std::cout << root->data << " ";
    if (root->rightChild)
        inorderPrint(root->rightChild);
}
void tst_bst_1() {
    BiTreeNode* T1 = createBiTree1();
    std::cout << "lastMax=" << getBiggestValue(T1) << std::endl;

    std::cout << "-------------inOrder print beg" << std::endl;
    inorderPrint(T1);
    std::cout << std::endl;
    std::cout << "-------------inOrder print end" << std::endl;
}

//给出如下一棵二叉树：

//     1
//   /   \
// -5     2
// / \   /  \
//0   3 -4  -5
//返回值为 3 的节点。

BiTreeNode* bst_2_maxnode(BiTreeNode* root) {
    if (!root) return nullptr;

    BiTreeNode *left, *right;
    left = right = root;

    if (root->leftChild)
        left = bst_2_maxnode(root->leftChild);
    if (root->rightChild)
        right = bst_2_maxnode(root->rightChild);

    BiTreeNode *childMax = left->data > right->data ? left : right;
    return root->data > childMax->data ? root : childMax;
}
void tst_bst_2() {
    BiTreeNode* n0 = createNode(1);

    BiTreeNode* n___5;
    BiTreeNode* n___2;
    {
        BiTreeNode* n1 = createNode(-5);
        BiTreeNode* n2 = createNode(2);
        n0->leftChild = n1; n0->rightChild = n2;
        n___5 = n1;
        n___2 = n2;
    }
    {
        BiTreeNode* n1 = createNode(0);
        BiTreeNode* n2 = createNode(3);
        n___5->leftChild = n1; n___5->rightChild = n2;
    }
    {
        BiTreeNode* n1 = createNode(-4);
        BiTreeNode* n2 = createNode(-5);
        n___2->leftChild = n1; n___2->rightChild = n2;
    }

    BiTreeNode* ret = bst_2_maxnode(n0);
    std::cout << "ret=" << ret->data << std::endl;

    std::cout << "-------------n0 inOrder print beg" << std::endl;
    inorderPrint(n0);
    std::cout << std::endl;
    std::cout << "-------------n0 inOrder print end" << std::endl;

}

void tst_bst_3() {
    BSTree<int> s;
    std::vector< int > vec_data = { 5, 3, 7, 2, 4, 6, 8 };
    for (int i = 0; i < vec_data.size(); ++i)
        s.insert(vec_data[i]);

    std::cout << "in order traverse:" << std::endl;
    s.inOrder();
    std::cout << std::endl;
}

// 专门进行翻转的函数
void reverse_string(std::string& str) {
    if (str.size() <= 1)return;
    int low = 0, high= str.size() - 1;

    while (low < high) {
        char tmpval = str[low];
        str[low]  = str[high];
        str[high] = tmpval;
        low ++; high --;
    }

    {
        //    printf( "pBeg=%s\n", pBeg);
        //    printf( "pEnd=%s\n", pEnd);

        //    if (!pBeg || !pEnd) return;

        //    while (pBeg < pEnd) {
        //        char tmpChar = *pBeg;
        //        *pBeg = *pEnd;
        //        *pEnd = tmpChar;
        //        pBeg ++; pEnd --;
        //    }
    }
}
std::string reverse_line(std::string& strline) {
    std::string ret;

    // 首先进行整句的翻转
    reverse_string(strline);
    std::cout << "strline=" << strline.c_str() << std::endl;

    // 进行单词的翻转，每找到一个空格说明找到了一个单词，此时取出该单词进行翻转，并将结果添加到resu的后面
    int left = 0;
    int i    = 0;
    for (; i < strline.size(); ++i) {
        if (strline[i] == ' ') {
            std::string tmp = strline.substr(left, i-left);
            reverse_string(tmp);
            ret += (tmp + " ");
            left = i + 1;
        }
    }
    std::string tmp = strline.substr(left, i-left);
    reverse_string(tmp);
    ret += (tmp);
    strline = ret;
    return ret;
}
void tst_reverse_line() {
    char  tmpString[1024] = { "It's am a student." };
    char* pbeg = tmpString;
    char* pend = tmpString + strlen(tmpString) - 1;

    std::string tmpstr = tmpString;
    reverse_line(tmpstr);
    std::cout << "test string=" << tmpstr.c_str() << std::endl;
}

int max_stock_value(int* arr, int length) {
    int min = arr[0];
    int maxIncome = arr[1] - min;

    for (int i = 2; i < length; ++i) {
        //检查前一天的值是不是最小值，是就更新最小值
        if (arr[i-1] < min)
            min = arr[i-1];
        if ((arr[i] - min) > maxIncome) {
            maxIncome = (arr[i] - min);
        }
    }
    return maxIncome;
}
// 1.可以转换成为求最大子数组问题，
// 数组相邻的数后一个减去前一个减得到股票的变化数组{2，-3，-3,2，5,4，-2}。
// 这样问题转换成了求数组的最大子数组问题。
// 2.也可以在遍历数组时记录下遍历到某个节点能够买入的最低价格，
// 在当前价格确定的情况下，在最低价格买入能够赚最多。这样遍历一次数组就能得到结果，时间复杂度为O（n）。
void tst_max_stock_value () {
    int numbers[] = { 4, 1, 3, 2, 5 };
    std::cout << "max=" << max_stock_value(numbers, array_size(numbers)) << std::endl;
}

void tst_KthNode() {
    BSTree<int> s;
    std::vector< int > vec_data = { 5, 3, 7, 2, 4, 6, 8 };
    for (int i = 0; i < vec_data.size(); ++i)
        s.insert(vec_data[i]);

    std::cout << "s.GetKthNode=" << s.GetKthNode(7) << std::endl;
    std::cout << std::endl;
}
void tst_bst_tree() {
//    tst_max_stock_value(); return;
//    tst_KthNode(); return ;
//    tst_reverse_line(); return;

//    tst_bst_3();
//    return;
//    tst_bst_2();
//    return;
//    tst_bst_1();
//    return;

    BSTree<int> s;
//    std::vector< int > vec_data = { 10922, 33, 79, 7890, 7, 1, 4, 2 };
    std::vector< int > vec_data = { 5, 3, 7, 2, 4, 6, 8 };
    for (int i = 0; i < vec_data.size(); ++i) {
        s.insert(vec_data[i]);
    }
    {
        s.PrintTree_ByLine_1();
        s.PrintTree_ByLine();
    }
return ;
//    while(std::cin>>a )
//        s.insert(a);
//    std::cin.clear();

    std::cout << "pre order traverse:" << std::endl;
    s.preOrder();
    std::cout << std::endl;

    cout<<"in order traverse:"<<endl;
    s.inOrder();
    cout<<endl;

    cout<<"post order traverse:"<<endl;
    s.postOrder();
    cout<<endl;

    cout<<"print binary search tree ------------- beg"<<endl;
    s.print();
    cout<<endl;
    cout<<"print binary search tree ------------- end"<<endl;

//    std::cin.clear();
//    std::cout << "xx=" << 89 << std::endl;

//    while ( std::cin >> a ) {// while beg
//        std::cout << "a=" << a << std::endl;

//        BSTNode< int >* findNode = s.search(a);
//        if ( !findNode ) {

//        } else {
//            cout<<findNode->_key<<"查找成功"<<endl;
//        }
//    }// while end
//    std::cin.clear();

    BSTNode<int>* minNode = s.minNode();
    if (minNode)
        cout<<"min node key="<<minNode->_key<<endl;

    BSTNode<int>* maxNode = s.maxNode();
    if (maxNode)
        cout<<"max node key="<<maxNode->_key<<endl;

    int iDepth = s.Depth();
    cout<<"iDepth="<<iDepth<<endl;
    {
        int calc_tree_layers = s.calc_tree_layers();
        cout<<"calc_tree_layers="<<calc_tree_layers<<endl;
    }

    {
        std::cout << "closet value1=" << s.closestValue(123) << std::endl;
        std::cout << "closet value2=" << s.closestValue(1) << std::endl;
    }

}
