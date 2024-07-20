#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexer.c"        /*before copyng to the ubuntu need to be changed to .h*/
#include "pre_pross.c"    /*before copyng to the ubuntu need to be changed to .h*/
#include "data_structs.c" /*before copyng to the ubuntu need to be changed to .h*/

#define HASH_SIZE 100


extern void find_macro(char *line, hash *hash_table[], FILE *input, FILE *output, char error[MAX_LINE]);
extern struct ast parse_line(char *line);

void run_tests(FILE *input_file)
{
    char line[MAX_LINE];
    hash *hash_table[HASH_SIZE];
    char the_error[MAX_LINE];
    struct ast result;
    int line_number = 0;
    char all_errors[MAX_LINE * 100] = ""; // Buffer to accumulate all errors

    FILE *am_file = tmpfile();
    if (am_file == NULL)
    {
        perror("Error creating temporary am_file");
        fclose(input_file);
        return;
    }

    hash_reset(hash_table);
    while (fgets(line, sizeof(line), input_file))
    {
        line_number++;
        line[strcspn(line, "\r\n")] = '\0';  // Remove newline character

        if (line[0] == '\0' || line[0] == ';')
            continue;

        printf("Testing line %d: %s\n", line_number, line);

        if (strstr(line, "macro"))
        {
            FILE *temp_input = tmpfile();
            fputs(line, temp_input);
            rewind(temp_input);

            find_macro(line, hash_table, temp_input, am_file, the_error);

            if (strcmp(the_error, "") != 0)
            {
                snprintf(all_errors + strlen(all_errors), sizeof(all_errors) - strlen(all_errors), "Error in pre-processor (line %d): %s\n", line_number, the_error);
            }

            fclose(temp_input);
        }
        else
        {
            result = parse_line(line);

            if (result.line_type == error_line)
            {
                snprintf(all_errors + strlen(all_errors), sizeof(all_errors) - strlen(all_errors), "Error in line number %d: %s\n", line_number, result.error.type);
            }
        }
    }

    if (strlen(all_errors) > 0)
    {
        printf("\nSummary of all errors:\n");
        printf("%s", all_errors);
    }
    else
    {
        printf("\nNo errors found.\n");
    }

    free_memory(hash_table);
    fclose(am_file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        perror("Error opening input file\n");
        return EXIT_FAILURE;
    }

    run_tests(input_file);

    fclose(input_file);
    return EXIT_SUCCESS;
}
