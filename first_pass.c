/*
This file is taking a line from the input file and parsing it to an AST node.
The AST node is then returned.
The file prosses the line and checks :
1) What is the line type (instruction, command, label, error line)
2) If the line is a label, it sets the label name and the next line type (instruction or command).
3) If the line is a command, it sets the command name and the operands by checking the command group by number of operands.
4) If the line is an instruction, it sets the instruction name and the operands.
5) If the line is an error line, it sets the error message and returns an eampty AST node with the error messege only.
*/

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

/**
 * @brief cheking to witch group of operand the command belongs
 *
 * @param ast
 * @param command
 * @return int
 */
int operand_group(struct ast *ast, char *command)
{
    if (strcmp(command, "mov") == 0 || strcmp(command, "cmp") == 0 ||
        strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 ||
        strcmp(command, "lea") == 0)
    {
        return 2;
    }
    if (strcmp(command, "clr") == 0 || strcmp(command, "not") == 0 ||
        strcmp(command, "inc") == 0 || strcmp(command, "dec") == 0 ||
        strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 ||
        strcmp(command, "red") == 0 || strcmp(command, "prn") == 0 ||
        strcmp(command, "jsr") == 0)
        return 1;
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0)
        return 0;
    else
        error_found(ast, "error-undefinded command");
    return -1;
}

/**
 * @brief
 *
 * @param ast
 * @param sep
 * @param command
 * @param group
 * @return struct ast
 */
struct ast two_group_command(struct ast *ast, struct sep_line sep,
                             char *command, int group)
{

    int current;
    current = 0;

    if (strcmp(sep.line[1], ",") == 0)
    {
        error_found(ast, "error-extra comma");
        return *ast;
    }

    while (strcmp(sep.line[current], command) != 0)
    {
        current++;
    }
    if (sep.line[++current] == NULL)
    {
        error_found(ast, "error-missing operand");
        return *ast;
    }

    if (strcmp(command, "lea") == 0)
        if (system_names(sep.line[current]) != TRUE || is_int(sep.line[current], ast))
        {
            error_found(ast, "error-invalid operand");
            return *ast;
        }
    if (system_names(sep.line[current]) == TRUE)
        set_label(ast, sep, current);
    if (is_int(sep.line[current], ast))
    {
        set_data(ast, sep);
    }
    if (system_names(sep.line[current]) == registerr)
    {
        ast->line_type_data.command.opcode_type[0].command_type = reg;
        if (strpbrk(sep.line[current], "*") == NULL)
        {
            ast->line_type_data.command.opcode_type[0].regg =
                atoi(sep.line[current] + 1);
        }
        else
            ast->line_type_data.command.opcode_type[0].regg =
                atoi(sep.line[current] + 2);
    }

    if (sep.line[++current] == NULL)
    {
        error_found(ast, "error-missing operand");
        return *ast;
    }
    if (strcmp(sep.line[current], ",") != 0)
    {
        error_found(ast, "error-missing comma");
        return *ast;
    }
    current++;
    if (sep.line[current] == NULL)
    {
        error_found(ast, "error-missing operand");
        return *ast;
    }

    if (sep.line[++current] != NULL)
    {
        error_found(ast, "error-too many operands");
        return *ast;
    }
    current--;

    if (is_int(sep.line[current], ast))
    {
        set_data(ast, sep);
        if (strcmp(command, "cmp") != 0)
        {
            error_found(ast, "error-invalid operand,command cant receive numbers");
            return *ast;
        }
    }
    if (system_names(sep.line[current]) == registerr)
    {
        ast->line_type_data.command.opcode_type[1].command_type = reg;
        if (strpbrk(sep.line[current], "*") == NULL)
        {
            ast->line_type_data.command.opcode_type[1].regg =
                atoi(sep.line[current] + 1);
        }
        else
            ast->line_type_data.command.opcode_type[1].regg =
                atoi(sep.line[current] + 2);
    }
    else if (system_names(sep.line[current]) == TRUE && !is_int(sep.line[current], ast))
    {
        set_label(ast, sep, current);
    }
    set_command_name(ast, command);
    return *ast;
}

/**
 * @brief operating the one group command
 *
 * @param ast
 * @param sep
 * @param command
 * @return struct ast
 */
struct ast one_group_command(struct ast *ast, struct sep_line sep,
                             char *command)
{
    int i;
    int current;
    current = 0;

    i = 0;

    while (strcmp(sep.line[current], command) != 0)
    {
        current++;
    }
    current++;
    if (system_names(sep.line[current]) == TRUE && !is_int(sep.line[current], ast))
    {
        set_label(ast, sep, current);
    }
    if (system_names(sep.line[current]) == registerr)
    {
        if (strpbrk(sep.line[current], "*") &&
            (strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0))
        {
            error_found(ast, "error-invalid operand,register must be indirect");
            return *ast;
        }
        else
        {
            ast->line_type_data.command.opcode_type[i].command_type = reg;
            if (strpbrk(sep.line[current], "*") == NULL)
            {
                ast->line_type_data.command.opcode_type[i++].regg =
                    atoi(sep.line[current] + 1);
            }
            else
                ast->line_type_data.command.opcode_type[i].regg =
                    atoi(sep.line[current] + 2);
        }
    }
    if (strcmp(command, "jmp") != 0 || strcmp(command, "bne") != 0)
    {
        if (is_int(sep.line[current], ast))
        {
            if (strcmp(command, "prn") == 0)
            {
                set_data(ast, sep);
            }
            else
            {
                error_found(ast, "error-invalid operand,only prn can receive numbers");
                return *ast;
            }
        }

        if (sep.line[current + 1] != NULL)
        {
            error_found(ast, "error-too many operands");
            return *ast;
        }
        if (strcmp(command, "not") == 0)
            ast->line_type_data.command.opcode = nt;
        else
            set_command_name(ast, command);
        return *ast;
    }
    else
        error_found(ast, "error-invalid operand");
    return *ast;
}
void no_operands_command(struct ast *ast, struct sep_line sep, char *command)
{
    int current;
    while (strcmp(sep.line[current], command) != 0)
    {
        current++;
    }
    if (sep.line[current + 1] != NULL)
    {
        error_found(ast, "error-too many operands");
        return;
    }
    set_command_name(ast, command);
    return;
}

/**
 * @brief Set the command object
 *
 * @param ast
 * @param sep
 * @param command
 */
void set_command(struct ast *ast, struct sep_line sep, char *command)
{
    int result;
    result = operand_group(ast, command);
    if (result == 2)
    {
        two_group_command(ast, sep, command, result);
    }
    else if (result == 1)
    {
        one_group_command(ast, sep, command);
    }
    else if (result == 0)
    {
        no_operands_command(ast, sep, command);
    }
}

/**
 * @brief Check the line type by the inserted line
 *
 * @param sep
 * @param ast
 * @return struct ast
 */
struct ast line_type(struct sep_line sep, struct ast *ast)
{

    if (process_token(sep.line[0], ast) == instruction)
    {
        ast->line_type = inst_line;
        set_instruction(ast, sep);
        return *ast;
    }
    if (process_token(sep.line[0], ast) == lable)
    {

        if (process_token(sep.line[1], ast) == instruction)
        {
            ast->line_type = inst_line;
            set_label(ast, sep, 0);
            set_instruction(ast, sep);
            return *ast;
        }
        else if (process_token(sep.line[1], ast) == command)
        {
            ast->line_type = command_line;
            set_label(ast, sep, 0);
            set_command(ast, sep, sep.line[1]);

            return *ast;
        }
    }
    else if (process_token(sep.line[0], ast) == command)
    {
        ast->line_type = command_line;
        set_command(ast, sep, sep.line[0]);
        return *ast;
    }
    else if (ast->line_type != error_line)
    {
        error_found(ast, "error-undefinded line type");
        return *ast;
    }
    return *ast;
}

/**
 * @brief parese the line and return the AST node
 *
 * @param line
 * @return struct ast
 */
struct ast parse_line(char *line)
{
    struct ast ast;
    struct sep_line separated = next_word(line);
    /*checking if we dont have enough operands*/
    if (separated.line_number == 1 && (strcmp(separated.line[0], "stop") != 0 || strcmp(separated.line[0], "rts") != 0))
    {
        error_found(&ast, "missing operand");
        return ast;
    }

    reset_ast(&ast);
    line_type(separated, &ast);
    return ast;
}

#endif
