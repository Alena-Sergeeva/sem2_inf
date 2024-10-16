/*Через аргументы командной строки программе подаются флаг (1 аргументом),
строка (2 аргументом); и (только для флага -c) целое число типа unsigned int и
далее произвольное количество строк. Флаг определяет действие, которое необходимо
выполнить над переданными строками:
● -c получить новую строку, являющуюся конкатенацией второй, четвёртой, пятой и т.
д. переданных в командную строку строк; строки конкатенируются в
псевдослучайном порядке; для засеивания генератора псевдослучайных чисел
функцией srand используйте seed равный числу, переданному в командную строку
третьим аргументом.
Созданные функциями строки должны располагаться в выделенной из динамической кучи памяти.
Продемонстрируйте работу реализованных функций.*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

enum err
{
    OK,
    NOT_NUM,
    WRONG_FLAG,
    NUM_TOO_BIG,
    WRONG_POINTER,
    MEMORY_ERROR,
    ARGUMETS
};

int valid(int argc, char *argv[], char *fl, unsigned int *seed);

int strlength(char *str, size_t *length);
int reverse(char **str_new, char const *str);
int up_first_letter(char **str_new, char const *str);

int main(int argc, char *argv[])
{
    char *str = "asjdjfe";
    size_t length = 0;
    char *str_new = NULL;
    reverse(&str_new, str);
    printf("%zd\nstr_new: %s\n", length, str_new);
    free(str_new);
    str_new = NULL;
    up_first_letter(&str_new, str);
    printf("str_new1: %s\n", str_new);
    free(str_new);
    /*
    char fl = ' ';
    enum err f_mistake = 0;
    unsigned int seed = 0;
    switch (valid(argc, argv, &fl, &seed))
    {
    case OK:
        switch (fl)
        {
        case 'l':
            mistake = strlength(str, &length);
            break;
        case 'r':
            mistake = reverse(&str_new, str);
            printf("%zd\nstr_new: %s\n", length, str_new);
            free(str_new);
            break;
        case 'u':

            break;
        case 'n':

            break;
        case 'c':

            break;
        }
        break;
    case WRONG_FLAG:
        printf("Флаг введен не верно\n");
        break;
    case NOT_NUM:
        printf("Строка не вляется десятичным целым числом\n");
        break;
    case NUM_TOO_BIG:
        printf("переполнение типа long int\n");
        break;
    case ARGUMETS:
        printf("Введено неверное колличество аргументов\n");
        break;
    }
*/
    return 0;
}

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
int up_first_letter(char **str_new, char const *str)
{
    size_t length = 0;
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
    *(*str_new + length) = '\0';
    while (--length > 0)
    {
        *(*str_new + length) = *(str + length);
    }
    if ((*str >= 'a') && (*str <= 'z'))
    {
        *(*str_new) = *str + 'A' - 'a';
    }
    return 0;
}

/*-n получить из символов переданной вторым аргументом строки новую строку так,
чтобы в начале строки находились символы цифр в исходном порядке, затем
символы букв в исходном порядке, а в самом конце – все остальные символы, также
в исходном порядке;*/
int group_str(char *str, char **new_str)
{
    char *str_end = str;
    int length = 0;
    enum err mistake = 0;
    if ((mistake = strlength(str, &length)) != 0)
    {
        return mistake;
    }
    if (*new_str = (char *)malloc(sizeof(char) * (strlength(str, &length) + 1)))
    {
        return MEMORY_ERROR;
    }
    while (*str_end++ != '/0')
    {
        if (('0' <= *str_end) && ('9' >= *str_end))
        {
            **new_str = *str_end;
            ++new_str;
        }
    }
    str_end = str;
    while (*str_end++ != '/0')
    {
        if ((('a' <= *str_end) && ('z' >= *str_end)) || (('A' <= *str_end) && ('Z' >= *str_end)))
        {
            **new_str = str_end;
            ++new_str;
        }
    }
    while (*str++ != '/0')
    {
        if (!((('a' <= *str_end) && ('z' >= *str_end)) || (('A' <= *str_end) && ('Z' >= *str_end))) && !(('0' <= *str_end) && ('9' >= *str_end)))
        {
            **new_str = str_end;
            ++new_str;
        }
    }
    return 0;
}

/*-c получить новую строку, являющуюся конкатенацией второй, четвёртой, пятой и т.
д. переданных в командную строку строк; строки конкатенируются в
псевдослучайном порядке; для засеивания генератора псевдослучайных чисел
функцией srand используйте seed равный числу, переданному в командную строку
третьим аргументом.*/
int str_cat(char *str1, char *str2, char **str_new)
{
    int length1 = 0, length2 = 0;
    enum err mistake = 0;
    if ((mistake = strlength(str1, &length1)) || (mistake = strlength(str2, &length2)))
    {
        return mistake;
    }
    if (*str_new = (char *)malloc(sizeof(char) * (length1 + length2 + 1)))
        ;
    {
        return MEMORY_ERROR;
    }
    while (str1 != '\0')
    {
        **str_new = *str1++;
        ++str_new;
    }
    while (str2 != '\0')
    {
        **str_new = *str2++;
        ++str_new;
    }
    return 0;
}
int cat_rundom_str(char *str, char **str_new, int seed, int cnt)
{

    for (i = 0; i < cnt)
}