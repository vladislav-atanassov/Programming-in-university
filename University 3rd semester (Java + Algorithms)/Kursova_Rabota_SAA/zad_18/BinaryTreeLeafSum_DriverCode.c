#include "BinaryTreeStruct.h"

#if !TEST 

#include <stdio.h>

int main() 
{
    TreeNode* root = createNode(5);

    insertToBST(root, 3);      
    insertToBST(root, 4);      
    insertToBST(root, 1);      
    insertToBST(root, 7);      
    insertToBST(root, 6);      
    insertToBST(root, 9);       

    printLeafSums(root);

    TreeNode* root2 = createNode(5);

    insertToBST(root2, 3);       
    insertToBST(root2, 7);       
    insertToBST(root2, 2);       
    insertToBST(root2, 8);       
    insertToBST(root2, 2);       
    insertToBST(root2, 13);      
    
    printLeafSums(root2);

    TreeNode* root3 = createNode(7);
    
    insertToBST(root3, 5);
    insertToBST(root3, 3);
    insertToBST(root3, 6);
    insertToBST(root3, 2);

    printLeafSums(root3);

    // Free the memory
    freeTree(root);
    freeTree(root2);
    freeTree(root3);

    return 0;
}

#endif