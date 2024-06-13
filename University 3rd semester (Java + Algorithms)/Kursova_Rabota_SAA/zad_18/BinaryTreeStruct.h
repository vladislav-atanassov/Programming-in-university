#ifndef BINARYTREESTRUCT
#define BINARYTREESTRUCT

#define EXIT_FAILURE 1

/// @brief struct to implement a node of a Binary Tree
typedef struct TreeNode
{
    int data;

    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

/// @brief Allocates memory for the new node
/// @param data value will be stored in the node
/// @return pointer to the created node
TreeNode* createNode(const int data);

/// @brief Inserts new nodes in a way to create a binary search tree
/// @param root The first node in the tree
/// @param data value will be stored in the node
/// @return pointer to the created node
TreeNode* insertToBST(TreeNode* root, const int data);

/// @brief Sums the leaf's nodes data
/// @param root The first node in the tree
/// @return Sum of the leaf's nodes data
int BinaryTreeLeafSum(TreeNode* root);

/// @brief Stores the sum of both even and odd leaf nodes
/// @param root The first node in the tree
/// @param evenSum Current sum of the even elements
/// @param oddSum Current sum of the odd elements
/// @return Sum of the leaf's nodes data
int BinaryTreeLeafSumEvenOdd(TreeNode* root, int* evenSum, int* oddSum);

/// @brief Prints the sum of both the even and odd leaf nodes
/// @param root The first node in the tree
void printLeafSums(TreeNode* root);

/// @brief Free allocated memory for the tree 
/// @param root The first node in the tree
void freeTree(TreeNode* root);

#endif