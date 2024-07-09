/*this function will resive a file and folde the macros to another file*/

/* Hash table */

#include "lexer.c"
#include <stdio.h>

enum
{
    Error = -1,
    regular_line = 0,
    endmacro,
    macro_defenition

};

typedef struct hash
{
    char *macro_name;
    char *line_data;
    struct hash *next;
} hash;

int line_defenition(char *line)
{
    struct sep_line separated = next_word(line);
    if (strcmp(separated.line[0], "macr") == 0) /*if the line is a macro defenition*/
    {
        if (system_names(separated.line[1]) != true) /*checking if the name is a system name*/
        {
            printf("Error: macro name is a system name\n");
            return Error;
        }
        else
        {
            return macro_defenition;
        }
    }
    else if (strcmp(separated.line[0], "endmacr") == 0) /*its the end of the macro defenition*/
    {
        return endmacro;
    }
    else /*its not a macro line*/
    {
        return regular_line;
    }
}

void find_macro(char *line, hash *hash_table[], FILE *input)
{

    FILE *as_file;
    FILE *am_file;
    int indicator; /*if indicator is macro_defenition its will define the lines as macros till the indicator is regular*/

    while (fgets(line, MAX_LINE, input))
    {

        switch (line_defenition(line))
        {
        case Error:
            /*erase the am file and return*/
            return;

        case macro_defenition:
            /*create a new file with the name of the macro*/
            indicator = macro_defenition;

            break;

        case endmacro:
            /*close the file and add it to the hash table*/
            indicator = regular_line;
            break;
        case regular_line:
            if (indicator == macro_defenition)
            {
                /*add the line to the file*/
            }
            else
            {
                /*add the line to the as file*/
            }
        }
    }
}
