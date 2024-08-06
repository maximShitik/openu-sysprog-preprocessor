
#ifndef BACK_END_C
#define BACK_END_C

#define OB 4
#define ENT_OR_EXT 5

#include "back_end.h"
#include "macros.h"

void print_object_file(char *file_name, struct translation_unit *program)
{
    int octal_base[] = {0, 1, 2, 3, 4, 5, 6, 7};
    FILE *object_file;
    char *object_file_name;
    int i, j;
    int line_number = 100;

    object_file_name = (char *)malloc(strlen(file_name) + OB);
    if (object_file_name == NULL)
    {
        MEMORY_FAIL;
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
    {
        fprintf(object_file, "%04d ", line_number);

        int value = program->code_array[i];
        if (value < 0)
        {
            value = (~(-value) + 1) & 0x7FFF; /*Convert negative to positive and mask*/
        }
        else
        {
            value = value & 0x7FFF; /*making it in the 15 but format*/
        }
        for (j = 4; j >= 0; j--)
        {
            fprintf(object_file, "%d", octal_base[(value >> (3 * j)) & (j == 4 ? 0x7F : 0x7)]);
        }
        fprintf(object_file, "\n");
        line_number++;
    }
    for (i = 0; i < program->DC; i++)
    {
        fprintf(object_file, "%04d ", line_number);

        int value = program->data_array[i];
        if (value < 0)
        {
            value = (~(-value) + 1) & 0x7FFF;
        }
        else
        {
            value = value & 0x7FFF;
        }
        for (j = 4; j >= 0; j--)
        {
            fprintf(object_file, "%d", octal_base[(value >> (3 * j)) & (j == 4 ? 0x7F : 0x7)]);
        }
        fprintf(object_file, "\n");
        line_number++;
    }

    fflush(object_file);
    fclose(object_file);
    free(object_file_name);
}

void print_ext_file(char *file_name, struct translation_unit *program)
{
    FILE *ext_file;
    char *ext_file_name;
    int i;
    struct ext *current_ext;
    struct address *current_address;
    if (program->exter_count == 0)
        return;
    else
    {
        ext_file_name = (char *)malloc(strlen(file_name) + ENT_OR_EXT);
        if (ext_file_name == NULL)
        {
            MEMORY_FAIL;
        }
        strcpy(ext_file_name, file_name);
        strcat(ext_file_name, ".ext");
        ext_file = fopen(ext_file_name, "w");
        if (ext_file == NULL)
        {
            printf("Error creating external file\n");
            exit(EXIT_FAILURE);
        }

        current_ext = program->ext_table;

        while (current_ext != NULL)
        {
            current_address = current_ext->address_head;
            if (current_ext->address_counter > 0)
            {
                while (current_address != NULL)
                {
                    fprintf(ext_file, "%s %04d \n",current_ext->ext_name, current_address->address);
                    current_address = current_address->next_address;
                }
            }
            else
            {
                fprintf(ext_file, "%s %04d \n", current_ext->ext_name, current_ext->address_head->address);
            }
            current_ext = current_ext->next;
            
        }

        fflush(ext_file);
        fclose(ext_file);
    }
    free(ext_file_name);
}

void print_entry_file(char *file_name, struct translation_unit *program)
{
    FILE *entry_file;
    char *entry_file_name;
    struct entry *current_entry;
    if (program->entry_count == 0)
        return;

    entry_file_name = (char *)malloc(strlen(file_name) + ENT_OR_EXT);
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
        free(entry_file_name);
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
    free(entry_file_name);
}

void printing_files(char *file_name, struct translation_unit *program)
{
    print_object_file(file_name, program);
    print_ext_file(file_name, program);
    print_entry_file(file_name, program);
}

#endif
