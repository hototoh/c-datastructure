/**
 * @author tkk <hototoh@gmail.com>
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "arraylist.h"

#define INITIAL_LEN 8

#define SIZEOF_ARRAY_LIST(x) (sizeof(void*) * x)

ArrayList* createArrayList() {
    ArrayList* array = (ArrayList*) malloc(sizeof(ArrayList));
    if (array == NULL) goto fail;
    
    void** data = (void**) calloc(INITIAL_LEN, sizeof(void*));
    if (data == NULL) goto fail1;

    array->_allocated = INITIAL_LEN;
    array->length = 0;
    array->data = data;
    return array;
fail1:
    free(array);
fail:
    return NULL;;
}

bool arrayListResize(ArrayList* array, int n) {
    if (n <= array->_allocated) return false;
    
    void** data = (void**) realloc(array->data, SIZEOF_ARRAY_LIST(n));
    if (data == NULL) goto fail;
    
    array->_allocated = n;
    array->data = data;
    return true;
fail:
    return false;
}

void* arrayListPop(ArrayList* array) {
    if (array->length == 0) return NULL;

    void* data = array->data[--array->length];
    array->data[array->length] = NULL;
    return data;
}

bool arrayListInsert(ArrayList* array, int index, void* data) {
    if (index < 0 || array->length < index) return false;    
    if (array->length == array->_allocated) {
        bool resized = arrayListResize(array, array->_allocated << 1);
        if (!resized) return false;
    }
    
    for (int i = array->length++; i > index; --i) {
        array->data[i] = array->data[i-1];
    }
    array->data[index] = data;
    return true;
}

bool arrayListRemove(ArrayList* array, int index) {
    if (index < 0 || array->length <= index) return false;    
    
    int len = --array->length;
    for (int i = index; i < len; ++i) {
        array->data[i] = array->data[i+1];
    }
    return true;   
}

void destroyArrayList(ArrayList* array) {
    free(array->data);
    free(array);
}

