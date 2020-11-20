#pragma once

#include <stdio.h>


struct ListNode {
public:
    int       m_nValue;
    ListNode* next_;

public:
    ListNode() : m_nValue( -1 ), next_(nullptr) {

    }
    ListNode(int val) : m_nValue( val ), next_(nullptr) {

    }
};

struct DoubleListNode {
public:
    int       m_nValue;
    DoubleListNode* left;
    DoubleListNode* right;

public:
    DoubleListNode() : m_nValue( -1 ), left(nullptr), right(nullptr) {

    }
    DoubleListNode(int val) : m_nValue( val ), left(nullptr), right(nullptr) {

    }
};


//#define open TRUE

#ifdef open
#define isdllexport __declspec(dllexport)
#else
#define  isdllexport
#endif

isdllexport ListNode* CreateListNode(int value);
isdllexport void ConnectListNodes(ListNode* pCurrent, ListNode* pNext);
isdllexport void PrintListNode(ListNode* pNode);
isdllexport void PrintList(ListNode* pHead);
isdllexport void DestroyList(ListNode* pHead);
isdllexport void AddToTail(ListNode** pHead, int value);
isdllexport void RemoveNode(ListNode** pHead, int value);
