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
    struct symbol *next;
};

struct ext
{
    char ext_name[MAX_LINE];
    int address;
    int exter_count;
    int address_counter;
    struct ext *next;
};


struct translation_unit
{
    int code_array[MAX_LINE];
    int data_array[MAX_LINE];
    struct symbol *symbol_table; 
    struct ext *ext_table;
    int IC;
    int DC;
    struct symbol *entry[MAX_LINE];
    int entry_count;
};

int first_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[]);
struct symbol *symbol_search(struct symbol *head, char *name);
#endif
