#ifndef BACK_END_H
#define BACK_END_H
#include "first_and_second_pass.h"
#include <stdlib.h>
#include <string.h>

void print_object_file(char *file_name, struct translation_unit *program);
void print_ext_file(char *file_name, struct translation_unit *program);
void print_entry_file(char *file_name, struct translation_unit *program);
void printing_files(char *file_name, struct translation_unit *program);

#endif
