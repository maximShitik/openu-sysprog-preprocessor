#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#define HASH_SIZE 100

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

unsigned int hash_function(char *macro_name);
void hash_reset(hash *hash_table[]);
void insert_to_hash(char *data, hash *hash_table[], int index, int is_macro_name);
void free_memory(hash *hash_table[]);
hash *search_macro_in_hash(char *macro_name, hash *hash_table[],int index);

#endif
