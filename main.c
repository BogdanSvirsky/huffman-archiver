#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman_coder.h"

void encodeFile(FILE* input_file, FILE* output_file) {
    FILE* temporary_file = tmpfile();
    Byte current_byte;
    unsigned long long bytes_frequency[MAX_BYTE_VALUE] = {0};
    while (fread(&current_byte, sizeof(Byte), 1, input_file) == 1) {
        bytes_frequency[current_byte]++;
        fwrite(&current_byte, sizeof(Byte), 1, temporary_file);
    }
    
    rewind(temporary_file);
    initHuffmanCodes(bytes_frequency);

    unsigned short output_tree_size = getOutputTreeSize();

    fwrite(&output_tree_size, sizeof(unsigned short), 1, output_file);

    fwrite(getOutputTree(), sizeof(Byte), (output_tree_size / 8) + (output_tree_size % 8 > 0), output_file);

    unsigned char bit_value, current_bit = 1, bits_scanned = 0;
    Byte output_byte = 0;

    while (fread(&current_byte, sizeof(Byte), 1, temporary_file)) {
        while (getBitInCode(current_byte, &bit_value)) {
            if (bit_value)
                output_byte |= current_bit;
            current_bit <<= 1;
            bits_scanned++;
            if (current_bit == 0) {
                fwrite(&output_byte, sizeof(Byte), 1, output_file);
                current_bit = 1;
                bits_scanned = 0;
                output_byte = 0;
            }
        }
    }

    if (current_bit != 1)
        fwrite(&output_byte, sizeof(Byte), 1, output_file);

    fwrite(&bits_scanned, sizeof(unsigned char), 1, output_file);

    fclose(temporary_file);
    destroyHuffmanCodes();
}

void decodeFile(FILE* input_file, FILE* output_file) {
    Byte current_byte = 0, out_byte = 0, first_byte, second_byte, third_byte;
    unsigned short output_tree_size;

    fread(&output_tree_size, sizeof(unsigned short), 1, input_file);
    Byte* output_tree = (Byte*) calloc(output_tree_size / 8 + (output_tree_size % 8 > 0), sizeof(Byte));
    fread(output_tree, sizeof(Byte), output_tree_size / 8 + (output_tree_size % 8 > 0), input_file);
    setOutputTree(output_tree_size, output_tree);

    fread(&first_byte, sizeof(Byte), 1, input_file);
    fread(&second_byte, sizeof(Byte), 1, input_file);

    while (fread(&third_byte, sizeof(Byte), 1, input_file)) {
        current_byte = first_byte;
        for (Byte current_bit = 1; current_bit != 0; current_bit <<= 1) {
            if (getByteByBit((current_byte & current_bit) > 0, &out_byte))
                fwrite(&out_byte, sizeof(Byte), 1, output_file);
        }
        first_byte = second_byte;
        second_byte = third_byte;
    }


    current_byte = first_byte;
    Byte current_bit = 1;
    for (int i = 0; i < second_byte; i++) {
        if (getByteByBit((current_byte & current_bit) > 0, &out_byte))
            fwrite(&out_byte, sizeof(Byte), 1, output_file);
        current_bit <<= 1;
    }

    if (output_tree)
        free(output_tree);
    destroyHuffmanCodes();
}

FILE* createOutputFile(char* file_name, char* file_extension) {
    unsigned int file_name_length = strlen(file_name);
    unsigned int file_extension_length = strlen(file_extension);
    char *output_file_name = (char*) calloc(file_name_length + 1 + file_extension_length + 1, sizeof(char));

    for (unsigned int i = 0; i < file_name_length; i++)
        output_file_name[i] = file_name[i];
    
    output_file_name[file_name_length] = '.';

    for (unsigned int i = 0; i < file_extension_length; i++)
        output_file_name[file_name_length + i + 1] = file_extension[i];
    
    FILE* ouput_file = fopen(output_file_name, "wb+");
    free(output_file_name);
    return ouput_file;
}

FILE* openInputFile(char* input_file_name) {
    FILE *input_file = fopen(input_file_name, "rb");
    
    if (input_file == NULL) {
        fprintf(stderr, "Bad filename: %s\n", input_file_name);
        return NULL;
    }

    return input_file;
}

int main(int argc, char** args) {
    if (argc < 2) {
        fprintf(stderr, "Bad arguments count\n");
    }

    FILE *input_file, *output_file;

    char *mode = args[1];
    char *file_extension, *input_file_name;

    if (strcmp(mode, "--D") == 0) {
        for (int i = 2; i < argc; i++) {
            if ((input_file = openInputFile(args[i])) == NULL)
                continue;

            file_extension = (char*) malloc(sizeof(char));
            input_file_name = strtok(args[i], ".");
            size_t file_extension_length = 0;
            Byte current_byte = 0;

            do {
                fread(&current_byte, sizeof(Byte), 1, input_file);
                file_extension = (char*) realloc(file_extension, ++file_extension_length * sizeof(char));
                file_extension[file_extension_length - 1] = current_byte;
            } while (current_byte);

            output_file = createOutputFile(input_file_name, file_extension);

            decodeFile(input_file, output_file);
            free(file_extension);
            fclose(input_file);
            fclose(output_file);
        }
    } else if (strcmp(mode, "--E") == 0) {
        for (int i = 2; i < argc; i++) {
            input_file = openInputFile(args[i]);
            if (input_file == NULL)
                continue;

            input_file_name = strtok(args[i], ".");
            
            output_file = createOutputFile(input_file_name, "harc");

            file_extension = strtok(NULL, ".");

            if (file_extension == NULL)
                file_extension = "\0";
            
            fwrite(file_extension, sizeof(char), strlen(file_extension) + 1, output_file);

            encodeFile(input_file, output_file);
            fclose(input_file);
            fclose(output_file);
        }
    } else
        fprintf(stderr, "Bad mode");
    return 0;
}