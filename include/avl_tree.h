#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <stdbool.h>

/**
 * @return: 
 *  0 : when both are same
 *  1 : when first arg is bigger
 *  -1: otherwise
 */
typedef int (*comparatorAVLTree)(void*, void*);

typedef struct AVLTreeNode {
    int height;
    void* data;
    struct AVLTreeNode* parent;
    struct AVLTreeNode* left;
    struct AVLTreeNode* right;
} AVLTreeNode;

typedef struct AVLTree {    
    AVLTreeNode* root;
    comparatorAVLTree comparator;
} AVLTree;

AVLTree* createAVLTree(comparatorAVLTree comparator);

ArrayList* destroyAVLTree(AVLTree* tree);

void destroyQuietlyAVLTree(AVLTree* tree);

bool insertAVLTree(AVLTree* tree, void* entry);

void* removeAVLTree(AVLTree* tree, void* entry);

bool containsAVLTree(AVLTree* tree, void* entry);
    
ArrayList* toArrayAVLTree(AVLTree* tree);

#endif
