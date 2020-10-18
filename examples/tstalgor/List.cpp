
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

ListNode* CreateListNode(int value)
{
    ListNode* pNode = new ListNode();
    pNode->m_nValue = value;
    pNode->m_pNext = NULL;

    return pNode;
}

void ConnectListNodes(ListNode* pCurrent, ListNode* pNext) {
    if(pCurrent == NULL) {
        printf("Error to connect two nodes.\n");
        exit(1);
    }
    pCurrent->m_pNext = pNext;
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
    printf("PrintList starts.\n");

    ListNode* pNode = pHead;
    while (pNode) {
        printf("%d\t", pNode->m_nValue);
        pNode = pNode->m_pNext;
    }

    printf("\nPrintList ends.\n");
}

void DestroyList(ListNode* pHead)
{
    ListNode* pNode = pHead;
    while (pNode) {
        pHead = pHead->m_pNext;
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
        while (pNode->m_pNext)
            pNode = pNode->m_pNext;
        pNode->m_pNext = pNew;
    }
}

void RemoveNode(ListNode** pHead, int delVal)
{
    if(pHead == NULL || *pHead == NULL)
        return;

    ListNode* pToBeDel = NULL;
    if ( (*pHead)->m_nValue == delVal ) {
        pToBeDel = *pHead;
        *pHead = (*pHead)->m_pNext;
    } else {// 运行到此处，pHead的值必然不等于查找值, 只能比较next的值跟查找值
        ListNode* pNode = *pHead;
        while (pNode && pNode->m_pNext->m_nValue != delVal)
            pNode = pNode->m_pNext;

        if (pNode && pNode->m_pNext->m_nValue == delVal) {
            pToBeDel = pNode->m_pNext;
            pNode->m_pNext = pNode->m_pNext->m_pNext;
        }
    }
    if (pToBeDel) {
        delete pToBeDel; pToBeDel = NULL;
    }
}

