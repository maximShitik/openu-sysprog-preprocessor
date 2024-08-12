#ifndef LEXER_C
#define LEXER_C

#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "help_func.h"
#include "help_func.c"
#include "lexer_func.h"
#include "lexer_func.c"
#include "data_structs.h"
#include "data_struct_functions.h"
#include "macros.h"
extern void set_instruction(struct ast *ast, struct sep_line sep);
extern int set_argument_amount(struct ast *ast, struct sep_line sep);
extern int system_names(char *line);

int operand_group(struct ast *ast, char *command)
{
    int commandsCount;
    int i;
    opcode_map commands[] = OP_CODE_ARRAY;

    commandsCount = 16;

    for (i = 0; i < commandsCount; i++)
    {
        if (strcmp(command, commands[i].name) == 0)
        {
            if (i < ONE_OPCODES_COUNT)
                return two_group;
            else if (i < TWO_OPCODES_COUNT)
                return one_group;
            else
                return no_operands;
        }
    }
    error_found(ast, "undefinded command");
    return error;
}

struct ast process_command_operands(struct ast *ast, struct sep_line sep, char *command, int group_count)
{
    int current = 0;
    set_command_name(ast, command);

    while (strcmp(sep.line[current], command) != 0) /*finding the command*/
    {
        current++;
    }
    current++;
    /*chekinng if the next opernd is null*/
    if (sep.line[current] == NULL)
    {
        error_found(ast, "missing operand");
        return *ast;
    }

    /*processing the first operand*/
    if (system_names(sep.line[current]) == LABEL && !is_int(sep.line[current], ast))
    {
        set_label(ast, sep, current);
    }
    else if (system_names(sep.line[current]) == registerr)
    {
        if (group_count == 2)
        {
            SET_REGISTER(FIRST_WORD);
        }
        else if (group_count == 1)
        {
            SET_REGISTER(SECOND_WORD);
        }
        if (JMP_BNE_JSR && ast->line_type_data.command.opcode_type[SECOND_WORD].command_type == reg_direct)
        {
            error_found(ast, "invalid operand, command can't receive direct registers");
            return *ast;
        }
    }
    else if (is_int(sep.line[current], ast))
    {
        if (strchr(sep.line[current], '#') != sep.line[current])
        {
            error_found(ast, "invalid operand, command can't receive numbers withouth #");
            return *ast;
        }
        else if (group_count == 2 || strcmp(command, "prn") == 0)
        {
            set_data(ast, sep, 1);
        }
        else
        {
            error_found(ast, "invalid operand, only prn and cmp can receive numbers as target operand");
            return *ast;
        }
    }
    if (strcmp(command, "lea") == 0 && (system_names(sep.line[current]) != LABEL || is_int(sep.line[current], ast)))
    {
        error_found(ast, "invalid operand");
        return *ast;
    }

    if (group_count == 1)
    {
        if (sep.line[current + 1] != NULL)
        {
            error_found(ast, "too many operands");
        }
        return *ast;
    }

    /*process the second operand*/
    if (sep.line[++current] == NULL || strcmp(sep.line[current], ",") != 0)
    {
        error_found(ast, sep.line[current] == NULL ? "missing operand" : "error-missing comma");
        return *ast;
    }

    current++;

    if (sep.line[++current] != NULL)
    {
        error_found(ast, "too many operands");
        return *ast;
    }
    current--;

    if (is_int(sep.line[current], ast))
    {
        if (strcmp(command, "cmp") != 0)
        {
            error_found(ast, "invalid operand, command can't receive numbers");
            return *ast;
        }
        else if (strchr(sep.line[current], '#') != sep.line[current])
        {
            error_found(ast, "invalid operand, command can't receive numbers withouth #");
            return *ast;
        }
        else
        {

            set_data(ast, sep, 0);
        }
    }
    else if (system_names(sep.line[current]) == registerr)
    {
        SET_REGISTER(SECOND_WORD);
    }
    else if (system_names(sep.line[current]) == LABEL && !is_int(sep.line[current], ast))
    {
        set_label(ast, sep, current);
    }

    return *ast;
}

void no_operands_command(struct ast *ast, struct sep_line sep, char *command)
{
    int current;
    current = 0;
    while (strcmp(sep.line[current], command) != 0)
    {
        current++;
    }
    if (sep.line[current + 1] != NULL)
    {
        error_found(ast, "too many operands");
        return;
    }
    set_command_name(ast, command);
    return;
}

void set_command(struct ast *ast, struct sep_line sep, char *command)
{
    int result;
    result = operand_group(ast, command);
    if (result == 2 || result == 1)
    {
        process_command_operands(ast, sep, command, result);
    }

    else if (result == 0)
    {
        no_operands_command(ast, sep, command);
    }
}

struct ast line_type(struct sep_line sep, struct ast *ast)
{

    if (process_token(sep.line[FIRST_WORD], ast) == label) /*checking if we  have label in the line*/
    {

        if (process_token(sep.line[SECOND_WORD], ast) == instruction)
        {
            ast->line_type = inst_line;
            set_label(ast, sep, FIRST_WORD);
            set_instruction(ast, sep);
            return *ast;
        }
        else if (process_token(sep.line[SECOND_WORD], ast) == command)
        {
            ast->line_type = command_line;
            set_label(ast, sep, FIRST_WORD);
            set_command(ast, sep, sep.line[SECOND_WORD]);

            return *ast;
        }
    }

    /*if we dont have label*/
    else if (process_token(sep.line[FIRST_WORD], ast) == command)
    {
        ast->line_type = command_line;
        set_command(ast, sep, sep.line[FIRST_WORD]);
        return *ast;
    }
    else if (process_token(sep.line[FIRST_WORD], ast) == instruction)
    {
        ast->line_type = inst_line;
        set_instruction(ast, sep);
        return *ast;
    }
    else if (ast->line_type != error_line)
    {
        error_found(ast, "undefinded line type");
        return *ast;
    }
    return *ast;
}

struct ast parse_line(char *line, struct hash *hash_table[])
{
    struct ast ast;
    struct sep_line separated = split_line(line);
    /*checking if we dont have enough operands*/
    if (separated.line_number == 1)
    {
        trim_whitespace(separated.line[0]);
        if (strcmp(separated.line[0], "stop") != 0 && strcmp(separated.line[0], "rts") != 0)
        {
            error_found(&ast, "missing operand");
            return ast;
        }
    }
    reset_ast(&ast);
    ast.argument_count = set_argument_amount(&ast, separated); /*setting the amount of words in the line*/
    line_type(separated, &ast);
    if (ast.label_name[0] != '\0' && search_in_hash(ast.label_name, hash_table) != NULL)
    {
        error_found(&ast, "error-label is a macro name");
    }
    return ast;
}

#endif