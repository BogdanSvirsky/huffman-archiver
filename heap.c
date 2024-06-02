#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned int size, capacity;
    void** elements;
    int (*elementsComparator) (void* a, void* b);
} Heap;

void swap(Heap* heap, unsigned int ind1, unsigned int ind2) {
    void* tmp = heap->elements[ind1];
    heap->elements[ind1] = heap->elements[ind2];
    heap->elements[ind2] = tmp;
}

Heap* initHeap(int (*elementsComparator) (void* a, void* b)) {
    Heap* heap = (Heap*) calloc(1, sizeof(Heap));
    heap->capacity = 1;
    heap->elements = (void**) calloc(1, sizeof(void*));
    heap->elementsComparator = elementsComparator;

    return heap;
}

void siftUp(Heap* heap, unsigned int ind) {
    if (ind >= heap->size)
        return;
    int parent;
    while (ind > 0) {
        parent = (ind - 1) / 2;
        if (heap->elementsComparator(heap->elements[parent], heap->elements[ind]) > 0) {
            swap(heap, ind, parent);
            ind = parent;
        } else
            break;
    }
}

void siftDown(Heap* heap, unsigned int ind) {
    int minChild;
    while (2 * ind + 1 < heap->size) {
        if (2 * ind + 2 < heap->size &&
            heap->elementsComparator(heap->elements[2 * ind + 1], heap->elements[2 * ind + 2]) > 0)
            minChild = 2 * ind + 2;
        else
            minChild = 2 * ind + 1;

        if (heap->elementsComparator(heap->elements[ind], heap->elements[minChild]) > 0) {
            swap(heap, ind, minChild);
            ind = minChild;
        } else
            break;
    }
}

void insertElement(Heap* heap, void* element) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->elements = (void**) realloc(heap->elements, sizeof(void*) * heap->capacity);
    }

    heap->elements[heap->size++] = element;
    siftUp(heap, heap->size - 1);
}

void* extractMin(Heap *heap) {
    if (heap->size == 0)
        return NULL;
    void* result = heap->elements[0];
    heap->size--;
    swap(heap, 0, heap->size);
    siftDown(heap, 0);
    return result;
}

void destroyHeap(Heap* heap) {
    free(heap->elements);
    free(heap);
}