#include <stdio.h>
#include <stdlib.h>
#include <tree_types.h>

#include "frontend.h"


char* open_file(const char* path);

int main(int argc, const char** argv) {
    char* source = NULL;
    if (argc != 0) {
        source = open_file(argv[1]);
        tokenize(source);
        ASTnode head = binary_expression();
        U32 num = ast_interpret(head);
        printf("\nNumber = %lu.\n", num);
        ast_print(head, 0);
    } else {
        fprintf(stderr, "No input files: bro :(\n");
    }
}

char* open_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) 
    {
        fprintf(stderr, "Could not open file \"%s\"\n", path);
        exit(74);
    }
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* text_buffer = (char*)malloc(fileSize + 1);
    if (text_buffer == NULL) 
    {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(text_buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize)
    {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    text_buffer[bytesRead] = '\0';
    printf("BYTES READ: %zu\n", bytesRead);
    fclose(file);
    return text_buffer;
}
