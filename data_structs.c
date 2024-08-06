#ifndef DATA_STRUCTS_C
#define DATA_STRUCTS_C
#include "data_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
unsigned int hash_function(char *name)
{
    unsigned int hash = 0;
    while (*name)
    {
        hash = (hash << 3) + *name++;
    }
    return hash % HASH_SIZE;
}

/**
 * @brief inserting data to the has table
 *
 * @param data
 * @param hash_table
 * @param index
 * @param is_macro_name
 */
void insert_to_hash(char *data, hash *hash_table[], int index, int name)
{
    hash *new_hash = (hash *)malloc(sizeof(hash));
    hash *current;
    if (new_hash == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (name)
    {
        new_hash->name = malloc(strlen(data) + 1);
        if (new_hash->name == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(new_hash->name, data);
        new_hash->data = NULL;
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

        current = hash_table[index];
        while (current->next != NULL)
        {
            current = current->next;
        }

        if (current->data == NULL)
        {
            current->data = new_line;
        }
        else
        {
            line_node *current_line = current->data;
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

/**
 * @brief freeting the memory allocated for the hash table
 *
 * @param hash_table
 */
void free_hash(hash *hash_table[])
{
    int i;
    hash *temp;
    line_node *current_line;
    line_node *temp_line;
    for (i = 0; i < HASH_SIZE; i++)
    {
        hash *current = hash_table[i];
        while (current != NULL)
        {
            temp = current;
            current = current->next;
            free(temp->name);
            current_line = temp->data;
            while (current_line != NULL)
            {
                temp_line = current_line;
                current_line = current_line->next;
                free(temp_line->line_data);
                free(temp_line);
            }
            free(temp);
        }
    }
}

/**
 * @brief
 *
 * @param macro_name
 * @param hash_table
 * @return hash*
 */
hash *search_in_hash(char *macro_name, hash *hash_table[])
{
    int index = hash_function(macro_name);
    hash *current;
    current = hash_table[index];
    while (current != NULL)
    {
        if (strcmp(current->name, macro_name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

#endif
