



#include "tstList.h"

#include "muduo/base/common.h"

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
    while ( pFast && pFast->next ) {
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
    // int index = 0;

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
void tst_deleteNode() {
    std::vector<int> v1{ 4, 5, 1, 9, 128, 214 };

    auto listHead = ConstructList(v1);
    PrintList(listHead);
    std::cout << std::endl << std::endl;

    auto pfnGetNodeByIdx = [&](ListNode* head, int n) -> ListNode* {
//    std::function< ListNode* (ListNode*, int) > pfnGet = [&](ListNode* head, int n) -> int {
        ListNode* ret = head;
        for ( int i = 0; i < n; i ++ ) {
            ret = ret->next;
        }
        return ret;
    };

    auto node = pfnGetNodeByIdx( listHead, 3 );
    deleteNode(node);

    PrintList(listHead);
}

/*
    剑指 Offer 18. 删除链表的节点
    给定单向链表的头指针和一个要删除的节点的值，定义一个函数删除该节点。
    返回删除后的链表的头节点。
    注意：此题对比原题有改动

    示例 1:
    输入: head = [4,5,1,9], val = 5
    输出: [4,1,9]
    解释: 给定你链表中值为 5 的第二个节点，那么在调用了你的函数之后，该链表应变为 4 -> 1 -> 9.

    示例 2:
    输入: head = [4,5,1,9], val = 1
    输出: [4,5,9]
    解释: 给定你链表中值为 1 的第三个节点，那么在调用了你的函数之后，该链表应变为 4 -> 5 -> 9.
*/
ListNode* deleteNodeEx(ListNode* head, int val) {
    if ( !head ) return head;
    if ( head->val == val ) return head->next;

    ListNode* pre = head;
    ListNode* cur = head;

    while ( cur && cur->val != val ) {
        pre = cur;
        cur = cur->next;
    }

    // cur == val || cur == nullptr
    if ( cur ) {
        pre->next = cur->next;
    }

    return head;
}
void tst_deleteNodeEx() {

    std::vector<int> v1{ 4, 5, 1, 9, 128, 214 };

    auto listHead = ConstructList(v1);
    PrintList(listHead);
    std::cout << std::endl << std::endl;

    auto newHead = deleteNodeEx( listHead, 1 );
    std::cout << "newHead val=" << newHead->val << std::endl;
    PrintList(newHead);

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

    {
        ListNode* fast = head, *slow = nullptr;

        while ( k ) {
            if ( !fast ) return nullptr;
            fast = fast->next;
            k --;
        }

        while ( fast ) {
            fast = fast->next;
            slow = slow->next;
        }

        return slow;
    }
    // ListNode* node = nullptr;

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
void tst_getKthFromEnd() {
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
    {
        std::vector<int> res;
        std::stack<int> stk;

        while ( head ) {
            stk.push(head->val);
            head = head->next;
        }

        while ( stk.size() ) {
            res.push_back(stk.top());
            stk.pop();
        }

        return res;
    }

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
void tst_reversePrint() {
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


-----------------------------------------------------------------------
复杂度分析

时间复杂度： O(N)，其中 N 是链表的节点数。
当链表中不存在环时，快指针将先于慢指针到达链表尾部，链表中每个节点至多被访问两次。
当链表中存在环时，每一轮移动后，快慢指针的距离将减小一。而初始距离为环的长度，因此至多移动 N 轮。
空间复杂度： O(1)。我们只使用了两个指针的额外空间。
-----------------------------------------------------------------------
*/
bool hasCycle(ListNode *head) {
    if (!head) return true;

    ListNode* fast = head, * slow = head;
    while ( fast && fast->next ) {
        fast = fast->next->next;
        slow = slow->next;
        if (slow == fast) return true;
    }
    return false;
}
void Test_hasCycle() {
    hasCycle(nullptr);
}

/*
    第142题.环形链表II
    题意：给定一个链表，返回链表开始入环的第一个节点。如果链表无环，则返回 null。

    为了表示给定链表中的环，使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。

    如果 pos 是 -1，则在该链表中没有环。

时间复杂度：O(N)，其中 N 为链表中节点的数目。
在最初判断快慢指针是否相遇时，slow 指针走过的距离不会超过链表的总长度；
随后寻找入环点时，走过的距离也不会超过链表的总长度。因此，总的执行时间为 O(N)+O(N)=O(N)。

空间复杂度：O(1)。我们只使用了 slow,fast,ptr 三个指针。


------------------------------------------------
此时我们已经可以判断链表是否有环了，那么接下来要找这个环的入口了

slow走过的路程: x+y
fast走过的路程: x+y+z+y

又因为fast走过的路程是slow的两倍
2*(x+y) == x+y+z+y

故x==z，所以从head到环起始节点的距离 == 从相遇点到环起始点的距离

----> 相遇时，令其中一个指针指向head，然后两个指针同步往后指，再相遇时，既是环起始节点.
*/
ListNode* detectCycle(ListNode* head) {
    if (!head) return nullptr;

    ListNode* fast = head, * slow = head;
    while ( fast && fast->next ) {

        fast = fast->next->next;
        slow = slow->next;

        if (slow == fast) {
            ListNode* ptr = head;
            while (ptr != slow) {
                ptr = ptr->next;
                slow = slow->next;
            }
            return ptr;
        }
    }
    return nullptr;
}
void tst_detectCycle() {
    
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

    ListNode* pre = nullptr;
    while (slow) { // 
        ListNode* nxtNode = slow->next;
        slow->next = pre;

        pre = slow;
        slow = nxtNode;
    }
    PrintList( head );
    PrintList( pre );

    while (head && pre) {
        if (head->val != pre->val) {
            return false;
        }
        head = head->next;
        pre = pre->next;
    }
    return true;
}
void Test_isPalindrome() {
    // std::vector < int > v1{ 1, 2, 3, 5, 4, 3, 2, 1};
    std::vector < int > v1{ 1, 2, 3, 5, 3, 2, 1};
    // std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // std::vector<int> v1{1, 2, 3, 4, 5, 6, 7};

    auto l1 = ConstructList(v1);
    PrintList(l1);

    std::string str = isPalindrome(l1) ? "is palin":("not palin");
    std::cout << "res=" << str << std::endl;
}


/*
    面试题 04.03. 特定深度节点链表
给定一棵二叉树，设计一个算法，创建含有某一深度上所有节点的链表（比如: 若一棵树的深度为 D，则会创建出 D 个链表）。
返回一个包含所有深度的链表的数组。

示例： 输入：[1,2,3,4,5,null,7,8]

        1
       /  \
      2    3
     / \    \
    4   5    7
   /
  8

输出：[[1],[2,3],[4,5,7],[8]]
*/
vector<ListNode*> listOfDepth(BstNode* tree) {
    vector<ListNode*> res;

    std::queue<BstNode*> que;
    que.push(tree);

    while (que.size()) {
        int sz = que.size();

        auto head = new ListNode(-1);
        ListNode* p = head;
        while (sz--) {

            BstNode* tmp = que.front();
            que.pop();

            if (tmp->left) {
                que.push(tmp->left);
            }
            if (tmp->right) {
                que.push(tmp->right);
            }

            p->next = new ListNode(tmp->val);
            p = p->next;
        }
        res.push_back(head->next);
        delete head;
    }
    return res;
}
void Test_listOfDepth() {
    // PrintTree(g_pBstTree);
    PrintInorder(g_pBstTree);
    std::cout << std::endl;

    auto ret = listOfDepth(g_pBstTree);
    for ( auto l : ret ) {

        while ( l ) {
            std::cout << " " << l->val;
            l = l->next;
        }

        std::cout << std::endl;
    }
}

/*
    5558. 合并两个链表

    给你两个链表 list1 和 list2 ，它们包含的元素分别为 n 个和 m 个。
    请你将 list1 中第 a 个节点到第 b 个节点删除，并将list2 接在被删除节点的位置。

示例 1：
输入：list1 = [0,1,2,3,4,5], a = 3, b = 4, list2 = [1000000,1000001,1000002]
输出：[0,1,2,1000000,1000001,1000002,5]

示例 2：
输入：list1 = [0,1,2,3,4,5,6], a = 2, b = 5, list2 = [1000000,1000001,1000002,1000003,1000004]
输出：[0,1,1000000,1000001,1000002,1000003,1000004,6]
*/
ListNode* mergeInBetween(ListNode* list1, int a, int b, ListNode* list2) {
    ListNode *tmp = nullptr;
    UNUSED(a);
    UNUSED(b);
    UNUSED(tmp);
    UNUSED(list1);
    UNUSED(list2);
    return tmp;
}

/*
    剑指 Offer 35. 复杂链表的复制

    请实现 copyRandomList 函数，复制一个复杂链表。
    在复杂链表中，每个节点除了有一个 next 指针指向下一个节点，还有一个 random 指针指向链表中的任意节点或者 null。
*/
void copyNodes(ListNode *head) {
    if (!head) return;
    ListNode *pos = head;
    while (pos) {
        ListNode *copy = new ListNode(pos->val);
        ListNode *next = pos->next;
        pos->next = copy;
        copy->next = next;
        pos = copy->next;
    }
}
ListNode* copyRandomList(ListNode* head) {
    ListNode *tmp = nullptr;

    copyNodes(head);
    return tmp;
}
void Test_copyRandomList() {
    PrintList(g_pListHead);
    std::cout << " ---------------------------------- " << std::endl;
    copyRandomList(g_pListHead);
    PrintList(g_pListHead);
    // std::cout << " ---------------------------------- " << std::endl;
}

/*
    148. 排序链表
给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。

进阶：你可以在 O(nlogn) 时间复杂度和常数级空间复杂度下，对链表进行排序吗？
*/
ListNode* sortList(ListNode* head) {
    ListNode *tmp = nullptr;
    return tmp;
}



/*
    剑指 Offer 24. 反转链表
    定义一个函数，输入一个链表的头节点，反转该链表，并输出反转后链表头节点。
    示例:
    输入: 1->2->3->4->5->NULL
    输出: 5->4->3->2->1->NULL

    好理解的双指针
    定义两个指针： pre 和 cur；pre 在前 cur 在后。
    每次让 pre 的 next 指向 cur ，实现一次局部反转
    局部反转完成之后， pre 和 cur 同时往前移动一个位置
    循环上述过程，直至 pre 到达链表尾部
*/
ListNode* reverselist(ListNode* head) {

    {
        ListNode *cur = nullptr, *pre = head;
        while ( pre ) {
            auto t = pre->next;
            pre->next = cur;

            cur = pre;
            pre = t;
        }
        return cur;
    }

    ListNode *cur = nullptr, *pre = head;
    while (pre) {

        ListNode* t = pre->next;
        pre->next = cur;

        cur = pre;
        pre = t;
    }
    return cur;
}
void tst_reverselist() {
    PrintList(g_pListHead);
    std::cout << "before handle list -----" << std::endl;

    auto retlist = reverselist(g_pListHead);
    std::cout << "after  handle list -----" << std::endl;
    PrintList(retlist);
}

/*
    136. 只出现一次的数字
    给定一个非空整数数组，除了某个元素只出现一次以外，其余每个元素均出现两次。
    找出那个只出现了一次的元素。

    说明： 你的算法应该具有线性时间复杂度。 你可以不使用额外空间来实现吗？

    示例 1:
    输入: [2,2,1]
    输出: 1

    示例 2:
    输入: [4,1,2,1,2]
    输出: 4
*/
int singleNumber(vector<int>& nums) {
    int res = nums[0];
    for (size_t i = 1; i < nums.size(); i++) {
        res ^= nums[i];
    }
    return res;
}
void Test_singleNumber() {
    std::vector<int> v1{ 9102, 4, 4, 1, 2, 1, 2 };
    auto retNum = singleNumber(v1);
    std::cout << "retnum=" << retNum << std::endl;
}

/*
    169. 多数元素
给定一个大小为 n 的数组，找到其中的多数元素。
多数元素是指在数组中出现次数大于 ⌊ n/2 ⌋ 的元素。

示例 1:
输入: [3,2,3]
输出: 3

示例 2:
输入: [2,2,1,1,1,2,2]
输出: 2
*/
int majorityElement(vector<int>& nums) {
    int cand = 0, cnt = 0;
    for ( auto e : nums ) {
        if ( 0 == cnt ) {
            cand = e;
        }
        if ( cand == e ) {
            cnt++;
        } else {
            cnt--;
        }
    }
    return cand;
}
void Test_majorityElement() {
    std::vector<int> v1{ 2, 2, 1, 1, 1, 2, 2 };
    std::cout << "maj ele=" << majorityElement(v1) << std::endl;
}

/*
LeetCode 229 [Majority Element II]
给定一个整型数组，找到所有主元素，它在数组中的出现次数严格大于数组元素个数的三分之一。

算法：每次删除三个不相同的数，最后留下的一定是出现次数超过1/3的数，这个思想可以推广到出现次数超过1/k次的元素有哪些。

因为出现次数大于n/3的元素最多只有两个，所以最开始可以维护两个数字(num1,num2)和两个计数器(counter1,counter2)；
遍历数组，当数组中元素和num1或者num2相同，对应的counter1或者counter2加1；
如果counter1或counter2为0，将遍历到的该元素赋给num1或者nums2；
否则counter1和counter2都减1。
*/
std::vector<int>  majorityElement_3(vector<int>& nums) {
    std::vector<int> ret;
    int y = 0, cy = 0;
    int z = 0, cz = 0;
    for ( auto x : nums ) {
        if (y == x) cy++;
        else if (z == x) cz++;
        else if (cy == 0) y = x, cy=1;
        else if (cz == 0) z = x, cz=1;
        else {
            --cy,--cz;
        }
    }
    cy = 0;
    cz = 0;
    for (auto x : nums) {
        if (x == y) cy++;
        if (x == z) cz++;
    }
    int tmp = nums.size() / 3;
    if (cy > tmp) ret.push_back(y);
    if (cz > tmp) ret.push_back(z);
    return ret;
}
void Test_majorityElement_3() {
    std::vector<int> v1{ 9, 9, 9,  13, 0, 9,  7, 7, 7 };
    auto res = majorityElement_3(v1);
    PrintInContainer(res);
    // std::cout << "maj ele_3=" << majorityElement_3(v1) << std::endl;
}

/*
    283. 移动零
    给定一个数组 nums，编写一个函数将所有 0 移动到数组的末尾，同时保持非零元素的相对顺序。

    示例: 输入: [0,1,0,3,12]
         输出: [1,3,12,0,0]

    说明: 必须在原数组上操作，不能拷贝额外的数组。尽量减少操作次数。
*/
void moveZeroes(vector<int>& nums) {
        
}
void Test_moveZeroes() {
    std::vector<int> v1{ 0, 1, 0, 3, 12 };
    moveZeroes(v1);
    
    PrintInContainer(v1);
}


/*
    27. 移除元素
    给你一个数组 nums 和一个值 val，你需要 原地 移除所有数值等于 val 的元素，并返回移除后数组的新长度。

    不要使用额外的数组空间，你必须仅使用 O(1) 额外空间并「原地」修改输入数组。

    元素的顺序可以改变。你不需要考虑数组中超出新长度后面的元素。

    示例 1:
    给定 nums = [3,2,2,3], val = 3,
    函数应该返回新的长度 2, 并且 nums 中的前两个元素均为 2。
    你不需要考虑数组中超出新长度后面的元素。

    示例 2:
    给定 nums = [0,1,2,2,3,0,4,2], val = 2,
    函数应该返回新的长度 5, 并且 nums 中的前五个元素为 0, 1, 3, 0, 4。

    「你不需要考虑数组中超出新长度后面的元素。」
*/
size_t removeElement(vector<int>& nums, int target) {
    size_t slow = 0;
    {
        for ( size_t fast = 0; fast < nums.size(); fast ++ ) {
            if ( target != nums[ fast ] ) {
                nums[ slow ] = nums[ fast ];
                slow ++;
            }
        }
        return slow;
    }
    size_t slowIdx = 0;
    for (size_t fastIdx = 0; fastIdx < nums.size(); fastIdx++) {
        if (target != nums[fastIdx]) {
            nums[ slowIdx ] = nums[ fastIdx ];
            slowIdx ++;
        }
    }
    return slowIdx;
}
void Test_removeEle() {
    size_t ret = 0;

    auto pfnPrint = [&](std::vector<int>& arr, int sz) {
        for ( int i = 0; i < sz; i ++ ) {
            std::cout << " " << arr[i];
        }
        std::cout << std::endl;
    };

    std::vector<int> v1{ 3, 2, 2, 3 };
    ret = removeElement(v1, 3);
    std::cout << "ret=" << ret << std::endl;
//    PrintInContainer(v1);
    pfnPrint(v1, ret);
//    return;

    std::vector<int> v2{ 0,1,2,2,3,0,4,2 };
    ret = removeElement(v2, 2);
    std::cout << "ret=" << ret << std::endl;
    pfnPrint(v2, ret);
//    PrintInContainer(v2);
}

/*
    448. 找到所有数组中消失的数字
    给定一个范围在 1 ≤ a[i] ≤ n ( n = 数组大小 ) 的 整型数组，数组中的元素一些出现了两次，另一些只出现一次。

    找到所有在 [1, n] 范围之间没有出现在数组中的数字。

    您能在不使用额外空间且时间复杂度为O(n)的情况下完成这个任务吗? 你可以假定返回的数组不算在额外空间内。

    示例:
    输入: [4,3,2,7,8,2,3,1]
    输出: [5,6]
*/
vector<int> findDisappearedNumbers(vector<int>& nums) {
    std::vector<int> res;
    return res;
}
void Test_findDisappearedNumbers() {
}


/*
    155. 最小栈
    设计一个支持 push，pop，top 操作，并能在常数时间内检索到最小元素的栈。

    push(x)  —— 将元素 x 推入栈中。
    pop()    —— 删除栈顶的元素。
    top()    —— 获取栈顶元素。
    getMin() —— 检索栈中的最小元素。

    示例:
    输入：
    ["MinStack","push","push","push","getMin","pop","top","getMin"]
    [[],[-2],[0],[-3],[],[],[],[]]

    输出：
    [null,null,null,null,-3,null,0,-2]

    解释：
    MinStack minStack = new MinStack();
    minStack.push(-2);
    minStack.push(0);
    minStack.push(-3);
    minStack.getMin();   --> 返回 -3.
    minStack.pop();
    minStack.top();      --> 返回 0.
    minStack.getMin();   --> 返回 -2.
*/
class MinStack {
public:
    MinStack() {
    }
    
    // void push(int x) {
    // }
    
    // void pop() {
    // }
    
    // int top() {
    // }
    
    // int min() {
    // }
};





/*
    53. 最大子序和
    给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

    示例:
    输入: [-2,1,-3,4,-1,2,1,-5,4]
    输出: 6
    解释: 连续子数组 [4,-1,2,1] 的和最大，为 6。
    进阶: 如果你已经实现复杂度为 O(n) 的解法，尝试使用更为精妙的分治法求解。
*/
int maxSubArray(vector<int>& nums) {
    {
        int maxSum = nums[ 0 ], res = maxSum;
        for ( int i = 1; i < nums.size(); i ++ ) {
            maxSum = std::max( maxSum + nums[ i ], nums[ i ]);
            res = std::max( maxSum, res );
        }
        return res;
    }

    int dp = 0, res = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        dp = std::max(dp + nums[i], nums[i]);
        res = std::max(dp, res);
    }
    return res;
}
void tst_maxSubArray() {
     std::vector<int> v1{ 4,-1,2,1 };
//    std::vector<int> v1{ -2,1,-3,4,-1,2,1,-5,4 };
    std::cout << "maxSubArray=" << maxSubArray(v1) << std::endl;
}




/*
    143. 重排链表
    给定一个单链表 L：L0→L1→…→Ln-1→Ln ，
    将其重新排列后变为： L0→Ln→L1→Ln-1→L2→Ln-2→…
    你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。

    示例 1:
    给定链表 1->2->3->4, 重新排列为 1->4->2->3.
    示例 2:
    给定链表 1->2->3->4->5, 重新排列为 1->5->2->4->3.

复杂度分析

时间复杂度：O(N)，其中 N 是链表中的节点数。
空间复杂度：O(1)。
*/
class CReorderList {

public:

    ListNode* middleNode(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;

        while ( fast && fast->next ) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    }

    ListNode* reverseList(ListNode* head) {
        ListNode* pre = nullptr;
        ListNode* cur = head;

        while (cur) {
            ListNode* tmp = cur->next;
            cur->next = pre;

            pre = cur;
            cur = tmp;
        }
        return pre;
    }

    void mergeList(ListNode* l1, ListNode* l2) {
        ListNode* l1_tmp;
        ListNode* l2_tmp;

        while (l1 && l2) {
            l1_tmp = l1->next;
            l2_tmp = l2->next;

            l1->next = l2;
            l1 = l1_tmp;

            l2->next = l1;
            l2 = l2_tmp;
        }
    }

    void reorderList(ListNode* head) {

        // 1、找到链表的中间位置，分成左右2个链表
        ListNode* midNode = middleNode(head);
        std::cout << "mid=" << midNode->val << std::endl;

        // 2、翻转链表右边链表
        // 这个时候slow、fast指针分别代表左右链表的头指针
        ListNode* l1 = head;
        ListNode* l2 = midNode->next;
        l2->next = nullptr;

        l2 = reverseList(l2);

        // 3、处理143题的逻辑，重排链表
        mergeList(l1, l2);
    }

};
void tst_reorderList() {

    PrintList(g_pListHead);

    CReorderList cr;
//    auto ret = cr.reverseList(g_pListHead);
//    PrintList(ret);

    cr.reorderList(g_pListHead);
    PrintList(g_pListHead);
//    return;
}

/*
    88. 合并两个有序数组
    给你两个有序整数数组 nums1 和 nums2，请你将 nums2 合并到 nums1 中，使 nums1 成为一个有序数组。
    初始化 nums1 和 nums2 的元素数量分别为 m 和 n 。你可以假设 nums1 的空间大小等于 m + n，这样它就有足够的空间保存来自 nums2 的元素。

    示例 1：
    输入：nums1 = [1,2,3,0,0,0], m = 3, nums2 = [2,5,6], n = 3
    输出：[1,2,2,3,5,6]

    示例 2：
    输入：nums1 = [1], m = 1, nums2 = [], n = 0
    输出：[1]

-----------------------------------------------------------------------
复杂度分析

-----------------------------------------------------------------------
*/
void mergeTwoArray(std::vector<int>& num1, int m, std::vector<int>& num2, int n) {
    std::cout << "m=" << m << ", n=" << n << std::endl;
    int k = m + n - 1;
    int i = m - 1;
    int j = n - 1;

    while ( i >= 0 && j >= 0 ) {

        std::cout << "k=" << k << std::endl;

        if ( num1[ i ] > num2 [ j ] ) {
            num1[ k -- ]  = num1[ i -- ];
        } else {
            num1[ k -- ]  = num2[ j -- ];
        }

    }// end --- for

    while ( j >= 0 ) {
        std::cout << "k=" << k << std::endl;

        num1 [ k -- ] = num2[ j -- ];
    }// end --- while
}
void tst_mergeTwoArray() {
    std::vector<int> v1;

    v1.reserve(10);
    v1.push_back(1); v1.push_back(3); v1.push_back(4); v1.push_back(7); v1.push_back(9);
    std::cout << "before print v1 ---" << std::endl;
    PrintInContainer(v1);
    std::cout << "v1 size=" << v1.size() << ", cap=" << v1.capacity() << std::endl;
    std::cout << "after  print v1 ---" << std::endl;

    std::vector<int> v2 { -11, 0, 987, 2456, 30099 };
    mergeTwoArray(v1, v1.size(), v2, v2.size());
    std::cout << std::endl;
    std::cout << "v1 size=" << v1.size() << ", cap=" << v1.capacity() << std::endl;
    std::cout << "after ,,,,,,, handle" << std::endl;
//    PrintInContainer(v1);
    for ( int i = v1.capacity() - 1; i >= 0; i -- )  {
        std::cout << " " << v1[i] ;
    }
    std::cout << std::endl;
}


/*
    剑指 Offer 57. 和为s的两个数字
    输入一个递增排序的数组和一个数字s，在数组中查找两个数，使得它们的和正好是s。如果有多对数字的和等于s，则输出任意一对即可。

    示例 1：
    输入：nums = [2,7,11,15], target = 9
    输出：[2,7] 或者 [7,2]

    示例 2：
    输入：nums = [10,26,30,31,47,60], target = 40
    输出：[10,30] 或者 [30,10]
-----------------------------------------------------------------------
复杂度分析
-----------------------------------------------------------------------
*/
std::vector<int> twoSum(std::vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;

    while ( left < right ) {

        int s = nums[ left ] + nums[ right ];
        if ( s == target ) {
            return std::vector<int>{ nums[ left ], nums[ right ] };
        } else if ( s > target ) {
            right --;
        } else {
            left ++;
        }
        std::cout << "s = " << s << std::endl;
    }

    std::cout << "no found two sum number" << std::endl;
    return std::vector<int>{};
}
void tst_twoSum() {
    std::vector<int> v1{ 10,26,30,31,47,60 };
    auto ves = twoSum(v1, 40);
    // auto ves = twoSum(v1, 17);
    for ( auto ele : ves ) {
        std::cout << " " << ele;
    }
    std::cout << std::endl;
}

/*
    剑指 Offer 03. 数组中重复的数字

    在一个长度为 n 的数组 nums 里的所有数字都在 0～n-1 的范围内。
    数组中某些数字是重复的，但不知道有几个数字重复了，也不知道每个数字重复了几次。请找出数组中任意一个重复的数字。

    示例 1：
    输入：[2, 3, 1, 0, 2, 5, 3]
    输出：2 或 3

-----------------------------------------------------------------------
复杂度分析
-----------------------------------------------------------------------
*/
int findRepeatNumber(vector<int>& nums) {

    int i  = 0;
    while ( i < nums.size() ) {

        if ( i == nums[ i ] ) {
            i ++;
            continue;
        }

        if ( nums[ nums[i] ] ==  nums[ i ] ) {
            return nums[ i ];
        }

        std::swap( nums[ nums[i] ],  nums[ i ] ) ;
    }

    return -1;
}
void tst_findRepeatNumber() {

    std::vector<int> v2 { 2, 0, 3, 3, 4 };

    auto ret = findRepeatNumber(v2);
    std::cout << std::endl;
    std::cout << "ret=" << ret << std::endl;
    std::cout << "after ,,,,,,, handle" << std::endl;
}


/*
    56. 合并区间
    以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [starti, endi] 。
    请你合并所有重叠的区间，并返回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间。

    示例 1：
    输入：intervals = [[1,3],[2,6],[8,10],[15,18]]
    输出：[[1,6],[8,10],[15,18]]
    解释：区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].

    示例 2：
    输入：intervals = [[1,4],[4,5]]
    输出：[[1,5]]
    解释：区间 [1,4] 和 [4,5] 可被视为重叠区间。

-----------------------------------------------------------------------
复杂度分析
-----------------------------------------------------------------------
*/
vector<vector<int>> mergeRegion(vector<vector<int>>& intervals) {
    vector<vector<int>> res;

    return res;
}


/*
    26. 删除排序数组中的重复项
    给定一个排序数组，你需要在 原地 删除重复出现的元素，使得每个元素只出现一次，返回移除后数组的新长度。

    不要使用额外的数组空间，你必须在 原地 修改输入数组 并在使用 O(1) 额外空间的条件下完成。

    示例 1:
    给定数组 nums = [1,1,2]
    函数应该返回新的长度 2, 并且原数组 nums 的前两个元素被修改为 1, 2。
    你不需要考虑数组中超出新长度后面的元素。

    示例 2:
    给定 nums = [0,0,1,1,1,2,2,3,3,4]
    函数应该返回新的长度 5, 并且原数组 nums 的前五个元素被修改为 0, 1, 2, 3, 4。
    你不需要考虑数组中超出新长度后面的元素。

    说明:
    为什么返回数值是整数，但输出的答案是数组呢?
    请注意，输入数组是以「引用」方式传递的，这意味着在函数里修改输入数组对于调用者是可见的。

    你可以想象内部操作如下:
    // nums 是以"引用"方式传递的。也就是说，不对实参做任何拷贝
    int len = removeDuplicates(nums);

    // 在函数里修改输入数组对于调用者是可见的。
    // 根据你的函数返回的长度, 它会打印出数组中该长度范围内的所有元素。
    for (int i = 0; i < len; i++) {
        print(nums[i]);
    }
*/
int removeDuplicates(vector<int>& nums) {
    {
        if(nums.size() == 0) return 0;
        int i = 0;
        for(int j = 1; j < nums.size(); j++) {
            if(nums[i] != nums[j]) {
                i++;
                std::cout << "i=" << i << ", nums[" << i << "]=" << nums[i] << ", nums[" << j << "]=" << nums[j]<< std::endl;
                nums[i] = nums[j];
            }
        }
        return i + 1;
    }
    {
        int size = nums.size();
        int cnt = 0; //统计当前元素需要前移的位数，注意还是计数排序思想！！！只不过不需要整个数组
        for(int i=1; i<size; ++i){
            if(nums[i] == nums[i-1])
                cnt++;
            nums[i-cnt] = nums[i]; //前移cnt个位置
        }
        return size-cnt;
    }
}
void tst_removeDuplicates() {

    std::vector<int> arr{ 0,0,1,1,1,2,2,3,3,4 };
    PrintInContainer(arr);

    auto len = removeDuplicates(arr);
    std::cout << "res=" << len  << std::endl;

    PrintVectorInts(arr, len);
}

/*
    14. 最长公共前缀
    编写一个函数, 查找字符串数组中的最长公共前缀。
    如果不存在公共前缀，返回空字符串 ""。

    示例 1：
    输入：strs = ["flower","flow","flight"]
    输出："fl"

    示例 2：
    输入：strs = ["dog","racecar","car"]
    输出：""
    解释：输入不存在公共前缀。
*/
string longestCommonPrefix(vector<string>& strs) {
    string res = strs.empty() ? "" : strs[0];
    for (string s : strs) {
        while (s.find(res) != 0) {
            res = res.substr(0, res.length() - 1);
        }
    }
    return res;
}
void tst_longestCommonPrefix() {

    std::string res = "1234";
    res = res.substr(0, res.length() - 1);
    std::cout << "res=" << res << std::endl;
    std::cout << std::endl;
//    return 1;

    std::vector<string> vec { "dog","racecar","car" };
//    std::vector<string> vec { "flower","flow","flight" };
    std::cout << "res=" << longestCommonPrefix(vec) << std::endl;
}


/*
    5. 最长回文子串
    给你一个字符串 s，找到 s 中最长的回文子串。

    示例 1：
    输入：s = "babad"
    输出："bab"
    解释："aba" 同样是符合题意的答案。

    示例 2：
    输入：s = "cbbd"
    输出："bb"

    示例 3：
    输入：s = "a"
    输出："a"

    示例 4：
    输入：s = "ac"
    输出："a"
*/
string longestPalindrome(string s) {
    std::string res;
    return res;
}
void tst_longestPalindrome() {
    std::string in_str = "";
    std::cout << "rest=" << longestPalindrome(in_str) << std::endl;
}


class CLongestHuiWen {
public:
    string longestPalindrome(string s) {
        int len=s.size();
        if(len==0||len==1)
            return s;
        int start=0;//记录回文子串起始位置
        int end=0;//记录回文子串终止位置
        int mlen=0;//记录最大回文子串的长度
        for(int i=0;i<len;i++)
        {
            int len1=expendaroundcenter(s,i,i);//一个元素为中心
            int len2=expendaroundcenter(s,i,i+1);//两个元素为中心
            mlen=max(max(len1,len2),mlen);
            if(mlen>end-start+1)
            {
                start=i-(mlen-1)/2;
                end=i+mlen/2;
            }
        }
        return s.substr(start,mlen);
        //该函数的意思是获取从start开始长度为mlen长度的字符串
    }
private:
    int expendaroundcenter(string s,int left,int right)
    //计算以left和right为中心的回文串长度
    {
        int L=left;
        int R=right;
        while(L>=0 && R<s.length() && s[R]==s[L])
        {
            L--;
            R++;
        }
        return R-L-1;
    }
};
void tst_CLongestHuiWen() {
    CLongestHuiWen cobj;
    std::cout << "res1=" << cobj.longestPalindrome("ttttaaaaabbbbaaaaazzzzzzzzzzzzz") << std::endl;
    std::cout << "res2=" << cobj.longestPalindrome("......11111xxxx11111.......................") << std::endl;
}

/*
    寻找两个正序数组的中位数
    给定两个大小为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
    请你找出并返回 这两个正序数组的中位数。

    进阶：你能设计一个时间复杂度为 O(log(m+n)) 的算法解决此问题吗？
    示例 1：
    输入：nums1 = [1,3], nums2 = [2]
    输出：2.00000
    解释：合并数组 = [1,2,3] ，中位数 2

    示例 2：
    输入：nums1 = [1,2], nums2 = [3,4]
    输出：2.50000
    解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5

    示例 3：
    输入：nums1 = [0,0], nums2 = [0,0]
    输出：0.00000

    示例 4：
    输入：nums1 = [], nums2 = [1]
    输出：1.00000

    示例 5：
    输入：nums1 = [2], nums2 = []
    输出：2.00000
*/
double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    double ret = 0;

    return ret;
}


/*
    19. 删除链表的倒数第 N 个结点
    给你一个链表，删除链表的倒数第 n 个结点，并且返回链表的头结点。

    进阶：你能尝试使用一趟扫描实现吗？

    示例 1：
    输入：head = [1,2,3,4,5], n = 2
    输出：[1,2,3,5]

    示例 2：
    输入：head = [1], n = 1
    输出：[]

    示例 3：
    输入：head = [1,2], n = 1
    输出：[1]
*/
ListNode* removeNthFromEnd(ListNode* head, int n) {
    if ( !head ) return nullptr;

    ListNode* fast = head;
    ListNode* slow = head;

    for ( int i = 0; fast && i < n + 1; i ++ ) {
        fast = fast->next;
    }
    std::cout << "111 fast=" << fast->val << std::endl;

    while ( fast ) {
        fast = fast->next;
        slow = slow->next;
    }

    if (fast)
        std::cout << "222 fast=" << fast->val << std::endl;
    std::cout << "slow=" << slow->val << std::endl;

    slow->next = slow->next->next;
    return head;
}
void tst_removeNthFromEnd() {

    PrintList(g_pListHead);

    auto retNode = removeNthFromEnd(g_pListHead, 3);

    PrintList(retNode);
}

/*
    121. 买卖股票的最佳时机
    给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。

    如果你最多只允许完成一笔交易（即买入和卖出一支股票一次），设计一个算法来计算你所能获取的最大利润。

    注意：你不能在买入股票前卖出股票。

    示例 1:
    输入: [7,1,5,3,6,4]
    输出: 5
    解释: 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5 。
         注意利润不能是 7-1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。

    示例 2:
    输入: [7,6,4,3,1]
    输出: 0
    解释: 在这种情况下, 没有交易完成, 所以最大利润为 0。

    -----------------------------------------------------------------------
    复杂度分析:
    时间复杂度：O(n)，只需要遍历一次。
    空间复杂度：O(1)，只使用了常数个变量。
    -----------------------------------------------------------------------
*/
int maxProfit(vector<int>& prices) {
    {
        int minPrice = 10000000, maxProfit = 0;
        for ( int price : prices ) {
            maxProfit = std::max( maxProfit, price - minPrice );
            minPrice = std::min( minPrice, price );
        }
        return maxProfit;
    }
    int inf = 1e9;
    int minprice = inf, maxprofit = 0;
    for (int price : prices) {
        maxprofit = max(maxprofit, price - minprice);
        minprice = min(price, minprice);
    }
    return maxprofit;
}
void tst_maxProfit() {
//    std::vector<int> v1{ 7, 1, 5, 3, 6, 4 }; // ret=5
    std::vector<int> v1{ 7,6,4,3,1 }; // ret=0
    std::cout << "maxProfit=" << maxProfit(v1) << std::endl;
}

template < typename T >
class ListNodeT {
    T val;
    ListNodeT* next;

    ListNodeT(const T& v) : val(v), next(nullptr) {
    }
    ListNodeT() : val(-1), next(nullptr) {
    }
    ListNodeT(const ListNodeT& other) : val( other.val ), next( nullptr/*other.next*/ ) {
    }
};
template < typename T >
void Insert( ListNodeT< T > * node, const T val ) {
    auto newEle = new ListNodeT<T>(val);
    newEle->next = node->next;
    node->next = newEle;
}


void tst_lingxingjicheng() {
    int x1 = -99;
    int x2 = -99;
    int a = -1;
    int b = -201;

    printf("x1-addr=%p\n", &x1);
    printf("x2-addr=%p\n", &x2);
    printf("a -addr=%p\n", &a);
    printf("b -addr=%p\n", &b);
    return;

    {
        class A  //大小为4
        {
        public:
            int a;
        };
        class B :virtual public A  //大小为16，变量a,b共8字节，虚基类表指针8
        {
        public:
            int b;
        };
        class C :virtual public A //与B一样16
        {
        public:
            int c;
        };
        class D :public B, public C //40,变量a,b,c,d共16，B的虚基类指针8，C的虚基类指针8
        {
        public:
            int d;
        };

        {
            A a;
            B b;
            C c;
            D d;
            cout << sizeof(a) << endl; // 4
            cout << sizeof(b) << endl; // 16
            cout << sizeof(c) << endl; // 16
            cout << sizeof(d) << endl; // 40
//            system("pause");
            return;
        }
    }

#include <iostream>
#include <string>
    using namespace std;

    class A
    {
    public:
        A(const char*s)
        {
            cout<<s<<endl;
        }
    };
    class B:virtual public A
    {
    public:
        B(const char*s1,const char*s2):A(s1)
        {
            cout <<s2<<endl;
        }
    };
    class C:virtual public A
    {
    public:
        C(const char*s1,const char*s2):A(s1)
        {
            cout<<s2<<endl;
        }
    };

    class D:public B,C
    {
    public:
        D(const char *s1,const char *s2,const char*s3,const char*s4):B(s1,s2),C(s1,s3),A(s1)
        {
            cout <<s4<<endl;
        }
    };
    {
        D *ptr = new D("class A","class B","class C","class D");
        delete ptr;
        ptr = NULL;
    }
}


ListNode* mergeKLists(vector<ListNode*>& lists) {

    {
//        class Upper
//        {
//        public:
//            Upper(int min = 0):m_min(min)
//            {}
//            bool operator() (int value) const
//            {
//                std::cout << "() is called" << std::endl;
//                return value >= m_min;
//            }
//        private:
//            int m_min;
//        };
//        vector<int> intVec;
//        intVec.push_back(10);
//        intVec.push_back(6);
//        intVec.push_back(12);
//        vector<int>::iterator targetIterator = find_if(intVec.begin(), intVec.end(), Upper(10));
//        if (targetIterator != intVec.end())
//        {
//            cout << "value: " << *targetIterator << endl;
//        }
//return nullptr;
    }
    {
        struct Cmp {
            bool operator () ( const ListNode* a, const ListNode* b) const {
                return a->val > b->val;
            }
        };

        std::priority_queue< ListNode*, std::vector<ListNode*>, Cmp > q;
        for ( auto ele : lists ) {
            if (ele) q.push( ele );
        }

        ListNode dummyNode(-9999);
        ListNode* p = &dummyNode;

        while ( !q.empty() ) {
            auto top = q.top(); q.pop();

            p->next = top;
            p = top;
            if ( top->next )
                q.push( top->next );
        }

        return dummyNode.next;
    }
    struct Status {
        int val;
        ListNode *ptr;
        bool operator < (const Status &rhs) const {
            return val > rhs.val;
        }
    };

    priority_queue <Status> q;
    for (auto node : lists) {
        if (node) q.push({node->val, node});
    }
    ListNode head, *tail = &head;
    while (!q.empty()) {
        auto f = q.top(); q.pop();
        tail->next = f.ptr;
        tail = tail->next;
        if (f.ptr->next) q.push({f.ptr->next->val, f.ptr->next});
    }
    return head.next;
    return nullptr;
}


/*                  合并k个已排序链表
    给你一个链表数组，每个链表都已经按升序排列。
    请你将所有链表合并到一个升序链表中，返回合并后的链表。

    示例 1：
    输入：lists = [[1,4,5],[1,3,4],[2,6]]
    输出：[1,1,2,3,4,4,5,6]
    解释：链表数组如下：
    [
      1->4->5,
      1->3->4,
      2->6
    ]
    将它们合并到一个有序链表中得到。
    1->1->2->3->4->4->5->6
*/
void tst_mergeKLists() {
    int listcount = 3;
    // vector<ListNode*> lists = ConstructTestListByParam(1, 3000, 4, listcount);

    vector<ListNode*> lists;
    std::vector< int > a1{ 1, 2, 3, 4, 6};
    auto l1 = ConstructTestList(a1);

    std::vector< int > a2{ 123, 987, 3000, 4213, 98886};
    auto l2 = ConstructTestList(a2);

    std::vector< int > a3{ 5678, 20009, 376654};
    auto l3 = ConstructTestList(a3);
    // for ( int i = 0; i < listcount; i ++ ) {
    //      PrintList(lists[i]);
    // }
    lists.push_back(l1);lists.push_back(l2);lists.push_back(l3);

    auto head = mergeKLists(lists);// heap sort

    PrintList( head );
}

/*
    61. 旋转链表
    给定一个链表，旋转链表，将链表每个节点向右移动 k 个位置，其中 k 是非负数。

    示例 1:
    输入: 1->2->3->4->5->NULL, k = 2
    输出: 4->5->1->2->3->NULL
    解释:
    向右旋转 1 步: 5->1->2->3->4->NULL
    向右旋转 2 步: 4->5->1->2->3->NULL

    示例 2:
    输入: 0->1->2->NULL, k = 4
    输出: 2->0->1->NULL
    解释:
    向右旋转 1 步: 2->0->1->NULL
    向右旋转 2 步: 1->2->0->NULL
    向右旋转 3 步: 0->1->2->NULL
    向右旋转 4 步: 2->0->1->NULL
*/
ListNode* rotateRight(ListNode* head, int k) {
    {
        int len = 0;
        for(auto p=head;p;p=p->next) len++;
        if (k >= len) return head;

        ListNode* fast = head;
        ListNode* slow = head;

        for (int i = 0; i < k; i ++ ) {
            fast = fast->next;
        }
        while ( fast && fast->next ) {
            fast = fast->next->next;
            slow = slow->next;
        }

        fast->next = head;

        auto newhead = slow->next;
        slow->next = nullptr;

        return newhead;
    }
    auto tmp = head;
    int len = GetListLength(head);// 第 1 步：先要知道链表有多少个结点

    k = k%len;
    if (k == 0) {
        return head;
    }
    ListNode* fast = head, *slow = head;
    for ( int i = 0; i < k; i ++ ) {
        fast = fast->next;
    }
    // std::cout << "fast=" << fast->val << std::endl;
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }
    ListNode* newHead = slow->next;
    slow->next = nullptr;
    fast->next = head;
    // std::cout << "fast=" << fast->val << std::endl;
    // std::cout << "slow=" << slow->val << std::endl;
    return newHead;
}
void tst_rotateRight() {
    PrintList(g_pListHead);
    std::cout << "before handle list -----" << std::endl;
    auto newlist = rotateRight(g_pListHead, 3);
    PrintList(newlist);
    // PrintList(g_pListHead);
}

/*
    剑指 Offer 40. 最小的k个数
    输入整数数组 arr，找出其中最小的 k 个数。
    例如，输入4、5、1、6、2、7、3、8这8个数字，则最小的4个数字是1、2、3、4。

    示例 1：
    输入：arr = [3,2,1], k = 2
    输出：[1,2] 或者 [2,1]

    示例 2：
    输入：arr = [0,1,2,1], k = 1
    输出：[0]

    -----------------------------------------------------------------------
    时间复杂度： O(N)
    空间复杂度： O(logN)
    -----------------------------------------------------------------------
*/
class CLeastNumbser {
public:

    std::vector<int> quickSort(std::vector<int>& arr, int k, int l, int r) {

        {
            //        int i = l, j = r;
            //        while (i < j) {
            //            while (i < j && arr[j] >= arr[l]) j--;
            //            while (i < j && arr[i] <= arr[l]) i++;
            //            swap(arr[i], arr[j]);
            //        }
            //        swap(arr[i], arr[l]);
            //        if (i > k) return quickSort(arr, k, l, i - 1);
            //        if (i < k) return quickSort(arr, k, i + 1, r);
        }

        int i = l, j = r;
        int pivot = arr[ l ];
        if ( i < j ) {
            while ( i < j && arr[ j ] >= pivot ) j --;
            if ( i < j ) arr[ i++ ] = arr[ j ];

            while ( i < j && arr[ i ] <= pivot ) i ++;
            if ( i < j ) arr[ j-- ] = arr[ i ];
        }
        arr[ i ] = pivot;
        if ( i > k ) return quickSort( arr, k, l, i - 1 );
        if ( i < k ) return quickSort( arr, k, i + 1, r );

        vector<int> res;
        res.assign(arr.begin(), arr.begin() + k);
        return res;
    }
    std::vector<int> getLeastNumbers(std::vector<int>& arr, int k) {
        if (k >= arr.size()) return arr;

        return quickSort(arr, k, 0, arr.size() - 1);
    }
};

void tst_getLeastNumbers() {
    std::vector<int> arr{ 4, 5, 6, 7, 8, -1, 77, 133, 89, 61 };

    CLeastNumbser cln;
    auto res = cln.getLeastNumbers(arr, 3);
    PrintInContainer(res);
}



int Test_ListEntry() {

        tst_twoSum(); return 1;
    tst_maxSubArray(); return 1;


    tst_getLeastNumbers(); return 1;

    tst_rotateRight(); return 1;

    tst_reversePrint(); return 1;

    tst_detectCycle(); return 1;

    tst_mergeKLists(); return 1;

    tst_getKthFromEnd(); return 1;

    tst_lingxingjicheng(); return 1;

    tst_reverselist(); return 1;

    tst_deleteNodeEx(); return 1;

    tst_deleteNode(); return 1;

    tst_maxProfit(); return 1;

    tst_removeNthFromEnd(); return 1;

    tst_CLongestHuiWen(); return 1;

    tst_longestPalindrome(); return 1;

    tst_removeDuplicates(); return 1;

    tst_longestCommonPrefix(); return 1;

    Test_removeEle(); return 1;

    tst_findRepeatNumber(); return 1;

    tst_mergeTwoArray(); return 1;

    tst_reorderList(); return 1;


    Test_moveZeroes(); return 1;
    
    Test_majorityElement_3(); return 1;
    
    Test_majorityElement(); return 1;
    
    Test_singleNumber(); return 1;

    Test_copyRandomList(); return 1;

    Test_listOfDepth(); return 1;

    Test_isPalindrome(); return 1;

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



