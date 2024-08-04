#ifndef FIRST_PASS_H
#define FIRST_PASS_H
#define SOURCE_AND_TARGET_ARE_REGISTERS (line_ast.line_type_data.command.opcode_type[0].command_type == reg || line_ast.line_type_data.command.opcode_type[0].command_type == reg_direct) && (line_ast.line_type_data.command.opcode_type[1].command_type == reg || line_ast.line_type_data.command.opcode_type[1].command_type == reg_direct)

#include "lexer.h"
#include <stdio.h>


int first_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[]);
struct symbol *symbol_search(struct symbol *head, char *name);
void free_translation_unit(struct translation_unit *program);
#endif
