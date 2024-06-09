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

/*removing spaces and skipping to the next word*/
struct sep_line next_word(char *line)
{
    int counter;
    int i;
    char *sep;
    struct sep_line seperated = {0};
    counter = 0;

    while (isspace(*line))
    {
        line++;
    }

    if (*line == '\0')
    {
        return seperated;
    }

    do
    {
        seperated.line[counter++] = line;
        sep = strpbrk(line, SPACES);
        if (sep != NULL)
        {
            *sep = '\0';
            sep++;

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
    seperated.line_number = counter;
    return seperated;
}

/**
 * @brief
 *
 * @param str
 * @return int 0= no uppercase, 1= uppercase
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
 * @brief
 *
 * @param line
 * @return int, 0 = command name ,1 = not system name ,2 =  inst name 3 = register name
 */
int system_names(char *line) /*פשוט בודק באופן כללי מה זה השם הזה*/
{
    int i;
    char *command_names[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
    char *inst_names[] = {"data", "string", "entry", "extrn"};
    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
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
 * @brief
 *
 * @param line
 * @return int 0 = string converted to an integer, 1 = Couldn't convert to int


 */
int is_int(char *line)
{
    /* Check for optional sign at the beginning*/
    if (*line == '+' || *line == '-' || *line == '#')
    {
        line++;
    }
    if (strchr(line, '.') != NULL)
    { /*checking if the string is a float*/
        return 0;
    }

    /* Check if the rest of the string is a valid integer*/
    char *endptr;
    strtol(line, &endptr, 10);

    /* Check if the whole string was successfully converted to an integer*/
    if (*endptr == '\0')
    {
        return 1; /* The whole string was successfully converted to an integer */
    }
    else
    {
        return 0; /* Couldn't convert to int */
    }
}

/**
 * @brief
 *
 * @param line
 * @return int 0 = not a string, 1 = is a string

 */
int is_string(char *line)
{
    if (line[0] == '"' && line[strlen(line) - 1] == '"')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief
 *
 * @param line
 * @return int 1 = is a valid label, 0 = not a valid label

 */
int is_label(char *line)
{

    char *line_copy;
    int result;
    if (strlen(line) < MAX_LABEL && line[strlen(line) - 1] == ':')
    {
        /* Create a copy of the line to work on */
        line_copy = strdup(line);
        if (line_copy == NULL)
        {

            return 0;
        }

        line_copy[strlen(line_copy) - 1] = '\0'; /* Remove the trailing colon */
        result = system_names(line_copy);        /*checking we didn't use any system saved name*/

        free(line_copy); /* free the memory */

        if (result != 1) /*if we used invalid name*/
        {
            return 0;
        }

        else
        {
            return 1;
        }
    }
    return 0;
}
void reset_string_array(char **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        array[i] = NULL;
    }
}

int is_instruction(char *line)
{
    char *line_copy;
    int result;
    if (line[0] == '.')
    {
        /* Create a copy of the line to work on */
        line_copy = strdup(line);
        if (line_copy == NULL)
        {

            return 0; /*error*/
        }

        memmove(line_copy, line_copy + 1, strlen(line_copy));
        if (contains_uppercase(line_copy))
        {
            return 0; /*need to add error*/
        }
        result = system_names(line_copy); /*checking we didn't use any system saved name*/
        free(line_copy);                  /* free the memory */
        if (result == 2)
        {
            return 1;
        }
        else
        {
            return 0; /*error */
        }
    }
    return 2;
}

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

void line_type_check(struct sep_line sep, struct ast *ast)
{
    int i;
    int result;
    char res;
    for (i = 0; i < sizeof(sep); i++)
    {

        if (is_label(sep.line[i]) == 0 || system_names(sep.line[i]) == 0)
        {
            if (system_names(sep.line[i]) == 1)
            {
            }
        }
    }
}
struct ast parese_line(char *line)
{
    struct sep_line seperated;
    int i;
    struct ast ast;
    reset_ast(&ast);
    seperated = next_word(line);
    if (seperated.line_number == 0)
    {
        return ast;
    }
    /* 0 = command name ,1 = not system name ,2 =  inst name 3 = register name */
    line_type_check(seperated, &ast);

    return ast;
}



int main()
{
    return 0;
}
