#ifndef LEXER_H
#define LEXER_H
#define MAX_LINE 80


typedef struct ast
{

    enum line_type
    {

        error_line = 1,
        inst_line,
        command_line,
        empty_line,
        macro_line

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
        char error_token[80];
        int line_number;
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
    struct macro
    {
        char name[80];
        int line_number;
    } macro;


} ast;

struct ast parse_line(char *line);
#endif
