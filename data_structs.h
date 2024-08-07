#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H
#define HASH_SIZE 100
#define MAX_LINE 81
#define MAX_ADDRESS 1000

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

typedef struct {
    int original_line_number;
    int is_macro_expansion;
    char macro_name[MAX_LINE];
    int macro_def_line_number;
} line_info;

struct address
{
    int address;
    struct address *next_address;
    struct address *prev_address;

};

struct ext
{
    char ext_name[MAX_ADDRESS];
    struct address *address_head;
    int address_counter;
    struct ext *next;
    struct ext *prev;
};

struct entry
{
    char entry_name[MAX_ADDRESS];
    int address;
    struct entry *next;
    struct entry *prev;
};

struct translation_unit
{
    int code_array[MAX_ADDRESS];
    int data_array[MAX_ADDRESS];
    struct symbol *symbol_table;
    struct ext *ext_table;
    int IC;
    int DC;
    struct entry *entry_table;
    int entry_count;
    int exter_count;
};
/*HASH*/
unsigned int hash_function(char *name);
void hash_reset(hash *hash_table[]);
void insert_to_hash(char *data, hash *hash_table[], int index, int is_name);
void free_hash(hash *hash_table[]);
hash *search_in_hash(char *macro_name, hash *hash_table[]);



/*---------LINED LIST FUNC------------*/
void symbol_insert(struct symbol **head, struct symbol *new_symbol);
struct symbol *symbol_search(struct symbol *head, char *name);
void free_translation_unit(struct translation_unit *program);
void add_entry(struct translation_unit *program, char *symbol_name, int address);
struct translation_unit *create_translation_unit();
struct entry *create_entry();
struct ext *create_ext();
struct symbol *create_symbol();
void add_extr(struct translation_unit *program, char *symbol_name, int address);
struct ext *ext_search(struct ext *head, char *name);
void add_address(struct translation_unit *program, struct ext *ext_found, int address);
void first_intilization(struct translation_unit *program);
#endif
