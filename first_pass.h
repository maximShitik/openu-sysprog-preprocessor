#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "lexer.h"
struct symbol {
    char *symbol_name[MAX_LINE];
    int address;
    enum symbol_type {
        code_type ,
        data_type,
        entry_type,
        extrn_type
    } symbol_type;
};

struct ext {
    char *ext_name[MAX_LINE];
    int address;
};


struct translation_unit {
    
    struct ext ext_table[MAX_LINE];
    int IC;
    int DC;
};

#endif
