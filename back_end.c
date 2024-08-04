
#ifndef BACK_END_C
#define BACK_END_C
#include "back_end.h"
int decimal_to_octal(int decimalNumber)
{
    int octalNumber = 0;
    int i;
    i = 1;
    while (decimalNumber != 0)
    {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }

    return octalNumber;
}

void print_object_file(char *file_name, struct translation_unit *program)
{
    FILE *object_file;
    char *object_file_name;
    struct symbol *current_symbol;
    int i;
    object_file_name = (char *)malloc(strlen(file_name) + 3);
    if (object_file_name == NULL)
    {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    strcpy(object_file_name, file_name);
    strcat(object_file_name, ".ob");
    object_file = fopen(object_file_name, "w");
    if (object_file == NULL)
    {
        printf("Error creating object file\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < program->IC; i++)
    current_symbol = program->symbol_table;
        fprintf(object_file, "%d %d\n",program->symbol_table->address, decimal_to_octal(program->code_array[i]));
    for (i = 0; i < program->DC; i++)
        fprintf(object_file, "%d\n", decimal_to_octal(program->data_array[i]));
    fflush(object_file);
    fclose(object_file);
}

void print_ext_file(char *file_name, struct translation_unit *program)
{
    FILE *ext_file;
    char *ext_file_name;
    int i;
    struct address *current_address;
    if (program->exter_count == 0)
        return;
    else
    {
        ext_file_name = (char *)malloc(strlen(file_name) + 4);
        if (ext_file_name == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(ext_file_name, file_name);
        strcat(ext_file_name, ".ext");
        ext_file = fopen(ext_file_name, "w");
        if (ext_file == NULL)
        {
            printf("Error creating external file\n");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < program->exter_count; i++)
        {
            current_address = program->ext_table[i].address_head;
            while (current_address != NULL)
            {
                fprintf(ext_file, "%s %d\n", program->ext_table[i].ext_name, current_address->address);
                current_address = current_address->next_address;
            }
        }
        fflush(ext_file);
        fclose(ext_file);
    }
}

void print_entry_file(char *file_name, struct translation_unit *program)
{
    FILE *entry_file;
    char *entry_file_name;
    struct entry *current_entry;
    int i;
    if (program->entry_count == 0)
        return;
    else
    {
        entry_file_name = (char *)malloc(strlen(file_name) + 5);
        if (entry_file_name == NULL)
        {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        strcpy(entry_file_name, file_name);
        strcat(entry_file_name, ".ent");
        entry_file = fopen(entry_file_name, "w");
        if (entry_file == NULL)
        {
            printf("Error creating entry file\n");
            exit(EXIT_FAILURE);
        }
        current_entry = program->entry_table;
        while (current_entry != NULL)
        {
            fprintf(entry_file, "%s %d\n", current_entry->entry_name, current_entry->address);
            current_entry = current_entry->next;
        }
        fflush(entry_file);
        fclose(entry_file);
    }
}

void printing_files(char *file_name, struct translation_unit *program)
{
    print_object_file(file_name, program);
    print_ext_file(file_name, program);
    print_entry_file(file_name, program);
}

#endif
