
#include "first_pass.h"
#include "second_pass.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

void print_object_file(char *file_name, struct translation_unit *);
void print_ext_file(char *file_name, struct translation_unit *program);
void print_entry_file(char *file_name, struct translation_unit *program);
void printing_files(char *file_name, struct translation_unit *program);

