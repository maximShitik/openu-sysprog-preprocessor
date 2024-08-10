#ifndef FIRST_PASS_C
#define FIRST_PASS_C

#include "first_and_second_pass.h"
#include "data_structs.h"
#include "data_struct_functions.h"
#include "lexer.h"
#include "help_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void reset_line(char *line, int size);

int first_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], line_mapping line_map[], int expanded_line_count)
{
    struct symbol *current;
    int i;
    int is_error;
    int expanded_line_number;
    int original_line_number;
    int ic, dc;
    char line[MAX_ADDRESS];
    struct symbol *symbol_found, *new_symbol;
    struct ast line_ast = {0};
    ic = 100, dc = 0, is_error = 0;
    expanded_line_number = 0;
    reset_line(line, MAX_ADDRESS);
    while (fgets(line, sizeof(line), am_file))
    {
        original_line_number = line_map[expanded_line_number].original_line_number;
        if (strlen(line) > 80)
        {
            printf("Error in line %d: line is too long\n", original_line_number);
            is_error = 1;
            expanded_line_number++;
            continue;
        }
        line_ast = parse_line(line, hash_table); /* Parsing the line to an AST */

        if (line_ast.line_type == error_line)
        {
            printf("Syntax error in line %d: %s\n", original_line_number, line_ast.error.type);
            is_error = 1;
            expanded_line_number++;
            continue;
        }

        /* Processing the label and checking if we have redefinition */
        if ((line_ast.label_name[0] != '\0') && (line_ast.line_type == inst_line || line_ast.line_type == command_line))
        {
            symbol_found = symbol_search(program->symbol_table, line_ast.label_name);
            /* Checking if the name is in the label table */
            if (symbol_found)
            {
                if (symbol_found->symbol_type == entry_type)
                {
                    symbol_found->symbol_type = line_ast.line_type == command_line ? entry_code_type : entry_data_type;
                    symbol_found->address = line_ast.line_type == command_line ? ic : dc;
                }
                else /* If the name is not entry and we already have it in the table */
                {
                    printf("Error in %s line %d: %s is already defined \n", file_name, original_line_number, line_ast.label_name);
                    is_error = 1;
                    expanded_line_number++;
                    continue;
                }
            }
            else
            {
                new_symbol = (struct symbol *)malloc(sizeof(struct symbol));
                strcpy(new_symbol->symbol_name, line_ast.label_name);
                new_symbol->address = line_ast.line_type == command_line ? ic : dc;
                new_symbol->symbol_type = line_ast.line_type == command_line ? code_type : data_type;
                symbol_insert(&program->symbol_table, new_symbol);
            }
        }

        if (line_ast.line_type == command_line) /* Updating the IC */
        {
            ic++;

            if (SOURCE_AND_TARGET_ARE_REGISTERS)
                ic++;
            else
            {
                /* Counting the number of arguments for the IC without the label name and command */
                if (line_ast.label_name[0] != '\0')
                {
                    ic += line_ast.argument_count - 2;
                }
                else
                {
                    ic += line_ast.argument_count - 1;
                }
            }
        }
        else if (line_ast.line_type == inst_line) /* Coding the instructions to the translation unit */
        {
            if (line_ast.line_type_data.inst.inst_type == data)
            {
                memcpy(&program->data_array[program->DC], line_ast.line_type_data.inst.data_array, sizeof(int) * line_ast.line_type_data.inst.data_counter);
                dc += line_ast.line_type_data.inst.data_counter;
                program->DC = dc;
            }
            else if (line_ast.line_type_data.inst.inst_type == string)
            {
                for (i = 0; i < line_ast.line_type_data.inst.data_counter; i++)
                {
                    program->data_array[program->DC + i] = (int)*line_ast.line_type_data.inst.string_array[i];
                }
                dc += line_ast.line_type_data.inst.data_counter + 1;
                program->DC = dc;
                free_string_array(line_ast.line_type_data.inst.string_array, line_ast.line_type_data.inst.data_counter);
            }
            /*checking if we have the entry / extern in the symbol table , if not we will set it as entry/extern without intilization*/
            else if (line_ast.line_type_data.inst.inst_type == entry || line_ast.line_type_data.inst.inst_type == extrn)
            {
                symbol_found = symbol_search(program->symbol_table, line_ast.line_type_data.inst.label_array[0]);
                if (symbol_found && line_ast.line_type_data.inst.inst_type == entry) /*if we have the symbol in the table and it's an entry we will 
                                                                                                update it's type to be entry withintilization */
                {
                    if (symbol_found->symbol_type == code_type)
                    {
                        symbol_found->symbol_type = entry_code_type;
                    }
                    else if (symbol_found->symbol_type == data_type)
                    {
                        symbol_found->symbol_type = entry_data_type;
                    }
                    else
                    {
                        printf("Error in %s line %d: %s is already defined \n", file_name, original_line_number, line_ast.line_type_data.inst.label_array[0]);
                        is_error = 1;
                    }
                }
                else if (!symbol_found)/*addeing the entry/ extern to the table as entry/extern without intilization */
                {
                    new_symbol = (struct symbol *)malloc(sizeof(struct symbol));
                    if (new_symbol == NULL)
                    {
                      MEMORY_FAIL
                    }
                    memset(new_symbol, 0, sizeof(struct symbol));
                    strcpy(new_symbol->symbol_name, line_ast.label_name);
                    strcpy(new_symbol->symbol_name, line_ast.line_type_data.inst.label_array[FIRST_WORD]);
                    new_symbol->symbol_type = line_ast.line_type == command_line ? code_type : data_type;
                    new_symbol->symbol_type = (line_ast.line_type_data.inst.inst_type == entry) ? entry_type : extrn_type;
                    symbol_insert(&program->symbol_table, new_symbol);
                }
                else /* If defined a label as entry and extern in the same file  */
                {
                    printf("Error in %s line %d: %s is already defined \n", file_name, original_line_number, line_ast.line_type_data.inst.label_array[0]);
                    is_error = 1;
                }
            }
        }
        expanded_line_number++;
    }

    current = program->symbol_table;
    while (current != NULL)/*setting the data in the end lines to be printed last in the ob file*/
    {
        if (current->symbol_type == entry_type && !is_error)
        {
            printf("Error in %s: %s\n", file_name, "label already defined as entry and never received data");
            is_error = 1;
        }
        if (current->symbol_type == data_type || current->symbol_type == entry_data_type)
        {
            current->address += ic;
        }
        if (current->symbol_type == entry_code_type || current->symbol_type == entry_data_type)
        {
            add_entry(program, current->symbol_name, current->address);
        }
        current = current->next;
    }

    return is_error;
}
#endif