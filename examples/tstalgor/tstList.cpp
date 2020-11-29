



#include "tstList.h"


using namespace std;



extern ListNode* mergeTwoList(ListNode* l1, ListNode* l2);






void PrintList(ListNodeEx* head) {
    ListNodeEx* p = head;
    while (p) {
        std::cout << p->val << " ";
        p = p->next;
    }
    std::cout << std::endl;
}

//递归方式：实现单链表反转
ListNodeEx* Recrusive_ReverseList(ListNodeEx* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
//    std::cout << "val=" << head->val << std::endl;
    ListNodeEx* newHead = Recrusive_ReverseList(head->next);// 倒数第二个节点 == > newHead
    head->next->next = head;//将后一个链表结点指向前一个结点
    head->next = NULL;//将原链表中前一个结点指向后一个结点的指向关系断开
    return newHead;
}

//非递归方式：实现单链表反转
ListNodeEx* NoRecrusive_ReverseList(ListNodeEx* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }
    ListNodeEx* pre = head;
    ListNodeEx* cur = head->next;
    ListNodeEx* tmp = NULL;
    while (cur) {
        tmp = cur->next;//temp作为中间节点，记录当前结点的下一个节点的位置
        cur->next = pre;//当前结点指向前一个节点
        pre = cur;//指针后移
        cur = tmp;//指针后移，处理下一个节点
    }
    head->next = NULL;// while结束后，将翻转后的最后一个节点（即翻转前的第一个结点head）的链域置为NULL
    return pre;
}

// 插入一个新节点到链表中(放在链表头部)
void CreateList(ListNodeEx*& pIn, int newVal) {
    ListNodeEx* pNewNode = new ListNodeEx;
    pNewNode->val  = newVal;
    pNewNode->next = NULL;
    if (pIn == NULL) {
        pIn = pNewNode;
        return;
    }
    pNewNode->next = pIn;
    pIn = pNewNode;
}
void tst_reverse_list_1() {
    ListNodeEx* phead = NULL;

    for (int i = 0 ; i < 9 ; ++ i) {
        CreateList(phead, i);
    }

    PrintList(phead);
//    phead = Recrusive_ReverseList(phead);
    phead = NoRecrusive_ReverseList(phead);
    PrintList(phead);
}


// 快慢指针方式
// 查找单链表中倒数第K个结点
ListNodeEx* RGetKthNode(ListNodeEx* pHead, int k) {
    if (!pHead || k <= 0) // 这里k的计数是从1开始的，若k为0或链表为空返回NULL
        return NULL;

    ListNodeEx* pQuick = pHead;
    ListNodeEx* pSlow  = pHead;

    while (k > 0 && pQuick) {
        pQuick = pQuick->next; // 前面的指针先走到正向第k个结点
        k --;
    }
    if (k > 1 || pQuick == NULL) {
        return NULL;
    }
    while (pQuick) {
        pSlow = pSlow->next;
        pQuick= pQuick->next;
    }
    return pSlow;
}
void tst_RGetKthNode_fun() {
    ListNodeEx* phead = NULL;

    for (int i = 0 ; i < 9; ++ i )
        CreateList(phead, i);

    PrintList(phead);
    std::cout << "RGetKthNode ret=" << RGetKthNode(phead, 3)->val << std::endl;
}

//查找单链表的中间结点
ListNodeEx* GetMiddleNode(ListNodeEx* phead) {
    if (!phead || !phead->next) return NULL;

    ListNodeEx* pQuick = phead;
    ListNodeEx* pSlow  = phead;

    while (pQuick->next) {
        pQuick = pQuick->next;
        pSlow  = pSlow->next;
        if (pQuick->next)
            pQuick = pQuick->next;
    }
    return pSlow;
    return NULL;
}
void tst_GetMiddleNode_fun() {
    ListNodeEx* phead = NULL;
    for (int i = 0 ; i < 9; ++ i )
        CreateList(phead, i);
    PrintList(phead);
    std::cout << "GetMiddleNode ret=" << GetMiddleNode(phead)->val << std::endl;
}

//5. 从尾到头打印单链表
void RPrintList(ListNodeEx* phead) {
    if (!phead || !phead->next) return ;

    std::stack<int> stackVal;
    ListNodeEx* pNode = phead;
    while (pNode) {
        stackVal.push(pNode->val);
        pNode = pNode->next;
    }
    while (!stackVal.empty()) {
        int val = stackVal.top();
        printf("%d ", val);
        stackVal.pop();
    }
    printf("\n");
}
void tst_RPrintList_fun() {
    ListNodeEx* phead = NULL;
    for (int i = 0 ; i < 9; ++ i )
        CreateList(phead, i);
    RPrintList(phead);
//    std::cout << "GetMiddleNode ret=" << RPrintList(phead)->val << std::endl;
}



//已知两个单链表pHead1 和pHead2 各自有序，把它们合并成一个链表依然有序
ListNodeEx* MergeTwoList(ListNodeEx* pHead1, ListNodeEx* pHead2) {
    if (!pHead1) return pHead2;
    if (!pHead2) return pHead1;

    ListNodeEx* pMerged = NULL;
    if (pHead1->val < pHead2->val) {
        pMerged = pHead1;
        pMerged->next = MergeTwoList(pHead1->next, pHead2);
    } else {
        pMerged = pHead2;
        pMerged = MergeTwoList(pHead1, pHead2->next);
    }
    return pMerged;
}
void Test_MergeTwoList() {
    std::vector<int> v1{6, 8, 12};
    auto l1 = ConstructList(v1);
    PrintList(l1);

    std::vector<int> v2{13, 88, 137};
    auto l2 = ConstructList(v2);
    PrintList(l2);

    std::cout << "merge result -------------------" << std::endl;
    PrintList( mergeTwoList(l1, l2) );
}


bool HasCircle(ListNodeEx * pHead) {
    ListNodeEx * pFast = pHead; // 快指针每次前进两步
    ListNodeEx * pSlow = pHead; // 慢指针每次前进一步
    while(pFast != NULL && pFast->next != NULL) {
        pFast = pFast->next->next;
        pSlow = pSlow->next;
        if(pSlow == pFast) // 相遇，存在环
            return true;
    }
    return false;
}

void tst_CircleList() {
//    ListNodeEx* phead1 = NULL;
//    for (int i = 0 ; i < 9; ++ i )
//        CreateList(phead1, i);

//    ListNodeEx* ptmp = phead1;
//    while (1) {
//        if (ptmp->next)
//    }
}

//洗牌
void shufferImpl(int arr[], int length) {

    srand(time(NULL));
    int index = 0;

    for (int i = length-1; i > 0; -- i) {
        int index = rand()%(length);

        std::swap( arr[i], arr[index] );
//        std::cout << "index=" << index << std::endl;
    }
    for (int i = 0; i < length; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

}
void tst_shuffle() {

    int arr [10] = {0};
    for (int i = 0; i < 10; ++ i ) {
        arr[i] = i;
    }

    shufferImpl(arr, 10);
}

void tst_max_array() {
    int arr[] = { 9, 12, 3, 889, 71, 63, 29, 367,1023 };
    int max = arr[0];

    for (size_t index = 1; index < array_size(arr); ++index) {
        if (max < arr[index])
            max = arr[index];
    }
    std::cout << "max=" << max << std::endl;
}

void tst_printfilecontent() {
    std::string fileName = "/host_WorkDir/file.txt";
    std::string strRet = GetFileContent_string(fileName);
    std::cout << "size=" << strRet.length() << ", strRet=" << strRet <<std::endl;

}


// 如何判断一个字符串是否是回文字符串的问题
/*
 * １．用快慢两个指针遍历，同时用栈copy慢指针指向的data。
 * ２．完成后，慢指针指向中间节点，耗时为N/2.
 * ３．最后用pop栈中的data和慢指针指向的data比较，耗时也是N/2.
 * 所以时间复杂度为：Ｏ(N)，空间复杂度因栈额外存储了一半的data，故为O(N/2)
*/
void fast_slow_ptr() {
//    int arr[10] = { 9, 12, 3, 889, 71, 889, 3, 12, 9 };

    std::stack<int> stackInts;

    ListNodeEx* pHead = nullptr;
    CreateList(pHead, 9); CreateList(pHead, 12); CreateList(pHead, 3); CreateList(pHead, 889);
    CreateList(pHead, 71);
    CreateList(pHead, 889);CreateList(pHead, 3); CreateList(pHead, 12); CreateList(pHead, 9);

    ListNodeEx* pshow = pHead;

    std::ostringstream otmp;
    while (pshow) {
        otmp << pshow->val << " ";
        pshow = pshow->next;
    }
    LOG_DEBUG << "array=" << otmp.str();

    ListNodeEx* pFast = pHead;
    ListNodeEx* pslow = pHead;

    while (pFast) {
        LOG_DEBUG << "val=" << pFast->val;

        if (pFast->next) {
            pFast = pFast->next->next;

            stackInts.push(pslow->val);
        } else {// pslow 坐标是数组中位,也就是刚好正中间
            pFast = nullptr;
            LOG_DEBUG << " middle val= " << pslow->val;

            if (pslow->next) {

                ListNodeEx* pcompare = pslow->next;
                while (pcompare) {
                    int icurVal = stackInts.top();
                    stackInts.pop();
                    if (pcompare->val != icurVal) {
                        LOG_ERROR << "compare failed!!! pcompare->val:" << pcompare->val << " != icurVal:" << icurVal;
                    } else {
                        LOG_INFO << "compare successedd!!!val=" << icurVal;
                    }
                    pcompare = pcompare->next;
                }

            }
        }

        pslow = pslow->next;
    }

}

// 插入排序实现_1
void insert_sort_1( ) {
    int arrInts[] = { 67, 1, 210, 37, 24, 76, 9, 7, 10 };
    int length = array_size(arrInts);
    LOG_INFO << "array_size(arrInts)=" << length;

    for ( int i = 1; i < length; ++i ) {
        int value = arrInts[i];
        int j = i - 1;

        for ( ; j >= 0; -- j ) {
            if ( arrInts[j] > value ) {
                arrInts[j + 1] = arrInts[j];
            } else {
                break;
            }
        }// for 2 end

        arrInts[j + 1] = value;

    }// for 1 end

    std::ostringstream otmp;
    for (int i = 0; i < length; ++i ) {
        otmp << " " << arrInts[i] ;
    }
    LOG_INFO << " print all = " << otmp.str();
}


/*
    237. 删除链表中的节点
    请编写一个函数，使其可以删除某个链表中给定的（非末尾）节点。传入函数的唯一参数为 要被删除的节点。

    现有一个链表 -- head = [4,5,1,9]，它可以表示为:

    示例 1：
    输入：head = [4,5,1,9], node = 5
    输出：[4,1,9]
    解释：给定你链表中值为 5 的第二个节点，那么在调用了你的函数之后，该链表应变为 4 -> 1 -> 9.

    示例 2：
    输入：head = [4,5,1,9], node = 1
    输出：[4,5,9]
    解释：给定你链表中值为 1 的第三个节点，那么在调用了你的函数之后，该链表应变为 4 -> 5 -> 9.
*/
void deleteNode(ListNode* node) {
    node->val = node->next->val;
    node->next = node->next->next;
}
void Test_deleteNode() {
    std::vector<int> v1{ 4, 5, 1, 9 };
    auto l1 = ConstructList(v1);
    PrintList(l1);
    std::cout << std::endl << std::endl;

    deleteNode(l1);
    PrintList(l1);
}

/*
    剑指 Offer 22. 链表中倒数第k个节点
    输入一个链表，输出该链表中倒数第k个节点。
    为了符合大多数人的习惯，本题从1开始计数，即链表的尾节点是倒数第1个节点。
    例如: 一个链表有6个节点，从头节点开始，它们的值依次是1、2、3、4、5、6。
         这个链表的倒数第3个节点是值为4的节点。
    示例：
    给定一个链表: 1->2->3->4->5, 和 k = 2.
    返回链表 4->5.
*/
ListNode* getKthFromEnd(ListNode* head, int k) {
    ListNode* node = nullptr;

    ListNode* fast = head, *slow = head;

    while (k) {
        if (!fast) return nullptr;

        fast = fast->next;
        k --;
    }

    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    return slow;
}
void Test_getKthFromEnd() {
    PrintList(g_pListHead);
    std::cout << std::endl << std::endl;

    auto retNode = getKthFromEnd(g_pListHead, 2);
    PrintList(retNode);
}


/*
    剑指 Offer 06. 从尾到头打印链表
    输入一个链表的头节点，从尾到头反过来返回每个节点的值（用数组返回）。
示例 1：
输入：head = [1,3,2]
输出：[2,3,1]
*/
std::vector<int> reversePrint(ListNode* head) {
    std::vector<int> res;
    std::stack<int> st;
    while (head) {
        st.push(head->val);
        head = head->next;
    }
    while (st.size()) {
        // std::cout << " " << st.top();
        res.push_back(st.top());
        st.pop();
    }
    return res;
}
void Test_reversePrint() {
    // auto list = ConstructList(g_pListHead);
    auto vec = reversePrint(g_pListHead);
    PrintInContainer(vec);
}



/*
141. 环形链表
    给定一个链表，判断链表中是否有环。
    如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。
    为了表示给定链表中的环，我们使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。
    如果链表中存在环，则返回 true 。否则，返回 false 。

    进阶： 你能用 O(1)（即，常量）内存解决此问题吗？ 

    示例 1：
    输入：head = [3,2,0,-4], pos = 1
    输出：true
    解释：链表中有一个环，其尾部连接到第二个节点。

    示例 2：
    输入：head = [1,2], pos = 0
    输出：true
    解释：链表中有一个环，其尾部连接到第一个节点。

    示例 3：
    输入：head = [1], pos = -1
    输出：false
    解释：链表中没有环。
*/
bool hasCycle(ListNode *head) {

    return false;        
}
void Test_hasCycle() {

}


/*
    面试题 02.06. 回文链表
    编写一个函数，检查输入的链表是否是回文的。

示例 1：
输入： 1->2
输出： false

示例 2：
输入： 1->2->2->1
输出： true
*/
bool isPalindrome(ListNode* head) {

    ListNode* fast = head, *slow = head;
    while (fast && fast->next) { // 寻找中间节点
        fast = fast->next->next;
        slow = slow->next;
    }

    std::cout << "slow val=" << slow->val << std::endl;
    // while (slow) { // 
    //     ListNode* tmp = slow->next;
    // }

    return true;
}
void Test_isPalindrome() {
    // std::vector < int > v1{ 1, 2, 3, 3, 2, 1};
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto l1 = ConstructList(v1);
    PrintList(l1);

    std::string str = isPalindrome(l1) ? "is palin":("not palin");
    std::cout << "res=" << str << std::endl;
}



int tst_ListEntry() { // 测试list操作总入口
    
    Test_isPalindrome(); return 1;

    Test_reversePrint(); return 1;

    Test_getKthFromEnd(); return 1;

    Test_deleteNode(); return 1;

    Test_MergeTwoList(); return 1;

    insert_sort_1(); return 1;

    fast_slow_ptr(); return 1;

    tst_printfilecontent(); return 1;

    tst_max_array(); return 1;

    tst_shuffle(); return 1;

    tst_CircleList(); return 1;

    tst_RPrintList_fun(); return 1;

    tst_GetMiddleNode_fun(); return 1;

    tst_RGetKthNode_fun(); return 1;

    tst_reverse_list_1(); return 1;

}




