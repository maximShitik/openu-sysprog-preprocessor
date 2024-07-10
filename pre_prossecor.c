#include "lexer.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 100

enum
{
    min_hash_index = 0,
    Error = -1,
    regular_line = -2,
    endmacro = -3,
    macro_defenition = -4,
};

typedef struct line_node
{
    char *line_data;
    struct line_node *next;
} line_node;

typedef struct hash
{
    char *macro_name;
    line_node *lines;
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
    for (int i = 0; i < HASH_SIZE; i++)
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
    unsigned int hash = 0;
    while (*macro_name)
    {
        hash = (hash << 3) + *macro_name++;
    }
    return hash % HASH_SIZE;
}

void insert_to_hash(char *data, hash *hash_table[], int index, int is_macro_name)
{
    hash *new_hash = (hash *)malloc(sizeof(hash));
    if (new_hash == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (is_macro_name)
    {
        new_hash->macro_name = malloc(strlen(data) + 1);
        if (new_hash->macro_name == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_hash->macro_name, data);
        new_hash->lines = NULL;
    }
    else
    {
        line_node *new_line = (line_node *)malloc(sizeof(line_node));
        if (new_line == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        new_line->line_data = malloc(strlen(data) + 1);
        if (new_line->line_data == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_line->line_data, data);
        new_line->next = NULL;

        hash *current = hash_table[index];
        while (current->next != NULL)
        {
            current = current->next;
        }

        if (current->lines == NULL)
        {
            current->lines = new_line;
        }
        else
        {
            line_node *current_line = current->lines;
            while (current_line->next != NULL)
            {
                current_line = current_line->next;
            }
            current_line->next = new_line;
        }

        free(new_hash);
        return;
    }

    new_hash->next = hash_table[index];
    hash_table[index] = new_hash;
}

void free_memory(hash *hash_table[])
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hash *current = hash_table[i];
        while (current != NULL)
        {
            hash *temp = current;
            current = current->next;
            free(temp->macro_name);
            line_node *current_line = temp->lines;
            while (current_line != NULL)
            {
                line_node *temp_line = current_line;
                current_line = current_line->next;
                free(temp_line->line_data);
                free(temp_line);
            }
            free(temp);
        }
    }
}

hash *search_macro_in_hash(char *macro_name, hash *hash_table[])
{
    unsigned int index = hash_function(macro_name);
    hash *current = hash_table[index];
    while (current != NULL)
    {
        if (strcmp(current->macro_name, macro_name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int line_defenition(char *line, struct sep_line separated)
{
    if (strcmp(separated.line[0], "macr") == 0)
    {
        if (strpbrk(line, ",") != NULL)
        {
            printf("Error: macro definition should not contain a comma\n");
            return Error;
        }
        if (separated.line_number == 1)
        {
            printf("Error: macro definition should contain a name\n");
            return Error;
        }
        else if (system_names(separated.line[1]) != true)
        {
            printf("Error: macro name is a system name\n");
            return Error;
        }
        else
        {
            return hash_function(separated.line[1]);
        }
    }
    else if (strcmp(separated.line[0], "endmacr") == 0)
    {
        if (separated.line_number > 1)
        {
            printf("Error: end of macro definition should not contain any other words\n");
            return Error;
        }
        else
        {
            return endmacro;
        }
    }
    else
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
    char *line_copy;

    while (fgets(line, MAX_LINE, input))
    {
        line_copy = malloc(strlen(line) + 1);
        if (line_copy == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(line_copy, line);
        struct sep_line separated = next_word(line);
        name_index = line_defenition(line_copy, separated);

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
                insert_to_hash(line_copy, hash_table, index_copy, 0);
            }
            else
            {
                macro_found = search_macro_in_hash(line_copy, hash_table);
                /*searching if the macro is in the hash so we will add it to the file*/
                if (macro_found != NULL)
                {
                    /*add the hash data to the as file*/
                    fprintf(input, "%s\n", macro_found->lines->line_data); // Note: This assumes the first line is used
                }
                /*add the line to the as file*/
                else
                {
                    fprintf(input, "%s\n", line_copy);
                }
            }
        }
        free(line_copy);
    }
}
