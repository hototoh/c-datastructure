#ifndef DLLIST_H
#define DLLIST_H

#include <stddef.h>

typedef struct DLListHead {
    struct DLListHead* prev;
    struct DLListHead* next;
} DLListHead;

typedef struct DLListContainer {
    DLListHead head;
    void* data;
} DLListContainer;

typedef struct DLList {
    int size;
    DLListHead head;    
} DLList;

#define typeof __typeof__

#define container_of(ptr, type, member) ({                              \
            const typeof( ((type*) 0)->member)* __mptr = (ptr);         \
            (type*)( (char*) __mptr - offsetof(type, member));})

#define list_entry(ptr, type, member)           \
    container_of(ptr, type, member)


DLList* createEmptyDLList();

DLList* createDLList(void** entries, int num);

void destroyDLList(DLList* list);

static inline int sizeDLList(DLList* list) {
    return list->size;
}

void* getDataDLList(DLList* list, int index);

bool insertDLList(DLList* list, void *entry, int index);

/* this function may cause unpleasant situation*/ 
bool insertAfterDLList(DLList* list, DLListHead* prev, void* entry);

/* this function may cause unpleasant situation*/ 
bool insertBeforeDLList(DLList* list, DLListHead* next, void* entry);

void* removeDLList(DLList* list, int index);

void removeAllEntriesDLList(DLList* list);

static inline bool appendDLList(DLList* list, void* entry) {
    return insertBeforeDLList(list, &list->head, entry);
}

static inline bool prependDLList(DLList* list, void* entry) {
    return insertAfterDLList(list, &list->head, entry);
}

#endif
