/*this function will resive a file and folde the macros to another file*/

#include "lexer.c"
#include <stdio.h>
#define HASH_SIZE 100

enum
{
    min_hash_index = 0,
    Error = -1,
    regular_line = -2,
    endmacro = -3,
    macro_defenition = -4,

};

typedef struct hash
{
    char *macro_name;
    char *line_data;
    struct hash *next;
} hash;

typedef struct hash_table
{
    hash *hash_table[HASH_SIZE];
} hash_table;

/**
 * @brief Reseting the hash table and setting all the pointers to NULL
 *
 * @param hash_table
 * @return void
 */
void hash_reset(hash *hash_table[])
{
    int i;
    for (i = 0; i < HASH_SIZE; i++)
    {
        hash_table[i] = NULL;
    }
}

/**
 * @brief calculating the hash value of a macro name
 *
 * @param str
 * @return unsigned int
 */
unsigned int hash_function(char *macro_name)
{
    unsigned int hash;
    hash = 0;
    while (*macro_name)
    {
        hash = (hash << 3) + *macro_name++; /*multiplying the number by 8 and addeding the ASCII value*/
    }
    return hash % HASH_SIZE; /*ensure that the value is in the hash range*/
}

void insert_to_hash(char *data, hash *hash_table[], int index, int is_macro_name)
{
    hash *new_hash;
    if (is_macro_name)
    {
        new_hash = (hash *)malloc(sizeof(hash));
        if (new_hash == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        new_hash->macro_name = malloc(strlen(data) + 1);
        if (new_hash->macro_name == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_hash->macro_name, data);
    }
    else
    {
        new_hash->line_data = malloc(strlen(data) + 1);
        if (new_hash->line_data == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_hash->line_data, data);
    }

    new_hash->next = NULL;

    if (hash_table[index] == NULL)
    {
        hash_table[index] = new_hash;
    }
    else
    {
        new_hash->next = hash_table[index];
        hash_table[index] = new_hash;
    }
}

/**
 * @brief freeying the memoery aloccated for the hash table and its nodes
 *
 * @param hash_table
 */
void free_memory(hash *hash_table[])
{
    int i;
    hash *current, *temp;
    for (i = 0; i < HASH_SIZE; i++)
    {
        current = hash_table[i];
        while (current != NULL)
        {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
}
hash *search_macro_in_hash(char *macro_name, hash *hash_table[])
{
    unsigned int index;
    hash *current;
    current = hash_table[index];
    index = hash_function(macro_name);

    while (current != NULL)
    {
        if (strcmp(current->macro_name, macro_name) == 0)
        {
            return current; /*macro found*/
        }
        current = current->next;
    }
    return NULL; /* macro not found*/
}

int line_defenition(char *line, struct sep_line separated)
{
    int name_index;

    if (strcmp(separated.line[0], "macr") == 0) /*if the line is a macro defenition*/
    {
        if (strpbrk(line, ",") != NULL)
        {
            printf("Error: macro defenition should not contain a comma\n");
            return Error;
        }
        if (separated.line_number == 1)
        {
            printf("Error: macro defenition should contain a name\n");
            return Error;
        }
        else if (system_names(separated.line[1]) != true) /*checking if the name is a system name*/
        {
            printf("Error: macro name is a system name\n");
            return Error;
        }
        else
        {
            name_index = hash_function(separated.line[1]); /*calculating the name index */
            return name_index;
        }
    }
    else if (strcmp(separated.line[0], "endmacr") == 0) /*its the end of the macro defenition*/
    {
        if (separated.line_number > 1)
        {
            printf("Error: end of macro defenition should not contain any other words\n");
            return Error;
        }
        else
        {
            return endmacro;
        }
    }
    else /*its not a macro line*/
    {
        return regular_line;
    }
}

void find_macro(char *line, hash *hash_table[], FILE *input)
{
    int index_copy;
    hash *macro_found;
    int indicator; /*if indicator is macro_defenition its will define the lines as macros till the indicator is regular*/
    int name_index;
    name_index = 0;

    while (fgets(line, MAX_LINE, input))
    {
        struct sep_line separated = next_word(line);
        name_index = line_defenition(line, separated);

        if (name_index == Error)
        {
            fclose(input);
            remove("output.am");
            return;
        }
        else if (name_index >= min_hash_index)
        {
            insert_to_hash(separated.line[1], hash_table, name_index, 1);
            indicator = macro_defenition;
            index_copy = name_index; /*saving the index so we will insert the lines in the right macro*/
        }
        else if (name_index == endmacro)
        {
            /*stop adding lines to the hash*/
            indicator = regular_line;
        }
        else if (name_index == regular_line)
        {
            if (indicator == macro_defenition)
            {
                /*using the index copy to add the lines in the right index*/
                insert_to_hash(line, hash_table, index_copy, 0);
            }
            else
            {
                macro_found = search_macro_in_hash(line, hash_table);
                /*searching if the macro is in the hash so we will add it to the file*/
                if (macro_found != NULL)
                {
                    /*add the hash data to the as file*/
                    fprintf(input, "%s\n", macro_found->line_data);
                }
                /*add the line to the as file*/
                else
                {
                    fprintf(input, "%s\n", line);
                }
            }
        }
    }
}
