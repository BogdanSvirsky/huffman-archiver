#include "heap.h"
#include <stdio.h>

int comparator(void *a, void* b) {
    int A = *(int*) a, B = *(int*) b;
    if (A < B)
        return -1;
    if (A > B)
        return 1;
    return 0;
}

int main() {
    Heap* heap = initHeap(comparator);
    int arr[5] = {10, 2, 4, 3, 1};

    for (int i = 0; i < 5; i++)
        insertElement(heap, &arr[i]);
    
    for (int i = 0; i < 5; i++)
        printf("%d ", *(int*) extractMin(heap));

    destroyHeap(heap);
    return 0;
}