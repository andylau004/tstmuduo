



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
void tst_MergeTwoList() {
    ListNodeEx* phead1 = NULL;
//    for (int i = 0 ; i < 9; ++ i )
    CreateList(phead1, 12);CreateList(phead1, 8);CreateList(phead1, 6);
    PrintList(phead1);

    ListNodeEx* phead2 = NULL;
//    for (int j = 31; j < 40; ++ j )
//        CreateList(phead2, j);
    CreateList(phead2, 198);CreateList(phead2, 89);CreateList(phead2, 13);
    PrintList(phead2);

    std::cout << "11111111111111" << std::endl;
    PrintList( MergeTwoList(phead1, phead2) );
    std::cout << "22222222222222" << std::endl;
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
    for (int index = 1; index < sizeof(arr)/sizeof(arr[0]); ++index) {
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

// 测试list操作算法总入口
int tst_ListOperation_() {
    tst_printfilecontent(); return 1;

    tst_max_array(); return 1;

    tst_shuffle(); return 1;

    tst_CircleList(); return 1;

    tst_MergeTwoList(); return 1;

    tst_RPrintList_fun(); return 1;

    tst_GetMiddleNode_fun(); return 1;

    tst_RGetKthNode_fun(); return 1;

    tst_reverse_list_1(); return 1;

}




