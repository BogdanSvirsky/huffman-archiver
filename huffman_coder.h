#define MAX_BYTE_VALUE 256
typedef unsigned char Byte;

void initHuffmanCodes(unsigned long long bytes_frequency[MAX_BYTE_VALUE]);
unsigned short getOutputTreeSize();
Byte* getOutputTree();
void destroyHuffmanCodes();
unsigned char getCode(unsigned char bit, Byte* result);
unsigned char getBitInCode(Byte byte_value, unsigned char* result);
void setOutputTree(unsigned short external_output_tree_size, Byte* external_output_tree);
unsigned char getByteByBit(unsigned char bit_value, Byte* result);