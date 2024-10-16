#ifndef STR_OPER_VALID_H_
#define STR_OPER_VALID_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

enum err
{
    OK,
    NOT_NUM,
    WRONG_FLAG,
    UINT_OVERFLOW,
    WRONG_POINTER,
    MEMORY_ERROR,
    ARGUMETS,
    NEGATIVE
};

int strlength(char *str, size_t *length);
int reverse(char **str_new, char const *str);
int up_letter(char **str_new, char const *str);
int str_cat(char *str_new, char *str1, size_t *length_str_new);
int group_str(char **new_str, char *str);
int cat_random_str(char *str_array[], char **str_new, int seed, int cnt);
int valid(int argc, char *argv[], char *fl, unsigned int *seed);

#endif