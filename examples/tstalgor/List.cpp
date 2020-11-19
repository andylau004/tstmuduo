
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

ListNode* CreateListNode(int value)
{
    ListNode* pNode = new ListNode();
    pNode->m_nValue = value;
    pNode->next_ = NULL;

    return pNode;
}

void ConnectListNodes(ListNode* pCurrent, ListNode* pNext) {
    if(pCurrent == NULL) {
        printf("Error to connect two nodes.\n");
        exit(1);
    }
    pCurrent->next_ = pNext;
}
void PrintListNode(ListNode* pNode) {
    if (pNode == NULL) {
        printf("The node is NULL\n");
    } else {
        printf("The key in node is %d.\n", pNode->m_nValue);
    }
}

void PrintList(ListNode* pHead)
{
//    printf("PrintList starts.\n");
    printf("\n");
    while (pHead) {
        printf("%d\t", pHead->m_nValue);
        pHead = pHead->next_;
    }
    printf("\n");
//    printf("\nPrintList ends.\n");
}

void DestroyList(ListNode* pHead)
{
    ListNode* pNode = pHead;
    while (pNode) {
        pHead = pHead->next_;
        delete pNode;
        pNode = pHead;
    }
}

void AddToTail(ListNode** pHead, int value)
{
    ListNode* pNew = new ListNode(value);
    if (*pHead == NULL) {
        *pHead = pNew;
    } else {
        ListNode* pNode = *pHead;
        while (pNode->next_)
            pNode = pNode->next_;
        pNode->next_ = pNew;
    }
}

void RemoveNode(ListNode** pHead, int delVal)
{
    if(pHead == NULL || *pHead == NULL)
        return;

    ListNode* pToBeDel = NULL;
    if ( (*pHead)->m_nValue == delVal ) {
        pToBeDel = *pHead;
        *pHead = (*pHead)->next_;
    } else {// 运行到此处，pHead的值必然不等于查找值, 只能比较next的值跟查找值
        ListNode* pNode = *pHead;
        while (pNode && pNode->next_->m_nValue != delVal)
            pNode = pNode->next_;

        if (pNode && pNode->next_->m_nValue == delVal) {
            pToBeDel = pNode->next_;
            pNode->next_ = pNode->next_->next_;
        }
    }
    if (pToBeDel) {
        delete pToBeDel; pToBeDel = NULL;
    }
}

