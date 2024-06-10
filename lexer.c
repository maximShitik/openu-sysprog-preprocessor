#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
#define MAX_LABEL 32
#define SPACES " \t\n\r"

struct sep_line
{
    char *line[MAX_LINE];
    int line_number;
};

/**
 * Function to split a line into words based on spaces.
 * Removes leading spaces and stops at the first null character.
 */
struct sep_line next_word(char *line)
{
    struct sep_line separated = {0};
    int counter = 0;
    char *sep;

    while (isspace(*line))
    {
        line++;
    }

    if (*line == '\0')
    {
        return separated;
    }

    do
    {
        separated.line[counter++] = line;
        sep = strpbrk(line, SPACES ",");
        if (sep != NULL)
        {
            if (*sep == ',')
            {
                *sep = '\0';
                sep++;
                separated.line[counter++] = ",";
            }
            else
            {
                *sep = '\0';
                sep++;
            }

            while (isspace(*sep))
            {
                sep++;
            }
            if (*sep == '\0')
            {
                break;
            }

            line = sep;
        }
    } while (sep != NULL);

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
            return 1;
        }
    }
    return 0;
}

/**
 * Identifies the type of system name (command, instruction, or register).
 * Returns:
 * 0 - command name
 * 1 - not a system name
 * 2 - instruction name
 * 3 - register name
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
            return 0;
        }
    }

    for (i = 0; i < sizeof(inst_names) / sizeof(inst_names[0]); i++)
    {
        if (strcmp(line, inst_names[i]) == 0)
        {
            return 2;
        }
    }

    for (i = 0; i < sizeof(registers) / sizeof(registers[0]); i++)
    {
        if (strcmp(line, registers[i]) == 0)
        {
            return 3;
        }
    }

    return 1;
}

/**
 * Checks if a string can be converted to an integer.
 * Returns 1 if it can, 0 otherwise.
 */
int is_int(char *line)
{
    if (*line == '+' || *line == '-' || *line == '#')
    {
        line++;
    }

    if (strchr(line, '.') != NULL)
    {
        return 0;
    }

    char *endptr;
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
 * Checks if a string is a valid label.
 * Returns 1 if it is, 0 otherwise.
 */
int is_label(char *line)
{
    int result;
    char *line_copy;
    if (strlen(line) >= MAX_LABEL || line[strlen(line) - 1] != ':')
    {
        return 0;
    }

    line_copy = strdup(line);
    if (line_copy == NULL)
    {
        return 0;
    }

    line_copy[strlen(line_copy) - 1] = '\0';
    result = system_names(line_copy);

    free(line_copy);

    return result == 1;
}

/**
 * Checks if a string is a valid instruction.
 * Returns:
 * 0 - error
 * 1 - instruction
 * 2 - not an instruction
 */
int is_instruction(char *line)
{
    if (line[0] != '.')
    {
        return 2;
    }

    char *line_copy = strdup(line + 1);
    if (line_copy == NULL)
    {
        return 0;
    }

    if (contains_uppercase(line_copy))
    {
        /*****need to add error*/
        free(line_copy);
        return 0;
    }

    int result = system_names(line_copy);
    free(line_copy);

    return result == 2 ? 1 : 0;
}

/**
 * Checks if a string is an entry or extern instruction.
 * Returns:
 * 0 - error
 * 1 - entry
 * 2 - extern
 * 3 - not an instruction
 */
int extern_and_entry(char *line)
{
    int result;
    char *line_copy;
    if (line[0] != '.')
    {
        return 3;
    }

    line_copy = strdup(line + 1);
    if (line_copy == NULL)
    {
        return 0;
    }

    if (contains_uppercase(line_copy))
    {
        free(line_copy);
        return 0;
    }

    result = system_names(line_copy);
    if (result == 2)
    {
        if (strcmp(line_copy, "entry") == 0)
        {
            free(line_copy);
            return 1;
        }
        if (strcmp(line_copy, "extern") == 0)
        {
            free(line_copy);
            return 2;
        }
    }

    free(line_copy);
    return 0;
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
    node->line_type = empty_line;
    node->line_type_data.inst.inst_type = data;
    reset_string_array(node->line_type_data.inst.inst_opt.label, 32);
    node->line_type_data.inst.inst_opt.data = 0;
    reset_string_array(node->line_type_data.inst.inst_opt.string, 50);
    node->line_type_data.command.opcode = 0;
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

/**
 * Determines the type of line and updates the AST node accordingly.
 */
struct ast line_type_check(struct sep_line sep, struct ast *ast)
{
    int i;
    int entry_extern_result;
    i = 0;
    if (sep.line[i] == NULL)
    {
        ast->line_type = empty_line;
        return *ast;
    }
    if (is_note(sep.line[0]))
    {
        ast->line_type = note_line;
        return *ast;
    }

    if (!is_label(sep.line[i]))
    {
        entry_extern_result = extern_and_entry(sep.line[i]);
        if (entry_extern_result == 1)
        {
            ast->line_type = inst_line;
            ast->line_type_data.inst.inst_type = entry;
        }
        else if (entry_extern_result == 2)
        {
            ast->line_type = inst_line;
            ast->line_type_data.inst.inst_type = extrn;
        }
        else
        {
            ast->line_type = error_line;
            ast->error.line_number = 1;
            strcpy(ast->error.error, "invalid label name");
            return *ast;
        }
    }
    else if (is_label(sep.line[i]) == 1)
    {
        ast->line_type = inst_line;
        ast->line_type_data.inst.inst_type = lable;

        if (system_names(sep.line[++i]) == 0)
        {
            ast->line_type = command_line;
        }
    }
    else if (is_instruction(sep.line[i]) == 1)
    {
        ast->line_type = inst_line;
    }
    else if (system_names(sep.line[i]) == 0)
    {
        ast->line_type = command_line;
    }
    else
    {
        ast->line_type = error_line;
        ast->error.line_number = 1;
        strcpy(ast->error.error, "invalid first name");
        return *ast;
    }
    return *ast;
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
    ast->line_type_data.inst.inst_opt.label[0] = sep.line[1];
    ast->line_type_data.inst.inst_opt.label[1] = NULL;
    
    return *ast;
}

void line_type_data_set(struct sep_line sep, struct ast *ast)
{
    if (ast->line_type == inst_line)
    {
        if (ast->line_type_data.inst.inst_type == entry || ast->line_type_data.inst.inst_type == extrn)
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

    line_type_check(separated, &ast);
    line_type_data_set(separated, &ast);
    return ast;
}

/*test functions*/
/**
 * Tests the line_type_check function.
 */
void test_line_type_check()
{
    struct sep_line sep;
    struct ast ast;

    char line[50] = "  .entry MOVE ";
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
