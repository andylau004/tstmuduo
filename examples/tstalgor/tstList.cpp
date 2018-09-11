



#include "tstList.h"












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
//插入一个新节点到链表中(放在链表头部)
void CreateList(ListNodeEx*& pHead, int newData) {
    ListNodeEx* p = new ListNodeEx;
    p->val  = newData;
    p->next = NULL;

    if (pHead == NULL) {
        pHead = p;
        return;
    }
    p->next = pHead;
    pHead = p;
}
void tst_reverse_list_1() {
    ListNodeEx* phead = NULL;

    for (int i = 0 ; i < 9 ; ++ i ) {
        CreateList(phead, i);
    }

    PrintList(phead);
    phead = Recrusive_ReverseList(phead);
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

    while (pQuick) {

    }
    return NULL;
}
void tst_GetMiddleNode_fun() {
    ListNodeEx* phead = NULL;
    for (int i = 0 ; i < 9; ++ i )
        CreateList(phead, i);
    PrintList(phead);
    std::cout << "GetMiddleNode ret=" << GetMiddleNode(phead)->val << std::endl;
}


// 测试list操作算法总入口
int tst_ListOperation_() {

    tst_RGetKthNode_fun(); return 1;

    tst_RGetKthNode_fun(); return 1;

    tst_reverse_list_1(); return 1;

}




