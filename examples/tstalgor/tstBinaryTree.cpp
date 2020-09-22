
#include "tstBinaryTree.h"


#include <stdio.h>
#include <iostream>

#include <cstdio>

#include <set>
#include <queue>
#include <sys/time.h>
#include <unistd.h>



#include <iterator>     // ostream_iterator
#include <algorithm>    // unique，distance


#include "muduo/base/BinaryTree.h"


using namespace std;




void print_my_ex(BinaryTreeNode* pRoot) {
    if (pRoot == NULL) return ;
    std::queue<BinaryTreeNode*> nodes;
    nodes.push(pRoot);

    int lineNodeCount = 0;
    int printTag = 1;

    while (!nodes.empty()) {
        BinaryTreeNode* pNode = nodes.front();
        printf("%d ", pNode->m_nValue);

        if (pNode->m_pLeft) {
            nodes.push(pNode->m_pLeft);
            ++ lineNodeCount;
        }
        if (pNode->m_pRight) {
            nodes.push(pNode->m_pRight);
            ++ lineNodeCount;
        }

        nodes.pop();
        --printTag;
        if (printTag == 0) {
            printf( "\n" );

            printTag = lineNodeCount;
            lineNodeCount = 0;
        }
    }
}

// 按层遍历二叉树
void Print(BinaryTreeNode* pRoot)
{
    if(pRoot == NULL) return;

    std::queue<BinaryTreeNode*> nodes;
    nodes.push(pRoot);

    int nextLevel = 0;
    int toBePrinted = 1;

    while(!nodes.empty())
    {
        BinaryTreeNode* pNode = nodes.front();
        printf("%d ", pNode->m_nValue);

        if(pNode->m_pLeft != NULL)
        {
            nodes.push(pNode->m_pLeft);
            ++nextLevel;
        }
        if(pNode->m_pRight != NULL)
        {
            nodes.push(pNode->m_pRight);
            ++nextLevel;
        }

        nodes.pop();
        --toBePrinted;
        if(toBePrinted == 0)
        {
            printf("\n");
            toBePrinted = nextLevel;
            nextLevel = 0;
        }
    }
}
void PrintEx(BinaryTreeNode* pRoot) {
    BinaryTreeNode* p;
    std::queue<BinaryTreeNode*> q;

    if (!pRoot) return;

    q.push(pRoot);

    while (!q.empty()) {
        p = q.front(); q.pop(); printf("%d ", p->m_nValue);
        if (p->m_pLeft)
            q.push(p->m_pLeft);
        if (p->m_pRight)
            q.push(p->m_pRight);
    }

}
// 按行遍历，需要自己加入0
void PrintMy(BinaryTreeNode* pRoot) {
    BinaryTreeNode* node = NULL;
    std::queue<BinaryTreeNode*> q;

    if (!pRoot) return;
    q.push(pRoot);
    q.push(0);

    do {
        node = q.front(); q.pop();
        if (node) {
            printf("%d ", node->m_nValue);

            if (node->m_pLeft)
                q.push(node->m_pLeft);
            if (node->m_pRight)
                q.push(node->m_pRight);
        } else if (!q.empty()) {
            q.push( 0 );
            std::cout << std::endl;
        }
    } while (!q.empty());

}

//void PrintExxxx(BinaryTreeNode* pRoot) {
//    BinaryTreeNode* queue[20]={0};
//    BinaryTreeNode* p = NULL;
//    int front, rear;
//    if (pRoot) {
//        queue[0] = pRoot;
//        front = -1;
//        rear = 0;
//        while (front < rear) {
//            p = queue[++front];
//            printf("%d ", p->m_nValue);

//            if (p->m_pLeft)
//                queue[++rear] = p->m_pLeft;
//            if (p->m_pRight)
//                queue[++rear] = p->m_pRight;
//        }
//    }
//}



// ====================测试代码====================
//            8
//        6      10
//       5 7    9  11
void ts_PrintLines()
{
    BinaryTreeNode* pNode8 = CreateBinaryTreeNode(8);
    BinaryTreeNode* pNode6 = CreateBinaryTreeNode(6);
    BinaryTreeNode* pNode10 = CreateBinaryTreeNode(10);
    BinaryTreeNode* pNode5 = CreateBinaryTreeNode(5);
    BinaryTreeNode* pNode7 = CreateBinaryTreeNode(7);
    BinaryTreeNode* pNode9 = CreateBinaryTreeNode(9);
    BinaryTreeNode* pNode11 = CreateBinaryTreeNode(11);

    ConnectTreeNodes(pNode8, pNode6, pNode10);
    ConnectTreeNodes(pNode6, pNode5, pNode7);
    ConnectTreeNodes(pNode10, pNode9, pNode11);

    printf("====Test1 Begins: ====\n");
    printf("Expected Result is:\n");
    printf("8 \n");
    printf("6 10 \n");
    printf("5 7 9 11 \n\n");

    printf("Print Actual Result is: \n");
    Print(pNode8);
    printf("\n");


    printf("PrintEx Actual Result is: \n");
    PrintEx(pNode8);
    printf("\n");

    printf("print_my_ex Actual Result is: \n");
    print_my_ex(pNode8);
    printf("\n");//

    printf("PrintExxxx Actual Result is: \n");
//    PrintExxxx(pNode8);
//    printf("\n");
    DestroyTree(pNode8);
}



const int N = 10;

// 大顶堆
class MaxHeap {
public:
    std::vector<int> m_arr;
    int m_curlen;

public:
    MaxHeap(int n) : m_curlen(0) {
        m_arr.reserve(n+1);

        for(size_t i=0; i< m_arr.capacity(); i++) {// 依次删除堆中的最大元素并输出
            m_arr[i] = 0;
        }

        std::cout << "init count=" << m_arr.size() << ", captiy=" << m_arr.capacity() << std::endl;
        traverse();
    }
    void traverse() {
        std::cout << "traverse begin ------------" << std::endl;
        for(size_t i=0; i< m_arr.size(); i++) {// 依次删除堆中的最大元素并输出
            std::cout << "val=" << m_arr[i] <<std::endl;
        }
        std::cout << "traverse end   ------------" << std::endl;
    }
    void exchange(int i, int j) {
        std::swap(m_arr[i], m_arr[j]);
    }
    /**
     * @param a 表示堆的数组
     * @param k 堆中的节点位置
     * @param N 堆中节点总数
     * 上浮操作
     *        arr == > 大顶堆
    */
    void Swim_Heap(int k) {
        while ( k > 1 && m_arr[k] > m_arr[k/2] ) {// 当该结点存在父节点，且大于父节点的时候
            printf( "exchange k=%d k/2=%d\n", k, k/2 );
            exchange(k, k/2);// 交换它和它的父节点值
            k = k/2;// 取得父节点的位置
        }
    }
    /**
     * @param a 表示堆的数组
     * @param k 堆中的节点位置
     * @param N 堆中节点总数
     * 下沉
     *        arr == > 大顶堆
    */
    void Sink_Heap(int k) {

        while (m_curlen >= 2*k) {// 当该节点存在至少一个子节点的时候
            int j = 2*k;// 取得左儿子的位置

            if ( j < m_curlen && m_arr[j] < m_arr[j+1] ) ++j;// 取得左儿子和右儿子中的较大者
            if ( m_arr[k] < m_arr[j] ) {// 当该节点的值小于较大的左儿子的时候
                exchange(k, j);// 交换它和该儿子节点的值
                k = j;// 取得该儿子节点的位置
            } else { break; }
        }// end while

    }
    /**
     * 向堆中插入值
     * @param v 要插入的值
     */
    void insert(int v) {
        m_arr[m_curlen + 1]= v;  // 元素被放入堆的最末端成为新节点
        std::cout << "m_arr[" << m_curlen + 1 << "]=" << m_arr[m_curlen + 1] << std::endl;

        m_curlen++;  // 增加堆的大小
        Swim_Heap(v); // 对末端节点进行上浮操作使其有序

        std::cout << "arr size=" << m_arr.size() << std::endl;
//        std::cout << "init count=" << m_arr.size() << ", captiy=" << m_arr.capacity() << std::endl;
    }
    /**
     * 删除堆中的最大值， 并且将其返回
     * @return 堆节点最大值
     */
    int delMax () {
        if (m_arr.empty())
            std::cout << "is null heap array" << std::endl;

        if (m_arr.empty()) return 0; // 当堆为空时， 返回
        int max = m_arr[1];  //  取得堆中根节点（最大值）
        exchange(1, m_curlen);   // 交换根节点和末端节点（最后一个元素）的值
        m_curlen--;  // 减少堆的大小 （“删除”完毕）
        Sink_Heap(1); // 下沉操作，让刚放上根节点的新元素下沉到合适的位置
        return max;
    }

};

void tst_maxheap_sort() {
//    std::vector<int> tmpV;
//    for (int i1 = 0; i1 < 3 ; ++ i1) {
//        tmpV.push_back(i1);
//    }
//    for(int i=0; i< tmpV.size(); i++) {// 依次删除堆中的最大元素并输出
//        std::cout << "val=" << tmpV[i] <<std::endl;
//    }
//    return ;

    int array[] = {2,6,3,9,1,5,4,3,0,2};
    int iLength = sizeof(array)/sizeof(int);
    std::cout << "length=" << iLength << std::endl;

    MaxHeap maxHeapObj(11);
    for(int i=0; i < iLength; i++) {// 将数组元素依次放入堆中
        maxHeapObj.insert(array[i]);
    }
    maxHeapObj.traverse();
    return;

    for(int i=0; i < iLength; i++) {// 依次删除堆中的最大元素并输出
        std::cout << "delMax=" << maxHeapObj.delMax() <<std::endl;
    }
    return;

//    vector<int> vect;
//    vect.resize(100);    //分配100个空间
//    vect.push_back(1);
//    vect.push_back(2);
//    vect.push_back(3);
//    vect.push_back(4);
////    cout<<vect.size()<<endl; //现在size和capacity都是104
//    int i = 0;
//    for (i = 0; i < 104; i++)
//    {
//        cout<<vect[i]<<endl;
//    }
//    return;

    std::vector<int> arrInt;
    arrInt.reserve(10);

    srand(time(NULL));

    for (int i = 0; i < 10; ++ i ) {
        arrInt.push_back( rand()%(10) );
        std::cout << "index_" << arrInt[i] << std::endl;
    }
    return;

    {
        srand(time(NULL));

        int count = 10;
        for (int i = 0; i < count; ++i) {
            int r = rand()%(10);
//            arrInt.insert(arrInt.end(), r);
            arrInt.push_back(r);
            std::cout << "rand=" << r << std::endl;
        }

        for (int i = 0; i < 10; ++ i ) {
            std::cout << "index_" << i << "=" << arrInt[i] << std::endl;
        }
    }


}



class CTest {
private:
    int m_val;
public:
    CTest() {
        m_val = 0;
    }
    CTest(int inVal) {
        m_val = inVal;
    }
    virtual ~CTest() {

    }
    bool operator ==(const CTest& other) const {
        return (this->m_val == other.m_val);
    }
    bool operator <(const CTest& other) const {
        return (this->m_val < other.m_val);
    }
    bool operator >(const CTest& other) const {
        return (this->m_val > other.m_val);
    }
    int getval() { return m_val; }
};

//bool compare_test(const CTest& t1, const CTest& t2) {
//    return (t1 < t2);
//}

// 测试 vector 去重
void tst_vec_quchong() {
    std::vector<CTest> vec_Tests{ CTest(123), CTest(222), CTest(123), CTest(123), CTest(666) };

    std::cout << "size=" << vec_Tests.size() << std::endl;

//    std::sort(vec_Tests.begin(), vec_Tests.end(), compare_test);
    std::set<CTest> setCpy(vec_Tests.begin(), vec_Tests.end());
    vec_Tests.clear();
    vec_Tests.assign(setCpy.begin(), setCpy.end());

//    auto it_end = std::unique(vec_Tests.begin(), vec_Tests.end());
//    vec_Tests.erase(it_end, vec_Tests.end());

    std::cout << "last size=" << vec_Tests.size() << std::endl;
    for (auto it : vec_Tests) {
        std::cout << "val=" << it.getval() << std::endl;
    }

}


typedef struct _tag_struct_1
{
    std::string serverIP;
    _tag_struct_1() {}
}struct_1;
typedef struct _tag_struct_2
{
    std::string serverIP;
    _tag_struct_2() {}
}struct_2;


template <class T>
void AssignKeyAndUser(T& dataStruct, std::string& strKeyFile, std::string& strUser) {
    strKeyFile.clear(); strUser.clear();
    if (dataStruct.serverIP == "172.16.8.153") {
        strKeyFile = std::string(" -i ") + "12" + "/login153";
        strUser = "root";
    } else {
        strUser = "ca_bxy";
    }
}

void tst_struct_fun() {

    std::string strKeyFile, strUser;
    struct_1 st1;
    st1.serverIP = "172.16.8.153";
    struct_2 st2;
    st2.serverIP = "172.16.8.151";

    AssignKeyAndUser(st1, strKeyFile, strUser);
    std::cout << "key=" << strKeyFile << ", user=" << strUser << std::endl;
    AssignKeyAndUser(st2, strKeyFile, strUser);
    std::cout << "key=" << strKeyFile << ", user=" << strUser << std::endl;

}




