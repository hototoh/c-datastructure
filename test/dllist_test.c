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

#include "dllist.h"

#define START_TEST printf("start %s....", __func__)
#define FIN_TEST printf("end\n")

void test_createEmptyDLList() {
    DLList* list =  createEmptyDLList();
    DLListHead* list_head = &list->head;
    assert(list != NULL);
    assert(list_head == list_head->next);
    assert(list_head == list_head->prev);
    destroyDLList(list);
}

void test_createNonEmptyDLList() {
    const int size = 8;
    int vals[size];
    int* val_ptrs[size];
    for (int i = 0; i < size; ++i) {
        vals[i] = i;
        val_ptrs[i] = &vals[i];
    }
    DLList* list =  createDLList((void**) val_ptrs, 8);
    DLListHead* list_head = &list->head;

    assert(list_head != NULL);
    assert(list_head != list_head->next);
    assert(list_head != list_head->prev);
    assert(sizeDLList(list) == size);
    assert(*(int*)getDataDLList(list, 0) == 0);
    assert(*(int*)getDataDLList(list, 1) == 1);
    assert(*(int*)getDataDLList(list, 2) == 2);
    assert(*(int*)getDataDLList(list, 3) == 3);
    assert(*(int*)getDataDLList(list, 4) == 4);
    assert(*(int*)getDataDLList(list, 5) == 5);
    assert(*(int*)getDataDLList(list, 6) == 6);
    assert(*(int*)getDataDLList(list, 7) == 7);
    destroyDLList(list);
}

void test_appendAndPrependDLList() {
    const int size = 8;
    int vals[size];
    int* val_ptrs[size];
    for (int i = 0; i < size; ++i) {
        vals[i] = i;
        val_ptrs[i] = &vals[i];
    }
    {
        DLList* list =  createEmptyDLList();
        DLListHead* list_head = &list->head;
        for(int i = 0; i < size; ++i) {
            appendDLList(list, val_ptrs[i]);
        }
        assert(list_head != NULL);
        assert(list_head != list_head->next);
        assert(list_head != list_head->prev);
        assert(sizeDLList(list) == size);
        assert(*(int*)getDataDLList(list, 0) == 0);
        assert(*(int*)getDataDLList(list, 1) == 1);
        assert(*(int*)getDataDLList(list, 2) == 2);
        assert(*(int*)getDataDLList(list, 3) == 3);
        assert(*(int*)getDataDLList(list, 4) == 4);
        assert(*(int*)getDataDLList(list, 5) == 5);
        assert(*(int*)getDataDLList(list, 6) == 6);
        assert(*(int*)getDataDLList(list, 7) == 7);
        destroyDLList(list);
    }
    {
        DLList* list =  createEmptyDLList();
        DLListHead* list_head = &list->head;
        for(int i = 0; i < size; ++i) {
            prependDLList(list, val_ptrs[i]);
        }
        assert(list_head != NULL);
        assert(list_head != list_head->next);
        assert(list_head != list_head->prev);
        assert(sizeDLList(list) == size);
        assert(*(int*)getDataDLList(list, 0) == 7);
        assert(*(int*)getDataDLList(list, 1) == 6);
        assert(*(int*)getDataDLList(list, 2) == 5);
        assert(*(int*)getDataDLList(list, 3) == 4);
        assert(*(int*)getDataDLList(list, 4) == 3);
        assert(*(int*)getDataDLList(list, 5) == 2);
        assert(*(int*)getDataDLList(list, 6) == 1);
        assert(*(int*)getDataDLList(list, 7) == 0);
        destroyDLList(list);    
    }
}

void test_insertAndRemoveList() {
    const int size = 8;
    int vals[size];
    int* val_ptrs[size];
    for (int i = 0; i < size; ++i) {
        vals[i] = i;
        val_ptrs[i] = &vals[i];
    }
    {
        DLList* list =  createEmptyDLList();
        DLListHead* list_head = &list->head;
        assert(insertDLList(list, val_ptrs[0], 0));
        assert(insertDLList(list, val_ptrs[1], 1));
        assert(insertDLList(list, val_ptrs[2], 1));
        assert(insertDLList(list, val_ptrs[3], 1));
        assert(insertDLList(list, val_ptrs[4], 4));
        assert(insertDLList(list, val_ptrs[5], 4));
        assert(insertDLList(list, val_ptrs[6], 6));
        assert(insertDLList(list, val_ptrs[7], 7));
        assert(!insertDLList(list, val_ptrs[7], 9));
        assert(*(int*)removeDLList(list, 1) == 3);
        assert(*(int*)removeDLList(list, 1) == 2);
        assert(*(int*)removeDLList(list, 1) == 1);
        assert(*(int*)removeDLList(list, 0) == 0);
        assert(removeDLList(list, 4) == NULL);
        assert(*(int*)removeDLList(list, 3) == 7);
        assert(*(int*)removeDLList(list, 1) == 4);
        assert(*(int*)removeDLList(list, 1) == 6);
        assert(*(int*)removeDLList(list, 0) == 5);
        assert(list_head == list_head->prev);
        assert(list_head == list_head->next);
        destroyDLList(list);
    }
}

int main() {
    test_createEmptyDLList();
    test_createNonEmptyDLList();
    test_appendAndPrependDLList();
    test_insertAndRemoveList();
}
