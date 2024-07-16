/*
The functions in this file are used in many other files in the project
to prevent of using the same code on diffrent missons and be as most generic
as possible .
*/

#ifndef HELP_FUNC_C
#define HELP_FUNC_C
#include "help_func.h"
#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Trimming the white spaces in the line.
 *
 * @param str
 */
void trim_whitespace(char *str)
{
    char *src, *dst, *end;
    src = str;
    while (isspace((unsigned char)*src))
        src++;
    dst = str;
    while (*src != 0)
        *dst++ = *src++;
    *dst = 0;
    end = dst - 1;
    while (end > str && isspace((unsigned char)*end))
        *end-- = 0;
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
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Identifies the type of system name (command, instruction, or register).
 * Returns:
 */
int system_names(char *line)
{
    char *command_names[] = {"mov", "cmp", "add", "sub", "not", "clr",
                             "lea", "inc", "dec", "jmp", "bne", "red",
                             "prn", "jsr", "rts", "stop"};

    char *inst_names[] = {"data", "string", "entry", "extern"};

    char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
                         "*r0", "*r1", "*r2", "*r3", "*r4", "*r5", "*r6", "*r7"};
    
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
    

    return TRUE;
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

#endif
