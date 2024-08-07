#ifndef MACROS_H
#define MACROS_H
#define MEMORY_FAIL                          \
    do                                       \
    {                                        \
        printf("Memory allocation error\n"); \
        exit(EXIT_FAILURE);                  \
    } while (0)

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
#define BITS_6_8 6
#define BITS_3_5 3
#define SET_SOURCE program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[FIRST_WORD]
#define SET_TARGET program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[SECOND_WORD]
#define COMMAND_TYPE line_ast.line_type_data.command.opcode_type[i].command_type
#define SET_ADRESS program->code_array[program->IC] |= line_ast.line_type_data.command.opcode_type[i]
#define FREE_AND_REMOVE   \
    free(as_file_name);   \
    free(am_file_name);   \
    remove(am_file_name); \
    free_hash(hash_table);
#define STRCAT_MALLOC(dest, str1, str2)                      \
    (dest = (char *)malloc(strlen(str1) + strlen(str2) + 1), \
     strcpy(dest, str1), strcat(dest, str2))

#define FREE_TRANSU_HASH_CLOSE      \
    free_translation_unit(program); \
    free(am_file_name);             \
    free_hash(hash_table);          \
    fclose(am_file);                \

#endif
