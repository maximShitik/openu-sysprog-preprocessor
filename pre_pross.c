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

int copy_file(const char *source_file, const char *target_file)
{
    char buffer[MAX_LINE];
    size_t bytes;
    FILE *dst;
    FILE *src;
    src = fopen(source_file, "rb");
    dst = fopen(target_file, "wb");

    if (src == NULL || dst == NULL)
    {
        perror("Error opening file");
        if (src)
            fclose(src);
        if (dst)
            fclose(dst);
        return -1;
    }

    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0)
    {
        fwrite(buffer, 1, bytes, dst);
    }

    fclose(src);
    fclose(dst);

    return 0;
}

void create_as_and_am_files(char *input, char **as_file_name, char **am_file_name, FILE **am_file, FILE **as_file)
{
    /*adding the extensions*/
    *as_file_name = STRCAT_AND_MALLOC(*as_file_name, input, ".as");
    *am_file_name = STRCAT_AND_MALLOC(*am_file_name, input, ".am");

    /*making a copy of the original file to run on */
    if (copy_file(input, *as_file_name) != 0)
    {
        free(*as_file_name);
        free(*am_file_name);
        *am_file = NULL;
        *as_file = NULL;
        return;
    }

    *am_file = fopen(*am_file_name, "w");
    *as_file = fopen(*as_file_name, "r");

    if (*am_file == NULL || *as_file == NULL)
    {
        printf("Error opening files\n");
        if (*am_file != NULL)
            fclose(*am_file);
        if (*as_file != NULL)
            fclose(*as_file);
        free(*as_file_name);
        free(*am_file_name);
        *am_file = NULL;
        *as_file = NULL;
        exit(EXIT_FAILURE);
    }
}

char *pre_prossesor(char *line, hash *hash_table[], char *input, line_mapping line_map[], int *expanded_line_count)
{
    int index_copy, indicator, name_index, expanded_line_number, original_line_number, error_flag;
    char *line_copy, *as_file_name, *am_file_name, error[MAX_ADDRESS];
    hash *macro_found;
    struct sep_line separated;
    FILE *am_file, *as_file;
    line_node *current_line;
    expanded_line_number = 0;
    original_line_number = 0;
    *expanded_line_count = 0;
    error_flag = 0;
    name_index = 0;
    index_copy = 0;
    indicator = first_int; /*initilizing it to somthing so it wont be NULL*/

    create_as_and_am_files(input, &as_file_name, &am_file_name, &am_file, &as_file);

    while (fgets(line, MAX_ADDRESS, as_file) != NULL)
    {
        original_line_number++;
        trim_whitespace(line);
        line_copy = malloc(strlen(line) + 1);
        if (line[0] == '\0' || line[0] == ';') /*skipping note and empty lines*/
        {
            free(line_copy);
            continue;
        }

        if (line_copy == NULL)
        {
            MEMORY_FAIL;
            error_flag = 1;
        }

        strcpy(line_copy, line); /*copying the line so we can use it in the hash*/
        separated = split_line(line);
        name_index = line_defenition(line_copy, separated, error);

        if (name_index == Error)
        {
            printf("Error in pre-prossesor: %s in line %d \n", error, original_line_number);
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
                        CALCULATE_LINE_OFFSET
                        current_line = current_line->next;
                    }
                    fflush(am_file);
                }
                /*add the line to the as file*/
                else
                {
                    CALCULATE_LINE_OFFSET
                    fprintf(am_file, "%s\n", line_copy);
                    fflush(am_file);
                }
            }
        }
        *expanded_line_count = expanded_line_number;
        free(line_copy);
    }
    if (error_flag)
    {
        FREE_AND_REMOVE
        return NULL;
    }
    else
    {
        free(as_file_name);
        fclose(as_file);
        fclose(am_file);
        return am_file_name;
    }
}
#endif
