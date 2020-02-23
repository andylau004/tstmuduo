

#pragma once

#include <stdio.h>
#include <iostream>

#include <deque>
#include <queue>

// 《剑指 Offer》题目的 Modern C++ 实现
// https://github.com/ZhiruiLi/CodingInterviewsWithModernCpp


template < typename T >
class BSTNode {
public:
    T  _key;
    BSTNode* _lchild;
    BSTNode* _rchild;
    BSTNode* _parent;

public:
    BSTNode(T key, BSTNode* lchild, BSTNode* rchild, BSTNode* parent) :
        _key( key ),
        _lchild(lchild),
        _rchild(rchild),
        _parent(parent)
    {}
    BSTNode(T key) :
        _key( key ),
        _lchild(nullptr),
        _rchild(nullptr),
        _parent(nullptr)
    {}
};


template < typename T >
class BSTree {

private:
    BSTNode< T >* _Root;

public:
    BSTree() : _Root (nullptr) {
    }
    ~BSTree() {
    }

    void insert( T key );
    BSTNode< T >* search( T key );
    BSTNode< T >* maxNode();
    BSTNode< T >* minNode();

    T maxKey();
    T minKey();

    void preOrder();//先序输出
    void inOrder(); //中序输出
    void postOrder();//后序输出

    void print();//打印二叉树
    void remove(T key);

    BSTNode<T>* predecessor(BSTNode<T>* x);//查找某个结点的前驱
    BSTNode<T>* sucessor(BSTNode<T>* x); //查找某个结点的后继

    void destory();
    // 计算二叉树的高度
    int  Depth();

    // 计算二叉树的层数
    int  calc_tree_layers();

    // 获取跟输入节点值最接近的节点值
    T    closestValue(T key);

    // 按行打印二进制查找树
    void PrintTree_ByLine();
    // 按行打印二进制查找树
    void PrintTree_ByLine_1();

    T GetKthNode(int k);

    // 内部使用函数，供外部接口调用
private:
    BSTNode<T>*    KthNodeImpl(BSTNode<T>*& root, int& k);
    T    closestValue(BSTNode<T>*& root, T key);

    int  calc_tree_layers(BSTNode<T>*& root);
    int  Depth(BSTNode<T>*& tree);

    void insert(BSTNode<T>* &tree,BSTNode<T>* z);
    BSTNode<T>* search(BSTNode<T>* &tree,T key) const;
    void preOrder(BSTNode<T>*&tree) const;
    void inOrder(BSTNode<T>*&tree) const;
    void postOrder(BSTNode<T>*&tree) const;
    BSTNode<T>* minimumNode(BSTNode<T> *&tree);
    BSTNode<T>* maximumNode (BSTNode<T> *&tree);
    void print(BSTNode<T>*& tree);
    BSTNode<T>* remove(BSTNode<T>* &tree, BSTNode<T> *z);
    void destory(BSTNode<T>*& tree);

};


// 二叉搜索树的第K大节点
template<typename T>
T BSTree<T>::GetKthNode(int k) {
    if (_Root == NULL) return -1;

    BSTNode<T>* pNode = KthNodeImpl(_Root, k);
    if (pNode) {
        return pNode->_key;
    }
    printf( "GetKthNode failed!!!\n" );
    return -1;
}
template<typename T>
BSTNode<T>* BSTree<T>::KthNodeImpl(BSTNode<T>*& root, int& k) {
    if (root == NULL) return NULL;

    BSTNode<T>* res = NULL;
    res = KthNodeImpl( root->_lchild, k );
    std::cout << "node val=" << root->_key << std::endl;
    if (k == 1) {
        return root;
    }
    k --;
    if ( k > 0 ) {
        res = KthNodeImpl( root->_rchild, k );
        std::cout << "node val=" << root->_key << std::endl;
    }
    return res;
}
/*
*插入操作
*非递归实现
*内部使用函数
*/
template<typename T>
void BSTree<T>::insert(BSTNode<T>* &tree, BSTNode<T>* z)
{
    BSTNode< T > * parent = NULL;
    BSTNode< T > * temp = tree;

    if (tree == NULL) {
        std::cout << "first tree root" << std::endl;
    }
    while (temp) {
        parent = temp;
        if (z->_key > temp->_key) {
            temp = temp->_rchild;
        } else {
            temp = temp->_lchild;
        }
    }
    z->_parent = parent;
    if (parent == nullptr) {//如果树本来就是空树，则直接把z节点插入根节点
        tree = z;
    } else if (z->_key > parent->_key) {//如果z的值大于其双亲，则z为其双亲的右孩
        parent->_rchild = z;
    } else {
        parent->_lchild = z;
    }
}
template <typename T>
void BSTree<T>::insert(T key)
{
    //创建一个新的节点，使用构造函数初始化
    BSTNode<T>* z = new BSTNode<T>(key, nullptr, nullptr, nullptr);
    if(!z) //如果创建失败则返回
        return;
    //调用内部函数进行插入
    insert(_Root,z);
}
/*
*查找操作
*非递归实现
*内部使用函数
*/
template <typename T>
BSTNode<T>*  BSTree<T>::search(BSTNode<T>* &tree, T key) const
{
    BSTNode<T>* temp = tree;
    while ( temp ) {
        if ( key == temp->_key )
            return temp;
        else if ( key > temp->_key ) {
            temp = temp->_rchild;
        } else {
            temp = temp->_lchild;
        }
    }
    return temp;
}
template <typename T>
BSTNode<T>*  BSTree<T>::search(T key)
{
    return search(_Root,key);
}
/*
*
*前序遍历算法
*外部使用接口
*
*/
template<typename T>
void BSTree<T>::preOrder(BSTNode<T>*&tree) const {
    if ( tree ) {
        std::cout << tree->_key << " ";
        preOrder(tree->_lchild);
        preOrder(tree->_rchild);
    }
}
template<typename T>
void BSTree<T>::inOrder(BSTNode<T>*&tree) const {
    if ( tree ) {
        inOrder(tree->_lchild);
        std::cout << tree->_key << " ";
        inOrder(tree->_rchild);
    }
}
template<typename T>
void BSTree<T>::postOrder(BSTNode<T>*&tree) const {
    if ( tree ) {
        postOrder(tree->_lchild);
        postOrder(tree->_rchild);
        std::cout << tree->_key << " ";
    }
}
/*
*遍历算法
*分别为前序、中序、后序
*BSTree 类外部接口函数
*/
template<typename T>
void BSTree<T>::preOrder() {
    preOrder(_Root);
}
template<typename T>
void BSTree<T>::inOrder() {
    inOrder(_Root);
}
template<typename T>
void BSTree<T>::postOrder() {
    postOrder(_Root);
}

/*
*查找最小的结点
*内部调用函数
*/
template <typename T>
BSTNode<T>* BSTree<T>::minimumNode(BSTNode<T>*&tree)
{
    if (tree == nullptr) return nullptr;

    BSTNode< T >* temp = tree;
    while (temp->_lchild) {
        temp = temp->_lchild;
    }
    return temp;
}
template<typename T>
BSTNode<T>* BSTree<T>::minNode()
{
    return minimumNode(_Root);
}
/*
*
*查找键值最大的节点
*内部调用函数
*非递归实现
*/
template<typename T>
BSTNode<T>* BSTree<T>::maximumNode(BSTNode<T>* &tree) {
    if (tree == nullptr) return nullptr;

    BSTNode<T>* temp = tree;
    while(temp->_rchild) {
        temp = temp->_rchild;
    }
    return temp;
}
template<typename T>
BSTNode<T>* BSTree<T>::maxNode() {
    return maximumNode(_Root);
}

/*
*
*查找最小的键值
*外部接口函数
*调用内部函数minimumNode实现
*/
template<typename T>
T BSTree<T>::minKey()
{
    BSTNode<T> *temp = minimumNode(_Root);
    return temp->_key;
}

/*
*
*查找最大的键值
*外部接口函数
*调用内部函数maximumKey
*/
template<typename T>
T BSTree<T>::maxKey()
{
    BSTNode<T> *temp = maximumNode(_Root);
    return temp->_key;
}

/*
*
*打印函数
*打印出平衡二叉树
*BStree内部函数
*/
template<typename T>
void BSTree<T>::print(BSTNode<T>*& tree)
{
    if (!tree) return;

    if (tree->_lchild) {
        std::cout<<"节点"<<tree->_key<<"有左孩子为"<<tree->_lchild->_key<<std::endl;
    } else {
        std::cout<<"节点"<<tree->_key<<"无左孩子"<<std::endl;
    }
    if (tree->_rchild) {
        std::cout<<"节点"<<tree->_key<<"有右孩子为"<<tree->_rchild->_key<<std::endl;
    } else {
        std::cout<<"节点"<<tree->_key<<"无右孩子"<<std::endl;
    }
    print(tree->_lchild);
    print(tree->_rchild);
}

template<typename T>
void BSTree<T>::print() {
    print(_Root);
}

template<typename T>
int BSTree<T>::Depth(BSTNode<T>*& tree)
{
    int depth;
    int depthLeft, depthRigth;

    if (!tree) return 0;
    depthLeft  = Depth(tree->_lchild);
    depthRigth = Depth(tree->_rchild);
    depth = 1 + (depthLeft > depthRigth ? depthLeft : depthRigth);
    return depth;
}
template<typename T>
int BSTree<T>::Depth() {
    return Depth(_Root);
}

template<typename T>
int BSTree<T>::calc_tree_layers() {
    return calc_tree_layers(_Root);
}
template<typename T>
int BSTree<T>::calc_tree_layers(BSTNode<T>*& root)
{
    int left_way_layers, right_way_layers;
    if (root==NULL)    return 0;    //递归出口，空结点的层数是0
    else {
        left_way_layers  = 1 + calc_tree_layers(root->_lchild);    //从左路返上来的层数
        right_way_layers = 1 + calc_tree_layers(root->_rchild);     //从右路返上来的层数
        if(left_way_layers>right_way_layers) return left_way_layers;
        else return right_way_layers;
    }
}

// 获取 跟输入数据 最接近的节点值
template<typename T>
T BSTree<T>::closestValue(BSTNode<T>*& root, T target)
{
    T a = root->_key;
    BSTNode<T>* t = target < a ? root->_lchild : root->_rchild;
    if (!t) return a;
    T b = closestValue(t, target);
    return abs(target - a) < abs(target - b) ? a : b;
}
template<typename T>
T BSTree<T>::closestValue(T key) {
    return closestValue(_Root,key);
}

// 按行打印二进制查找树
template<typename T>
void BSTree<T>::PrintTree_ByLine_1() {
    if (!_Root) return;

    std::queue< BSTNode<T>* > qNodes;
    qNodes.push(_Root);

    int curline = 1;
    while (!qNodes.empty()) {
        int nexline = 0;

        for (; curline > 0; -- curline) {
            BSTNode<T>* pNode = qNodes.front();
            qNodes.pop();
            std::cout << pNode->_key << "\t";

            if (pNode->_lchild) {
                qNodes.push(pNode->_lchild);
                nexline ++;
            }
            if (pNode->_rchild) {
                qNodes.push(pNode->_rchild);
                nexline ++;
            }
        }
        curline = nexline;
        std::cout << "\n";
    }
}
template<typename T>
void BSTree<T>::PrintTree_ByLine() {
    if (!_Root) return;

    std::queue< BSTNode<T>* > queueNodes;

    BSTNode<T>* last  = _Root;
    BSTNode<T>* nlast = _Root;

    queueNodes.push( _Root );

    while (!queueNodes.empty()) {
        BSTNode<T>* pNode = queueNodes.front();
        queueNodes.pop();
        std::cout << pNode->_key << "\t";

        if (pNode->_lchild) {
            queueNodes.push(pNode->_lchild);
            nlast = pNode->_lchild;
        }
        if (pNode->_rchild) {
            queueNodes.push(pNode->_rchild);
            nlast = pNode->_rchild;
        }

        if (pNode == last) {
            last = nlast;
            std::cout << std::endl;
        }
    }
}

/*
*查找某个节点x的前驱
*
*外部函数调用
*
*/
template <typename T>
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T>* x)
{
    //如果x是最小的结点，则它没有前驱
    if (x->_key == minimumNode(_Root)->_key)
        return NULL;

    //否则
    //先获取二叉树中键值与x的键值相同的结点y
    BSTNode<T>* y = search(_Root, x->_key);
    if (!y)
        return nullptr;

    //如果y有左孩子，则x的前驱为“以x的左孩为根的子树的最大结点”
    if (y->_lchild)
        return maximumNode(y->_lchild);

    //如果y没有左孩子，则x有两种可能：
    //1.y是一个右孩子，此时x的前驱为其双亲节点
    BSTNode<T>* parent = y->_parent;
    if (parent->_rchild == y)
        return parent;

    //2.y是一个左孩子，则其前驱为其双亲结点中“第一个拥有右孩子结点”的结点
    while (parent && parent->_rchild == nullptr) {
        parent = parent->_parent;
    }
    return parent;
}
/*
*查找某个节点x的后继
*
*外部调用接口
*
*/
template <typename T>
BSTNode<T>* BSTree<T>::sucessor(BSTNode<T>* x)
{
    //如果x是键值最大的，则x没有后继结点
    if (x->_key == maximumNode(_Root))
        return nullptr;

    //获取x在二叉树中的结点y
    BSTNode<T>* y = nullptr;
    y = search(_Root, x->_key);
    if (!y)
        return nullptr;//若二叉树没有此结点

    //如果y有右孩子，则y的后继为其右孩子的最小结点
    if (y->_rchild)
        return minimumNode(y->_rchild);

    //如果y没有右孩子，则可分为两种情况：
    //1.y 是左孩子。此时y的后继为y的父结点
    BSTNode<T>* parent = y->_parent;
    if (parent->_lchild == y)
        return parent;

    //2.y是右孩子。此时y的后继结点为“第一个拥有左孩且不是y的直接双亲”的结点
    while (parent) {
        if (parent->_lchild && parent != y->_parent)
            return parent;
        parent = parent->_parent;
    }
    return nullptr;
}
/*
*
*删除结点
*BSTree类内部调用函数
*
*/
template <class T>
BSTNode<T>* BSTree<T>::remove(BSTNode<T>* &tree, BSTNode<T> *z)
{
    BSTNode<T> *x=NULL;
    BSTNode<T> *y=NULL;
    return nullptr;
}
template<typename T>
void BSTree<T>::remove(T key)
{
    BSTNode<T> *z, *node;
    if ((z = search(_Root, key)) != NULL)
        if ((node = remove(_Root, z)) != NULL)
            delete node;
}
/*
*销毁查找二叉树
*内部调用函数
*/
template<typename T>
void BSTree<T>::destory(BSTNode<T>*& tree)
{
    if (tree->_lchild)
        destory(tree->_lchild);
    if (tree->_rchild)
        destory(tree->_rchild);
    if (tree->_lchild && tree->_rchild) {
        delete tree;
        tree = nullptr;
    }
}
template<typename T>
void BSTree<T>::destory() {
    destory(_Root);
}


extern void tst_bst_tree();
