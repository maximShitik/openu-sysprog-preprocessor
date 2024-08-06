#ifndef SECOND_PASS_H
#define SECOND_PASS_H
#include "first_pass.h"
int second_pass(char *file_name, FILE *am_file, struct translation_unit *program, struct hash *hash_table[]);
void free_translation_unit(struct translation_unit *program);
#endif
