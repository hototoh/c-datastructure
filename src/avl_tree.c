/**
 * @author tkk <hototoh@gmail.com>
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
#include "dllist.h"
#include "avl_tree.h"

#define CHILD_HEIGTH_AVL_TREE_NODE(node, member) \
    node->member != NULL ? node->member->height : -1;

static inline int max(int a, int b) {
    return a > b ? a : b;
}

static inline int leftHeighAVLTreeNode(AVLTreeNode* node) {
    return CHILD_HEIGTH_AVL_TREE_NODE(node, left);
}

static inline int rightHeighAVLTreeNode(AVLTreeNode* node) {
    return CHILD_HEIGTH_AVL_TREE_NODE(node, right);
}

static inline int diffHeightAVLTreeNode(AVLTreeNode* node) {
    return leftHeighAVLTreeNode(node) - rightHeighAVLTreeNode(node);
}

static inline int maxChildHeightAVLTreeNode(AVLTreeNode* node) {
    return max(leftHeighAVLTreeNode(node),
               rightHeighAVLTreeNode(node));
}

AVLTreeNode* createAVLTreeNode(void* data) {
    AVLTreeNode* node = (AVLTreeNode*) malloc(sizeof(AVLTreeNode));
    if (node == NULL) goto fail;

    node->parent = NULL;
    node->height = 0;
    node->left = node->right = NULL;
    node->data = data;
fail:
    return node;
}

void* destroyAVLTreeNode(AVLTreeNode* node) {
    void* data = node->data;
    free(node);
    return data;
}

AVLTree* createAVLTree(comparatorAVLTree comparator) {
    AVLTree* tree = (AVLTree*) malloc(sizeof(AVLTree));
    if (tree == NULL) goto fail;

    tree->root = NULL;
    tree->comparator = comparator;
fail:
    return tree;
}

ArrayList* destroyAllNodesAVLTree(AVLTree* tree, bool quiet) {
    AVLTreeNode* cur = tree->root;
    if (cur == NULL) return NULL;

    ArrayList* arr = NULL;
    if (!quiet) arr = createArrayList();
    
    DLList* stack = createEmptyDLList();
    while(cur != NULL || sizeDLList(stack) > 0) {
        for(; cur != NULL; cur = cur->left) prependDLList(stack, cur);

        AVLTreeNode* prev = popDLList(stack);
        cur = prev->right;

        void* data = destroyAVLTreeNode(prev);
        if (!quiet) arrayListPush(arr, data);        
    }
    free(tree);    
    return arr;
}

ArrayList* destroyAVLTree(AVLTree* tree) {
    return destroyAllNodesAVLTree(tree, false);
}

void destroyQuietlyAVLTree(AVLTree* tree) {
    destroyAllNodesAVLTree(tree, true);    
}

#define UPDATE_NODE_HEIGHT(node) ({                             \
            node->height = maxChildHeightAVLTreeNode(node) + 1; \
        })

#define ROTATE_AVL_TREE(sub_root, dir1, dir2) ({                        \
            AVLTreeNode* new_sub_root = sub_root->dir1;                 \
            sub_root->dir1 = new_sub_root->dir2;                        \
            new_sub_root->dir2 = sub_root;                              \
            new_sub_root->parent = sub_root->parent;                    \
            sub_root->parent = new_sub_root;                            \
            UPDATE_NODE_HEIGHT(sub_root);                               \
            UPDATE_NODE_HEIGHT(new_sub_root);                           \
            new_sub_root; })

#define UPDATE_PARENT_NODE(tree, node, dir1, dir2) ({                   \
            AVLTreeNode* parent = node->parent;                         \
            if (parent == NULL) { tree->root = node; }                  \
            else {                                                      \
                if (parent->dir1 == node->dir2)                         \
                { parent->dir1 = node; }                                \
                else if (parent->dir2 == node->dir2)                    \
                { parent->dir2 = node; }                                \
                else assert(false);                                     \
                parent->height = UPDATE_NODE_HEIGHT(parent);            \
            }                                                           \
        })

static AVLTreeNode* leftRotateAVLTree(AVLTree* tree, AVLTreeNode* root) {
    AVLTreeNode* new_root = ROTATE_AVL_TREE(root, right, left);
    UPDATE_PARENT_NODE(tree, new_root, right, left);
    return new_root;
}

static AVLTreeNode* rightRotateAVLTree(AVLTree* tree, AVLTreeNode* root) {
    AVLTreeNode* new_root = ROTATE_AVL_TREE(root, left, right);
    UPDATE_PARENT_NODE(tree, new_root, left, right);
    return new_root;
}

static void* balanceSubAVLTree(AVLTree* tree, AVLTreeNode* root) {
    UPDATE_NODE_HEIGHT(root);
    int gap = diffHeightAVLTreeNode(root);
    debug("cur:%d height:%d l:%d r:%d",  *(int*) root->data, root->height,
          leftHeighAVLTreeNode(root), rightHeighAVLTreeNode(root));
    if (abs(gap) <= 1) return root;
    if (abs(gap) != 2) assert(false);  // invalid tree structure
        
    bool isLeftNode = gap == 2;
    AVLTreeNode* sub_root = isLeftNode ? root->left: root->right;
    bool isLeftSubNode = diffHeightAVLTreeNode(sub_root) >= 0;

    AVLTreeNode* new_root = NULL;
    if (isLeftNode && isLeftSubNode) {
        new_root = rightRotateAVLTree(tree, root);
    } else if (!isLeftNode && !isLeftSubNode) {
        new_root = leftRotateAVLTree(tree, root);
    } else if (isLeftNode && !isLeftSubNode) {        
        leftRotateAVLTree(tree, sub_root);
        new_root = rightRotateAVLTree(tree, root);
    } else {
        rightRotateAVLTree(tree, sub_root);
        new_root = leftRotateAVLTree(tree, root);
    }
    debug("new_cur:%d height:%d l:%d r:%d",  *(int*) new_root->data, new_root->height,
           leftHeighAVLTreeNode(new_root), rightHeighAVLTreeNode(new_root));    
    return new_root;
}

static void balanceAVLTree(AVLTree* tree, AVLTreeNode* node) {
    AVLTreeNode* cur = node;

    while(cur != NULL) {
        cur = balanceSubAVLTree(tree, cur);
        cur = cur->parent;
    }
}

bool insertAVLTree(AVLTree* tree, void* entry) {
    AVLTreeNode* node = createAVLTreeNode(entry);
    if (node == NULL) return false;

    if (tree->root == NULL) {
        tree->root = node;
    } else {
        AVLTreeNode* cur = tree->root, *prev = NULL;
        comparatorAVLTree comparator = tree->comparator;
        int left_or_right = 0;
        while(cur != NULL) {
            left_or_right = comparator(cur->data, entry);
            prev = cur;
            if (left_or_right == 0) { // already exist.
                goto already_exist;
            }
            cur = left_or_right > 0 ? cur->left: cur->right;
        }
        
        if (left_or_right > 0) {
            prev->left = node;
        } else {
            prev->right = node;
        }
        node->parent = prev;

        prev->height = maxChildHeightAVLTreeNode(prev) + 1;        
    }

    balanceAVLTree(tree, node);
    return true;
already_exist:
    destroyAVLTreeNode(node);
    return false;
}

/**
 * This function returns an edges node whose data moved to the place where
 * removed data exists.
 */
void removeNodeAVLTree(AVLTree* tree, AVLTreeNode* node) {
    int gap = diffHeightAVLTreeNode(node);
    bool left = gap >= 0;
    AVLTreeNode* prev = node;
    AVLTreeNode* cur = left ? node->left : node->right;
    while(cur != NULL) {
        prev = cur;
        if (left) cur = cur->right;
        else cur = cur->left;        
    }
    
    
    node->data = prev->data;
    if (left) cur = prev->left;
    else cur = prev->right;
    prev->data = cur != NULL ? cur->data : NULL;
    
    if (node == NULL && cur == NULL) tree->root = NULL;

    if (cur != NULL) destroyAVLTreeNode(cur);
    else destroyAVLTreeNode(prev);            
}

void* removeAVLTree(AVLTree* tree, void* entry) {
    void* ret = NULL;;
    if (tree->root == NULL) return NULL;

    AVLTreeNode* cur = tree->root, **prev = &tree->root;
    comparatorAVLTree comparator = tree->comparator;
    int left_or_right = 0;
    while(cur != NULL) {
        left_or_right = comparator(cur->data, entry);
        if (left_or_right == 0) goto remove_node;

        prev = left_or_right > 0 ? &cur->left : &cur->right;
        cur = *prev;
    }

    return NULL;
remove_node:
    
    ret = cur->data;
    removeNodeAVLTree(tree, cur);
    balanceAVLTree(tree, cur);
    
    return ret;
}

bool containsAVLTree(AVLTree* tree, void* entry) {
    AVLTreeNode* cur = tree->root;
    comparatorAVLTree comparator = tree->comparator;

    while(cur != NULL) {
        int left_or_right = comparator(cur->data, entry);
        if (left_or_right == 0) {
            return true;
        }
        cur = left_or_right > 0 ? cur->left: cur->right;
    }

    return false;
}

ArrayList* toArrayAVLTree(AVLTree* tree) {
    AVLTreeNode* cur = tree->root;
    if (cur == NULL) return NULL;
    
    ArrayList* arr = createArrayList();    
    DLList* stack = createEmptyDLList();
    while(cur != NULL || sizeDLList(stack) > 0) {
        for(; cur != NULL; cur = cur->left) prependDLList(stack, cur);

        cur = popDLList(stack);
        arrayListPush(arr, cur->data);        
        cur = cur->right;
    }
    return arr;
}
