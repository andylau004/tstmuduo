
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

ListNode* CreateListNode(int value)
{
    ListNode* pNode = new ListNode();
    pNode->val = value;
    pNode->next = NULL;

    return pNode;
}

void ConnectListNodes(ListNode* pCurrent, ListNode* pNext) {
    if(pCurrent == NULL) {
        printf("Error to connect two nodes.\n");
        exit(1);
    }
    pCurrent->next = pNext;
}
void PrintListNode(ListNode* pNode) {
    if (pNode == NULL) {
        printf("The node is NULL\n");
    } else {
        printf("The key in node is %d.\n", pNode->val);
    }
}

void PrintList(ListNode* pHead)
{
//    printf("PrintList starts.\n");
    printf("\n");
    while (pHead) {
        printf("%d\t", pHead->val);
        pHead = pHead->next;
    }
    printf("\n");
//    printf("\nPrintList ends.\n");
}

void DestroyList(ListNode* pHead)
{
    ListNode* pNode = pHead;
    while (pNode) {
        pHead = pHead->next;
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
        while (pNode->next)
            pNode = pNode->next;
        pNode->next = pNew;
    }
}

void RemoveNode(ListNode** pHead, int delVal)
{
    if(pHead == NULL || *pHead == NULL)
        return;

    ListNode* pToBeDel = NULL;
    if ( (*pHead)->val == delVal ) {
        pToBeDel = *pHead;
        *pHead = (*pHead)->next;
    } else {// 运行到此处，pHead的值必然不等于查找值, 只能比较next的值跟查找值
        ListNode* pNode = *pHead;
        while (pNode && pNode->next->val != delVal)
            pNode = pNode->next;

        if (pNode && pNode->next->val == delVal) {
            pToBeDel = pNode->next;
            pNode->next = pNode->next->next;
        }
    }
    if (pToBeDel) {
        delete pToBeDel; pToBeDel = NULL;
    }
}

ListNode* ConstructList(std::vector<int>& vecInts) {
    std::vector< ListNode* > vecNodes;

    for (auto ii : vecInts) {
        vecNodes.push_back(CreateListNode(ii));
    }

    for (size_t i = 0; i < vecNodes.size(); i++) {
        auto next = i + 1;
        if (next != vecNodes.size()) {
            ConnectListNodes(vecNodes[i], vecNodes[next]);
        }
    }
    return *vecNodes.begin();
}

int GetListLength(ListNode* listHead) {
    if (!listHead) return 0;
    int len = 0;;
    while (listHead) {
        len++;
        listHead = listHead->next;
    }
    return len;
}

