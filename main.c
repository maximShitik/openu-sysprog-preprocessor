#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer.c"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character if present
        line[strcspn(line, "\r\n")] = '\0';

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == ';')
            continue;

        printf("Processing line: %s\n", line); // Debug print

        struct ast result = parse_line(line);
        if(result.line_type==error_line){
            printf("Error in line: %s\n", line);
        }

        // For debugging: print the result or handle it as needed
        // printf("Processed line: %s\n", line);
        // print_ast(result); // Assume there's a function to print the AST
    }

    fclose(file);
    
    return EXIT_SUCCESS;
}
