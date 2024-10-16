#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE (sizeof(long int) << 3 + 1 + 1)

enum err
{
    OK,
    NUM_TOO_BIG,
    BUFER_OVERFLOW,
    NOT_NUM,
    NOT_SUIT_BASE_VALUE,
    NOT_CORRECT_NUM,
    UNCORRECT_IN_THIS_BASE,
    ONLY_BASE,
    STOP
};

int read_check_base(int *base)
{
    char buf[BUFSIZE];
    int mistake = scanf("%66s", buf);
    if (strlen(buf) >= BUFSIZE)
    {
        return NUM_TOO_BIG;
    }
    *base = 0;
    int i = (buf[0] == '-') ? 1 : 0;
    while (i < strlen(buf))
    {
        if (!isdigit(buf[i]))
        {
            return NOT_NUM;
        }
        if ((*base * 10 + buf[i] - '0') >= 0)
        {
            *base = *base * 10 + buf[i] - '0';
        }
        else
        {
            return NUM_TOO_BIG;
        }
        ++i;
    }
    if ((buf[0] == '-') || ((*base > 36) || (*base < 2)))
    {
        return NOT_SUIT_BASE_VALUE;
    }
    return 0;
}

int to_base_10(char *str, int base_from, long int *num, int *fl)
{
    char *pt_str = str;
    enum err mistake = 0;
    int str_len = strlen(str);
    if (str_len > sizeof(long int) * 8)
    {
        return NUM_TOO_BIG;
    }
    if (str[0] == '-')
    {
        ++pt_str;
    }
    if (strcmp(str + str_len - 4, "Stop") == 0)
    {
        *(str + str_len - 4) = 0;
        mistake = STOP;
    }
    while (*pt_str != '\0')
    {
        *fl = 1;
        if ((((*pt_str >= 'A') && (*pt_str <= 'Z')) || (!isalnum(*pt_str))) && (!isspace(*pt_str)))
        {
            return NOT_CORRECT_NUM;
        }
        if ((*pt_str - ((isalpha(*pt_str)) ? ('a' - 10) : ('0'))) >= base_from)
        {
            return UNCORRECT_IN_THIS_BASE;
        }
        if (*num < 0)
        {
            return NUM_TOO_BIG;
        }
        *num = *num * base_from + *pt_str - ((isalpha(*pt_str)) ? ('a' - 10) : ('0'));
        ++pt_str;
    }
    return mistake;
}

int find_max(char *str, int base_from, long int *max_num, int *fl)
{
    long int num = 0;
    enum err mistake = to_base_10(str, base_from, &num, fl);
    if (*fl == 0)
    {
        return ONLY_BASE;
    }
    if (num > *max_num)
    {
        *max_num = num;
    }
    return mistake;
}

void to_base(long int num, int base_to, char *str, char **str_begin)
{
    int y;
    *str_begin = str + sizeof(long int) * 8;
    while (num > 0)
    {
        y = num % base_to;
        (*(--(*str_begin))) = ((y > 9) ? ('a' + y - 10) : ('0' + y));
        num /= base_to;
    }
    return;
}

int main()
{

    int base = 0;
    long int max_num = -1;
    enum err mistake = read_check_base(&base);
    int max_len_num = sizeof(long int) << 3; // максимальная длина 2-го числа влезающего в long int
    char buf[BUFSIZE];
    char str[max_len_num + 1];
    int fl = 0;

    while ((mistake == 0) && (mistake != STOP))
    {
        if (scanf("%66s", buf) == 1)
        {
            mistake = find_max(buf, base, &max_num, &fl);
        }
        else
        {
            mistake = NUM_TOO_BIG;
            break;
        }
    }
    switch (mistake)
    {
    case STOP:
        char *str_begin = NULL;
        *(str + max_len_num) = 0;

        printf("В 10-ой %ld\n", max_num);
        to_base(max_num, 8, str, &str_begin);
        printf("В 8-ой %s\n", str_begin);

        to_base(max_num, 18, str, &str_begin);
        printf("В 18-ой %s\n", str_begin);

        to_base(max_num, 27, str, &str_begin);
        printf("В 27-ой %s\n", str_begin);

        to_base(max_num, 36, str, &str_begin);
        printf("В 36-ой %s\n", str_begin);
        break;
    case ONLY_BASE:
        printf("Введена только база\n");
        break;
    case NUM_TOO_BIG:
        printf("Переполенение типа long int\n");
        break;
    case NOT_NUM:
        printf("Аргумент не является целым числом\n");
        break;
    case NOT_SUIT_BASE_VALUE:
        printf("Значение базы должно находится в отрезке [2;36]\n");
        break;
    case NOT_CORRECT_NUM:
        printf("Число записано не верное, либо не является числом\n");
        break;
    case UNCORRECT_IN_THIS_BASE:
        printf("Данный аргумент не может быть записан в %d-ой системе исчисления\n", base);
        break;
    case BUFER_OVERFLOW:
        printf("\n");
        break;
    }
    return 0;
}