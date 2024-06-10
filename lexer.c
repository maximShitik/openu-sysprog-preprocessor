#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
#define MAX_LABEL 32
#define SPACES " \t\n , "

struct sep_line
{
    char *line[MAX_LINE];
    int line_number;
};

typedef enum
{
    error = 1,
    note,
    instruction,
    command,
    empty,
    registerr,
    Entry,
    Extrn

} return_types;

typedef enum
{
    false,
    true
} bool;

/**
 * @brief Trimming the white spaces in the line.
 * 
 * @param str 
 */
void trim_whitespace(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)
        return;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = 0;
}
/**
 * Function to split a line into words based on spaces.
 * Removes leading spaces and stops at the first null character.
 */
struct sep_line next_word(char *line)
{
    struct sep_line separated = {0};
    int counter = 0;
    char *token, *sep;

    while (isspace((unsigned char)*line))
        line++;
    if (*line == '\0')
        return separated;

    token = line;

    do
    {
        sep = strpbrk(token, SPACES ",");

        if (sep != NULL)
        {
            if (*sep == ',')
            {
                *sep = '\0';
                trim_whitespace(token);
                if (*token != '\0')
                {
                    separated.line[counter++] = token;
                }
                separated.line[counter++] = ",";
                token = sep + 1;
            }
            else
            {
                *sep = '\0';
                trim_whitespace(token);
                if (*token != '\0')
                {
                    separated.line[counter++] = token;
                }
                token = sep + 1;
            }

            while (isspace((unsigned char)*token))
                token++;
        }
        else
        {
            trim_whitespace(token);
            if (*token != '\0')
            {
                separated.line[counter++] = token;
            }
        }
    } while (sep != NULL && *token != '\0');

    separated.line_number = counter;
    return separated;
}

/**
 * Checks if a string contains any uppercase letters.
 * Returns 1 if it does, 0 otherwise.
 */
int contains_uppercase(const char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (isupper(str[i]))
        {
            return true;
        }
    }
    return false;
}

/**
 * Identifies the type of system name (command, instruction, or register).
 * Returns:

 * 1 - not a system name
 * 3 - instruction name
 * 4 - command name
 * 6 - register name
 */
int system_names(char *line)
{
    char *command_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    char *inst_names[] = {"data", "string", "entry", "extrn"};
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    int i;
    for (i = 0; i < sizeof(command_names) / sizeof(command_names[0]); i++)
    {
        if (strcmp(line, command_names[i]) == 0)
        {
            return command;
        }
    }

    for (i = 0; i < sizeof(inst_names) / sizeof(inst_names[0]); i++)
    {
        if (strcmp(line, inst_names[i]) == 0)
        {
            return instruction;
        }
    }

    for (i = 0; i < sizeof(registers) / sizeof(registers[0]); i++)
    {
        if (strcmp(line, registers[i]) == 0)
        {
            return registerr;
        }
    }

    return true;
}

/**
 * @brief
 *
 * @param line
 * @param ast
 * @return int false if the token is invalid,
 * 1 if it is a label,
 * 2 if it is an instruction,
 * 5 if it is a command

 */
int process_token(char *line, struct ast *ast)
{
    int result;
    char *line_copy;
    size_t len = strlen(line);

    if (len >= MAX_LABEL)
    {
        return false;
    }

    if (line[len - 1] == ':')
    {
        line_copy = (char *)malloc(len);
        if (line_copy == NULL)
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "memory allocation error");
            return false;
        }
        strncpy(line_copy, line, len - 1);
        line_copy[len - 1] = '\0';
        result = system_names(line_copy);
        free(line_copy);
        return result == lable; /*valid labale name */
    }

    if (line[0] == '.')
    {
        line_copy = (char *)malloc(len + 1);
        if (line_copy == NULL)
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "memory allocation error");
            return false;
        }
        strcpy(line_copy, line + 1);
        if (contains_uppercase(line_copy))
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "invalid instruction name");
            free(line_copy);
            return false;
        }
        result = system_names(line_copy);

        if (result == instruction)
        {
            if (strcmp(line_copy, "entry") == 0)
            {
                free(line_copy);
                return Entry;
            }
            if (strcmp(line_copy, "extern") == 0)
            {
                free(line_copy);
                return Extrn;
            }
            free(line_copy);
            return instruction; /*valid instruction*/
        }
        else if (result == true)
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "invalid instruction name");
            return false;
        }
    }

    if (system_names(line) == command)
    {
        return command;
    }
    else

        return 0;
}

/**
 * Checks if a string can be converted to an integer.
 * Returns 1 if it can, 0 otherwise.
 */
int is_int(char *line)
{
    char *endptr;
    if (*line == '+' || *line == '-' || *line == '#')
    {
        line++;
    }

    if (strchr(line, '.') != NULL)
    {
        return false;
    }

    strtol(line, &endptr, 10);

    return *endptr == '\0';
}

/**
 * Checks if a string is enclosed in double quotes.
 * Returns 1 if it is, 0 otherwise.
 */
int is_string(char *line)
{
    return line[0] == '"' && line[strlen(line) - 1] == '"';
}

int is_note(char *line)
{
    return line[0] == ';';
}

/**
 * Resets a string array to NULL values.
 */
void reset_string_array(char **array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        array[i] = NULL;
    }
}

/**
 * Resets an AST (Abstract Syntax Tree) node to its default state.
 */
void reset_ast(struct ast *node)
{
    int i;
    i = 0;
    node->line_type = empty_line;
    node->line_type_data.inst.inst_type = data;
    reset_string_array(node->line_type_data.inst.inst_opt.label, 32);
    reset_string_array(node->line_type_data.inst.inst_opt.string, 50);
    node->line_type_data.command.opcode = 0;
    while (i < 80)
    {
        node->line_type_data.inst.inst_opt.data[i] = 0;
        i++;
    }
    for (i = 0; i < 2; i++)
    {
        node->line_type_data.command.opcode_type[i].command_type = none;
        reset_string_array(node->line_type_data.command.opcode_type[i].command_operand.label, 50);
        node->line_type_data.command.opcode_type[i].command_operand.number = 0;
        node->line_type_data.command.opcode_type[i].command_operand.reg = 0;
    }
    node->line_type_data.note = 0;
    memset(node->error.error, 0, sizeof(node->error.error));
    node->error.line_number = 0;
    node->ARE.ARE_type = A;
}

struct ast set_entry_extern(struct ast *ast, struct sep_line sep)
{

    if (sep.line_number < 2)
    {
        ast->line_type = error_line;
        ast->error.line_number = 1;
        strcpy(ast->error.error, "error-missing label name");
        return *ast;
    }
    if (sep.line_number > 2)
    {
        ast->line_type = error_line;
        ast->error.line_number = 1;
        strcpy(ast->error.error, "error-too many arguments");
        return *ast;
    }
    ast->line_type_data.inst.inst_opt.label[0] = sep.line[0];
    ast->line_type_data.inst.inst_opt.label[1] = NULL;

    return *ast;
}

struct ast line_type(struct sep_line sep, struct ast *ast)
{
    if (sep.line_number == 0)
    {
        ast->line_type = error_line;
        ast->error.line_number = 1;
        strcpy(ast->error.error, "error-empty line");
        return *ast;
    }
    if (is_note(sep.line[0]))
    {
        ast->line_type_data.note = 1;
        ast->line_type = note_line;
        return *ast;
    }
    if (process_token(sep.line[0], ast) == lable)
    {
        ast->line_type = inst_line;
        if (process_token(sep.line[1], ast) == instruction)
        {
            return *ast;
        }
        else if (process_token(sep.line[1], ast) == command)
        {
            ast->line_type = command_line;
            return *ast;
        }
    }
    else if (process_token(sep.line[0], ast) == command)
    {
        ast->line_type = command_line;
        return *ast;
    }
    return *ast;
}
void set_lable(struct ast *ast, struct sep_line sep)
{

    ast->line_type_data.inst.inst_opt.label[0] = sep.line[0];
    ast->line_type_data.inst.inst_opt.label[1] = NULL;
}
struct ast set_data(struct ast *ast, struct sep_line sep)
{
    int i, j;
    struct sep_line index;
    index = sep;
    j = 0;
    for (i = 0; i < sep.line_number; i++)
    {

        if (is_int(index.line[i]))
        {
            if ((i + 1) < index.line_number && is_int(index.line[i + 1]))
            {
                ast->line_type = error_line;
                ast->error.line_number = 1;
                strcpy(ast->error.error, "error-missing comma");
                return *ast;
            }

            ast->line_type_data.inst.inst_opt.data[j++] = atoi(index.line[i]);
        }
        else if (strcmp(index.line[i], ",") == 0)
        {
            if ((i + 1) < index.line_number && strcmp(index.line[i + 1], ",") == 0)
            {
                ast->line_type = error_line;
                ast->error.line_number = 1;
                strcpy(ast->error.error, "error-too many commas");
                return *ast;
            }
            else
                continue;
        }
    }
    ast->line_type_data.inst.inst_type = data;
    return *ast;
}
void set_instruction(struct ast *ast, struct sep_line sep)
{
    if (strcmp(sep.line[1], ".data") == 0)
    {
        set_data(ast, sep);
    }
    else if (strcmp(sep.line[1], "string") == 0)
    {
        ast->line_type_data.inst.inst_type = string;
        if (is_string(sep.line[2]))
        {
            ast->line_type_data.inst.inst_opt.string[0] = sep.line[2];
            ast->line_type_data.inst.inst_opt.string[1] = NULL;
        }
        else
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "error-invalid string");
        }
    }
}

void line_type_data_set(struct sep_line sep, struct ast *ast)
{
    if (ast->line_type == inst_line)
    {
        if (process_token(sep.line[0], ast) == lable)
        {
            set_lable(ast, sep);
            if (process_token(sep.line[1], ast) == instruction)
            {
                set_instruction(ast, sep);
            }
        }
        else if (process_token(sep.line[0], ast) == Entry || (process_token(sep.line[1], ast) == Extrn))
        {
            set_entry_extern(ast, sep);
        }
    }
}
/**
 * Parses a line and generates an AST node.
 */
struct ast parse_line(char *line)
{
    struct sep_line separated = next_word(line);
    struct ast ast;
    reset_ast(&ast);
    line_type(separated, &ast);
    line_type_data_set(separated, &ast);
    return ast;
}

/*test functions*/
/**
 * Tests the line_type_check function.
 */
void test_line_type_check()
{

    char line[50] = "LAble: 1";
    parse_line(line);
}

/**
 * Main function to run all tests.
 */
int main()
{

    test_line_type_check();
    printf("All tests completed.\n");
    return 0;
}
