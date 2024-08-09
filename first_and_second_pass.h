#ifndef FIRST_AND_SECOND_PASS_H
#define FIRST_AND_SECOND_PASS_H
#define SOURCE_AND_TARGET_ARE_REGISTERS (line_ast.line_type_data.command.opcode_type[0].command_type == reg || line_ast.line_type_data.command.opcode_type[0].command_type == reg_direct) && (line_ast.line_type_data.command.opcode_type[1].command_type == reg || line_ast.line_type_data.command.opcode_type[1].command_type == reg_direct)

#include "lexer.h"
#include <stdio.h>

/**
 * @brief Performs the first pass of the assembly file.
 *
 * This function parses each line of the assembly file, processes labels, updates the symbol table, and calculates 
 * the instruction and data counters (IC and DC). 
 * It handles errors such as syntax errors, redefinitions, and long lines.
 *
 * @param file_name The name of the assembly file being processed.
 * @param am_file A pointer to the file stream of the assembly file.
 * @param program A pointer to the translation unit structure holding the program's data.
 * @param hash_table An array of pointers to hash table structures for managing symbols.
 * @param line_map An array mapping expanded lines to their original line numbers.
 * @param expanded_line_count The total number of expanded lines in the assembly file.
 * @return int Returns 0 if the first pass is successful, otherwise returns 1 if an error occurs.
 */
int first_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], line_mapping line_map[], int expanded_line_count) ;
int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], line_mapping line_map[], int expanded_line_count);
struct symbol *symbol_search(struct symbol *head, char *name);
#endif
