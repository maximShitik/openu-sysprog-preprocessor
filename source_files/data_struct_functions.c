#ifndef DATA_STRUCTS_C
#define DATA_STRUCTS_C
#include "data_structs.h"
#include "data_struct_functions.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hash_reset(hash *hash_table[])
{
    int i;

    for (i = 0; i < HASH_SIZE; i++)
    {
        hash_table[i] = NULL;
    }
}

unsigned int hash_function(char *name)
{
    unsigned int hash = 0;
    while (*name)
    {
        hash = (hash << 3) + *name++;
    }
    return hash % HASH_SIZE;
}

void insert_to_hash(char *data, hash *hash_table[], int index, int name)
{
    hash *new_hash = (hash *)malloc(sizeof(hash));
    hash *current;
    line_node *new_line;
    if (new_hash == NULL)
    {
        MEMORY_FAIL
    }
    new_hash->name = NULL;
    new_hash->data = NULL;
    new_hash->next = NULL;

    if (name) /*if its a new macro and we have to make a new node for the macro*/
    {
        new_hash->name = malloc(strlen(data) + 1);
        if (new_hash->name == NULL)
        {
            MEMORY_FAIL;
        }
        strcpy(new_hash->name, data);
    }
    else /*if its an excisting macro and we need to insert the lines to it*/
    {
        new_line = (line_node *)malloc(sizeof(line_node));
        if (new_line == NULL)
        {
            MEMORY_FAIL;
        }
        new_line->line_data = malloc(strlen(data) + 1);
        if (new_line->line_data == NULL)
        {
            MEMORY_FAIL;
        }
        strcpy(new_line->line_data, data);
        new_line->next = NULL;

        current = hash_table[index];
        if (current == NULL)
        {
            hash_table[index] = new_hash;
            new_hash->data = new_line;
        }
        else
        {
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
        }

        free(new_hash);
        return;
    }
    new_hash->next = hash_table[index];
    hash_table[index] = new_hash;
}

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

struct symbol *create_symbol()
{
    struct symbol *new_symbol = malloc(sizeof(struct symbol));
    if (new_symbol == NULL)
    {
        MEMORY_FAIL;
    }
    memset(new_symbol->symbol_name, DEFAULT, MAX_LINE);
    new_symbol->address = DEFAULT;
    new_symbol->symbol_type = code_type;
    new_symbol->next = NULL;
    return new_symbol;
}

void first_intilization(struct translation_unit *program)
{
    program->symbol_table = create_symbol();
    program->ext_table = create_ext();
}

struct ext *create_ext()
{
    struct ext *new_ext = malloc(sizeof(struct ext));
    if (new_ext == NULL)
    {
        MEMORY_FAIL;
    }
    memset(new_ext->ext_name, DEFAULT, MAX_LINE);
    new_ext->address_head = NULL;
    new_ext->address_counter = DEFAULT;
    new_ext->next = NULL;
    new_ext->prev = NULL;
    return new_ext;
}

struct entry *create_entry()
{
    struct entry *new_entry = malloc(sizeof(struct entry));
    if (new_entry == NULL)
    {
        MEMORY_FAIL;
    }
    memset(new_entry->entry_name, DEFAULT, MAX_LINE);
    new_entry->address = DEFAULT;
    new_entry->next = NULL;
    new_entry->prev = NULL;
    return new_entry;
}

struct translation_unit *create_translation_unit()
{
    struct translation_unit *new_unit = malloc(sizeof(struct translation_unit));
    if (new_unit == NULL)
    {
        MEMORY_FAIL;
    }
    memset(new_unit->code_array, DEFAULT, sizeof(new_unit->code_array));
    memset(new_unit->data_array, DEFAULT, sizeof(new_unit->data_array));
    new_unit->symbol_table = NULL;
    new_unit->ext_table = NULL;
    new_unit->IC = DEFAULT;
    new_unit->DC = DEFAULT;
    new_unit->entry_table = NULL;
    new_unit->entry_count = DEFAULT;
    new_unit->exter_count = DEFAULT;
    return new_unit;
}

void add_entry(struct translation_unit *program, char *symbol_name, int address)
{
    struct entry *entry_symbol;
    struct entry *current_entry;

    entry_symbol = (struct entry *)malloc(sizeof(struct entry));
    if (entry_symbol == NULL)
    {
        MEMORY_FAIL;
    }
    strcpy(entry_symbol->entry_name, symbol_name);
    entry_symbol->address = address;
    entry_symbol->next = NULL;
    entry_symbol->prev = NULL;

    if (program->entry_table == NULL) /*If its the first entry in the table it will be in the head*/
    {
        program->entry_table = entry_symbol;
    }
    else /*if its not the first ,inserting the entry in the end of the list*/
    {
        current_entry = program->entry_table;
        while (current_entry->next != NULL)
        {
            current_entry = current_entry->next;
        }
        current_entry->next = entry_symbol;
        entry_symbol->prev = current_entry;
    }
    program->entry_count++;
    sort_entries_by_address(&program->entry_table);
}

void free_translation_unit(struct translation_unit *program)
{
    struct symbol *current_symbol;
    struct symbol *next_symbol;
    struct ext *current_ext;
    struct ext *next_ext;
    struct entry *current_entry;
    struct entry *next_entry;
    struct address *current_address;
    struct address *next_address;
    int i;

    current_symbol = program->symbol_table;
    while (current_symbol) /*free the symbole table*/
    {
        next_symbol = current_symbol->next;
        free(current_symbol);
        current_symbol = next_symbol;
    }
    program->symbol_table = NULL;
    current_ext = program->ext_table;
    while (current_ext) /*free the extr table*/
    {
        next_ext = current_ext->next;
        current_address = current_ext->address_head;
        if (program->exter_count > 0)
        {
            while (current_address) /*free each address so we will not lose pointer*/
            {
                next_address = current_address->next_address;
                free(current_address);
                current_address = next_address;
            }
        }
        current_ext->address_head = NULL;
        free(current_ext);
        current_ext = next_ext;
    }
    program->ext_table = NULL;
    current_entry = program->entry_table;
    while (current_entry) /*free entry table*/
    {
        next_entry = current_entry->next;
        free(current_entry);
        current_entry = next_entry;
    }
    program->entry_table = NULL;
    for (i = 0; i < program->IC; i++) /*settign back the valuse to deafult*/
    {
        program->code_array[i] = DEFAULT;
        program->data_array[i] = DEFAULT;
    }
    program->IC = DEFAULT;
    program->DC = DEFAULT;
    program->entry_count = DEFAULT;
    program->exter_count = DEFAULT;
    free(program); /*free the whole translation unit struct after we freed all of his noeds*/
}

struct symbol *symbol_search(struct symbol *head, char *name)
{
    struct symbol *current = head;
    while (current != NULL)
    {
        if (strcmp(current->symbol_name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

struct translation_unit symbol_insert(struct symbol **head, struct symbol *new_symbol, struct ast ast, struct translation_unit *program, int ic, int dc)

{
  

    if (ast.label_name[0] == '\0')/*if we dont have label name in the begining of the line and its an istruction*/

    {
        memset(new_symbol, 0, sizeof(struct symbol));
        strcpy(new_symbol->symbol_name, ast.label_name);
        strcpy(new_symbol->symbol_name, ast.line_type_data.inst.label_array[FIRST_WORD]);
        new_symbol->symbol_type = (ast.line_type_data.inst.inst_type == entry) ? entry_type : extrn_type;
        
    }

    else

    {
        new_symbol = (struct symbol *)malloc(sizeof(struct symbol));
        strcpy(new_symbol->symbol_name, ast.label_name);
        new_symbol->address = ast.line_type == command_line ? ic : dc;
        new_symbol->symbol_type = ast.line_type == command_line ? code_type : data_type;
        
        
    }

    new_symbol->next = *head;
    *head = new_symbol;
    return *program;
}

void add_address(struct translation_unit *program, struct ext *ext_found, int address)
{
    struct address *new_address;
    struct address *current_address;
    current_address = NULL;

    new_address = (struct address *)malloc(sizeof(struct address));
    if (new_address == NULL)
    {
        MEMORY_FAIL;
    }

    new_address->address = address;
    new_address->next_address = NULL;
    new_address->prev_address = NULL;

    if (ext_found->address_head == NULL) /*first initilizing if the adress is null*/
    {
        ext_found->address_head = new_address;
    }
    else /*insering the new adress to the end of the struct so it will be in order*/
    {
        current_address = ext_found->address_head;
        if (ext_found->address_counter > 0)
        {
            while (current_address->next_address != NULL)
            {
                current_address = current_address->next_address;
            }
        }
        current_address->next_address = new_address;
        new_address->prev_address = current_address;
    }
    ext_found->address_counter++;
}

void add_extr(struct translation_unit *program, char *symbol_name, int address)
{
    struct ext *curr_ext;
    struct ext *new_ext;
    new_ext = (struct ext *)malloc(sizeof(struct ext));
    if (new_ext == NULL)
    {
        MEMORY_FAIL;
    }
    new_ext->address_head = (struct address *)malloc(sizeof(struct address));
    if (new_ext->address_head == NULL)
    {
        MEMORY_FAIL;
    }

    strcpy(new_ext->ext_name, symbol_name);
    new_ext->address_head->address = address;
    new_ext->address_head->next_address = NULL;
    new_ext->address_head->prev_address = NULL;
    new_ext->next = NULL;
    new_ext->prev = NULL;
    new_ext->address_counter = 1;
    if (program->ext_table == NULL) /*is its the first external in the table*/
    {
        program->ext_table = new_ext;
    }
    else /*if its not the first external we will add it in the end of the struct to keep*/
    {
        curr_ext = program->ext_table;
        while (curr_ext->next != NULL)
        {
            curr_ext = curr_ext->next;
        }
        curr_ext->next = new_ext;
        new_ext->prev = curr_ext;
    }

    program->exter_count++;
}

struct ext *ext_search(struct ext *head, char *name)
{
    struct ext *current = head;
    while (current != NULL)
    {
        if (strcmp(current->ext_name, name) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void sort_entries_by_address(struct entry **head)
{
    struct entry *current, *index;
    int temp_address;
    char temp_name[MAX_LINE];
    if (head == NULL || *head == NULL)
    {
        return;
    }

    for (current = *head; current != NULL; current = current->next)
    {
        for (index = current->next; index != NULL; index = index->next)
        {
            if (current->address > index->address)
            {
                temp_address = current->address;
                current->address = index->address;
                index->address = temp_address;

                strcpy(temp_name, current->entry_name);
                strcpy(current->entry_name, index->entry_name);
                strcpy(index->entry_name, temp_name);
            }
        }
    }
}

#endif