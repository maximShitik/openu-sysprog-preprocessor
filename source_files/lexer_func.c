#ifndef LEXER_FUNC_C
#define LEXER_FUNC_C

#include "lexer_func.h"
#include "help_func.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int process_label(char *line, struct ast *ast)
{
    int result;
    char *line_copy;
    size_t len = strlen(line);

    line_copy = (char *)malloc(len);
    if (line_copy == NULL)
    {
        error_found(ast, "memory allocation error");
        return FALSE;
    }
    strncpy(line_copy, line, len - 1);
    line_copy[len - 1] = '\0';
    result = system_names(line_copy);
    free(line_copy);
    if (result == LABEL)
    {
        return LABEL;
    }
    else
    {
        error_found(ast, "error-invalid label name");
        return FALSE;
    }
}

int process_instruction(char *line, struct ast *ast)
{
    int result;
    char *line_copy;
    size_t len = strlen(line);

    line_copy = (char *)malloc(len + 1);
    if (line_copy == NULL)
    {
        error_found(ast, "memory allocation error");
        return FALSE;
    }
    strcpy(line_copy, line + 1);
    if (contains_uppercase(line_copy)) /*checking if there are any upper case letters*/
    {
        error_found(ast, "error-invalid instruction name");
        free(line_copy);
        return FALSE;
    }
    result = system_names(line_copy);

    if (result == instruction)
    {
        free(line_copy);
        return instruction;
    }
    else if (result == LABEL)
    {
        error_found(ast, "error-invalid instruction name");
        free(line_copy);
        return FALSE;
    }
    else
    {
        error_found(ast, "error-undefined instruction");
        free(line_copy);
        return FALSE;
    }
}

int process_token(char *line, struct ast *ast)
{
    size_t len = strlen(line);

    if (line[len - 1] == ':')
    {
        return process_label(line, ast);
    }

    if (line[0] == '.')
    {
        return process_instruction(line, ast);
    }

    if (system_names(line) == command)
    {
        return command;
    }

    else

        error_found(ast, "error-undefinded token");

    return 0;
}

void error_found(struct ast *ast, char *error_message)
{
    reset_ast(ast);
    ast->line_type = error_line;
    strcpy(ast->error.type, error_message);
}

void reset_ast(struct ast *node)
{
    int i;
    i = 0;
    node->line_type = empty_line;
    node->line_type_data.inst.inst_type = DEFAULT;

    reset_string_array(node->line_type_data.inst.label_array, LABEL_ARRAY_SIZE);
    reset_string_array(node->line_type_data.inst.string_array, STRING_ARRAY_SIZE);
    node->line_type_data.command.opcode = DEFAULT;
    memset(node->line_type_data.inst.data_array, DEFAULT,
           sizeof(node->line_type_data.inst.data_array));

    for (i = 0; i < 2; i++)
    {
        node->line_type_data.command.opcode_type[i].command_type = none;
        reset_string_array(node->line_type_data.command.opcode_type[i].labell, STRING_ARRAY_SIZE);
        node->line_type_data.command.opcode_type[i].numberr = DEFAULT;
        node->line_type_data.command.opcode_type[i].regg = DEFAULT;
    }
    memset(node->error.type, DEFAULT, sizeof(node->error.type));
    memset(node->label_name, DEFAULT, sizeof(node->label_name));
    node->line_type_data.inst.data_counter = DEFAULT;
    node->are.are_type = A;
    node->argument_count = DEFAULT;
}

int check_command_or_instruction(struct ast *ast)
{
    if (ast->line_type == inst_line)
    {
        return instruction;
    }
    if (ast->line_type == command_line)
    {
        return command;
    }
    return -1;
}

int is_int(char *line, struct ast *ast)
{
    char *endptr;

    if (*line == '+' || *line == '#')
    {
        line++;
        if (!strtol(line, &endptr, 10)) /*cheking if the next sign is a number*/
        {
            error_found(ast, "error-invalid number");
            return FALSE;
        }
    }

    if (strchr(line, '.') != NULL && strcmp(line, ".data") != 0)
    {
        error_found(ast, "error-invalid number");
        return FALSE;
    }

    return strtol(line, &endptr, 10); /*returning the result of the converting to a number*/
}

int is_string(char *line, struct ast *ast)
{
    if (line != NULL && strlen(line) > 0 && line[0] == '"')
    {
        size_t len = strlen(line);
        if (line[len - 1] == '"') /*checking if ther are more than 1 " */
        {
            return TRUE;
        }
        else
        {
            error_found(ast, "error-invalid string");
        }
    }
    return FALSE;
}

struct ast set_data(struct ast *ast, struct sep_line sep, int first_operand)
{
    int i, j;
    int int_num;
    int_num = 0;
    j = 0;
    for (i = 0; i < sep.line_number; i++)
    {
        int_num = is_int(sep.line[i], ast);
        if (int_num)
        {
            if ((i + 1) < sep.line_number && is_int(sep.line[i + 1], ast))
            {
                error_found(ast, "error-missing comma");
                return *ast;
            }

            if (check_command_or_instruction(ast) == instruction)
            {
                ast->line_type_data.inst.data_array[j++] = int_num;
                ast->line_type_data.inst.data_counter++;
                ast->line_type_data.inst.inst_type = data;
            }
            else if (check_command_or_instruction(ast) == command)
            {
                if (PRN_AND_ONE_OPERAND(==)) /*prn can only resive numbers in the second place*/
                {
                    SET_NUMBER(SECOND_WORD, SECOND_WORD, int_num)
                }
                else if (PRN_AND_ONE_OPERAND(!=))
                {
                    error_found(ast, "prn can resive only one number as target operand");
                    return *ast;
                }
                else
                {
                    if (first_operand) /*if its the first operand in the line*/
                    {
                        SET_NUMBER(j, j, int_num)
                        return *ast;
                    }

                    else /*if its the second */
                    {
                        if (sep.line[i + 1] == NULL) /*check if we are the last operand in a line with 2 numbers*/
                        {
                            if (j == 0) /*if the argument before us wasnt a number */
                            {
                                j++;
                                SET_NUMBER(j, j, int_num)
                            }

                            else /*if the argument before us was a number*/
                            {
                                SET_NUMBER(j, j, int_num)
                            }
                            return *ast;
                        }
                        SET_NUMBER(j, j, int_num)
                        j++;
                    }
                }
            }
        }

        /*checking for lexical errors*/
        else if (strcmp(sep.line[i], ",") == 0)
        {
            if ((i + 1) < sep.line_number && strcmp(sep.line[i + 1], ",") == 0)
            {
                error_found(ast, "too many commas");
                return *ast;
            }
            else
                continue;
        }
        else if ((i + 1) < sep.line_number && (!is_int(sep.line[i], ast) && strcmp(sep.line[i + 1], ",") == 0) && check_command_or_instruction(ast) == instruction)
        {
            error_found(ast, "comma before first number");
            return *ast;
        }
        else if (strcmp(sep.line[sep.line_number - 1], ",") == 0)
        {
            error_found(ast, "comma after last number");
            return *ast;
        }
    }
    return *ast;
}

struct ast set_string(struct ast *ast, struct sep_line sep)
{
    int i, j, k;
    char *string_start;
    size_t len;
    j = 0;
    i = 0;
    while (!is_string(sep.line[i], ast) && i < sep.line_number) /*searching for the string*/
    {
        i++;
    }
    if (sep.line[i] == NULL)
    {
        error_found(ast, "Invalid string");
        return *ast;
    }
    if (i < sep.line_number && is_string(sep.line[i], ast)) /*setting the string*/
    {

        string_start = sep.line[i] + 1;
        len = strlen(string_start);
        for (k = 0; k < len - 1; k++)
        {
            ast->line_type_data.inst.string_array[j] = (char *)malloc(2 * sizeof(char));
            if (ast->line_type_data.inst.string_array[j] == NULL)
            {
                error_found(ast, "memory allocation error");
                return *ast;
            }
            ast->line_type_data.inst.string_array[j][FIRST_WORD] = string_start[k];
            ast->line_type_data.inst.string_array[j][SECOND_WORD] = '\0';
            j++;
        }
    }
    if (i + 1 < sep.line_number && is_string(sep.line[i + 1], ast))
    {
        error_found(ast, "error-too many strings");
        free(ast->line_type_data.inst.string_array[j]);
        return *ast;
    }
    ast->line_type_data.inst.data_counter = j;
    ast->line_type_data.inst.inst_type = string;

    return *ast;
}

void set_instruction(struct ast *ast, struct sep_line sep)
{
    if (CMPR_LINE_INST(FIRST_WORD, SECOND_WORD, ".entry"))
    {
        set_entry_extern(ast, sep);
    }
    else if (CMPR_LINE_INST(FIRST_WORD, SECOND_WORD, ".extern"))
    {

        set_entry_extern(ast, sep);
    }
    else if (CMPR_LINE_INST(FIRST_WORD, SECOND_WORD, ".data"))
    {
        set_data(ast, sep, 1);
    }
    else if (CMPR_LINE_INST(FIRST_WORD, SECOND_WORD, ".string"))
    {
        set_string(ast, sep);
    }

    else
    {
        error_found(ast, "error-undefined instruction");
    }
}

struct ast set_entry_extern(struct ast *ast, struct sep_line sep)
{

    if (sep.line_number < 2)
    {
        error_found(ast, "missing label name");
        return *ast;
    }
    /*checking if the extern/entry has a label on it*/
    if (sep.line_number >= 2 && ast->label_name[0] != '\0')
    {
        printf("Warning - entry/extern will not use the label name\n");
        if (sep.line[THIRD_WORD] != NULL && system_names(sep.line[THIRD_WORD]) != LABEL)
        {
            error_found(ast, "invalid label name");
            return *ast;
        }
        if (sep.line[THIRD_WORD] == NULL)
        {
            error_found(ast, "missing label name");
            return *ast;
        }
        reset_ast(ast);
        ast->line_type = inst_line;
        if (strcmp(sep.line[SECOND_WORD], ".entry") == 0 || strcmp(sep.line[SECOND_WORD], "entry") == 0)
        {

            ast->line_type_data.inst.inst_type = entry, ast->are.are_type = R;
        }
        else
        {
            ast->line_type_data.inst.inst_type = extrn, ast->are.are_type = E;
        }

        set_label(ast, sep, THIRD_WORD);
        ast->label_name[0] = '\0'; /*setting only the first letter to \0 so we will not use the name*/
        return *ast;
    }

    /*handling case if we dont have label*/
    if (strcmp(sep.line[0], ".entry") == 0 || strcmp(sep.line[0], "entry") == 0)
    {

        ast->line_type_data.inst.inst_type = entry, ast->are.are_type = R;
    }
    else
    {
        ast->line_type_data.inst.inst_type = extrn, ast->are.are_type = E;
    }
    set_label(ast, sep, 1);

    return *ast;
}

struct ast set_label(struct ast *ast, struct sep_line sep, int index)
{
    size_t len = strlen(sep.line[index]);
    if (ast->line_type != command_line)
    {
        if ((is_int(sep.line[index], ast) || is_string(sep.line[index], ast)))
        {
            error_found(ast, "invalid label name");
            return *ast;
        }
    }
    /* Handle case where line ends with ':'*/
    if (len > 0 && sep.line[index][len - 1] == ':')
    {
        len--;
        strncpy(ast->label_name, sep.line[index], len);
        return *ast;
    }
    if (ast->line_type == inst_line)
    {
        if (ast->line_type_data.inst.label_array[FIRST_WORD] == NULL) /*chking if we are the first operand or the second*/
        {

            ast->line_type_data.inst.label_array[FIRST_WORD] = sep.line[index];
            ast->line_type_data.inst.label_array[SECOND_WORD] = NULL;
        }

        else
        {
            ast->line_type_data.inst.label_array[SECOND_WORD] = sep.line[index];
        }
    }

    else if (ast->line_type == command_line)
    {
            /*chking if we are the first operand or the second
            if we in the command group of 2 operdands the label will be set in the second array*/
        if (ast->line_type_data.command.opcode_type[FIRST_WORD].command_type != none || ast->line_type_data.command.opcode > lea) 
        {

            ast->line_type_data.command.opcode_type[SECOND_WORD].command_type = lable;
            ast->line_type_data.command.opcode_type[SECOND_WORD].labell[FIRST_WORD] = sep.line[index];
        }
        else
        {
            ast->line_type_data.command.opcode_type[FIRST_WORD].command_type = lable;
            ast->line_type_data.command.opcode_type[FIRST_WORD].labell[FIRST_WORD] = sep.line[index];
        }
    }

    else
    {
        error_found(ast, "undefined line type");
        return *ast;
    }

    return *ast;
}

void set_command_name(struct ast *ast, char *command)
{
    int i;
    int num_commands;
    opcode_map command_map[] = OP_CODE_ARRAY;
    num_commands = sizeof(command_map) / sizeof(command_map[0]);
    for (i = 0; i < num_commands; i++)
    {
        if (strcmp(command, command_map[i].name) == 0)
        {
            ast->line_type_data.command.opcode = command_map[i].opcode;
            return;
        }
    }

    ast->line_type_data.command.opcode = 0;
}

int set_argument_amount(struct ast *ast, struct sep_line sep)
{
    int i;
    int count = 0;
    for (i = 0; i < sep.line_number; i++)
    {
        if (strcmp(sep.line[i], ",") == 0)
        {
            continue;
        }
        count++;
    }
    return count;
}

#endif