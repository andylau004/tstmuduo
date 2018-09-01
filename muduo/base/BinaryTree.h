
#pragma once




struct BinaryTreeNode
{
    int                    m_nValue;
    BinaryTreeNode*        m_pLeft;
    BinaryTreeNode*        m_pRight;
};

 BinaryTreeNode* CreateBinaryTreeNode(int value);
 void ConnectTreeNodes(BinaryTreeNode* pParent, BinaryTreeNode* pLeft, BinaryTreeNode* pRight);
 void PrintTreeNode(const BinaryTreeNode* pNode);
 void PrintTree(const BinaryTreeNode* pRoot);
 void DestroyTree(BinaryTreeNode* pRoot);


//__declspec( dllexport ) BinaryTreeNode* CreateBinaryTreeNode(int value);
//__declspec( dllexport ) void ConnectTreeNodes(BinaryTreeNode* pParent, BinaryTreeNode* pLeft, BinaryTreeNode* pRight);
//__declspec( dllexport ) void PrintTreeNode(const BinaryTreeNode* pNode);
//__declspec( dllexport ) void PrintTree(const BinaryTreeNode* pRoot);
//__declspec( dllexport ) void DestroyTree(BinaryTreeNode* pRoot);


