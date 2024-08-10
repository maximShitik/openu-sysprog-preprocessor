#ifndef BACK_END_H
#define BACK_END_H
#include "first_and_second_pass.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Prints the object file.ob for the given file.
 *
 * This function creates and writes to the object file based on the code and data arrays in the translation unit. 
 * It outputs the content in octal format.
 *
 * @param file_name The name of the file (without extension) to generate the object file.
 * @param program Pointer to the translation unit containing the program's code and data.
 */
void print_object_file(char *file_name, struct translation_unit *program);

/**
 * @brief Prints the external file.ext for the given program.
 * 
 * @param file_name The name of the file (without extension) to generate the external file.
 * @param program Pointer to the translation unit containing the program's external references.
 */
void print_ext_file(char *file_name, struct translation_unit *program);


/**
 * @brief Prints the entry file.ent for the given program.
 * 
 * @param file_name The name of the file (without extension) to generate the entry file.
 * @param program Pointer to the translation unit containing the program's entry references.
 */
void print_entry_file(char *file_name, struct translation_unit *program);

/**
 * @brief Generates and prints the object, external, and entry files for the given program.
 * 
 * @param file_name The name of the file (without extension) to generate the files.
 * @param program Pointer to the translation unit containing the program's data.
 */
void printing_files(char *file_name, struct translation_unit *program);

#endif