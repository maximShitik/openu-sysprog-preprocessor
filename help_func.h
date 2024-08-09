#ifndef HELP_FUNC_H
#define HELP_FUNC_H
/*
The functions in this file are used in many other files in the project
to prevent of using the same code on diffrent missons and be as most generic
as possible .
*/

/**
 * @brief Trimming the white spaces in the line.
 *
 * @param str
 */
void trim_whitespace(char *str);

/**
 * @brief Frees the memory of a string array.
 * @param array
 * @param size
 */
void free_string_array(char **array, int size);

/**
 * @brief Splits a line of text into tokens, handling quoted strings and commas.
 *
 * This function takes a line of text and breaks it into an array of tokens,
 * where tokens can be words, numbers, or commas. It processes quoted strings
 * as single tokens and trims whitespace from the beginning and end of each token.
 *
 * @param line A pointer to the line of text to be split. The line is modified
 *              in-place to replace delimiters with null terminators.
 *
 * @return A struct sep_line containing:
 * line: An array of strings where each element is a token fromthe input line.
 *  Commas are included as separate tokens.
 * line_number:The total number of tokens in the line array.
 *
 * @note The function assumes that struct sep_line has enough space in its
 *  linearray to hold all tokens. If the array size is exceeded,
 *   behavior is undefined.
 *
 */
struct sep_line split_line(char *line);

/**
 * @brief Checks if a string contains any uppercase letters.
 * Returns 1 if it does, 0 otherwise.
 *
 * @param str
 * @return int
 */
int contains_uppercase(const char *str);

/**
 * @brief Identifies the type of system name (command, instruction, or register).
 * if the name is not a system name, returns LABEL.
 * label cannot be any syatem name so if its not a system name it can be a label.
 * @param line
 * @return int
 */
int system_names(char *line);

/**
 * @brief Resets a string array to NULL values.
 *
 * @param array
 * @param size
 */
void reset_string_array(char **array, int size);

/**
 * @brief resets a line to NULL values.
 *
 * @param line
 * @param size
 */
void reset_line(char line[], int size);

#endif
