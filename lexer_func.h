#ifndef LEXER_FUNC_H
#define LEXER_FUNC_H
#include "lexer.h"
#include "help_func.h"
#include "data_structs.h"
#define SPACES " \t\n , "
void error_found(struct ast *ast, char *error_message);
struct ast set_label(struct ast *ast, struct sep_line sep, int index);
struct ast set_lea(struct ast *ast, struct sep_line sep);
int process_token(char *line, struct ast *ast);
int is_int(char *line, struct ast *ast);
int is_string(char *line, struct ast *ast);
void reset_ast(struct ast *node);
void error_found(struct ast *ast, char *error_message);
struct ast set_entry_extern(struct ast *ast, struct sep_line sep);
int check_command_or_instruction(struct ast *ast);
struct ast set_data(struct ast *ast, struct sep_line sep);
void set_instruction(struct ast *ast, struct sep_line sep);
void set_command_name(struct ast *ast, char *command);
#endif
