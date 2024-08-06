#ifndef PRE_PROSS_C
#define PRE_PROSS_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "data_structs.c"
#include "pre_pross.h"
#include "help_func.h"

int line_defenition(char *line, struct sep_line separated, char error[MAX_LINE])
{
    if (strcmp(separated.line[0], "macr") == 0)
    {
        if (strpbrk(line, ",") != NULL)
        {
            strcpy(error, " macro definition should not contain a comma\n");
            return Error;
        }
        if (separated.line_number == 1)
        {

            strcpy(error, "macro definition should contain a name\n");
            return Error;
        }
        else if (system_names(separated.line[1]) != TRUE)
        {

            strcpy(error, " macro name is a system name\n");
            return Error;
        }
        else
        {
            return hash_function(separated.line[1]);
        }
    }
    else if (strcmp(separated.line[0], "endmacr") == 0)
    {
        if (separated.line_number > 1)
        {

            strcpy(error, "end of macro definition should not contain any other words\n");

            return Error;
        }
        else
        {
            return endmacro;
        }
    }
    else
    {
        return regular_line;
    }
}

void find_macro(char *line, hash *hash_table[], FILE *input, FILE *output, char error[MAX_LINE])
{
    int index_copy;
    hash *macro_found;
    int indicator; /*if indicator is macro_defenition its will define the lines as macros till the indicator is regular*/
    int name_index;
    name_index = 0;
    char *line_copy;
    index_copy = 0;

    while (fgets(line, MAX_LINE, input))
    {

        trim_whitespace(line);
        line_copy = malloc(strlen(line) + 1);
        if (line_copy == NULL)
        {
            strcpy(error, "Memory allocation error");
            exit(EXIT_FAILURE);
        }
        if (line[0] == '\0' || line[0] == ';')
            continue;
        strcpy(line_copy, line);

        struct sep_line separated = next_word(line);
        name_index = line_defenition(line_copy, separated, error);

        if (name_index == Error)
        {
            free(line_copy);
            return;
        }
        else if (name_index >= min_hash_index)
        {
            insert_to_hash(separated.line[1], hash_table, name_index, 1);
            indicator = macro_defenition;
            index_copy = name_index; /*saving the index so we will insert the lines in the right macro*/
        }
        else if (name_index == endmacro)
        {
            /*stop adding lines to the hash*/
            indicator = regular_line;
        }
        else if (name_index == regular_line)
        {
            if (indicator == macro_defenition)
            {
                /*using the index copy to add the lines in the right index*/
                insert_to_hash(line_copy, hash_table, index_copy, 0);
            }
            else
            {
                macro_found = search_in_hash(line_copy, hash_table);
                /*searching if the macro is in the hash so we will add it to the file*/

                if (macro_found != NULL)
                {
                    /*add the hash data to the as file*/
                    line_node *current_line = macro_found->data;
                    while (current_line != NULL)
                    {
                        fprintf(output, "%s\n", current_line->line_data);
                        current_line = current_line->next;
                    }
                    fflush(output);
                }
                /*add the line to the as file*/
                else
                {
                    fprintf(output, "%s\n", line_copy);
                    fflush(output);
                }
            }
        }
    }
    free(line_copy);
}
#endif
