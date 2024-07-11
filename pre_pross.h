#ifndef PRE_PROSS_H
#define PRE_PROSS_H
#include "data_structs.h"
#include <stdio.h>

enum
{
    min_hash_index = 0,
    Error = -1,
    regular_line = -2,
    endmacro = -3,
    macro_defenition = -4,
};

int line_defenition(char *line, struct sep_line separated);
void find_macro(char *line, hash *hash_table[], FILE *input,FILE *output);



#endif
