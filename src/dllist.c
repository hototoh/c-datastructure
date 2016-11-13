/**
 * Hiroshi Tokaku <tkk@hongo.wide.ad.jp>
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "log.h"
#include "dllist.h"

static DLListContainer* createDLListContainer(void* entry);
static void destroyDLListContainer(DLListContainer* container);
static inline void insertAfterDLListHead(DLListHead* prev,
                                         DLListContainer* container);
static inline void insertBeforeDLListHead(DLListHead* next,
                                         DLListContainer* container);


static DLListContainer* createDLListContainer(void* entry) {
    size_t size = sizeof(DLListContainer);
    DLListContainer* container = (DLListContainer*) malloc(size);
    if (container == NULL) {
        log_perror("Failed to allocate memory");
        goto fail;
    }
    
    container->head.prev = container->head.next = NULL;
    container->data = entry;
fail:
    return container;
}

static void destroyDLListContainer(DLListContainer* container) {
    free(container);
}

DLList* createEmptyDLList() {
    DLList* list = (DLList*) malloc(sizeof(DLList));
    if (list == NULL) {
        log_perror("Failed to allocate memory");
        goto fail;
    }

    list->size = 0;
    list->head.prev = list->head.next = &list->head;    
fail:
    return list;
}

DLList* createDLList(void** entries, int num) {
    DLList* list = (DLList*) malloc(sizeof(DLList));
    if (list == NULL) {
        log_perror("Failed to allocate memory");
        goto fail;
    }
   
    for (int i = 0; i < num; ++i)
        if (!appendDLList(list, entries[i])) goto destroy;

    return list;
destroy:
    removeAllEntriesDLList(list);
fail:
    return NULL;
}

void destroyDLList(DLList* list) {
    removeAllEntriesDLList(list);
    free(list);
}

void* getDataDLList(DLList* list, int index) {
    if (index < 0) goto outofrange;
    
    DLListHead* list_head = &list->head;
    DLListHead* cur = list_head->next;
    for (int i = 0; i < index; ++i, cur = cur->next)
        if (cur == list_head) goto outofrange;

    DLListContainer* container = list_entry(cur, DLListContainer, head);    
    return container->data;

outofrange:
    log_error("Out of range.");
    return NULL;
}

void removeAllEntriesDLList(DLList* list) {
    DLListHead* list_head = &list->head;
    while(list_head->next != list_head) {        
        removeDLList(list, 0);
    }
}

void* removeDLList(DLList* list, int index) {    
    DLListHead* list_head = &list->head;
    DLListHead* prev = list_head;
    DLListHead* cur = list_head->next;
    if (index < 0 || prev == cur) goto outofrange;

    for (int i = 0; i < index; ++i) {
        prev = cur;
        cur = cur->next;
        if (cur == list_head) goto outofrange;
    }

    DLListHead* new_next = cur->next;
    prev->next = new_next;
    new_next->prev = prev;
    cur->prev = cur->next = NULL;
    list->size--;
    
    DLListContainer* container = list_entry(cur, DLListContainer, head);
    void* data = container->data;
    destroyDLListContainer(container);
    return data;

outofrange:
    log_error("Out of range.");
    return NULL;
}

bool insertDLList(DLList* list, void *entry, int index) {    
    DLListHead* list_head = &list->head;
    DLListHead* prev = list_head;
    DLListHead* cur = list_head->next;
    for (int i = 0; i < index; ++i, prev = cur, cur = cur->next) {
        if (cur == list_head) goto outofrange;                  
    }

    DLListContainer* container = createDLListContainer(entry);
    if (container == NULL) goto fail;

    list->size++;
    insertAfterDLListHead(prev, container);
    return true;

outofrange:
    log_error("Out of range");
fail:
    return false;
}


bool insertAfterDLList(DLList* list, DLListHead* prev, void* entry) {
    DLListContainer* container = createDLListContainer(entry);
    if (container == NULL) goto fail;

    list->size++;
    insertAfterDLListHead(prev, container);
    return true;
fail:
    return false;
}

bool insertBeforeDLList(DLList* list, DLListHead* next, void* entry) {
    DLListContainer* container = createDLListContainer(entry);
    if (container == NULL) goto fail;

    list->size++;
    insertBeforeDLListHead(next, container);
    return true;
fail:
    return false;

}

static inline void insertAfterDLListHead(DLListHead* prev,
                                         DLListContainer* container) {
    assert(prev != NULL);
    assert(container != NULL);

    DLListHead* next = prev->next;
    DLListHead* new_next = &container->head;
    
    new_next->next = next;
    new_next->prev = prev;
    prev->next = new_next;
    next->prev = new_next;
}

static inline void insertBeforeDLListHead(DLListHead* next,
                                          DLListContainer* container) {
    assert(next != NULL);
    assert(container != NULL);

    DLListHead* prev = next->prev;
    DLListHead* new_prev = &container->head;

    new_prev->prev = prev;
    new_prev->next = next;
    prev->next = new_prev;
    next->prev = new_prev;
}
