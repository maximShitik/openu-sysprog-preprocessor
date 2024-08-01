#include "first_pass.h"
#define OP_CODE_SET program->code_array[program->IC] = line_ast.line_type_data.command.opcode << 11;
#define SOURCE_METHOD_0 program->code_array[program->IC] |= 1 << 7;
#define SOURCE_METHOD_1 program->code_array[program->IC] |= 1 << 8;
#define SOURCE_METHOD_2 program->code_array[program->IC] |= 1 << 9;
#define SOURCE_METHOD_3 program->code_array[program->IC] |= 1 << 10;
#define TARGET_METHOD_0 program->code_array[program->IC] |= 1 << 3;
#define TARGET_METHOD_1 program->code_array[program->IC] |= 1 << 4;
#define TARGET_METHOD_2 program->code_array[program->IC] |= 1 << 5;
#define TARGET_METHOD_3 program->code_array[program->IC] |= 1 << 6;
#define ARE program->code_array[program->IC] |= 1 << 2;
#define BITS_6_8 6
#define BITS_3_5 3
#define SET_SOURCE program->code_array[program->IC] = line_ast.line_type_data.command.opcode_type[FIRST_WORD]
#define SET_TARGET program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[SECOND_WORD]
#define SOURCE_AND_TARGET_ARE_REGISTERS (line_ast.line_type_data.command.opcode_type[0].command_type == reg || line_ast.line_type_data.command.opcode_type[0].command_type == reg_direct) && (line_ast.line_type_data.command.opcode_type[1].command_type == reg || line_ast.line_type_data.command.opcode_type[1].command_type == reg_direct)
#define COMMAND_TYPE line_ast.line_type_data.command.opcode_type[i].command_type
#define SET_ADRESS program->code_array[program->IC] = line_ast.line_type_data.command.opcode_type[i]
int second_pas(struct translation_unit *program, const char *file_name, FILE am_file);
