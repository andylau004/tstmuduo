
#include "BinaryTree.h"


#include <cstdio>
#include "BinaryTree.h"


//typedef NULL NULL ;

BinaryTreeNode* CreateBinaryTreeNode(int value)
{
    BinaryTreeNode* pNode = new BinaryTreeNode();
    pNode->val = value;
    pNode->m_pLeft  = NULL;
    pNode->m_pRight = NULL;
    return pNode;
}

void ConnectTreeNodes(BinaryTreeNode* pParent, BinaryTreeNode* pLeft, BinaryTreeNode* pRight) {
    if (!pParent) return ;
    pParent->m_pLeft = pLeft;
    pParent->m_pRight = pRight;
}

void PrintTreeNode(const BinaryTreeNode* pNode)
{
    if(pNode != NULL)
    {
        printf("value of this node is: %d\n", pNode->val);

        if(pNode->m_pLeft != NULL)
            printf("value of its left child is: %d.\n", pNode->m_pLeft->val);
        else
            printf("left child is NULL.\n");

        if(pNode->m_pRight != NULL)
            printf("value of its right child is: %d.\n", pNode->m_pRight->val);
        else
            printf("right child is NULL.\n");
    }
    else
    {
        printf("this node is NULL.\n");
    }

    printf("\n");
}

void PrintTree(const BinaryTreeNode* pRoot)
{
    PrintTreeNode(pRoot);

    if(pRoot != NULL)
    {
        if(pRoot->m_pLeft != NULL)
            PrintTree(pRoot->m_pLeft);

        if(pRoot->m_pRight != NULL)
            PrintTree(pRoot->m_pRight);
    }
}

void DestroyTree(BinaryTreeNode* pRoot)
{
    if(pRoot != NULL)
    {
        BinaryTreeNode* pLeft = pRoot->m_pLeft;
        BinaryTreeNode* pRight = pRoot->m_pRight;

        delete pRoot;
        pRoot = NULL;

        DestroyTree(pLeft);
        DestroyTree(pRight);
    }
}



