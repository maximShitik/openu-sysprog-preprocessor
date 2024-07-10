
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "pre_prossesor.c"

#define HASH_SIZE 100

int main(int argc, char *argv[])
{
    char parsed_line[MAX_LINE];
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file.as>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *as_file = fopen(argv[1], "r");
    if (as_file == NULL)
    {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *am_file = fopen("output.am", "w");
    if (am_file == NULL)
    {
        perror("Error creating output.am");
        fclose(as_file);
        return EXIT_FAILURE;
    }

    hash *hash_table[HASH_SIZE] = {NULL};
    char line[MAX_LINE];
    find_macro(line, hash_table, as_file);
    free_memory(hash_table);
    fclose(as_file);
    fclose(am_file);

    FILE *parsed_file = fopen("output.am", "r");
    if (parsed_file == NULL)
    {
        perror("Error opening output.am for parsing");
        return EXIT_FAILURE;
    }

    while (fgets(parsed_line, sizeof(parsed_line), parsed_file))
    {

        parsed_line[strcspn(parsed_line, "\r\n")] = '\0';

        if (parsed_line[0] == '\0' || parsed_line[0] == ';')
            continue;

        struct ast result = parse_line(parsed_line);
        if (result.line_type == error_line)
        {
            printf("Error in line: %s\n", result.error.type);
        }
    }

    fclose(parsed_file);

    return EXIT_SUCCESS;
}
