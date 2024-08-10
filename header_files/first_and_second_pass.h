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
 * The functions codes in to the translation unit the instruction type lines
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


/**
 * @brief Sets the first and second word of the instruction in the code array based on the given AST and translation unit by the cours book rules
 * 
 * This function processes the first and second operands of a command from the abstract syntax tree (AST) and
 * updates the instruction code in the translation unit accordingly.
 * It handles various operand types and setsthe appropriate bits in the instruction code array.
 * 
 * The function codes the command lines:
 * OP_CODE_SET: sets the operation code in the instruction array.
 * SOURCE_METHOD: macros set specific bits for the source operand type.
 * TARGET_METHOD: macros set specific bits for the target operand type.
 * ARE:sets the addressing mode bits in the instruction array.
 * 
 * @param line_ast The abstract syntax tree containing the command and its operands.
 * @param program Pointer to the translation unit containing the code array .
 */
void set_first_word(struct ast line_ast, struct translation_unit *program);


/**
 * @brief Processes the assembly code in the second pass to generate machine code and cpmmand lines.
 * 
 * This function reads lines from an assembly file, parses them into an abstract syntax tree (AST), and
 * processes each line to generate machine code based on the instruction and its operands. It also handles
 * label resolutions and updates the external symbols and addresses accordingly.
 * 
 * The function performs the following steps:
 * Reads lines from the provided assembly file.
 * Parses each line into an AST.
 * For lines with string instructions, frees the allocated memory for the string array.
 * For command lines, sets the appropriate bits in the instruction code array based on the operands.
 * Handles various operand types including numbers, registers, and labels.
 * Resolves labels by looking them up in the symbol table and updating the code.
 * Adds addresses to the external symbol table if necessary.
 * Reports errors for undefined labels and updates the error flag.
 * 
 * @param file_name The name of the assembly file being processed.
 * @param am_file A file pointer to the assembly file.
 * @param program Pointer to the translation unit containing the code array, symbol table, and other info.
 * @param hash_table The hash table used for macro expansion.
 * @param line_map Array mapping expanded line numbers to original line numbers.
 * @param expanded_line_count The number of lines in the expanded file.
 * 
 * @return int Returns 1 if an error occurred during processing, otherwise returns 0.
 */
int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[], line_mapping line_map[], int expanded_line_count);


#endif