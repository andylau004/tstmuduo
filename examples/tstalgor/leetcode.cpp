

#include "leetcode.h"

#include "List.h"


//#define GOOGLE_STRIP_LOG 1
#include <glog/logging.h>
#define GOOGLE_STRIP_LOG 1



//#define LOG_TRACE LOG(INFO)
//#define LOG_DEBUG LOG(INFO)
#define LOG_INFOEX   LOG(INFO)
//#define GDLOG_TRACEEX LOG(INFO) << __PRETTY_FUNCTION__ << " this=" << this << " "
//#else



extern BstNode* g_pBstTree;


/* --------------------------------------------------------------------------
 1. Convert Sorted Array to Binary Search Tree
 2. Convert Sorted List to Binary Search Tree

 * --------------------------------------------------------------------------*/


// Convert Sorted Array to Binary Search Tree
// Given an array where elements are sorted in ascending order, convert it to a height balanced BST.
// 思路：
// 给定一个区间[left, right]，取其中值mid=(left+right)/2对应的元素作为二叉树的根。二叉树的左子树根的值为对[left, mid-1]继续操作的结果，右子树类似。

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {
    }
};

void someInOrder(TreeNode* root);
void InOrtderPtrint(TreeNode* root);


//class Solution {
//public:
    TreeNode* generateBST(int left, int right, std::vector<int>& vec) {
        if (left > right) {
            return nullptr;
        } else if (left == right) {
            return new TreeNode(vec[left]);
        } else {
            int mid1 = (left + right) >> 1;
            printf("mid1=%d\n", mid1);

            TreeNode* newnode = new TreeNode(vec[mid1]);
            newnode->left  = generateBST(0, mid1 - 1, vec);
            newnode->right = generateBST(mid1 + 1, right, vec);
            return newnode;
        }
    }
    TreeNode* sortedArrayToBst(std::vector<int>& vec) {
        return generateBST(0, vec.size() - 1, vec);
    }
    void InOrtderPtrint(TreeNode* root) {
        std::cout << std::endl;
        someInOrder(root);
        std::cout << std::endl;
    }
    void someInOrder(TreeNode* root) {
        if (!root) {
            printf("root is null\n");
            return;
        }
        if (root->left)
            someInOrder(root->left);
        std::cout << root->val << '\t';
        if (root->right)
            someInOrder(root->right);
    }

    ListNode* ConstructTestList(const std::vector<int>& vecInts) {
        std::vector< ListNode* > vecNodes;

        for ( auto ii : vecInts ) {
            vecNodes.push_back(CreateListNode(ii));
        }

        for ( int i = 0; i < vecNodes.size(); i ++ ) {
            auto next = i + 1;
            if ( next != vecNodes.size() ) {
                ConnectListNodes(vecNodes[i], vecNodes[next]);
            }
        }
        return *vecNodes.begin();
    }

// Convert Sorted List to Binary Search Tree -- LeetCode 将有序链表转为二叉搜索树
class CSortedListToBst {
public:

    ListNode* reverseList( ListNode* pHead ) {
        if (!pHead) return nullptr;
        {
            ListNode* prePtr = nullptr, *curPtr = pHead, *nextPtr = curPtr->m_pNext;
            while (nextPtr) {
                curPtr->m_pNext = prePtr;
                prePtr = curPtr;

                curPtr = nextPtr;
                nextPtr = nextPtr->m_pNext;
            }
            curPtr->m_pNext = prePtr;
            return curPtr;
        }

        ListNode* prePtr = nullptr, *curPtr = pHead, *nextPtr = curPtr->m_pNext;
        while (nextPtr) {
            curPtr->m_pNext = prePtr;
            prePtr = curPtr;

            curPtr = nextPtr;
            nextPtr = nextPtr->m_pNext;
        }
        curPtr->m_pNext = prePtr;
        return curPtr;
    }

    void TstList1() {
        ListNode* pNode1 = CreateListNode(11);
        ListNode* pNode2 = CreateListNode(23);
        ListNode* pNode3 = CreateListNode(35);
        ListNode* pNode4 = CreateListNode(47);
        ListNode* pNode5 = CreateListNode(51);
        ListNode* pNode6 = CreateListNode(53);
        ListNode* pNode7 = CreateListNode(198);
        ListNode* pNode8 = CreateListNode(2739);
        ListNode* pNode9 = CreateListNode(31798);

        ConnectListNodes(pNode1, pNode2);
        ConnectListNodes(pNode2, pNode3);
        ConnectListNodes(pNode3, pNode4);
        ConnectListNodes(pNode4, pNode5);
        ConnectListNodes(pNode5, pNode6);
        ConnectListNodes(pNode6, pNode7);
        ConnectListNodes(pNode7, pNode8);
        ConnectListNodes(pNode8, pNode9);

        PrintList(pNode1);

        ListNode* pReverseList = reverseList( pNode1 );
        PrintList(pReverseList);
    }



    ListNode* ConstructTestList() {
        ListNode* pNode1 = CreateListNode(11);
        ListNode* pNode2 = CreateListNode(23);
        ListNode* pNode3 = CreateListNode(35);
        ListNode* pNode4 = CreateListNode(47);
        ListNode* pNode5 = CreateListNode(51);
        ListNode* pNode6 = CreateListNode(53);
        ListNode* pNode7 = CreateListNode(198);
        ListNode* pNode8 = CreateListNode(2739);
        ListNode* pNode9 = CreateListNode(31798);

        ConnectListNodes(pNode1, pNode2);
        ConnectListNodes(pNode2, pNode3);
        ConnectListNodes(pNode3, pNode4);
        ConnectListNodes(pNode4, pNode5);
        ConnectListNodes(pNode5, pNode6);
        ConnectListNodes(pNode6, pNode7);
        ConnectListNodes(pNode7, pNode8);
        ConnectListNodes(pNode8, pNode9);
        return pNode1;
    }

    void TstEntry() {
        ListNode* pNode1 = CreateListNode(11);
        ListNode* pNode2 = CreateListNode(23);
        ListNode* pNode3 = CreateListNode(35);
        ListNode* pNode4 = CreateListNode(47);
        ListNode* pNode5 = CreateListNode(51);
        ListNode* pNode6 = CreateListNode(53);
        ListNode* pNode7 = CreateListNode(198);
        ListNode* pNode8 = CreateListNode(2739);
        ListNode* pNode9 = CreateListNode(31798);

        ConnectListNodes(pNode1, pNode2);
        ConnectListNodes(pNode2, pNode3);
        ConnectListNodes(pNode3, pNode4);
        ConnectListNodes(pNode4, pNode5);
        ConnectListNodes(pNode5, pNode6);
        ConnectListNodes(pNode6, pNode7);
        ConnectListNodes(pNode7, pNode8);
        ConnectListNodes(pNode8, pNode9);

        TreeNode* root = SortedListToBst(pNode1);
        PrintList(pNode1);

        std::cout << std::endl << std::endl
                  << " in order print tree beg---------" << std::endl;
        someInOrder(root);
        std::cout << std::endl
                  << " in order print tree end---------" << std::endl;

    }
    TreeNode* SortedListToBst(ListNode* head) {
        if (!head) return nullptr;
        helper(head, nullptr);
    }
    TreeNode* helper(ListNode* head, ListNode* tail) {
        if (head == tail) return nullptr;

        ListNode* slow = head, *fast = head;
//        std::cout << "slow=" << slow <<std::endl;
//        std::cout << "fast=" << fast <<std::endl;

        while (fast != tail && fast->m_pNext != tail) {
            slow = slow->m_pNext;
            fast = fast->m_pNext->m_pNext;
        }

//        std::cout << "slow->val=" << slow->m_nValue << std::endl;
//        std::cout << "fast->val=" << fast->m_nValue << std::endl;
//        return nullptr;

        TreeNode* root = new TreeNode(slow->m_nValue);
        root->left = helper(head, slow);
        root->right = helper(slow->m_pNext, tail);
        return root;
    }

};





















// Given preorder and inorder traversal of a tree, construct the binary tree.
// Note:
// You may assume that duplicates do not exist in the tree.

// For example, given
// preorder = [3,9,20,15,7]
// inorder  = [9,3,15,20,7]

// Return the following binary tree:

//    3
//   / \
//  9  20
//    /  \
//   15   7

/*
 *
 *  首先要知道一个结论，前序/后序+中序序列可以唯一确定一棵二叉树，所以自然而然可以用来建树。
 *  看一下前序和中序有什么特点，前序1,2,4,7,3,5,6,8 ，中序4,7,2,1,5,3,8,6；
 *
 *  有如下特征：
 *
 *  1. 前序中左起第一位1肯定是根结点，我们可以据此找到中序中根结点的位置rootin；
 *  2. 中序中根结点左边就是左子树结点，右边就是右子树结点，即[左子树结点，根结点，右子树结点]，
 *     我们就可以得出左子树结点个数为int left = rootin - leftin;；
 *  3. 前序中结点分布应该是：[根结点，左子树结点，右子树结点]；
 *  4. 根据前一步确定的左子树个数，可以确定前序中左子树结点和右子树结点的范围；
 *  5. 如果我们要前序遍历生成二叉树的话，下一层递归应该是：
 *      左子树：root->left = pre_order(前序左子树范围，中序左子树范围，前序序列，中序序列);；
 *      右子树：root->right = pre_order(前序右子树范围，中序右子树范围，前序序列，中序序列);。
 *  6. 每一层递归都要返回当前根结点root；
 */

class CBuildBstByPreInorder {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        int pos = 0;
        return buildTree(preorder, pos, inorder, 0, preorder.size() - 1);
    }

    TreeNode* buildTree(vector<int>& preorder, int& pos, vector<int>& inorder, int left, int right) {
        if (pos >= preorder.size()) return 0;
        int i = left;
        for (; i <= right; ++i) {
            if (inorder[i] == preorder[pos]) break;
        }
        TreeNode* newNode = new TreeNode(preorder[pos]);
        if (left <= i-1)  newNode->left  = buildTree(preorder, ++pos, inorder, left, i-1);  // 左子树
        if (i+1 <= right) newNode->right = buildTree(preorder, ++pos, inorder, i + 1, right);  // 右子树
        return newNode;
    }

    void WorkEntry() {
        std::vector < int > preOrder = {3,9,20,15,7};
        std::vector < int > inOrder  = {9,3,15,20,7};
        TreeNode* root = buildTree(preOrder, inOrder);
        InOrtderPtrint(root);
    }
};




    void tst_1_lc() {
        std::vector<int> vecArr{ -10,-3,0,5,9};
        TreeNode* root = sortedArrayToBst(vecArr);
        std::cout << std::endl;
        someInOrder(root);
        std::cout << std::endl;
    }



// LeetCode 215. Kth Largest Element in an Array--数字第K大的元素--最大堆或优先队列--C++,Python解法

    int findKthLargest(vector<int>& nums, int k) {

        std::priority_queue < int, vector< int > > p;

        for (size_t i = 0; i < nums.size(); ++ i) {

            if (p.size() < k) p.push(nums[i]);
            else if (p.top() < nums[i]) {
                p.pop();
                p.push(nums[i]);
            }
        }

        return p.top();
    }
    void tst_KthBig() {
        std::vector < int > vRandom = { 9, 1, 123, 6613, 31, 25 };
        std::cout << "Kth=" << findKthLargest( vRandom, 1 ) << std::endl;
    }



/*
    给定两个二叉树，想象当你将它们中的一个覆盖到另一个上时，两个二叉树的一些节点便会重叠。

    你需要将他们合并为一个新的二叉树。合并的规则是如果两个节点重叠，那么将他们的值相加作为节点合并后的新值，否则不为 NULL 的节点将直接作为新二叉树的节点。

    示例 1:

    输入:
        Tree 1                     Tree 2
              1                         2
             / \                       / \
            3   2                     1   3
           /                           \   \
          5                             4   7
    输出:
    合并后的树:
             3
            / \
           4   5
          / \   \
         5   4   7
    注意: 合并必须从两个树的根节点开始。
*/

    struct TreeNode* mergeTrees(struct TreeNode* t1, struct TreeNode* t2){
        if (!t1 && !t2) {
            return nullptr;
        }
        if (!t1 && t2) {
            return t2;
        }
        if (t1 && !t2) {
            return t1;
        }
        if (t1 && t2) {
            t1->val = t1->val + t2->val;
        }
        t1->left=mergeTrees(t1->left,t2->left);
        t1->right=mergeTrees(t1->right,t2->right);
        return t1;
    }



/*
 * 根据每日 气温 列表，请重新生成一个列表，对应位置的输入是你需要再等待多久温度才会升高超过该日的天数。如果之后都不会升高，请在该位置用 0 来代替。
 * 例如，给定一个列表 temperatures = [73, 74, 75, 71, 69, 72, 76, 73]，你的输出应该是 [1, 1, 4, 2, 1, 1, 0, 0]。
 * 提示：气温 列表长度的范围是 [1, 30000]。每个气温的值的均为华氏度，都是在 [30, 100] 范围内的整数。
 * // https://leetcode-cn.com/problems/daily-temperatures/solution/leetcode-tu-jie-739mei-ri-wen-du-by-misterbooo/
*/
std::vector<int> dailyTemperatures(std::vector<int>& temperatures) {
    int n = temperatures.size();
    std::vector<int> res(n, 0);
    std::stack<int> st;
    for (size_t i = 0; i < temperatures.size(); ++i) {
        while (!st.empty() && temperatures[i] > temperatures[st.top()]) {
            auto t = st.top(); st.pop();
            res[t] = i - t;
        }
        st.push(i);
    }
    return res;
}


/*
        给定一个整数 n，生成所有由 1 ... n 为节点所组成的二叉搜索树。

        示例:

        输入: 3
        输出:
        [
          [1,null,3,2],
          [3,2,null,1],
          [3,1,null,null,2],
          [2,1,3],
          [1,null,2,null,3]
        ]

        解释:
        以上的输出对应以下 5 种不同结构的二叉搜索树：

           1         3     3      2      1
            \       /     /      / \      \
             3     2     1      1   3      2
            /     /       \                 \
           2     1         2                 3
*/

class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n + 2, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i <= n; ++i) {
            for (int j = 0; j < i; ++j) {
                dp[i] += dp[j] * dp[i - j - 1];
            }
        }
        return dp[n];
    }
};




/*
    思路一：中心扩散
    从开始每个字符进行判断，回文串字符个数可能为奇数或偶数

    如果是奇数，则中心字符为当前字符，然后向两边扩散
    如果是偶数，则中心字符为当前字符和下一个字符，然后向两边扩散
*/
class CheckHuiWen
{
public:
    void IsHuiWen() {
        std::string str = "abtx21ba";

        int sum = 0;
        for (size_t i = 0; i < str.size(); ++ i) {
            sum += helper(str, i, i);
            sum += helper(str, i, i+1);
        }
        std::cout << "sum=" << sum << std::endl;
        std::cout << "str.size=" << str.size() << std::endl;
    }

    int helper(std::string& str, int i, int j) {
        int sum = 0;
        if (i >= 0 && j < str.size() && str[i] == str[j]) {
            --i, j++, sum++;
        }
        return sum;
    }
};


/*
    给定一个二叉树，判断它是否是高度平衡的二叉树。

    本题中，一棵高度平衡二叉树定义为：

    一个二叉树每个节点 的左右两个子树的高度差的绝对值不超过1。

    示例 1:

    给定二叉树 [3,9,20,null,null,15,7]

        3
       / \
      9  20
        /  \
       15   7
    返回 true

    示例 2:

    给定二叉树 [1,2,2,3,3,null,null,4,4]

           1
          / \
         2   2
        / \
       3   3
      / \
     4   4
    返回 false
*/

int Depth(BstNode* root) {
    if (!root) return 0;
    return std::max(Depth(root->lchild_), Depth(root->rchild_)) + 1;
}
bool isBalanced(BstNode* root) {
    if (!root) return true;
    int d = abs( Depth(root->lchild_) - Depth(root->rchild_) );
    return d <= 1 && isBalanced(root->lchild_) && isBalanced(root->rchild_);
}

// Leetcode 98. 验证二叉搜索树
/*
        给定一个二叉树，判断其是否是一个有效的二叉搜索树。

        假设一个二叉搜索树具有如下特征：

        节点的左子树只包含小于当前节点的数。
        节点的右子树只包含大于当前节点的数。
        所有左子树和右子树自身必须也是二叉搜索树。
        示例 1:
        输入:
            2
           / \
          1   3
        输出: true

        示例 2:
        输入:
            5
           / \
          1   4
             / \
            3   6
        输出: false
        解释: 输入为: [5,1,4,null,null,3,6]。
             根节点的值为 5 ，但是其右子节点值为 4 。
*/
class CheckIfBst {
public:
//    bool IsValidBst(BstNode* root) {
//        if (!root) return true;
//        return IsLeftBst(root->lchild_, root->key_) && IsRightBst(root->rchild_, root->key_);
//    }
//    bool IsLeftBst(BstNode* root, int val) {
//        if (IsValidBst(root)) {
//            while (root) {
//                if (root->key_ >= val) return fasle;
//                root = root->rchild_;
//            }
//            return true;
//        } else {
//            return false;
//        }
//    }
//    bool IsRightBst(BstNode* root, int val) {
//        if (IsValidBst(root)) {

//        }
//    }

    bool IsValidBst_1(BstNode* root) {
//        std::vector<int> arrVal;
//        inorder(root, arrVal);
//        for (size_t i = 0; i < arrVal.size(); i++) {
//            if (arrVal[i] <= arrVal[i-1]) return false;
//        }
        return true;
    }
    void inorder(BstNode* root, std::vector<int>& arrVal) {
//        if (!root) return;
//        inorder(root->lchild_, arrVal);
//        arrVal.push_back(root->key_);
//        inorder(root->rchild_, arrVal);
    }

};



#define DISABLE_COPY(ClassName) \
    ClassName(const ClassName&) = delete;
#define DISABLE_ASSIGN(ClassName) \
    ClassName& operator=(const ClassName&) = delete;


class ObjA
{
//public:
    DISABLE_COPY(ObjA)
    DISABLE_ASSIGN(ObjA)
public:
    ObjA() {

    }
public:
    void PrintSomeInfo() {
        std::cout << "val ---- " << std::endl;
    }
    static ObjA* GetInstance() {
        return (new ObjA);
    }
};

/**
 * 二分查找
 * @param n int整型 数组长度
 * @param v int整型 查找值
 * @param a int整型vector 有序数组
 * @return int整型
 */
int upper_bound_(int n, int v, vector<int>& a) {
    int left = 0, right = n - 1;
    int mid = 0;
    while ( left < right ) {
        mid = (left + right) >> 1;
//        sleep(1);
//        std::cout << "mid=" << mid  << std::endl;
//        if (a[mid] < v) {
//            right = mid + 1;
//        } else {
//            right = mid;
//        }
        if ( a[ mid ] >= v ) {
            if ( mid == 0 || a[mid - 1] < v ) {
                return mid/* + 1*/;
            }
            else {
                right = mid;
            }
        } else {
            left = mid + 1;
        }
    }
    return n + 1;
}

void tstupper_bound() {
    std::vector<int> a{1, 2, 3, 4, 4, 5};
    std::cout << upper_bound_(a.size(), 4, a) << std::endl;
}

void LeetCodeEntry() {

    std::vector<int> a{11, 23, 35, 47, 51, 53, 198, 2739, 31798};

    ListNode* retnode = ConstructTestList(a);
    PrintList(retnode);
    return;

    tstupper_bound();
    return;
    {
        list<int> l1 = { 1, 2, 3, 4, 5 };
        list<int> l2 = { 6, 7, 8 };
        list<int>::iterator it;

        // iterator pointing to 1
        it = l1.begin();

        // advance the iterator by 2 positions
        printf("--- it=%d\n", *it);

        printf("--- l2 beg\n");
        advance(it, 2);
        printf("--- l2 end\n");
        printf("--- it=%d\n", *it);
        printf("\n"); //

        // transfer 3, 4 and 5 at the
        // beginning of l2
        l2.splice(l2.begin(), l2, it, l1.end());

        cout << "list l2 after splice operation" << endl;
        for (auto x : l2)
            cout << x << " ";

        cout << std::endl;


        printf("l1 beg\n");
        PrintInContainer(l1);//
        printf("l1 end\n");
        return;
    }
    {
        std::list < int > l1, l2;
        std::list < int >::iterator it;

        for ( int i = 0; i < 4; i ++ ) {
            l1.push_back(i+1);
        }
        for ( int i = 10; i <= 30; i += 10 ) {
            l2.push_back(i);
        }
        printf("--- l2 beg\n");
        PrintInContainer(l2);//  1 10 20 30 2 3 4
        printf("--- l2 end\n");
        printf("\n"); //

        it = l1.begin();
        ++ it;
        printf("it=%d\n", *it); // 2

        l1.splice(it, l2);

        PrintInContainer(l1);//  1 10 20 30 2 3 4
        printf("\n"); //

        printf("l2 beg\n");
        PrintInContainer(l2);//
        printf("l2 end\n");

        return;
    }

    {
        CSortedListToBst listObj;
        listObj.TstList1();
        return;
    }
    {
        ObjA* oa = ObjA::GetInstance();
        oa->PrintSomeInfo();
        ObjA ot;
//        ObjA ov = ot;
        return ;
    }

    {
//        string home = "./logs";  //要先创建此目录,否则运行报错.
//        google::InitGoogleLogging("tstout");

//        string info_log = home + "master_info_";
//        info_log = home;
//        google::SetLogDestination(google::INFO, info_log.c_str());

//        string warning_log = home + "master_warning_";
//        warning_log = home;
//        google::SetLogDestination(google::WARNING, warning_log.c_str());

//        string error_log = home + "master_error_";
//        error_log = home;
//        google::SetLogDestination(google::ERROR, error_log.c_str());

//        string fatal_log = home + "master_fatal_";
//        fatal_log = home;
//        google::SetLogDestination(google::FATAL, fatal_log.c_str());

//        google::SetStderrLogging(google::GLOG_INFO);
        FLAGS_alsologtostderr = 1;

        // You can specify one of the following severity levels (in increasing order of severity)
        LOG(INFO) << "info";
        LOG(WARNING) << "warning";
        LOG(ERROR) << "error";
//        LOG(FATAL) << "fatal";   // Logging a FATAL message terminates the program (after the message is logged)!

//        return ;
    }

//    google::InitGoogleLogging("outlog");
//    google::SetLogDestination(google::GLOG_INFO, "loge");
//    google::SetStderrLogging(google::GLOG_WARNING);
//    google::SetStderrLogging(google::GLOG_INFO);
//    FLAGS_colorlogtostderr=true;

//    LOG(INFO) << "check tst";
//    google::InitGoogleLogging("outlog");
    for (int i = 0; i < 10; ++i) {
        LOG_IF(INFO, i > 5) << "LOG_IF i > 5 i:" << i;
        LOG_EVERY_N(INFO, 3) << "LOG_EVERY_N 3 i:" << i << " google::COUNTER:" << google::COUNTER;
        LOG_IF_EVERY_N(INFO, (i > 5), 2) << "LOG_IF_EVERY_N i > 5 2 i:" << i;
        LOG_FIRST_N(INFO, 4) << "LOG_FIRST_N 4 i:" << i << " google::COUNTER:" << google::COUNTER;
    }
    return ;

    CheckIfBst chck;
    std::cout << "chck result=" << chck.IsValidBst_1(g_pBstTree) << std::endl;

    return ;
    CheckHuiWen checkHuiwen;
    checkHuiwen.IsHuiWen();
    return;

    tst_KthBig();
    return;

    int i1 = 4;
    int i2 = 12;
    int ret = i1 & (-i1) ;
    std::cout << "-i1=" << -i1 << std::endl;
    std::cout << "ret=" << ret << std::endl;
    return ;

    CBuildBstByPreInorder buildBst;
    buildBst.WorkEntry();
    return;

    CSortedListToBst sortl2bst;
    sortl2bst.TstEntry();
    return;

    tst_1_lc();

}
