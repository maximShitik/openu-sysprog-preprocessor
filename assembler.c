#ifndef ASSEMBLER_C
#define ASSEMBLER_C

#include "lexer.c"        /*before copyng to the ubuntu need to be changed to .h*/
#include "pre_pross.c"    /*before copyng to the ubuntu need to be changed to .h*/
#include "data_structs.c" /*before copyng to the ubuntu need to be changed to .h*/
#include "first_pass.h"
#include "first_pass.c"
#include "second_pass.c"
#include "back_end.h"
#include "back_end.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    int i;
    struct translation_unit program = {0};
    char line[MAX_LINE];
    char *am_file_name;
    hash *hash_table[HASH_SIZE] = {0};
    FILE *am_file;
    FILE *as_file;
    for (i = 1; i < argc; i++)
    {
        printf("Input file: %s\n", argv[i]);
       
        hash_reset(hash_table);
        am_file_name = pre_prossesor(line, hash_table,  argv[i]);
        if (am_file_name != NULL)
        {
            am_file = fopen(am_file_name, "r");
            if (am_file == NULL)
            {
                fprintf(stderr, "Error: failed to open file %s\n", am_file_name);
                free_hash(hash_table);
                return EXIT_FAILURE;
            }
            else if (first_pass(am_file_name, am_file, &program, hash_table))
            {
                fclose(am_file);
                free_translation_unit(&program);
                free_hash(hash_table);
                remove(am_file_name);
                return EXIT_FAILURE;
            }
            else

            {
                rewind(am_file);
                if (second_pass(am_file_name, am_file, &program, hash_table))
                {
                    free_translation_unit(&program);
                    free_hash(hash_table);
                    fclose(am_file);
                    remove(am_file_name);
                }
                else
                    printing_files(am_file_name, &program);
            }
            fclose(am_file);
            free_translation_unit(&program);
            free_hash(hash_table);
            free(am_file_name);
        }
    }
    return 0;
}

#endif
