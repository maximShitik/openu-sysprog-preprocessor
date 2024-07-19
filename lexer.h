#ifndef LEXER_H
#define LEXER_H
#define MAX_LINE 80
#define MAX_LABEL 32
#define FALSE 0
#define TRUE 1
#define SPACES " \t\n , "

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
                mov = 1,
                cmp,
                add,
                sub,
                lea,

                nt,
                clr,
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
                    reg,
                    label

                } command_type;

                char *labell[2];
                int numberr;
                int regg;

            } opcode_type[2];

        } command;

    } line_type_data;
    struct
    {
        char type[300];

    } error;
    struct
    {
        enum
        {
            A,
            R,
            E
        } ARE_type;
    } ARE;

    char label_name[MAX_LABEL];

} ast;

struct ast parse_line(char *line);
struct ast one_group_command(struct ast *ast, struct sep_line sep, char *command);
int operand_group(struct ast *ast, char *command);

#endif
