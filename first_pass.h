#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "lexer.h"
#include <stdio.h>
struct symbol
{
    char symbol_name[MAX_LINE];
    int address;
    enum symbol_type
    {
        code_type,
        data_type,
        entry_type,
        extrn_type,
        entry_code_type,
        entry_data_type
    } symbol_type;
    
};


struct ext
{
    char *ext_name[MAX_LINE];
    int address;
    int exter_count;
};

struct translation_unit
{
    int code_array[MAX_LINE];
    int data_array[MAX_LINE];
    struct symbol symbol_table[MAX_LINE];
    int symbol_count;
    struct ext ext_table[MAX_LINE];
    int IC;
    int DC;
    struct symbol *enrty[MAX_LINE];
    int enrty_count;
};


int first_pass(char *file_name, FILE *am_file, struct translation_unit *program);

#endif
