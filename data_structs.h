#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#define HASH_SIZE 100
#define MAX_LINE 81

typedef struct symbol symbol;
typedef struct symbol_table symbol_table;

typedef struct line_node
{
    char *line_data;
    struct line_node *next;
} line_node;

typedef struct hash
{
    char *name;
    line_node *data;
    struct hash *next;

} hash;

struct symbol
{
    char symbol_name[MAX_LINE];
    int address;
    enum symbol_type
    {
        code_type,
        data_type,
        entry_type,
        extrn_type,
        entry_code_type,
        entry_data_type
    } symbol_type;
    struct symbol *next;
};
struct address
{
    int address;
    struct address *next_address;
};

struct ext
{
    char ext_name[MAX_LINE];
    struct address *address_head;
    int address_counter;
    struct ext *next;
};

struct entry
{
    char entry_name[MAX_LINE];
    int address;
    int entry_count;
    struct entry *next;
};

struct translation_unit
{
    int code_array[MAX_LINE];
    int data_array[MAX_LINE];
    struct symbol *symbol_table;
    struct ext *ext_table;
    int IC;
    int DC;
    struct entry *entry_table;
    int entry_count;
    int exter_count;
};

unsigned int hash_function(char *name);
void hash_reset(hash *hash_table[]);
void insert_to_hash(char *data, hash *hash_table[], int index, int is_name);
void free_hash(hash *hash_table[]);
hash *search_in_hash(char *macro_name, hash *hash_table[]);
#endif
