#ifndef HELP_FUNC_C
#define HELP_FUNC_C


#include "help_func.h"
#include "data_structs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


void free_string_array(char **array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        free(array[i]);
    }
}


struct sep_line split_line(char *line) {
    struct sep_line separated = {0};
    int counter = 0;
    char *token, *sep;

    /*skip the white spaces in the beggining*/
    while (isspace((unsigned char)*line)) {
        line++;
    }
    if (*line == '\0') return separated;

    token = line;

    do {
        if (*token == '"') {
            sep = strchr(token + 1, '"'); 
            if (sep) sep++;
        } else {
            sep = strpbrk(token, SPACES ",");
        }

        if (sep != NULL) {
            char temp = *sep;
            *sep = '\0';

            /*storing the token*/
            trim_whitespace(token);
            if (*token != '\0') {
                separated.line[counter++] = token;
            }

            /*handeling comma as a separated toekn*/
            if (temp == ',') {
                separated.line[counter++] = ",";
                token = sep + 1;
            } else {
                token = sep + 1;
            }

            while (isspace((unsigned char)*token)) {
                token++;
            }
        } else {
            trim_whitespace(token);
            if (*token != '\0') {
                separated.line[counter++] = token;
            }
        }
    } while (sep != NULL && *token != '\0');

    separated.line_number = counter;
    return separated;
}

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

int system_names(char *line)
{
    char *command_names[] = {"mov", "cmp", "add", "sub", "not", "clr",
                             "lea", "inc", "dec", "jmp", "bne", "red",
                             "prn", "jsr", "rts", "stop"};

    char *inst_names[] = {"data", "string", "entry", "extern", ".data", ".string", ".entry", ".extern"};

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

    return LABEL;
}


void reset_string_array(char **array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        array[i] = NULL;
    }
}

void reset_line(char line[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        line[i] = '\0';
    }
}

#endif
