#ifndef MAIN_C
#define MAIN_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer.c"        /*before copyng to the ubuntu need to be changed to .h*/
#include "pre_pross.c"    /*before copyng to the ubuntu need to be changed to .h*/
#include "data_structs.c" /*before copyng to the ubuntu need to be changed to .h*/
#include "first_pass.h"
#include "first_pass.c"

#define HASH_SIZE 100

int main(int argc, char *argv[])
{
    int line_number;
    char parsed_line[MAX_LINE];
    struct translation_unit program;
    struct ast result;
    FILE *am_file;
    FILE *as_file;
    FILE *parsed_file;
    char line[MAX_LINE];
    hash *hash_table[HASH_SIZE];
    char the_error[MAX_LINE];
    strcpy(the_error, "");
    line_number = 0;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file.as>\n", argv[0]);
        return EXIT_FAILURE;
    }

    as_file = fopen(argv[1], "r");
    if (as_file == NULL)
    {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    am_file = fopen("output.am", "w");
    if (am_file == NULL)
    {
        perror("Error creating output.am");
        fclose(as_file);
        return EXIT_FAILURE;
    }
    hash_reset(hash_table);
    find_macro(line, hash_table, as_file, am_file, the_error);

    if (strcmp(the_error, "") != 0)
    {
        printf("Error in pre-prossesor: %s\n", the_error);
        fclose(am_file);
        remove("output.am");
        free_memory(hash_table);
    }
    else
    {
        fclose(as_file);
        fclose(am_file);

        parsed_file = fopen("output.am", "r");
        if (parsed_file == NULL)
        {
            perror("Error opening output.am for parsing");
            return EXIT_FAILURE;
        }

        if (first_pass("am_file", parsed_file, &program, hash_table))
        {
            fclose(parsed_file);
            free_memory(hash_table);
            remove("output.am");
            return EXIT_FAILURE;
        }

        fclose(parsed_file);
    }
    return EXIT_SUCCESS;
}
#endif
