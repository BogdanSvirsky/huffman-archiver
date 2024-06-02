#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "huffman_coder.h"

int main() {
    srand(time(0));
    unsigned long long bytes_frequency[256] = {0}, c = 0;
    unsigned char tmp;
    bytes_frequency[1] = 10;
    bytes_frequency[2] = 5;
    bytes_frequency[5] = 25;
    bytes_frequency[7] = 1;
    // for (int j = 0; j < 1000; j++) {
        // for (int i = 0; i < 256; i++) {
        //     if (bytes_frequency[i] = ((((unsigned long long) rand()) << 32) + rand())) {
        //         c++;
        //     }
    //         // printf("%d] =%llu\n", i, bytes_frequency[i]);
        // }
        initHuffmanCodes(bytes_frequency);
    // }
    // unsigned char bytes[6] = {1, 1, 1, 1, 1, 0};
    // Byte out = 0;
    // for (int i = 0; i < 6; i++) {
    //     if (getByteByBit(bytes[i], &out))
    //         printf("%d\n", out);
    // }
    // bytes_frequency[5] = 1000000000;
    // unsigned short size = getOutputTreeSize();
    // Byte* tree = (Byte*) calloc(size / 8 + (size % 8 > 0), sizeof(Byte)), *old = getOutputTree();
    // for (int i = 0; i < size / 8 + (size % 8 > 0); i++)
    //     tree[i] = old[i];
    // setOutputTree(size, tree);
    // free(tree);
    destroyHuffmanCodes();
    return 0;
}