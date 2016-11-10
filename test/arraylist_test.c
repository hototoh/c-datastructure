/**
 * @author tkk <hototoh@gmail.com>
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "arraylist.h"

#define START_TEST printf("start %s....", __func__)
#define FIN_TEST printf("end\n")


void test_createArrayList() {
    START_TEST;
    ArrayList* array = createArrayList();
    assert(array != NULL);
    assert(arrayListLength(array) == 0);
    destroyArrayList(array);
    FIN_TEST;
}

void test_pushAndPopArrayList() {
    START_TEST;
    const int size = 8;
    ArrayList* array = createArrayList();
    assert(array != NULL);
    int v[size];
    for (int i = 0; i < size; ++i) {
        v[i] = i;
        assert(arrayListPush(array, &v[i]));
        assert(arrayListLength(array) == i+1);
    }
    assert(arrayListLength(array) == size);

    void** data = arrayListData(array);
    for (int i = 0; i < size; ++i) {
        int* actual = (int*)data[i];
        assert(*actual == i);
    }

    for (int i = 0; i < size; ++i) {
        int* actual = (int*) arrayListPop(array);
        assert(*actual == size - i - 1);
        assert(arrayListLength(array) == size - i - 1);
    }
    
    assert(arrayListPop(array) == NULL);
    destroyArrayList(array);
    FIN_TEST;
}

void test_resizeArrayList() {
    START_TEST;
    const int size = 32;
    ArrayList* array = createArrayList();
    assert(array != NULL);
    arrayListResize(array, 16);
    assert(array->_allocated == 16);

    int v[size];
    for (int i = 0; i < size; ++i) {
        v[i] = i;
        assert(arrayListPush(array, &v[i]));
        assert(arrayListLength(array) == i+1);
    }
    assert(array->_allocated == 32);
    assert(array->length == 32);
    
    destroyArrayList(array);
    FIN_TEST;
}

void test_insertAndRemoveArrayList() {
    START_TEST;
    const int size = 32;
    ArrayList* array = createArrayList();
    assert(array != NULL);

    int v[size];
    for (int i = 0; i < size; ++i) {
        v[i] = i+1;
        assert(arrayListInsert(array, 0, &v[i]));
    }
    assert(array->_allocated == 32);
    assert(array->length == 32);
    
    for (int i = 0; i < size; ++i) {
        assert(arrayListRemove(array, 0));
        if (i != size - 1)
            assert(*(int*)array->data[0] == size-i-1);
        assert(arrayListLength(array) == size - i - 1);
    }
    assert(arrayListRemove(array, 0) == false);
    destroyArrayList(array);
    FIN_TEST;
}

int main() {
    test_createArrayList();
    test_pushAndPopArrayList();
    test_resizeArrayList();
    test_insertAndRemoveArrayList();
}
