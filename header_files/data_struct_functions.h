#ifndef DATA_STRUCT_FUNCTIONS_H
#define DATA_STRUCT_FUNCTIONS_H

#include "data_structs.h"

/*************************************************HASH TABLE FUNCTIONS*******************************************/
/**
 * @brief Reseting the hash table and setting all the pointers to NULL
 *
 * @param hash_table
 * @return void
 */
void hash_reset(hash *hash_table[]);


/**
 * @brief calculating the hash value of a macro name
 *
 * @param str
 * @return unsigned int
 */
unsigned int hash_function(char *name);

/**
 * @brief Inserts data into a hash table, either as a new macro or as a line in an existing macro.
 *
 * This function inserts a new macro name or a line of data into a hash table. 
 * If the name parameter is true, it creates a new hash entry for a macro name. 
 * If name is false, it appends the line of data to an existing macro.
 *
 * @param data The data to insert, which can be either a macro name or a line of data.
 * @param hash_table[] The hash table where the data will be inserted.
 * @param index The index in the hash table corresponding to the data.
 * @param name A flag indicating whether the data is a new macro (true) or a line in an existing macro (false).
 *
 * @return void
 */
void insert_to_hash(char *data, hash *hash_table[], int index, int is_name);


/**
 * @brief freeing the memory allocated for the hash table
 *
 * @param hash_table
 */
void free_hash(hash *hash_table[]);

/**
 * @brief Searches for a macro in the hash table by its name.
 *
 * This function searches the hash table for a macro with the specified name using the key given to the name by hash_function.
 * If found, it returns a pointer to the corresponding hash structure.
 * If the macro is not found, it returns `NULL`.
 *
 * @param macro_name The name of the macro to search for.
 * @param hash_table[] The hash table to search within.
 * 
 * @return hash* A pointer to the hash structure if the macro is found, or `NULL` if not.
 */
hash *search_in_hash(char *macro_name, hash *hash_table[]);

/*************************************************HASH TABLE FUNCTIONS*******************************************/


/*************************************************LINED LIST FUNCTIONS*******************************************/

/**
 * @brief Create a symbol object and set it to default values.
 * 
 * @return struct symbol* 
 */
struct symbol *create_symbol();

/**
 * @brief firts intilization of the data structs
 * creating the symbole table and the extern table only
 * 
 * @param program 
 */
void first_intilization(struct translation_unit *program);


/**
 * @brief Create a ext object and set it to default values.
 * 
 * @return struct ext* 
 */
struct ext *create_ext();

/**
 * @brief Create a entry object and set it to default values.
 * 
 * @return struct entry* 
 */
struct entry *create_entry();


/**
 * @brief Create a translation unit object and set it to default values.
 * 
 * @return struct translation_unit* 
 */
struct translation_unit *create_translation_unit();


/**
 * @brief Adds a new entry to the entry table of the translation unit.
 *
 * This function creates a new entry with the specified symbol name and address,
 * and adds it to the end of the entry list in the translation unit. It also increments
 * the entry count in the translation unit.
 *
 * @param program A pointer to the translation unit where the entry will be added.
 * @param symbol_name The name of the symbol to be added as an entry.
 * @param address The address associated with the symbol.
 *
 * @note If memory allocation fails, the function prints an error message and exits the program.
 */
void add_entry(struct translation_unit *program, char *symbol_name, int address);

/**
 * @brief Frees all allocated memory associated with a translation unit.
 *
 * This function deallocates memory used by the symbol table, external symbols table,
 * entry table, and other components of the translation unit.
 *  It also resets the code and data arrays, and resets the instruction and data counters.
 *
 * @param program A pointer to the translation unit to be freed. 
 *  This function will free all memory associated with the translation unit and set all pointers and counters to NULL or zero.
 *
 * @note This function assumes that the translation unit was dynamically allocated.
 *       After calling this function, the program pointer should not be used.
 */
void free_translation_unit(struct translation_unit *program);

/**
 * @brief Searches for a symbol in the symbol table.
 * 
 * This function traverses the linked list of symbols to find a symbol
 * with the specified name.
 * 
 * @param head Pointer to the head of the symbol list.
 * @param name The name of the symbol to search for.
 * @return Pointer to the symbol if found, otherwise NULL.
 */
struct symbol *symbol_search(struct symbol *head, char *name);


/**
 * @brief Inserts a new symbol into the symbol table.
 *
 * This function initializes a new symbol and inserts it into the symbol table based on the provided AST and other parameters.
 * It handles both instruction lines and labels, setting the symbol's properties accordingly.
 *
 * @param head Pointer to the head of the symbol list.
 * @param new_symbol Pointer to the symbol to be initialized and inserted.
 * @param ast The abstract syntax tree containing the parsed line data.
 * @param program Pointer to the translation unit containing the symbol table.
 * @param ic The current instruction counter.
 * @param dc The current data counter.
 *
 * @return The updated translation unit with the new symbol inserted.
 */
struct translation_unit symbol_insert(struct symbol **head, struct symbol *new_symbol, struct ast ast, struct translation_unit *program, int ic, int dc);


/**
 * @brief Adds a new address to the list of addresses associated with an external symbol.
 * 
 * This function appends a new address to the list of addresses for a given
 * external symbol and updates the address counter.
 * 
 * @param program Pointer to the translation unit.
 * @param ext_found Pointer to the external symbol structure.
 * @param address The address to add.
 */
void add_address(struct translation_unit *program, struct ext *ext_found, int address);


/**
 * @brief Adds a new external symbol to the translation unit.
 * 
 * This function creates a new external symbol and adds it to the list of external
 * symbols in the translation unit. It also initializes the address list with
 * the provided address.
 * 
 * @param program Pointer to the translation unit.
 * @param symbol_name The name of the external symbol.
 * @param address The address associated with the external symbol.
 */
void add_extr(struct translation_unit *program, char *symbol_name, int address);

/**
 * @brief Searches for an external symbol in the external symbol list.
 * 
 * This function travels the linked list of external symbols to find a symbol with the specified name.
 * 
 * @param head Pointer to the head of the external symbol list.
 * @param name The name of the external symbol to search for.
 * @return Pointer to the external symbol if found, otherwise NULL.
 */
struct ext *ext_search(struct ext *head, char *name);


/**
 * @brief Sorts a linked list of entries by their address in ascending order.
 *
 * This function iterates through the linked list of entries and sorts them
 * in place based on the addres field in ascending order.
 * The sorting algorithm used is bubble sort.
 *
 * @param head A pointer to the head pointer of the linked list to be sorted.
 */
void sort_entries_by_address(struct entry **head);

#endif