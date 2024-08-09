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
 * @brief Folding the as file in to the am file and folding the macros if there are any
 * 
 * @param line 
 * @param hash_table 
 * @param input 
 * @param line_map 
 * @param expanded_line_count 
 * @return char* 
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
 * @brief Copy the source file to the target file so we wont change the original file entered by user
 * 
 * @param source_file 
 * @param target_file 
 * @return int 
 */
int copy_file(const char *source_file, const char *target_file);
#endif
