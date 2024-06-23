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

typedef enum return_types
{
    error = 1,
    note,
    instruction,
    command,
    empty,
    registerr,
    Entry,
    Extrn,
    macro

} return_types;

typedef enum bool
{
    false,
    true
} bool;

/*reset the ast but without error*/
void error_found(struct ast *ast, char *error_message);

struct ast set_lea(struct ast *ast, struct sep_line sep);
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
 */
int system_names(char *line)
{
    char *command_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    char *inst_names[] = {"data", "string", "entry", "extrn"};
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    char *macros[] = {"macr", "endmacr"};
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
    for (i = 0; i < sizeof(macros) / sizeof(macros[0]); i++)
    {
        if (strcmp(line, macros[i]) == 0)
        {
            return macro;
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
            return false;
        }
        strncpy(line_copy, line, len - 1);
        line_copy[len - 1] = '\0';
        result = system_names(line_copy);
        free(line_copy);
        if (result == true)
        {
            return true;
        }
        else
        {
            error_found(ast, "error-invalid label name");
            return false;
        }
    }

    if (line[0] == '.')
    {
        line_copy = (char *)malloc(len + 1);
        if (line_copy == NULL)
        {
            error_found(ast, "memory allocation error");
            return false;
        }
        strcpy(line_copy, line + 1);
        if (contains_uppercase(line_copy))
        {
            error_found(ast, "error-invalid instruction name");

            free(line_copy);
            return false;
        }
        result = system_names(line_copy);

        if (result == instruction)
        {
            free(line_copy);
            return instruction; /*valid instruction*/
        }
        else if (result == true)
        {
            error_found(ast, "error-invalid instruction name");
            return false;
        }
    }

    if (system_names(line) == command)
    {
        return command;
    }
    if (system_names(line) == macro)
    {
        return macro;
    }
    else

        error_found(ast, "error-undefinded token");
    strcpy(ast->error.error_token, line); /*checking if the error causing token is saved as a macro before*/
    return 0;
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
    node->line_type_data.inst.inst_type = 0;
    reset_string_array(node->line_type_data.inst.label_array, 2);
    reset_string_array(node->line_type_data.inst.string_array, 50);
    node->line_type_data.command.opcode = 0;
    memset(node->line_type_data.inst.data_array, 0, sizeof(node->line_type_data.inst.data_array));

    for (i = 0; i < 2; i++)
    {
        node->line_type_data.command.opcode_type[i].command_type = none;
        reset_string_array(node->line_type_data.command.opcode_type[i].labell, 50);
        node->line_type_data.command.opcode_type[i].numberr = 0;
        node->line_type_data.command.opcode_type[i].regg = 0;
    }
    node->line_type_data.note = 0;
    memset(node->error.error, 0, sizeof(node->error.error));
    node->error.line_number = 0;
    node->ARE.ARE_type = A;
}

void error_found(struct ast *ast, char *error_message)
{
    reset_ast(ast);
    ast->line_type = error_line;
    ast->error.line_number = 1;
    strcpy(ast->error.error, error_message);
}

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
    if (sep.line_number == 2)
    {
        ast->line_type_data.inst.label_array[0] = sep.line[1];
    }
    ast->line_type_data.inst.label_array[0] = sep.line[0];
    ast->line_type_data.inst.label_array[1] = NULL;
    strcmp(sep.line[0], "entry") == 0 ? (ast->line_type_data.inst.inst_type = entry, ast->ARE.ARE_type = R) : (ast->line_type_data.inst.inst_type = extrn, ast->ARE.ARE_type = E);

    return *ast;
}

struct ast set_macro(struct ast *ast, struct sep_line sep)
{
    if (strcmp(sep.line[0], "macr") == 0)
    {
        if (sep.line_number < 2)
        {
            error_found(ast, "error-missing macro name");
            return *ast;
        }
        if (sep.line_number > 2)
        {
            error_found(ast, "error-too many macro arguments");

            return *ast;
        }
        strcpy(ast->macro.name, sep.line[1]);
    }

    ast->line_type = macro_line;
    return *ast;
}

struct ast set_label(struct ast *ast, struct sep_line sep, int index)
{
    char *line_copy;
    size_t len = strlen(sep.line[index]);
    line_copy = (char *)malloc(len + 1);
    if (line_copy == NULL)
    {
        error_found(ast, "memory allocation error");
        return *ast;
    }
    strncpy(line_copy, sep.line[index], len);
    if (line_copy[len - 1] == ':')
    {
        line_copy[len - 1] = '\0';
    }
    if (ast->line_type == inst_line)
    {
        ast->line_type_data.inst.label_array[0] = line_copy;
        ast->line_type_data.inst.label_array[1] = NULL;
    }
    else if (ast->line_type == command_line)
    {
        ast->line_type_data.command.opcode_type[0].command_type = label;
        ast->line_type_data.command.opcode_type[0].labell[0] = line_copy;
    }
    else
    {
        error_found(ast, "error-undefinded line type");
        return *ast;
    }

    return *ast;
}

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
            ast->line_type_data.inst.data_array[j++] = atoi(sep.line[i]);
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
        else if ((i + 1) < sep.line_number && (!is_int(sep.line[i], ast) && strcmp(sep.line[i + 1], ",") == 0))
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
    j = 0;
    char *string_start;
    size_t len;
    int string_counter;
    string_counter = 0;
    for (i = 0; i < sep.line_number; i++)
    {
        if (is_string(sep.line[i]))
        {
            break;
        }
    }

    if (i < sep.line_number && is_string(sep.line[i]))
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
    if (i + 1 < sep.line_number && is_string(sep.line[i + 1]))
    {
        error_found(ast, "error-too many strings");
        return *ast;
    }

    ast->line_type_data.inst.inst_type = string;
    return *ast;
}

void set_instruction(struct ast *ast, struct sep_line sep)
{
    if (strcmp(sep.line[0], ".entry") == 0 || strcmp(sep.line[0], ".extern") == 0)
    {
        set_entry_extern(ast, sep);
    }

    else if (strcmp(sep.line[1], ".data") == 0 || strcmp(sep.line[0], ".data") == 0)
    {
        set_data(ast, sep);
    }
    else if (strcmp(sep.line[1], ".string") == 0 || strcmp(sep.line[0], ".string") == 0)
    {
        set_string(ast, sep);
    }
}

int operand_group(struct ast *ast, char *command)
{
    if (strcmp(command, "mov") == 0 || strcmp(command, "cmp") == 0 || strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || strcmp(command, "lea") == 0)
    {
        return 2;
    }
    if (strcmp(command, "clr") == 0 || strcmp(command, "not") == 0 || strcmp(command, "inc") == 0 || strcmp(command, "dec") == 0 || strcmp(command, "jmp") == 0 || strcmp(command, "bne") == 0 || strcmp(command, "red") == 0 || strcmp(command, "prn") == 0 || strcmp(command, "jsr") == 0)
        return 1;
    if (strcmp(command, "rts") == 0 || strcmp(command, "stop") == 0)
        return 0;
}

struct ast two_group_command(struct ast *ast, struct sep_line sep)
{
    if (strcmp(sep.line[1], ",") == 0)
    {
        error_found(ast, "error-extra comma");
        return *ast;
    }
    if (sep.line[2] == NULL)
    {
        error_found(ast, "error-missing operand");
        return *ast;
    }

    if (strcmp(sep.line[0], "lea") == 0 || strcmp(sep.line[1], "lea") == 0)
    {

        set_lea(ast, sep);
    }
   
}

struct ast set_lea(struct ast *ast, struct sep_line sep)
{
    char *line_copy;
    int current;
    current = 0;
    int i;
    while (strcmp(sep.line[current], "lea") != 0)
    {
        sep.line[current];
        current++;
    }
    current++;

    if (system_names(sep.line[current]) != true)
    {
        error_found(ast, "error-invalid operand");
        return *ast;
    }
    set_label(ast, sep, current);

    current++;
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
    if (is_int(sep.line[current], ast))
    {
        error_found(ast, "error-invalid operand,lea command cant receive numbers");
        return *ast;
    }
    if (sep.line[++current] != NULL)
    {
        error_found(ast, "error-too many operands");
        return *ast;
    }
    current--;
    ast->line_type_data.command.opcode = lea;
    if (system_names(sep.line[current]) == registerr)
    {
        ast->line_type_data.command.opcode_type[1].command_type = reg;

        int register_number = atoi(sep.line[current] + 1);
        ast->line_type_data.command.opcode_type[1].regg = register_number;
    }
    else if (process_token(sep.line[current], ast) == true)
    {
        set_label(ast, sep, current);
    }
    else
    {
        error_found(ast, "error-invalid operand");
        return *ast;
    }
    return *ast;
}

void set_command(struct ast *ast, struct sep_line sep, char *command)
{
    int result;
    result = operand_group(ast, command);
    if (result == 2)
    {
        two_group_command(ast, sep);
    }
}

struct ast line_type(struct sep_line sep, struct ast *ast)
{
    if (is_note(sep.line[0]))
    {
        ast->line_type_data.note = 1;
        ast->line_type = note_line;
        return *ast;
    }
    if (process_token(sep.line[0], ast) == macro)
    {
        set_macro(ast, sep);
        return *ast;
    }
    if (process_token(sep.line[0], ast) == instruction)
    {
        ast->line_type = inst_line;
        set_instruction(ast, sep);
        return *ast;
    }
    if (process_token(sep.line[0], ast) == lable)
    {
        ast->line_type = inst_line;
        set_label(ast, sep, 0);
        if (process_token(sep.line[1], ast) == instruction)
        {
            set_instruction(ast, sep);
            return *ast;
        }
        else if (process_token(sep.line[1], ast) == command)
        {
            ast->line_type = command_line;
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
 * Parses a line and generates an AST node.
 */
struct ast parse_line(char *line)
{
    struct sep_line separated = next_word(line);
    struct ast ast;
    reset_ast(&ast);
    if (separated.line_number == 0)
    {
        ast.line_type = empty_line;
        return ast;
    }
    line_type(separated, &ast);

    return ast;
}

/*test functions*/
/**
 * Tests the line_type_check function.
 */
void test_line_type_check()
{

    char line[50] = "main: lea STR,r6";
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
