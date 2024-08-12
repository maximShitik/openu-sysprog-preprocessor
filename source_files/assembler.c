#ifndef ASSEMBLER_C
#define ASSEMBLER_C

#include "lexer.c"     /*before copyng to the ubuntu need to be changed to .h*/
#include "pre_pross.c" /*before copyng to the ubuntu need to be changed to .h*/
#include "first_and_second_pass.h"
#include "first_pass.c"
#include "second_pass.c"
#include "back_end.h"
#include "back_end.c"
#include "macros.h"
#include "data_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i;
    struct translation_unit *program;
    char line[MAX_ADDRESS];
    char *am_file_name;
    FILE *am_file;
    hash *hash_table[HASH_SIZE] = {0};
    line_mapping line_map[MAX_ADDRESS] = {0};
    int expanded_line_count = 0;

    for (i = 1; i < argc; i++)
    {
        /*initilizing the data structs for the assembler*/
        program = create_translation_unit();
        first_intilization(program);
        hash_reset(hash_table);

        /*starting the pre-prosses*/
        am_file_name = pre_prossesor(line, hash_table, argv[i], line_map, &expanded_line_count);
        if (am_file_name != NULL)
        {
            /*no errors were found opening the am file and reading from him*/
            am_file = fopen(am_file_name, "r");

            if (am_file == NULL)
            {
                remove(am_file_name);
                FREE_ALL_STRUCTS
                fprintf(stderr, "Error: failed to open file %s\n", am_file_name);
                continue;
            }

            else if (first_pass(am_file_name, am_file, program, hash_table, line_map, expanded_line_count))
            {
                /*if error flag is 1 = there is an error*/
                remove(am_file_name);
                FREE_ALL_STRUCTS
                continue;
            }
            else
            {
                rewind(am_file);
                if (second_pass(am_file_name, am_file, program, hash_table, line_map, expanded_line_count))
                {
                    remove(am_file_name);
                    FREE_ALL_STRUCTS
                    continue;
                }
                else
                {
                    printing_files(am_file_name, program);
                }
            }
            printf("\n %s was processed successfully \n", am_file_name);
            FREE_ALL_STRUCTS
        }
        else
        {
            remove(am_file_name);
            free_translation_unit(program);
            free(am_file_name);
        }
    }
    printf("Done\n");
    return 0;
}

#endif
