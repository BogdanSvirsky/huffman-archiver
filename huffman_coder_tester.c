#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "huffman_coder.h"
#define TEST_SIZE 100000

int main() {
    srand(time(0));
    unsigned long long bytes_frequency[256] = {0};
    int code_capacity = 1, code_length = 0, unique_bytes_count, decoded_code_size = 0;
    unsigned char tmp, *code = (unsigned char*) malloc(sizeof(unsigned char));
    Byte byte_sequence[TEST_SIZE];

    unique_bytes_count = rand() % MAX_BYTE_VALUE;

    for (int i = 0; i < unique_bytes_count; i++)
        bytes_frequency[i] = (((unsigned long long) rand()) << 32) + rand();

    initHuffmanCodes(bytes_frequency);

    unsigned char bit;

    for (int i = 0; i < TEST_SIZE; i++) {
        byte_sequence[i] = rand() % unique_bytes_count;
        while (getBitInCode(byte_sequence[i], &bit)) {
            if (code_capacity == code_length) {
                code_capacity *= 2;
                code = (unsigned char*) realloc(code, sizeof(unsigned char) * code_capacity);
            }
            code[code_length++] = bit;
        }
    }

    Byte byte_value;

    for (int i = 0; i < TEST_SIZE; i++) {
        while (getByteByBit(code[decoded_code_size++], &byte_value) == 0);
        assert(((unsigned char) byte_sequence[i]) == (unsigned char) byte_value);
    }

    free(code);
    destroyHuffmanCodes();
    return 0;
}