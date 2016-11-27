/**
 * tkk <hototoh@gmial.com>
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "log.h"
#include "arraylist.h"
#include "avl_tree.h"

#define START_TEST printf("start %s....", __func__)
#define FIN_TEST printf("end\n")

#define CHILD_HEIGTH_AVL_TREE_NODE(node, member) \
    node->member != NULL ? node->member->height : -1;

static inline int leftHeighAVLTreeNode(AVLTreeNode* node) {
    return CHILD_HEIGTH_AVL_TREE_NODE(node, left);
}

static inline int rightHeighAVLTreeNode(AVLTreeNode* node) {
    return CHILD_HEIGTH_AVL_TREE_NODE(node, right);
}

static void int_printer(void* a) {
    AVLTreeNode* node = (AVLTreeNode*) a;
    if (node == NULL) debug("NULL");
    else
        debug("data:%d height:%d left: %d right:%d",
              *(int*) node->data,
              node->height,
              leftHeighAVLTreeNode(node),
              rightHeighAVLTreeNode(node)
              );
}

static int int_comparator(void* a, void* b) {
    int aa = *(int*) a;
    int bb = *(int*) b;
    return aa == bb ? 0 : 
            aa > bb ? 1 : -1;
}

void test_createEmptyAVLTree() {
    AVLTree* tree =  createAVLTree(int_comparator);
    assert(tree != NULL);
    destroyQuietlyAVLTree(tree);
}

void test_insertAndRemoveAVLTree() {
    AVLTree* tree =  createAVLTree(int_comparator);
    assert(tree != NULL);

    const int size = 6;
    int int_array[size];
    for (int i = 0; i < size; ++i) {
        int_array[i] = i + 1;
        printf("######### Adding %d\n", i+1);
        insertAVLTree(tree, &int_array[i]);
        printf("######### added %d: root:%d height: %d\n", i+1,
               *((int*)tree->root->data),
               tree->root->height);               
    }
    printf("successfully added elements\n");

    ArrayList* list = toArrayAVLTree(tree);
    printf("success to toList\n");
    assert(size == arrayListLength(list));
    for (int i = 0; i < size; ++i) {
        assert(&int_array[i] == arrayListDataAt(list, i));
    }
    destroyArrayList(list);
    
    assert(*(int*) tree->root->data == 4);
    assert(*(int*) tree->root->left->data == 2);
    assert(*(int*) tree->root->left->left->data == 1);
    assert(*(int*) tree->root->left->right->data == 3);
    assert(*(int*) tree->root->right->data == 5);
    assert(*(int*) tree->root->right->right->data == 6);
    assert(tree->root->left->right->left == NULL);
    assert(tree->root->left->right->right == NULL);
    assert(tree->root->left->left->left == NULL);
    assert(tree->root->left->left->right == NULL);
    assert(tree->root->right->left == NULL);
    assert(tree->root->right->right->left == NULL);
    assert(tree->root->right->right->right == NULL);
    assert(tree->root->parent == NULL);
    assert(tree->root->left->parent == tree->root);
    assert(tree->root->left->left->parent == tree->root->left);
    assert(tree->root->left->right->parent == tree->root->left);
    assert(tree->root->right->parent == tree->root);
    assert(tree->root->right->right->parent == tree->root->right);

    int removing = 4;
    removeAVLTree(tree, &removing);

    assert(*(int*) tree->root->data == 3);
    assert(*(int*) tree->root->left->data == 2);
    assert(*(int*) tree->root->left->left->data == 1);
    assert(*(int*) tree->root->right->data == 5);
    assert(*(int*) tree->root->right->right->data == 6);
    assert(tree->root->left->right == NULL);
    assert(tree->root->left->left->left == NULL);
    assert(tree->root->left->left->right == NULL);
    assert(tree->root->right->left == NULL);
    assert(tree->root->right->right->left == NULL);
    assert(tree->root->right->right->right == NULL);
    assert(tree->root->parent == NULL);
    assert(tree->root->left->parent == tree->root);
    assert(tree->root->left->left->parent == tree->root->left);
    assert(tree->root->right->parent == tree->root);
    assert(tree->root->right->right->parent == tree->root->right);
        
    removing = 2;
    removeAVLTree(tree, &removing);

    assert(*(int*) tree->root->data == 3);
    assert(*(int*) tree->root->left->data == 1);
    assert(*(int*) tree->root->right->data == 5);
    assert(*(int*) tree->root->right->right->data == 6);
    assert(tree->root->left->left == NULL);
    assert(tree->root->left->right == NULL);
    assert(tree->root->right->left == NULL);
    assert(tree->root->right->right->left == NULL);
    assert(tree->root->right->right->right == NULL);
    assert(tree->root->parent == NULL);
    assert(tree->root->left->parent == tree->root);
    assert(tree->root->right->parent == tree->root);
    assert(tree->root->right->right->parent == tree->root->right);        
    
    removing = 5;
    removeAVLTree(tree, &removing);

    assert(*(int*) tree->root->data == 3);
    assert(*(int*) tree->root->left->data == 1);
    assert(*(int*) tree->root->right->data == 6);
    assert(tree->root->left->left == NULL);
    assert(tree->root->left->right == NULL);
    assert(tree->root->right->left == NULL);
    assert(tree->root->right->right == NULL);
    assert(tree->root->parent == NULL);
    assert(tree->root->left->parent == tree->root);
    assert(tree->root->right->parent == tree->root);

    destroyQuietlyAVLTree(tree);
}

int main() {
    test_createEmptyAVLTree();
    test_insertAndRemoveAVLTree();
}
