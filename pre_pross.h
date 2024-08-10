#ifndef PRE_PROSS_H
#define PRE_PROSS_H
#include "data_structs.h"
#include <stdio.h>
#include "lexer.h"
enum
{
    min_hash_index = 0,
    Error = -1,
    regular_line = -2,
    endmacro = -3,
    macro_defenition = -4,
    first_int=-5
};

/**
 * @brief Checks the line and returns the type of line
 * if there is an error it will return Error and will set the error message in the error array
 * @param line 
 * @param separated 
 * @param error 
 * @return int 
 */
int line_defenition(char *line, struct sep_line separated, char error[MAX_LINE]);

/**
 * @brief Pre-processes an assembly file to expand macros and generate an intermediate file.
 *
 * This function processes the input assembly file, handling macro definitions and expansions.
 * It generates an intermediate am file with expanded macros and retains the original lines that are not part of a macro definition.
 * At the end of the process, the temporary .as file created earlier is deleted.
 *
 * @param line A buffer to store each line read from the file.
 * @param hash_table An array of hash pointers used to store macro definitions.
 * @param input The name of the input file to be pre-processed.
 * @param line_map An array that maps the expanded lines to their original line numbers.
 * @param expanded_line_count A pointer to an integer that will store the total count of expanded lines.
 * @return The name of the generated am file, or NULL if an error occurred.
 *
 * @note The function dynamically allocates memory for line_copy, as_file_name, and am_file_name.
 *       The .as file created during processing is deleted at the end of the function.
 *       If an error occurs during processing, the function will clean up by freeing allocated memory and closing open files.
 */
char *pre_prossesor(char *line, hash *hash_table[], char *input, line_mapping line_map[], int *expanded_line_count) ;

/**
 * @brief Create an as and am files 
 * 
 * @param input 
 * @param as_file_name 
 * @param am_file_name 
 * @param am_file 
 * @param as_file 
 */
void create_as_and_am_files(char *input, char **as_file_name, char **am_file_name, FILE **am_file, FILE **as_file);

/**
 * @brief Copy the source file to the target file using the fwrite function so we wont change the original file entered by user
 * 
 * @param source_file 
 * @param target_file 
 * @return int 
 */
int copy_file(const char *source_file, const char *target_file);
#endif