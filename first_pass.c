#ifndef FIRST_PASS_C
#define FIRST_PASS_C

#include "first_pass.h"
#include "data_structs.h"
#include "lexer.h"
#include "help_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int first_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], int original_line_numbers[], int expanded_line_count)
{
    struct symbol *current;
    int i;
    int is_error;
    char line[MAX_LINE];
    int expanded_line_number;
    struct symbol *symbol_found;
    struct symbol *new_symbol;
    int ic, dc;
    struct ast line_ast = {0};
    ic = 100, dc = 0;
    is_error = 0;
    i = 0;
    expanded_line_number = 0;

    while (fgets(line, sizeof(line), am_file))
    {
        line_ast = parse_line(line, hash_table); /*parsing the line to an AST*/
        if (line_ast.line_type == error_line)
        {
            if (expanded_line_number < expanded_line_count)
            {
                printf("Syntax error in line %d: %s\n", original_line_numbers[expanded_line_number], line_ast.error.type);
            }
            else
            {
                printf("Syntax error in line: %s\n", line_ast.error.type);
            }
            is_error = 1; /*indicating there is an error in the syntax*/
            expanded_line_number++;
            continue;
        }

        /*processing the label and checking if we have redefinition*/
        if ((line_ast.label_name[0] != '\0') && (line_ast.line_type == inst_line || line_ast.line_type == command_line))
        {
            symbol_found = symbol_search(program->symbol_table, line_ast.label_name);
            /*checking if the name is in the label table*/
            if (symbol_found)
            {
                if (symbol_found->symbol_type == entry_type)
                {
                    symbol_found->symbol_type = line_ast.line_type == command_line ? entry_code_type : entry_data_type;
                    symbol_found->address = line_ast.line_type == command_line ? ic : dc;
                }
                else /*if the name is not entry and we already have it in the table */
                {
                    if (expanded_line_number < expanded_line_count)
                    {
                        printf("Error in %s line %d: %s is already defined \n", file_name, original_line_numbers[expanded_line_number], line_ast.label_name);
                    }
                    else
                    {
                        printf("Error in %s: %s is already defined \n", file_name, line_ast.label_name);
                    }
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

        if (line_ast.line_type == command_line) /*updating the ic */
        {
            ic++;

            if (SOURCE_AND_TARGET_ARE_REGISTERS)
                ic++;
            else
            {
                /*counting the number of the arguments for the ic without the label name and command*/
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

        else if (line_ast.line_type == inst_line) /*coding the instructions to the translation unit */
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
            else if (line_ast.line_type_data.inst.inst_type == entry || line_ast.line_type_data.inst.inst_type == extrn)
            {
                symbol_found = symbol_search(program->symbol_table, line_ast.line_type_data.inst.label_array[0]);
                if (symbol_found && line_ast.line_type_data.inst.inst_type == entry)
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
                        if (expanded_line_number < expanded_line_count)
                        {
                            printf("Error in %s line %d: %s is already defined \n", file_name, original_line_numbers[expanded_line_number], line_ast.line_type_data.inst.label_array[0]);
                        }
                        else
                        {
                            printf("Error in %s: %s is already defined \n", file_name, line_ast.line_type_data.inst.label_array[0]);
                        }
                        is_error = 1;
                    }
                }
                else if (!symbol_found)
                {
                    struct symbol *new_symbol = (struct symbol *)malloc(sizeof(struct symbol));
                    strcpy(new_symbol->symbol_name, line_ast.line_type_data.inst.label_array[0]);
                    new_symbol->symbol_type = (line_ast.line_type_data.inst.inst_type == entry) ? entry_type : extrn_type;
                    symbol_insert(&program->symbol_table, new_symbol);
                }
                else /*if we don't have the symbol in the table and it's not an entry it's a redefinition*/
                {
                    if (expanded_line_number < expanded_line_count)
                    {
                        printf("Error in %s line %d: %s is already defined \n", file_name, original_line_numbers[expanded_line_number], line_ast.line_type_data.inst.label_array[0]);
                    }
                    else
                    {
                        printf("Error in %s: %s is already defined \n", file_name, line_ast.line_type_data.inst.label_array[0]);
                    }
                    is_error = 1;
                }
            }
        }
        expanded_line_number++;
    }

    current = program->symbol_table;
    while (current != NULL)
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
