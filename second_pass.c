#include "first_pass.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include "second_pass.h"

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

    if (line_ast.line_type_data.command.opcode_type[SECOND_WORD].command_type == none)
        return;
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
            }
            else
            {
                
                for(i = 0; i < 2; i++)
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
                        }
                        else
                        {
                            SET_TARGET.regg << BITS_3_5;
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
                                    ext_found->address = program->IC + 100;
                                    new_ext->address_counter++;
                                }
                                else
                                {
                                    new_ext = (struct ext *)malloc(sizeof(struct ext));
                                    strcpy(new_ext->ext_name, line_ast.line_type_data.command.opcode_type[i].labell[0]);
                                    new_ext->address = program->IC + 100;
                                    new_ext->exter_count++;
                                    new_ext->next = program->ext_table;
                                    new_ext->address_counter++;
                                    program->ext_table = new_ext;
                                }
                            }
                            else
                            {
                                program->code_array[program->IC] |= 2;
                            }
                        }
                        else
                        {
                            printf("Error in %s line %d: %s is not defined label \n", file_name, line_number, line_ast.line_type_data.command.opcode_type[i].labell[0]);
                              is_error = 1;
                        }
                    }
                    if(COMMAND_TYPE!=none)
                    {
                         program->IC++;
                    }
                }
            }
        }
        line_number++;
    }
    return 0;
}
