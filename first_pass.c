#ifndef FIRST_PASS_C
#define FIRST_PASS_C

#include "first_pass.h"
#include <stdio.h>
#include <string.h>
#include "lexer.h"

struct symbol *symbol_search(struct symbol *sym_table, int size, char *name)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (strcmp(sym_table->symbol_name, name) == 0)
        {
            return &sym_table[i];
        }
    }
    return NULL;
}

int first_pass(char *file_name, FILE *am_file, struct translation_unit *program,struct hash *hash_table[])
{
    struct ast line_ast = {0};
    int i;
    int is_error;
    char line[MAX_LINE];
    int line_number;
    char command[MAX_LINE];
    struct symbol *symbol_found;
    size_t len;
    line_number = 1;
    int ic, dc;
    ic = 100, dc = 0;
    is_error = 0;
    i = 0;

    while (fgets(line, sizeof(line), am_file))
    {
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\r')
        { /*skipping the eampty lines and note lines*/
            continue;
        }
        line_ast = parse_line(line,hash_table); /*parsing the line to an AST*/
        if (line_ast.line_type == error_line)
        {
            printf("Syntex error in line %d: %s\n", line_number, line_ast.error.type);
            is_error = 1; /*indicating there is an error in the syntex*/
            line_number++;
            continue;
        }

        /*prosessing the label and checking if we have redefenition*/
        if ((line_ast.label_name[0] != '\0') && (line_ast.line_type == inst_line || line_ast.line_type == command_line))
        {
            /*cheking if the name is in the label table*/
            symbol_found = symbol_search(program->symbol_table, program->symbol_count, line_ast.label_name);
            if (symbol_found)
            {
                if (symbol_found->symbol_type == entry_type)
                {
                    program->symbol_table[program->symbol_count].symbol_type = line_ast.line_type == command_line ? code_type : data_type;
                    program->symbol_table[program->symbol_count].address = line_ast.line_type == command_line ? ic : dc;
                }
                else /*if the name*/
                {

                    printf("Error in line %d: %s\n", line_number, "label already defined");
                    is_error = 1;
                }
            }
            else
            {
                strcpy(program->symbol_table[program->symbol_count].symbol_name, line_ast.label_name);
                program->symbol_table[program->symbol_count].address = line_ast.line_type == command_line ? ic : dc;
                program->symbol_table[program->symbol_count].symbol_type = line_ast.line_type == command_line ? code_type : data_type;
                program->symbol_count++;
            }
        }

        if (line_ast.line_type == command_line)
        {
            ic++;
            if (line_ast.line_type_data.command.opcode_type[0].command_type == reg && line_ast.line_type_data.command.opcode_type[1].command_type == reg)
                ic++;
            else
            {
                ic = ic + 2;
            }
        }

        else if (line_ast.line_type == inst_line)
        {

            if (line_ast.line_type_data.inst.inst_type == data)
            {
                program->DC = dc;

                memcpy(&program->data_array[program->DC], &line_ast.line_type_data.inst.data_array, sizeof(int) * line_ast.line_type_data.inst.data_counter);

                dc += line_ast.line_type_data.inst.data_counter;
                program->DC = dc;
            }
            else if (line_ast.line_type_data.inst.inst_type == string)
            {
                program->DC = dc;

                memcpy(&program->data_array[program->DC], line_ast.line_type_data.inst.string_array, sizeof(int) * line_ast.line_type_data.inst.data_counter);

                dc += line_ast.line_type_data.inst.data_counter;
                program->DC = dc;
            }

            if (line_ast.line_type_data.inst.inst_type == entry || line_ast.line_type_data.inst.inst_type == extrn)
            {
                symbol_found = symbol_search(program->symbol_table, program->symbol_count, line_ast.line_type_data.inst.label_array[1]);
                if (symbol_found && symbol_found->symbol_type == entry_type)
                {
                    if (symbol_found->symbol_type == code_type)
                    {
                        symbol_found->symbol_type = entry_code_type;
                    }
                    else if (symbol_found->symbol_type == data_type)
                    {
                        symbol_found->symbol_type = entry_data_type;
                    }

                    else if (!symbol_found)
                    {
                        strcpy(program->symbol_table[program->symbol_count].symbol_name, line_ast.line_type_data.inst.label_array[0]);
                        program->symbol_table[program->symbol_count].symbol_type = line_ast.line_type_data.inst.inst_type == entry ? entry_type : extrn_type;
                        program->symbol_count++;
                    }

                    else
                    {
                        printf("Error in line %d: %s\n", line_number, "label already defined");
                        is_error = 1;
                    }
                }
            }
        }
        line_number++;
    }

    for (i = 0; i < program->symbol_count; i++)
    {
        if (program->symbol_table[i].symbol_type == entry_type)
        {
            printf("Error in line %d: %s\n", line_number, "label already defined");
            is_error = 1;
        }
        if (program->symbol_table[i].symbol_type == data_type || program->symbol_table[i].symbol_type == entry_data_type)
        {
            program->symbol_table[i].address += ic;
        }
        if (program->symbol_table[i].symbol_type == entry_code_type)
        {
            program->enrty[program->enrty_count] = &program->symbol_table[i];
            program->enrty_count++;
        }
    }
    return is_error;
}

#endif
