#ifndef SECOND_PASS_C
#define SECOND_PASS_C
#include "data_structs.h"
#include "lexer.h"
#include "first_pass.h"
#include "second_pass.h"
#include <stdlib.h>
#include <string.h>
#include "macros.h"

void add_address(struct translation_unit *program, struct ext *ext_found, int address)
{
    struct address *new_address;
    struct address *current_address;

    new_address = (struct address *)malloc(sizeof(struct address));
    if (new_address == NULL)
    {
        MEMORY_FAIL;
    }

    new_address->address = address;
    new_address->next_address = NULL;
    new_address->prev_address = NULL;

    if (ext_found->address_head == NULL)
    {
        ext_found->address_head = new_address;
    }
    else
    {
        current_address = ext_found->address_head;

        while (current_address->next_address != NULL)
        {
            current_address = current_address->next_address;
        }
        current_address->next_address = new_address;
        new_address->prev_address = current_address;
    }
    ext_found->address_counter++;
}

void add_extr(struct translation_unit *program, char *symbol_name, int address)
{
    struct ext *curr_ext;
    struct ext *new_ext;
    new_ext = (struct ext *)malloc(sizeof(struct ext));
    if (new_ext == NULL)
    {
        MEMORY_FAIL;
    }
    new_ext->address_head = (struct address *)malloc(sizeof(struct address));
    if (new_ext->address_head == NULL)
    {
        MEMORY_FAIL;
    }

    strcpy(new_ext->ext_name, symbol_name);
    new_ext->address_head->address = address;
    new_ext->next = NULL;
    new_ext->prev = NULL;
    if (program->ext_table == NULL)
    {
        program->ext_table = new_ext;
    }
    else
    {
        curr_ext = program->ext_table;
        while (curr_ext->next != NULL)
        {
            curr_ext = curr_ext->next;
        }
        curr_ext->next = new_ext;
        new_ext->prev = curr_ext;
    }

    program->exter_count++;
}

void set_first_word(struct ast line_ast, struct translation_unit *program)
{
    OP_CODE_SET

    if (line_ast.line_type_data.command.opcode_type[FIRST_WORD].command_type == number)
    {
        SOURCE_METHOD_0
    }
    else if (line_ast.line_type_data.command.opcode_type[FIRST_WORD].command_type == label)
    {
        SOURCE_METHOD_1
    }
    else if (line_ast.line_type_data.command.opcode_type[FIRST_WORD].command_type == reg)
    {
        SOURCE_METHOD_2
    }
    else if (line_ast.line_type_data.command.opcode_type[FIRST_WORD].command_type == reg_direct)
    {
        SOURCE_METHOD_3
    }

    if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type != none)

    {
        if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type == number)
        {
            TARGET_METHOD_0
        }
        else if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type == label)
        {
            TARGET_METHOD_1
        }
        else if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type == reg)
        {
            TARGET_METHOD_2
        }
        else if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type == reg_direct)
        {
            TARGET_METHOD_3
        }
    }

    ARE

        program->IC++;
}

struct ext *ext_search(struct ext *head, char *name)
{
    struct ext *current = head;
    while (current != NULL)
    {
        if (strcmp(current->ext_name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[])
{
    char line[MAX_LINE];
    int is_error;
    int line_number;
    struct ast line_ast = {0};
    struct symbol *symbol_found;
    struct ext *ext_found;
    struct ext *new_ext;
    int i;
    char char_command[MAX_LINE];
    line_number = 0;
    is_error = 0;

    while (fgets(line, sizeof(line), am_file))
    {
        line_ast = parse_line(line, hash_table); /*parsing the line to an AST*/

        if (line_ast.line_type == command)
        {
            set_first_word(line_ast, program);

            if (SOURCE_AND_TARGET_ARE_REGISTERS)
            {
                i = 0;
                SET_SOURCE.regg << BITS_6_8;
                SET_TARGET.regg << BITS_3_5;
                ARE
                    program->IC++;
            }
            else
            {

                for (i = 0; i < 2; i++)
                {
                    if (COMMAND_TYPE == number)
                    {
                        SET_ADRESS.numberr << 3;
                        ARE
                    }
                    else if (COMMAND_TYPE == reg || COMMAND_TYPE == reg_direct)
                    {
                        if (i == 0)
                        {
                            SET_SOURCE.regg << BITS_6_8;
                            ARE
                        }
                        else
                        {
                            SET_TARGET.regg << BITS_3_5;
                            ARE
                        }
                    }
                    else if (COMMAND_TYPE == label)
                    {
                        symbol_found = symbol_search(program->symbol_table, line_ast.line_type_data.command.opcode_type[i].labell[0]);
                        if (symbol_found)
                        {
                            program->code_array[program->IC] = symbol_found->address << 3;
                            if (symbol_found->symbol_type == extrn_type)
                            {
                                program->code_array[program->IC] |= 1; /*adding the E*/
                                ext_found = ext_search(program->ext_table, line_ast.line_type_data.command.opcode_type[i].labell[0]);
                                if (ext_found)
                                {

                                    add_address(program, ext_found, program->IC + 100);
                                }
                                else
                                {

                                    add_extr(program, line_ast.line_type_data.command.opcode_type[i].labell[0], program->IC + 100);
                                }
                            }
                            else
                            {
                                program->code_array[program->IC] |= 2;
                            }
                        }
                        else
                        {
                            printf("Error in %s line %d: %s is undefined label \n", file_name, line_number, line_ast.line_type_data.command.opcode_type[i].labell[0]);

                            is_error = 1;
                        }
                    }
                    if (COMMAND_TYPE != none)
                    {
                        program->IC++;
                    }
                }
            }
        }
        line_number++;
    }

    if (is_error)
    {
        free_translation_unit(program);
    }
    return is_error;
}

#endif
