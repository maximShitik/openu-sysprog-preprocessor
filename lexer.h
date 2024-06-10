

typedef struct ast
{

    enum line_type
    {

        error_line = 1,
        note_line,
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
                data = 1,
                string,
                entry,
                extrn,
                lable
            } inst_type;
            union
            {
                char *label[32]; /*entry/extern*/
                int data;
                char *string[50];
            } inst_opt;
        } inst;

        struct command
        {
            enum
            {
                mov,
                cmp,
                add,
                sub,
                nt,
                clr,
                lea,
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
                union
                {
                    char *label[50];
                    int number;
                    int reg;
                } command_operand;

            } opcode_type[2];

        } command;

        int note;
    } line_type_data;
    struct
    {
        char error[300];
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

} ast;

struct ast parse_line(char *line);
