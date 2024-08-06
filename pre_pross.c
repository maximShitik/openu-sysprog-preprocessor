#ifndef PRE_PROSS_C
#define PRE_PROSS_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"
#include "data_structs.c"
#include "pre_pross.h"
#include "help_func.h"
#include "macros.h"

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

char *pre_prossesor(char *line, hash *hash_table[], char *input)
{
    int index_copy;
    int line_number;
    hash *macro_found;
    int indicator; /*if indicator is macro_defenition its will define the lines as macros till the indicator is regular*/
    int name_index;
    struct sep_line separated;
    char *line_copy;
    FILE *am_file;
    FILE *as_file;
    char *as_file_name;
    char *am_file_name;
    char error[MAX_LINE];
    line_node *current_line;
    int error_flag;
    error_flag = 0;
    name_index = 0;
    indicator = first_int;
    index_copy = 0;
    line_number = 0;

    as_file_name = STRCAT_MALLOC(as_file_name, input, ".as");
    am_file_name = STRCAT_MALLOC(am_file_name, input, ".am");
    as_file = fopen(input, "r");
    am_file = fopen(am_file_name, "w");
    if (as_file_name == NULL || am_file_name == NULL || am_file == NULL || as_file == NULL)
    {
        printf("Error opening files\n");
        remove(am_file_name);
        free(as_file_name);
        free(am_file_name);

        return NULL;
    }

    while (fgets(line, MAX_LINE, as_file) != NULL)
    {
        line_number++;
        trim_whitespace(line);
        line_copy = malloc(strlen(line) + 1);
        if (line_copy == NULL)
        {
            printf("Memory allocation error");
            error_flag = 1;
        }
        if (line[0] == '\0' || line[0] == ';')
            continue;
        strcpy(line_copy, line);

        separated = next_word(line);
        name_index = line_defenition(line_copy, separated, error);

        if (name_index == Error)
        {
            printf("Error in pre-prossesor: %s in line %d \n", error, line_number);
            error_flag = 1;
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
                    current_line = macro_found->data;
                    while (current_line != NULL)
                    {
                        fprintf(am_file, "%s\n", current_line->line_data);
                        current_line = current_line->next;
                    }
                    fflush(am_file);
                }
                /*add the line to the as file*/
                else
                {
                    fprintf(am_file, "%s\n", line_copy);
                    fflush(am_file);
                }
            }
        }
    }
    if (error_flag)
    {
        FREE_AND_REMOVE
        return NULL;
    }
    else
    {
        free(line_copy);
        free(as_file_name);
        fclose(as_file);
        fclose(am_file);
        return am_file_name;
    }
}
#endif
