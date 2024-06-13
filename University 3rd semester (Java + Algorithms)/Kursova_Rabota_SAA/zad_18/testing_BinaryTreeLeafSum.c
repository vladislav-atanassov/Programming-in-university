#include "BinaryTreeStruct.h"

#define TEST 0
#if TEST

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// ANSI escape codes for text color
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define EXPECTED_FIRST_TEST_EVEN 10
#define EXPECTED_FIRST_TEST_ODD 10

#define EXPECTED_SECOND_TEST_EVEN 2
#define EXPECTED_SECOND_TEST_ODD 13

#define EXPECTED_THIRD_TEST_EVEN 8
#define EXPECTED_THIRD_TEST_ODD 0

void testing_BinaryTreeLeafSum(TreeNode* root, int expectedEven, int expectedOdd)
{
    int evenSum = 0;
    int oddSum = 0;

    BinaryTreeLeafSumEvenOdd(root, &evenSum, &oddSum);

    assert(evenSum == expectedEven);
    assert(oddSum == expectedOdd);

    printf("TEST PASSED!!!\n");
}

int main()
{
    /*      5
          /   \ 
         3     7
        / \   / \ 
       1   4 6   9
    */

    TreeNode* root = createNode(5);

    insertToBST(root, 3);      
    insertToBST(root, 4);      
    insertToBST(root, 1);      
    insertToBST(root, 7);      
    insertToBST(root, 6);      
    insertToBST(root, 9);

    /*    5
         / \ 
        3   7
       /     \ 
      1       8
       \       \ 
        2       13
    */

    TreeNode* root2 = createNode(5);

    insertToBST(root2, 3);       
    insertToBST(root2, 7);       
    insertToBST(root2, 2);       
    insertToBST(root2, 8);       
    insertToBST(root2, 2);       
    insertToBST(root2, 13);

    /*    7
         / \  
        5   8
       /   /
      3   2
       \ 
        6 
    */

    TreeNode* root3 = createNode(7);

    insertToBST(root3, 5);
    insertToBST(root3, 1);
    insertToBST(root3, 6);
    insertToBST(root3, 2);

    testing_BinaryTreeLeafSum(root, EXPECTED_FIRST_TEST_EVEN, EXPECTED_FIRST_TEST_ODD);
    testing_BinaryTreeLeafSum(root2, EXPECTED_SECOND_TEST_EVEN, EXPECTED_SECOND_TEST_ODD);
    testing_BinaryTreeLeafSum(root3, EXPECTED_THIRD_TEST_EVEN, EXPECTED_THIRD_TEST_ODD);

    printf(ANSI_COLOR_GREEN "\nALL TESTS PASSED!!!\n" ANSI_COLOR_RESET);

    freeTree(root);
    freeTree(root2);
    freeTree(root3);

    return 0;
}

#endif