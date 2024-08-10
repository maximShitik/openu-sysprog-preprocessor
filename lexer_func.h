#ifndef LEXER_FUNC_H
#define LEXER_FUNC_H
#include "lexer.h"
#include "help_func.h"
#include "data_structs.h"

/*
The functions in this file are used to help the lexer do its job and prosess the line in to an AST.
There are 2 types of functions here :
1)the global functions that are used to check if the line is an int , string and process it.
2)the functions that are used to set the data in the AST.
*/


/**
 * @brief Prosses the line from the file and checks if the label is valid.
 * 
 * @param line 
 * @param ast 
 * @return int 
 */
int process_label(char *line, struct ast *ast);

/**
 * @brief Prosses the line from the file and checks if the instruction is valid.
 * 
 * @param line 
 * @param ast 
 * @return int 
 */
int process_instruction(char *line, struct ast *ast);


/**
 * @brief Prosses the line from the file and checks if the token is valid.
 *
 * @param line
 * @param ast
 * @return int false if the token is invalid,

 */
int process_token(char *line, struct ast *ast);

/**
 * @brief Resets the AST and sets the line type as error.
 * 
 * @param ast 
 * @param error_message 
 */
void error_found(struct ast *ast, char *error_message);



/**
 * Resets an AST node to its default state.
 */
void reset_ast(struct ast *node);


/**
 * @brief Checks if the line is a command or an instruction.
 *
 * @param ast
 * @return int
 */
int check_command_or_instruction(struct ast *ast);

/**
 * Checks if a string can be converted to an integer.
 * Returns 1 if it can, 0 otherwise.
 */
int is_int(char *line, struct ast *ast);

/**
 * @brief reseting the AST and setting the line type as error with the error message.
 * 
 * @param ast 
 * @param error_message 
 */
void error_found(struct ast *ast, char *error_message);

/**
 * @brief Checks if a string is a valid string.
 *
 * @param line
 * @param ast
 * @return int
 */
int is_string(char *line, struct ast *ast);


/**
 * @brief Set the data object
 *
 * @param ast
 * @param sep
 * @return struct ast
 */
struct ast set_data(struct ast *ast, struct sep_line sep,int first_operand);

/**
 * @brief Set the string object
 *
 * @param ast
 * @param sep
 * @return struct ast
 */
struct ast set_string(struct ast *ast, struct sep_line sep);



/**
 * @brief Checks the given line for an instruction and sets in the AST.
 *
 * @param ast
 * @param sep
 */
void set_instruction(struct ast *ast, struct sep_line sep);

/**
 * @brief Set the entry extern object
 *
 * @param ast
 * @param sep
 * @return struct ast
 */
struct ast set_entry_extern(struct ast *ast, struct sep_line sep);


/**
 * @brief Set the label object by checkig which label array is empty and setting in the AST.
 * 
 *
 * @param ast
 * @param sep
 * @param index
 * @return struct ast
 */
struct ast set_label(struct ast *ast, struct sep_line sep, int index);



/**
 * @brief Searching the command name in the command map and setting the command name in the AST.
 *
 * @param ast
 * @param command
 */
void set_command_name(struct ast *ast, char *command);

/**
 * @brief Sets the amount of argumets in the line
 *  
 * @param ast
 * @param sep
 * @return int
 */
int set_argument_amount(struct ast *ast, struct sep_line sep);



#endif