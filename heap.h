typedef struct {
    unsigned int size, capacity;
    void** elements;
    int (*elementsComparator) (void* a, void* b);
} Heap;


Heap* initHeap(int (*elementsComparator) (void* a, void* b));
void insertElement(Heap* heap, void* element);
void* extractMin(Heap *heap);
void destroyHeap(Heap* heap);