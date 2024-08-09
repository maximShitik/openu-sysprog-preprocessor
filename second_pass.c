#ifndef SECOND_PASS_C
#define SECOND_PASS_C
#include "data_structs.h"
#include "lexer.h"
#include "first_and_second_pass.h"
#include "help_func.h"
#include "macros.h"

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

int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], line_mapping line_map[], int expanded_line_count)
{
    char line[MAX_LINE];
    int is_error;
    struct ast line_ast = {0};
    struct ext *ext_found;
    int i;
    int expanded_line_number;
    int original_line_number;
    is_error = 0;
    expanded_line_number = 0;

        while (fgets(line, sizeof(line), am_file))
    {
        original_line_number = line_map[expanded_line_number].original_line_number;

        line_ast = parse_line(line, hash_table); /*parsing the line to an AST*/

        if (line_ast.line_type_data.inst.inst_type == string) /*free memory if we have a string */
        {
            free_string_array(line_ast.line_type_data.inst.string_array, line_ast.line_type_data.inst.data_counter);
        }
        if (line_ast.line_type == command_line)
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
                        struct symbol *symbol_found = symbol_search(program->symbol_table, line_ast.line_type_data.command.opcode_type[i].labell[0]);
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
                            printf("Error in line %d: %s is undefined label \n", original_line_number, line_ast.line_type_data.command.opcode_type[i].labell[0]);
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
        expanded_line_number++;
    }

    return is_error;
}

#endif
