/**
 * Hiroshi Tokaku <tkk@hongo.wide.ad.jp>
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "arraylist.h"
#include "avl_tree.h"

#define START_TEST printf("start %s....", __func__)
#define FIN_TEST printf("end\n")

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
    printf("here\n");
    destroyAVLTree(tree);
}

int main() {
    test_createEmptyAVLTree();
    test_insertAndRemoveAVLTree();
}
