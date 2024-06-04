#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_BYTE_VALUE 256
typedef unsigned char Byte;

typedef struct Node_t 
{
    struct Node_t *left, *right;
    unsigned long long frequency;
    Byte byte_value;
} Node;

int nodesComparator(void* a, void* b) {
    Node *node_a = (Node*) a, *node_b = (Node*) b;
    if (node_a->frequency == node_b->frequency)
        return 0;
    else if (node_a->frequency > node_b->frequency)
        return 1;
    else
        return -1;
}

static Node nodes[2 * MAX_BYTE_VALUE - 1], *root, *current_node = NULL;
static unsigned short nodes_count = 0, output_tree_size = 0, current_bit_number = 0, bytes_codes_sizes[MAX_BYTE_VALUE] = {0};
static Byte* output_tree = NULL, *bytes_codes[MAX_BYTE_VALUE] = {NULL};
static unsigned char current_bit = 1;

void initStaticVariables() {
    for (int i = 0; i < MAX_BYTE_VALUE; i++) {
        if (bytes_codes[i]) {
            free(bytes_codes[i]);
        }
        bytes_codes_sizes[i] = 0;
        bytes_codes[i] = NULL;
    }
    current_node = NULL;
    current_bit_number = 0;
    current_bit = 1;
}


Node* createNode(Byte byte_value, unsigned long long frequency, Node* left, Node* right) {
    nodes[nodes_count].byte_value = byte_value;
    nodes[nodes_count].frequency = frequency;
    nodes[nodes_count].left = left;
    nodes[nodes_count].right = right;
    return &nodes[nodes_count++];
}

unsigned char isLeaf(Node* node) {
    if (node)
        return (node->left == NULL) && (node->right == NULL);
    else
        return 0;
}

void addBitToOutputTree(unsigned char bit) {
    if (output_tree_size == 0)
        current_bit = 1;
    
    if (output_tree_size % 8 == 0) {
        output_tree = (Byte*) realloc(output_tree, (output_tree_size / 8 + 1) * sizeof(Byte));
        output_tree[output_tree_size / 8] = 0;
    }

    if (bit)
        output_tree[output_tree_size / 8] |= current_bit;

    current_bit <<= 1;
    output_tree_size++;

    if (current_bit == 0) {
        current_bit = 1;
    }
}

void addBitToCode(Byte byte_value, unsigned char bit) {
    if (bytes_codes_sizes[byte_value] % 8 == 0) {
        bytes_codes[byte_value] = (Byte*) realloc(bytes_codes[byte_value], ((bytes_codes_sizes[byte_value] / 8) + 1) * sizeof(Byte));
        bytes_codes[byte_value][bytes_codes_sizes[byte_value] / 8] = 0;
    }
    
    if (bit)
        bytes_codes[byte_value][bytes_codes_sizes[byte_value] / 8] |= (1 << (bytes_codes_sizes[byte_value] % 8));

    bytes_codes_sizes[byte_value]++;
}

void traverseHuffmanTree(Node* node, unsigned char flag) {
    if (node == NULL) {
        fprintf(stderr, "node is NULL\n");
        return;
    }

    if (isLeaf(node)) {
        addBitToCode(node->byte_value, flag);
    } else {
        traverseHuffmanTree(node->left, flag);
        traverseHuffmanTree(node->right, flag);
    }
}

void createOutputTree(Node* node, unsigned long long code, unsigned short depth) {
    if (node == NULL) {
        fprintf(stderr, "node is NULL\n");
        return;
    }


    if (node->left || node->right) {
        addBitToOutputTree(0);
        if (node->left) {
            traverseHuffmanTree(node->left, 1);
            createOutputTree(node->left, code + (1ULL << depth), depth + 1);
        }
        if (node->right) {
            traverseHuffmanTree(node->right, 0);
            createOutputTree(node->right, code, depth + 1);
        }
    } else {
        addBitToOutputTree(1);
        Byte byte_value = node->byte_value;
        
        for (int i = 0; i < 8; i++) {
            addBitToOutputTree(byte_value & 1);
            byte_value >>= 1;
        }
    }
}

void destroyHuffmanCodes();
Node* traverseOutputTree();

void initHuffmanCodes(unsigned long long bytes_frequency[MAX_BYTE_VALUE]) {
    initStaticVariables();
    Heap* heap = initHeap(nodesComparator);

    for (int i = 0; i < MAX_BYTE_VALUE; i++) {
        if (bytes_frequency[i])
            insertElement(heap, (void*) createNode((Byte) i, bytes_frequency[i], NULL, NULL));
    }

    if (nodes_count == 1)
        root = createNode(0, 0, &nodes[0], createNode(~nodes[0].byte_value, 0, NULL, NULL));
    else {
        Node *left, *right;

        while (heap->size > 1) {
            left = extractMin(heap);
            right = extractMin(heap);

            insertElement(heap, (void*) (root = createNode(0, left->frequency + right->frequency, left, right)));
        }
    }

    destroyHeap(heap);
    createOutputTree(root, 0, 0);
}

Byte* getOutputTree() {
    return output_tree;
}

unsigned short getOutputTreeSize() {
    return output_tree_size;
}

unsigned char getBitFromOutputTree() {
    if (output_tree_size <= current_bit_number) {
        current_bit_number = 0;
        return 2;
    }

    current_bit_number++;
    
    return (output_tree[(current_bit_number - 1) / 8] & (1 << ((current_bit_number - 1) % 8))) > 0;
}

Node* traverseOutputTree() {
    unsigned char bit = getBitFromOutputTree();
    
    if (bit == 2)
        return NULL;

    if (bit == 1) {
        Byte byte_value = 0, current_bit = 1;
        for (Byte current_bit = 1; current_bit != 0; current_bit <<= 1)
            byte_value |= (getBitFromOutputTree() == 1? current_bit: 0);
        return createNode(byte_value, 0, NULL, NULL);
    } else {
        Node *left, *right;
        left = traverseOutputTree();
        right = traverseOutputTree();
        return createNode(0, 0, left, right);
    }
}

void setOutputTree(unsigned short external_output_tree_size, Byte* external_output_tree) {
    destroyHuffmanCodes();
    output_tree = external_output_tree;
    output_tree_size = external_output_tree_size;
    root = traverseOutputTree();

    if (root == NULL)
        fprintf(stderr, "Bad tree scan\n");

    current_bit_number = 0;
    output_tree = NULL;
    output_tree_size = 0;
}

unsigned char search(Node* node, Byte byte_value) {
    if (node == NULL)
        return 0;

    if (isLeaf(node) && node->byte_value == byte_value)
        return 1;
    else
        return (node->left || node->right)?
            search(node->left, byte_value) || search(node->right, byte_value):
            0;
}

unsigned char getBitInCode(Byte byte_value, unsigned char* result) { // 0 - change byte
    if (current_bit_number >= bytes_codes_sizes[byte_value]) {
        current_bit_number = 0;
        return 0;
    } else {
        *result = ((bytes_codes[byte_value][current_bit_number / 8] & (1 << (current_bit_number % 8))) > 0);
        current_bit_number++;
        return 1;
    }
}

unsigned char getByteByBit(unsigned char bit_value, Byte* result) {
    if (current_node == NULL)
        current_node = root;

    current_node = (bit_value == 1)? current_node->left: current_node->right;

    if (isLeaf(current_node)) {
        *result = current_node->byte_value;
        current_node = NULL;
        return 1;
    } else
        return 0;
}

void destroyHuffmanCodes() {
    nodes_count = 0;
    root = NULL;
    if (output_tree)
        free(output_tree);
    output_tree = NULL;
    output_tree_size = 0;
    initStaticVariables();
}