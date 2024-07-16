
/*
The functions in this file are used to help the lexer do its job and prosess the line in to an AST.
There are 2 types of functions here :
1)the global functions that are used to check if the line is an int , string and process it.
2)the functions that are used to set the data in the AST.
*/


#ifndef LEXER_FUNC_C
#define LEXER_FUNC_C
#include "lexer_func.h"
#include "help_func.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>




/**
 * @brief
 *
 * @param line
 * @param ast
 * @return int false if the token is invalid,

 */
int process_token(char *line, struct ast *ast)
{
    int result;
    char *line_copy;
    size_t len = strlen(line);

    if (line[len - 1] == ':')
    {
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
        if (result == TRUE)
        {

            return TRUE;
        }
        else
        {
            error_found(ast, "error-invalid label name");

            return FALSE;
        }
    }

    if (line[0] == '.')
    {
        line_copy = (char *)malloc(len + 1);
        if (line_copy == NULL)
        {
            error_found(ast, "memory allocation error");
            return FALSE;
        }
        strcpy(line_copy, line + 1);
        if (contains_uppercase(line_copy))
        {
            error_found(ast, "error-invalid instruction name");
            free(line_copy);
            return FALSE;
        }
        result = system_names(line_copy);

        if (result == instruction)
        {
            free(line_copy);
            return instruction; /*valid instruction*/
        }
        else if (result == TRUE)
        {
            error_found(ast, "error-invalid instruction name");
            free(line_copy);
            return FALSE;
        }
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
    ast->error.line_number = 1;
    strcpy(ast->error.type, error_message);
}


/**
 * Resets an AST (Abstract Syntax Tree) node to its default state.
 */
void reset_ast(struct ast *node)
{
    int i;
    i = 0;
    node->line_type = empty_line;
    node->line_type_data.inst.inst_type = 0;
    reset_string_array(node->line_type_data.inst.label_array, 2);
    reset_string_array(node->line_type_data.inst.string_array, 50);
    node->line_type_data.command.opcode = 0;
    memset(node->line_type_data.inst.data_array, 0,
           sizeof(node->line_type_data.inst.data_array));

    for (i = 0; i < 2; i++)
    {
        node->line_type_data.command.opcode_type[i].command_type = none;
        reset_string_array(node->line_type_data.command.opcode_type[i].labell, 50);
        node->line_type_data.command.opcode_type[i].numberr = 0;
        node->line_type_data.command.opcode_type[i].regg = 0;
    }

    memset(node->error.type, 0, sizeof(node->error.type));
    node->error.line_number = 0;
    node->ARE.ARE_type = A;
}

/**
 * @brief Checks if the line is a command or an instruction.
 * 
 * @param ast 
 * @return int 
 */
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


/**
 * Checks if a string can be converted to an integer.
 * Returns 1 if it can, 0 otherwise.
 */
int is_int(char *line, struct ast *ast)
{
    char *endptr;
    if (*line == '+' || *line == '-' || *line == '#')
    {
        line++;
    }

    if (strchr(line, '.') != NULL && strcmp(line, ".data") != 0)
    {
        error_found(ast, "error-invalid number");
        return FALSE;
    }

    strtol(line, &endptr, 10);

    return *endptr == '\0';
}


/**
 * @brief Checks if a string is a valid string.
 * 
 * @param line 
 * @param ast 
 * @return int 
 */
int is_string(char *line, struct ast *ast)
{
    if (line[0] == '"' && line[strlen(line) - 1] == '"')
        return TRUE;
    else
    {
        error_found(ast, "error-invalid string");
        return FALSE;
    }
}



/**************************************************************SETTERS******************************************/
/**
 * @brief Set the data object
 * 
 * @param ast 
 * @param sep 
 * @return struct ast 
 */
struct ast set_data(struct ast *ast, struct sep_line sep)
{
    int i, j;

    j = 0;
    for (i = 0; i < sep.line_number; i++)
    {

        if (is_int(sep.line[i], ast))
        {

            if ((i + 1) < sep.line_number && is_int(sep.line[i + 1], ast))
            {
                error_found(ast, "error-missing comma");
                return *ast;
            }

            char *num_str = sep.line[i];
            if (num_str[0] == '#')
            {
                num_str++;
            }

            if (check_command_or_instruction(ast) == instruction)
            {
                ast->line_type_data.inst.data_array[j++] = atoi(num_str);
            }
            else if (check_command_or_instruction(ast) == command)
            {
                if (ast->line_type_data.command.opcode_type[j].command_type == none)
                {
                    ast->line_type_data.command.opcode_type[j].command_type = number;
                    ast->line_type_data.command.opcode_type[j].numberr = atoi(num_str);
                }
                else
                {
                    ast->line_type_data.command.opcode_type[++j].command_type = number;
                    ast->line_type_data.command.opcode_type[j].numberr = atoi(num_str);
                }
            }
        }
        else if (strcmp(sep.line[i], ",") == 0)
        {
            if ((i + 1) < sep.line_number && strcmp(sep.line[i + 1], ",") == 0)
            {
                error_found(ast, "error-too many commas");
                return *ast;
            }
            else
                continue;
        }
        else if ((i + 1) < sep.line_number &&
                 (!is_int(sep.line[i], ast) &&
                  strcmp(sep.line[i + 1], ",") == 0) &&
                 check_command_or_instruction(ast) == instruction)
        {
            error_found(ast, "error-comma before first number");
            return *ast;
        }
        else if (strcmp(sep.line[sep.line_number - 1], ",") == 0)
        {
            error_found(ast, "error-comma after last number");
            return *ast;
        }
    }

    ast->line_type_data.inst.inst_type = data;
    return *ast;
}



struct ast set_string(struct ast *ast, struct sep_line sep)
{
    int i, j, k;

    char *string_start;
    size_t len;
    j = 0;
    for (i = 0; i < sep.line_number; i++)
    {
        if (is_string(sep.line[i], ast))
        {
            break;
        }
    }

    if (i < sep.line_number && is_string(sep.line[i], ast))
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
            ast->line_type_data.inst.string_array[j][0] = string_start[k];
            ast->line_type_data.inst.string_array[j][1] = '\0';
            j++;
        }
    }
    if (i + 1 < sep.line_number && is_string(sep.line[i + 1], ast))
    {
        error_found(ast, "error-too many strings");
        free(ast->line_type_data.inst.string_array[j]);
        return *ast;
    }

    ast->line_type_data.inst.inst_type = string;
    return *ast;
}


/**
 * @brief Set the instruction object
 * 
 * @param ast 
 * @param sep 
 */
void set_instruction(struct ast *ast, struct sep_line sep)
{
    if (strcmp(sep.line[0], ".entry") == 0 ||
        strcmp(sep.line[0], ".extern") == 0)
    {
        set_entry_extern(ast, sep);
    }

    else if (strcmp(sep.line[1], ".data") == 0 ||
             strcmp(sep.line[0], ".data") == 0)
    {
        set_data(ast, sep);
    }
    else if (strcmp(sep.line[1], ".string") == 0 ||
             strcmp(sep.line[0], ".string") == 0)
    {
        set_string(ast, sep);
    }
}


/**
 * @brief Set the entry extern object
 * 
 * @param ast 
 * @param sep 
 * @return struct ast 
 */
struct ast set_entry_extern(struct ast *ast, struct sep_line sep)
{

    if (sep.line_number < 2)
    {
        error_found(ast, "error-missing label name");
        return *ast;
    }
    if (sep.line_number > 2)
    {
        error_found(ast, "error-too many arguments");

        return *ast;
    }

    if (strcmp(sep.line[0], ".entry") == 0 || strcmp(sep.line[0], "entry") == 0)
    {

        ast->line_type_data.inst.inst_type = entry, ast->ARE.ARE_type = R;
    }
    else
    {
        ast->line_type_data.inst.inst_type = extrn, ast->ARE.ARE_type = E;
    }
    set_label(ast, sep, 1);

    return *ast;
}


/**
 * @brief Set the label object
 * 
 * @param ast 
 * @param sep 
 * @param index 
 * @return struct ast 
 */
struct ast set_label(struct ast *ast, struct sep_line sep, int index)
{
    size_t len = strlen(sep.line[index]);

    /* Handle case where line ends with ':'*/
    if (len > 0 && sep.line[index][len - 1] == ':')
    {
        len--;
    }
    if (ast->line_type_data.inst.label_array[0] == NULL)
    {
        if (ast->line_type == inst_line)
        {
            ast->line_type_data.inst.label_array[0] = sep.line[index];
            ast->line_type_data.inst.label_array[1] = NULL;
        }
        else if (ast->line_type == command_line)
        {
            ast->line_type_data.command.opcode_type[0].command_type = label;
            ast->line_type_data.command.opcode_type[0].labell[0] = sep.line[index];
        }
    }
    else if (ast->line_type == inst_line)
    {
        ast->line_type_data.inst.label_array[1] = sep.line[index];
    }
    else if (ast->line_type == command_line)
    {
        ast->line_type_data.command.opcode_type[1].command_type = label;
        ast->line_type_data.command.opcode_type[1].labell[0] = sep.line[index];
    }

    else
    {
        error_found(ast, "undefined line type");
        return *ast;
    }

    return *ast;
}

/**
 * @brief Set the command name object
 * 
 * @param ast 
 * @param command 
 */
void set_command_name(struct ast *ast, char *command)
{
    int i;
    int num_commands;
    struct
    {
        const char *name;
        int opcode;
    } command_map[] = {{"mov", mov}, {"cmp", cmp}, {"add", add}, {"sub", sub}, {"lea", lea}, {"not", nt}, {"clr", clr}, {"inc", inc}, {"dec", dec}, {"jmp", jmp}, {"bne", bne}, {"red", red}, {"prn", prn}, {"jsr", jsr}, {"rts", rts}, {"stop", stop}};

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





#endif
