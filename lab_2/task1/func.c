#include "str_oper_valid.h"

//-l подсчёт длины переданной строки, переданной вторым аргументом;
int strlength(char *str, size_t *length)
{
    if (!length)
    {
        return WRONG_POINTER;
    }
    *length = 0;
    while (*str++ != '\0')
    {
        ++(*length);
    }
    return 0;
}

int check_unsigned_int(char *str, unsigned int *x)
{
    unsigned int res = 0;
    int sign = 0;
    printf("%s", str);
    if (*str == '-')
    {
        sign = -1;
        ++str;
    }
    while (*str != '\0')
    {
        if ((*str >= '0') && (*str <= '9'))
        {
            if (res > res * 10 + (*str - '0'))
            {
                return UINT_OVERFLOW;
            }
            res = res * 10 + (*str - '0');
        }
        else
        {
            return NOT_NUM;
        }
        ++str;
    }
    if (sign == (-1))
    {
        return NEGATIVE;
    }
    *x = res;
    return 0;
}
int valid(int argc, char *argv[], char *fl, unsigned int *seed)
{
    enum err mistake = 0;
    if (argc < 3)
    {
        return ARGUMETS;
    }
    *fl = argv[1][1];
    int is_fl = (('u' == *fl) || ('l' == *fl) || ('r' == *fl) || ('u' == *fl) || ('n' == *fl) || ('c' == *fl));
    if (!((argv[1][0] == '-') && (is_fl) && (argv[1][2] == '\0')))
    {
        return WRONG_FLAG;
    }
    if (*fl == 'c')
    {
        if (argc < 4)
        {
            return ARGUMETS;
        }
        if (mistake = check_unsigned_int(argv[2], seed))
        {
            return mistake;
        }
        return 0;
    }
    if (argc > 3)
    {
        return ARGUMETS;
    }
    return 0;
}

//-r получить новую строку, являющуюся перевёрнутой (reversed) переданной вторым аргументом строкой;
int reverse(char **str_new, char const *str)
{
    size_t length = 0, i;
    if (strlength((char *)str, &length))
    {
        return WRONG_POINTER;
    }
    if (!(*str_new = (char *)malloc(sizeof(char) * (length + 1))))
    {
        return MEMORY_ERROR;
    }
    for (i = 0; i < length; ++i)
    {
        *(*str_new + i) = *(str + length - i - 1);
    }
    *(*str_new + i) = '\0';
    return 0;
}
/*-u получить новую строку, идентичную переданной вторым аргументом, при этом
каждый символ, стоящий на нечётной позиции (первый символ строки находится на
позиции 0), должен быть преобразован в верхний регистр;*/
int up_letter(char **str_new, char const *str)
{
    size_t length = 0;
    int i = 0;
    if (!str)
    {
        return WRONG_POINTER;
    }
    if (strlength((char *)str, &length))
    {
        return WRONG_POINTER;
    }
    if (!(*str_new = (char *)malloc(sizeof(char) * (length + 1))))
    {
        return MEMORY_ERROR;
    }
    for (i = 0; i < length; ++i)
    {
        // printf("%d", (1 & i));
        (*str_new)[i] = str[i] + ((((1 & i)) && (str[i] >= 'a') && (str[i] <= 'z')) ? 'A' - 'a' : 0);
    }
    (*str_new)[i] = '\0';
    return 0;
}

/*-n получить из символов переданной вторым аргументом строки новую строку так,
чтобы в начале строки находились символы цифр в исходном порядке, затем
символы букв в исходном порядке, а в самом конце – все остальные символы, также
в исходном порядке;*/
int group_str(char **new_str, char *str)
{
    size_t length = 0;
    int i = 0;
    enum err mistake = 0;
    int end_str_new = 0;
    if ((mistake = strlength(str, &length)) != 0)
    {
        return mistake;
    }
    if (!(*new_str = (char *)malloc(sizeof(char) * ((length) + 1))))
    {
        return MEMORY_ERROR;
    }

    for (i = 0; i < length; ++i)
    {
        if (('0' <= str[i]) && ('9' >= str[i]))
        {
            (*new_str)[end_str_new++] = str[i];
        }
    }
    for (i = 0; i < length; ++i)
    {
        if ((('a' <= str[i]) && ('z' >= str[i])) || (('A' <= str[i]) && ('Z' >= str[i])))
        {
            (*new_str)[end_str_new++] = str[i];
        }
    }
    for (i = 0; i <= length; ++i)
    {
        if (!((('a' <= str[i]) && ('z' >= str[i])) || (('A' <= str[i]) && ('Z' >= str[i]))) && !(('0' <= str[i]) && ('9' >= str[i])))
        {
            (*new_str)[end_str_new++] = str[i];
        }
    }
    return 0;
}

/*-c получить новую строку, являющуюся конкатенацией второй, четвёртой, пятой и т.
д. переданных в командную строку строк; строки конкатенируются в
псевдослучайном порядке; для засеивания генератора псевдослучайных чисел
функцией srand используйте seed равный числу, переданному в командную строку
третьим аргументом.*/
int str_cat(char *str_new, char *str1, size_t *length_str_new)
{
    size_t length1 = 0;
    int i = 0;
    enum err mistake = 0;
    if (mistake = strlength(str1, &length1))
    {
        return mistake;
    }
    for (i = 0; i <= length1; ++i)
    {
        str_new[*length_str_new + i] = str1[i];
    }
    (*length_str_new) += length1;
    return 0;
}

int cat_random_str(char *str_array[], char **str_new, int seed, int cnt)
{
    size_t length = 0;
    int i, capacity = 0;
    enum err mistake = 0;
    srand(seed);
    *str_new = "\0";
    for (i = 0; i < cnt; ++i)
    {
        if (mistake = strlength(str_array[i], &length))
        {
            return mistake;
        }
        capacity += length;
    }
    if (!(*str_new = (char *)malloc(sizeof(char) * capacity)))
    {
        return MEMORY_ERROR;
    }
    length = 0;
    for (i = 0; i < cnt; ++i)
    {
        int j = rand() % (cnt);
        str_cat(*str_new, str_array[j], &length);
        //    printf("%s %d\n", *str_new, j);
    }
    return 0;
}