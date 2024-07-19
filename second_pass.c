#include "first_pass.h"
#include <string.h>

void convert_opcode_to_char(char *command, struct ast *ast)
{
    if (ast->line_type_data.command.opcode == mov)
        command = "mov";
    else if (ast->line_type_data.command.opcode == cmp)
        command = "cmp";
    else if (ast->line_type_data.command.opcode == add)
        command = "add";
    else if (ast->line_type_data.command.opcode == sub)
        command = "sub";
    else if (ast->line_type_data.command.opcode == lea)
        command = "lea";
    else if (ast->line_type_data.command.opcode == clr)
        command = "clr";
    else if (ast->line_type_data.command.opcode == nt)
        command = "not";
    else if (ast->line_type_data.command.opcode == inc)
        command = "inc";
    else if (ast->line_type_data.command.opcode == dec)
        command = "dec";
    else if (ast->line_type_data.command.opcode == jmp)
        command = "jmp";
    else if (ast->line_type_data.command.opcode == bne)
        command = "bne";
    else if (ast->line_type_data.command.opcode == red)
        command = "red";
    else if (ast->line_type_data.command.opcode == prn)
        command = "prn";
    else if (ast->line_type_data.command.opcode == jsr)
        command = "jsr";
    else if (ast->line_type_data.command.opcode == rts)
        command = "rts";
    else if (ast->line_type_data.command.opcode == stop)
        command = "stop";
}

int second_pas(struct translation_unit *program, const char *file_name, FILE *am_file)
{
    char line[MAX_LINE];
    int is_error;
    int line_number;
    struct ast line_ast = {0};
    struct symbol *symbol_found;
    struct ext *ext_found;
    int i;
    char char_command[MAX_LINE];

    while (fgets(line, sizeof(line), am_file))
    {
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\r')
        { /*skipping the eampty lines and note lines*/
            continue;
        }
        line_ast = parse_line(line); /*parsing the line to an AST*/

        /*building the first word*/
        if (line_ast.line_type == command)
        {
            convert_opcode_to_char(char_command, &line_ast);

            /*setting the opcode tramslated to oktal*/
            program->code_array[program->IC] = 
            program->code_array[program->IC] = *char_command<<5;

        }
    }
}
