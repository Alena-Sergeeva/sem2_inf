#include "str_oper_valid.h"

int main(int argc, char *argv[])
{
    char fl = ' ';
    enum err mistake = 0;
    unsigned int seed = 0;
    size_t length = 0;
    char *str_new = NULL;
    switch (valid(argc, argv, &fl, &seed))
    {
    case OK:
        switch (fl)
        {
        case 'l':
            mistake = strlength(argv[2], &length);
            break;
        case 'r':
            mistake = reverse(&str_new, argv[2]);
            break;
        case 'u':
            mistake = up_letter(&str_new, argv[2]);
            break;
        case 'n':
            mistake = group_str(&str_new, argv[2]);
            break;
        case 'c':
            cat_random_str(argv + 3, &str_new, seed, argc - 3);
            break;
        }
        if (mistake == MEMORY_ERROR)
        {
            printf("Не удалось выделить память\n");
            return 0;
        }
        if (mistake == WRONG_POINTER)
        {
            printf("Был передан неправильный адресс\n");
            return 0;
        }
        if (str_new == NULL)
        {
            printf("%ld\n", length);
        }
        else
        {
            printf("%s\n", str_new);
            free(str_new);
            str_new = NULL;
        }
        break;
    case WRONG_FLAG:
        printf("Флаг введен не верно\n");
        break;
    case NOT_NUM:
        printf("seed должно быть десятичным целым числом\n");
        break;
    case UINT_OVERFLOW:
        printf("переполнение типа unsigned int\n");
        break;
    case ARGUMETS:
        printf("Введено неверное колличество аргументов\n");
        break;
    case NEGATIVE:
        printf("seed должно быть неотрицательным\n");
        break;
    }
    return 0;
}