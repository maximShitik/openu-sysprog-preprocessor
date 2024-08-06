#ifndef HELP_FUNC_H
#define HELP_FUNC_H



void trim_whitespace(char *str);
struct sep_line next_word(char *line);
int contains_uppercase(const char *str);
int system_names(char *line);
void reset_string_array(char **array, int size);
void remove_backslashes(char *str);


#endif
