#include "heap.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>
#define TESTS_COUNT 100000

int comparator(void *a, void* b) {
    int A = *(int*) a, B = *(int*) b;
    if (A < B)
        return -1;
    if (A > B)
        return 1;
    return 0;
}

int main() {
    srand(time(0));
    Heap* heap = initHeap(comparator);
    int arr[TESTS_COUNT], res[TESTS_COUNT];

    for (int i = 0; i < TESTS_COUNT; i++) {
        arr[i] = rand();
        insertElement(heap, &arr[i]);
    }

    int res_count = 0;

    while (heap->size > 0)
        res[res_count++] = *(int*) extractMin(heap);
    
    assert(res_count == TESTS_COUNT);

    for (int i = 1; i < TESTS_COUNT; i++)
        assert(res[i - 1] <= res[i]);
    

    destroyHeap(heap);
    return 0;
}