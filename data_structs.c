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

/**
 * @brief inserting data to the has table
 * 
 * @param data 
 * @param hash_table
 * @param index 
 * @param is_macro_name 
 */
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


/**
 * @brief freeting the memory allocated for the hash table
 * 
 * @param hash_table 
 */
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


/**
 * @brief 
 * 
 * @param macro_name 
 * @param hash_table 
 * @return hash* 
 */
hash *search_macro_in_hash(char *macro_name, hash *hash_table[],int index)
{
   
    hash *current;
    current = hash_table[index];
    while (current != NULL)
    {
        if (strcmp(current->macro_name, macro_name)==0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}




#endif
