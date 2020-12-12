



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


-----------------------------------------------------------------------
复杂度分析

时间复杂度： O(N)，其中 N 是链表的节点数。
当链表中不存在环时，快指针将先于慢指针到达链表尾部，链表中每个节点至多被访问两次。
当链表中存在环时，每一轮移动后，快慢指针的距离将减小一。而初始距离为环的长度，因此至多移动 NN 轮。
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
题意：给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。

为了表示给定链表中的环，使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。

如果 pos 是 -1，则在该链表中没有环。
*/
ListNode* detectCycle(ListNode* head) {
    if (!head) return nullptr;

    ListNode* fast = head, * slow = head;
    while ( fast && fast->next ) {
        fast = fast->next->next;
        slow = slow->next;
        if (slow == fast) {
            ListNode* idx1 = fast;
            ListNode* idx2 = head;
            while ( idx1 != idx2 ) {
                idx1 = idx1->next;
                idx2 = idx2->next;
            }
            return idx2;
        }
    }
    return nullptr;
}
void Test_detectCycle() {
    
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

    给你两个链表 list1 和 list2 ，它们包含的元素分别为 n 个和 m 个。
    请你将 list1 中第 a 个节点到第 b 个节点删除，并将list2 接在被删除节点的位置。

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
给你链表的头结点 head ，请将其按 升序 排列并返回 排序后的链表 。

进阶：你可以在 O(nlogn) 时间复杂度和常数级空间复杂度下，对链表进行排序吗？
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
*/
ListNode* reverselist(ListNode* head) {

    ListNode* pre = head, *cur = nullptr;
    while (pre) {

        ListNode* t = pre->next;
        pre->next = cur;

        cur = pre;
        pre = t;
    }
    return cur;
}
void Test_reverselist() {
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

示例 2:
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
    编号：27. 移除元素
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
size_t removeElement(vector<int>& nums, int checkVal) {
    size_t slowIdx = 0;
    for (size_t fastIdx = 0; fastIdx < nums.size(); fastIdx++) {
        if (checkVal != nums[fastIdx]) {
            nums[slowIdx++] = nums[fastIdx];
        }
    }
    return slowIdx;
}
void Test_removeEle() {
    size_t ret = 0;
    
    std::vector<int> v1{ 3,2,2,3 };
    ret = removeElement(v1, 3);
    std::cout << "ret=" << ret << std::endl;
    PrintInContainer(v1);
    
    std::vector<int> v2{ 0,1,2,2,3,0,4,2 };
    ret = removeElement(v2, 2);
    std::cout << "ret=" << ret << std::endl;
    PrintInContainer(v2);
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
复杂度分析

时间复杂度：O(n)，只需要遍历一次。
空间复杂度：O(1)，只使用了常数个变量。
-----------------------------------------------------------------------
*/
int maxProfit(vector<int>& prices) {
    int inf = 1e9;
    int minprice = inf, maxprofit = 0;
    for (int price : prices) {
        maxprofit = max(maxprofit, price - minprice);
        minprice = min(price, minprice);
    }
    return maxprofit;
}
void Test_maxProfit() {
    std::vector<int> v1{ 7, 1, 5, 3, 6, 4 };
    std::cout << "maxProfit=" << maxProfit(v1) << std::endl;
}


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
    int maxValue = INT_MIN;
    int dp = 0, res = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        dp = std::max(dp + nums[i], nums[i]);
        res = std::max(dp, res);
    }
    return res;
}
void Test_maxSubArray() {
    // std::vector<int> v1{ 7, 1, 5, 3, 6, 4 };
    std::vector<int> v1{ -2,1,-3,4,-1,2,1,-5,4 };
    std::cout << "maxSubArray=" << maxSubArray(v1) << std::endl;
}







int Test_ListEntry() {


    Test_maxSubArray(); return 1;

    Test_maxProfit(); return 1;

    Test_removeEle(); return 1;
    
    Test_moveZeroes(); return 1;
    
    Test_majorityElement_3(); return 1;
    
    Test_majorityElement(); return 1;
    
    Test_singleNumber(); return 1;
    Test_reverselist(); return 1;

    Test_copyRandomList(); return 1;

    Test_listOfDepth(); return 1;

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



