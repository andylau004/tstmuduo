#pragma once

#include <stdio.h>
#include <vector>





struct ListNode {
public:
    int       val;
    ListNode* next;

public:
    ListNode() : val( -1 ), next(nullptr) {

    }
    ListNode(int val) : val( val ), next(nullptr) {

    }
};

struct DoubleListNode {
public:
    int       val;
    DoubleListNode* left;
    DoubleListNode* right;

public:
    DoubleListNode() : val( -1 ), left(nullptr), right(nullptr) {

    }
    DoubleListNode(int val) : val( val ), left(nullptr), right(nullptr) {

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


extern int GetListLength(ListNode* listHead);
extern ListNode* ConstructList(std::vector<int>& vecInts);

extern std::vector< ListNode* > ConstructTestListByParam(int start, int end, int numCount, int listCount);

