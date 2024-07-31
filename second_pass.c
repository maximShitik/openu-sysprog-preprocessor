#include "first_pass.h"
#include "lexer.h"

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
void set_first_word(struct ast line_ast, struct translation_unit *program)
{
    int i;
    i = 0;
    program->code_array[program->IC] = line_ast.line_type_data.command.opcode << 11;
    

        if (line_ast.line_type_data.command.opcode_type[0].command_type == number)
        {
            program->code_array[program->IC] |= 1 << 7;
        }
        else if (line_ast.line_type_data.command.opcode_type[0].command_type == label)
        {
            program->code_array[program->IC] |= 1 << 8;
        }
        else if (line_ast.line_type_data.command.opcode_type[0].command_type == reg)
        {
            program->code_array[program->IC] |= 1 << 9;
        }
        else if (line_ast.line_type_data.command.opcode_type[0].command_type == reg_direct)
        {
            program->code_array[program->IC] |= 1 << 10;
        }

        if(line_ast.line_type_data.command.opcode_type[1].command_type == none)return;
        if (line_ast.line_type_data.command.opcode_type[1].command_type == number)
        {
            program->code_array[program->IC] |= 1 << 3;
        }
        else if (line_ast.line_type_data.command.opcode_type[1].command_type == label)
        {
            program->code_array[program->IC] |= 1 << 4;
        }
        else if (line_ast.line_type_data.command.opcode_type[1].command_type == reg)
        {
            program->code_array[program->IC] |= 1 << 5;
        }
        else if (line_ast.line_type_data.command.opcode_type[1].command_type == reg_direct)
        {
            program->code_array[program->IC] |= 1 << 6;
        }
    
    program->IC++;
}
int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[])
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
        line_ast = parse_line(line, hash_table); /*parsing the line to an AST*/

        /*building the first word*/
        if (line_ast.line_type == command)
        {
            set_first_word(line_ast, program);
        }
    }
    return 0;
}
