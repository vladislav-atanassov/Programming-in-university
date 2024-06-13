#include <stdio.h>
#include <stdlib.h>
#include "BinaryTreeStruct.h"

TreeNode* createNode(const int data) 
{
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    
    if(newNode == NULL) 
    {
        perror("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

int BinaryTreeLeafSum(TreeNode* root)
{
    // Checks if the tree is empty or only has one node
    if(root == NULL) 
        return 0;
    else if(root->left == NULL && root->right == NULL) 
        return root->data; // Leaf node

    return BinaryTreeLeafSum(root->left) + BinaryTreeLeafSum(root->right);
}

int BinaryTreeLeafSumEvenOdd(TreeNode* root, int* evenSum, int* oddSum) 
{
    // Checks if the tree is empty
    if(root == NULL) 
        return 0;

    if(root->left == NULL && root->right == NULL) 
    {
        if(root->data % 2 == 0) 
            *evenSum += root->data;
        else 
            *oddSum += root->data;

        return root->data; // Leaf node
    }

    int leftSum = BinaryTreeLeafSumEvenOdd(root->left, evenSum, oddSum);
    int rightSum = BinaryTreeLeafSumEvenOdd(root->right, evenSum, oddSum);

    return leftSum + rightSum;
}

void printLeafSums(TreeNode* root) 
{
    int evenSum = 0;
    int oddSum = 0;

    BinaryTreeLeafSumEvenOdd(root, &evenSum, &oddSum);

    printf("The sum of the even leaf nodes is: %d\n", evenSum);
    printf("The sum of the odd leaf nodes is: %d\n", oddSum);

    printf("\n---------------------------------------------------\n\n");
}

void freeTree(TreeNode* root) 
{
    if(root) 
    {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

TreeNode* insertToBST(TreeNode* root, const int data)
{
    // If the tree is empty return a new node
    if(root == NULL)
        return createNode(data);
 
    // Go down the tree
    if(data < root->data)
        root->left = insertToBST(root->left, data);
    else if(data > root->data)
        root->right = insertToBST(root->right, data);

    return root;
}
