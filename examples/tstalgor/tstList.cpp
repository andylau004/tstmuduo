



#include "tstList.h"

#include <stack>


using namespace std;










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

    for (int i = 0 ; i < 9 ; ++ i ) {
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


// 测试list操作算法总入口
int tst_ListEntry_() {
    insert_sort_1(); return 1;

    fast_slow_ptr(); return 1;

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




