#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct ArrayList {
    int _allocated;
    int length;    
    void** data;
} ArrayList;

ArrayList* createArrayList();

bool arrayListResize(ArrayList* array, int n);

static inline int arrayListLength(ArrayList* array) {
    return array->length;
}

static inline void** arrayListData(ArrayList* array) {
    return array->data;
}

bool arrayListInsert(ArrayList* array, int index, void* data);

bool arrayListRemove(ArrayList* array, int index);

static inline bool arrayListPush(ArrayList* array, void* data) {
    return arrayListInsert(array, array->length, data);
}

void* arrayListPop(ArrayList* array);

void destroyArrayList(ArrayList* array);

#endif
