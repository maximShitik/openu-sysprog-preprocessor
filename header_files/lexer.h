#ifndef LEXER_H
#define LEXER_H
#include "macros.h"
#include "data_structs.h"

/*
This file is taking a line from the input file and parsing it to an AST node.
The AST node is then returned.
The file prosses the line and checks :
1) What is the line type (instruction, command, label, error line)
2) If the line is a label, it sets the label name and the next line type (instruction or command).
3) If the line is a command, it sets the command name and the operands by checking the command group by number of operands.
4) If the line is an instruction, it sets the instruction name and the operands.
5) If the line is an error line, it sets the error message and returns an eampty AST node with the error messege only.
*/



/**
 * @brief parese the line and return the AST node
 *
 * @param line
 * @return struct ast
 */
struct ast parse_line(char *line, struct hash *hash_table[]);


/**
 * @brief A for loop checking the command name in the op code map
 *
 * @param ast
 * @param command
 * @return int
 */
int operand_group(struct ast *ast, char *command);

/**
 * @brief Resives from the "set_command" function the number of parameters the opcode expects to get.
 * prossesing the line and setting the AST wuth the right operands.
 * searching lexical errors.
 * 
 * @param ast 
 * @param sep 
 * @param command 
 * @param group_count 
 * @return struct ast 
 */
struct ast process_command_operands(struct ast *ast, struct sep_line sep, char *command, int group_count);

/**
 * @brief setting the command name in the AST and searching for lexical error.
 * 
 * @param ast 
 * @param sep 
 * @param command 
 */
void no_operands_command(struct ast *ast, struct sep_line sep, char *command);

/**
 * @brief Resives from the "operand_group" function the number of parameters the opcode expects to get.
 * setting to the right function with the number of operands
 *
 * @param ast
 * @param sep
 * @param command
 */
void set_command(struct ast *ast, struct sep_line sep, char *command);



/**
 * @brief Check the line type by the inserted line and setting the AST node
 *
 * @param sep
 * @param ast
 * @return struct ast
 */
struct ast line_type(struct sep_line sep, struct ast *ast);
#endif