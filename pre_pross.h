#ifndef PRE_PROSS_H
#define PRE_PROSS_H
#include "data_structs.h"
#include <stdio.h>
#include "lexer.h"
enum
{
    min_hash_index = 0,
    Error = -1,
    regular_line = -2,
    endmacro = -3,
    macro_defenition = -4,
    first_int=-5
};

int line_defenition(char *line, struct sep_line separated, char error[MAX_LINE]);
char *pre_prossesor(char *line, hash *hash_table[], char *input);



#endif
