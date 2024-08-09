#ifndef MACROS_H
#define MACROS_H

#define SPACES " \t\n , "
#define BITS_6_8 6
#define BITS_3_5 3
#define LABEL_ARRAY_SIZE 2
#define STRING_ARRAY_SIZE 50
#define FIRST_WORD 0
#define SECOND_WORD 1
#define THIRD_WORD 2
#define MAX_LABEL 32
#define MAX_DATA 4069
#define FALSE 0
#define TRUE 1
#define LABEL 1
#define DEAFULT 0
#define SYMBOLS "! @ # $ ^ & * ( ) _ + - = { } [ ] | \\ : ; \" ' < > , . ? / ` ~"
#define ONE_OPCODES_COUNT 5
#define TWO_OPCODES_COUNT 14
#define HASH_SIZE 100
#define MAX_LINE 83
#define MAX_LINE_LIMIT 81
#define MAX_ADDRESS 1000

#define OP_CODE_SET program->code_array[program->IC] = line_ast.line_type_data.command.opcode << 11;
#define SOURCE_METHOD_0 program->code_array[program->IC] |= 1 << 7;
#define SOURCE_METHOD_1 program->code_array[program->IC] |= 1 << 8;
#define SOURCE_METHOD_2 program->code_array[program->IC] |= 1 << 9;
#define SOURCE_METHOD_3 program->code_array[program->IC] |= 1 << 10;
#define TARGET_METHOD_0 program->code_array[program->IC] |= 1 << 3;
#define TARGET_METHOD_1 program->code_array[program->IC] |= 1 << 4;
#define TARGET_METHOD_2 program->code_array[program->IC] |= 1 << 5;
#define TARGET_METHOD_3 program->code_array[program->IC] |= 1 << 6;
#define ARE program->code_array[program->IC] |= 4;
#define SET_SOURCE program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[FIRST_WORD]
#define SET_TARGET program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[SECOND_WORD]
#define COMMAND_TYPE line_ast.line_type_data.command.opcode_type[i].command_type
#define SET_ADRESS program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[i]
#define PRN_AND_ONE_OPERAND(cmp) (ast->line_type_data.command.opcode == prn && ast->line_type_data.command.opcode_type[SECOND_WORD].command_type cmp none)
#define OP_CODE_ARRAY {{"mov", mov}, {"cmp", cmp}, {"add", add}, {"sub", sub}, {"lea", lea}, {"not", nt}, {"clr", clr}, {"inc", inc}, {"dec", dec}, {"jmp", jmp}, {"bne", bne}, {"red", red}, {"prn", prn}, {"jsr", jsr}, {"rts", rts}, {"stop", stop}};
#define CMPR_LINE_INST(index1, index2, inst) (strcmp(sep.line[(index1)], (inst)) == 0 || strcmp(sep.line[(index2)], (inst)) == 0)
#define JMP_BNE_JSR (ast->line_type_data.command.opcode==jmp||ast->line_type_data.command.opcode==bne||ast->line_type_data.command.opcode==jsr)



#define MEMORY_FAIL                      \
    printf("Memory allocation error\n"); \
    exit(EXIT_FAILURE);

#define FREE_AND_REMOVE   \
    free(as_file_name);   \
    free(am_file_name);   \
    remove(am_file_name); \
    free_hash(hash_table);

#define STRCAT_AND_MALLOC(target, source, extention)                  \
    (target = (char *)malloc(strlen(source) + strlen(extention) + 1), \
     strcpy(target, source), strcat(target, extention))

#define CALCULATE_LINE_OFFSET                                                       \
    line_map[expanded_line_number].original_line_number = original_line_number;     \
    line_map[expanded_line_number].expanded_line_number = expanded_line_number + 1; \
    expanded_line_number++;

#define SET_NUMBER(first, second, num)                                      \
    ast->line_type_data.command.opcode_type[(first)].command_type = number; \
    ast->line_type_data.command.opcode_type[(second)].numberr = (num);

#define SET_REGISTER(index)  \
    ast->line_type_data.command.opcode_type[(index)].command_type = reg_direct;   \
    ast->line_type_data.command.opcode_type[(index)].regg = atoi(sep.line[current] + (strpbrk(sep.line[current], "*") ? 2 : 1)); \
    ast->line_type_data.command.opcode_type[(index)].command_type = strpbrk(sep.line[current], "*") ? reg : reg_direct;

#endif
