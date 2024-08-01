#ifndef LEXER_H
#define LEXER_H
#define MAX_LINE 81
#define FIRST_WORD 0
#define SECOND_WORD 1
#define THIRD_WORD 2
#define MAX_LABEL 32
#define MAX_DATA 300
#define FALSE 0
#define TRUE 1
#define SPACES " \t\n , "
#include "data_structs.h"

struct sep_line
{
    char *line[MAX_LINE];
    int line_number;
};

typedef enum return_types
{
    error = 1,
    instruction,
    command,
    registerr,
    Entry,
    Extrn,

} return_types;

typedef struct ast
{

    enum line_type
    {

        error_line = 1,
        inst_line,
        command_line,
        empty_line

    } line_type;

    union line_type_data
    {

        struct inst
        {
            enum inst_type
            {
                lable = 1,
                string,
                entry,
                extrn,
                data
            } inst_type;

            char *label_array[2]; /*entry/extern*/
            int data_array[80];
            char *string_array[50];
            int data_counter;

        } inst;

        struct command
        {
            enum
            {
                mov = 0,
                cmp,
                add,
                sub,
                lea,
                clr,
                nt,
                inc,
                dec,
                jmp,
                bne,
                red,
                prn,
                jsr,
                rts,
                stop
            } opcode;

            struct
            {
                enum
                {
                    none,
                    number,
                    label,
                    reg,
                    reg_direct
                } command_type;

                char *labell[2];
                int numberr;
                int regg;

            } opcode_type[2];

        } command;

    } line_type_data;
    struct
    {
        char type[MAX_DATA];

    } error;
    struct
    {
        enum
        {
            E,
            R,
            A
        } ARE_type;
    } ARE;

    char label_name[MAX_LABEL];
    int argument_count;

} ast;

struct ast parse_line(char *line, struct hash *hash_table[]);
struct ast one_group_command(struct ast *ast, struct sep_line sep, char *command);
int operand_group(struct ast *ast, char *command);

#endif
