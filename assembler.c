#ifndef ASSEMBLER_C
#define ASSEMBLER_C

#include "lexer.c"     /*before copyng to the ubuntu need to be changed to .h*/
#include "pre_pross.c" /*before copyng to the ubuntu need to be changed to .h*/
#include "first_pass.h"
#include "first_pass.c"
#include "second_pass.c"
#include "back_end.h"
#include "back_end.c"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structs.h"

int main(int argc, char **argv)
{
    int i;
    struct translation_unit *program;
    char line[MAX_LINE];
    char *am_file_name;
    hash *hash_table[HASH_SIZE] = {0};
    FILE *am_file;
    int original_line_numbers[MAX_ADDRESS] = {0};
    int expanded_line_count = 0;

    for (i = 1; i < argc; i++)
    {
        program = create_translation_unit();
        first_intilization(program);
        hash_reset(hash_table);
        am_file_name = pre_prossesor(line, hash_table, argv[i], original_line_numbers, &expanded_line_count);
        if (am_file_name != NULL)
        {
            am_file = fopen(am_file_name, "r");
            if (am_file == NULL)
            {
                free_translation_unit(program);
                free(am_file_name);
                free_hash(hash_table);
                fprintf(stderr, "Error: failed to open file %s\n", am_file_name);
                continue;
            }

            else if (first_pass(am_file_name, am_file, program, hash_table, original_line_numbers, expanded_line_count))
            {
                FREE_TRANSU_HASH_CLOSE
                remove(am_file_name);
                continue;
            }
            else

            {
                rewind(am_file);
                if (second_pass(am_file_name, am_file, program, hash_table))
                {   remove(am_file_name);
                    FREE_TRANSU_HASH_CLOSE
                    
                    continue;
                }
                else
                    printing_files(am_file_name, program);
            }
            FREE_TRANSU_HASH_CLOSE
        }
        else
        {
            free_hash(hash_table);
            free_translation_unit(program);
            remove(am_file_name);
        }
    }
    return 0;
}
#endif
