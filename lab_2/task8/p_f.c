/*Реализуйте функцию с переменным числом аргументов, вычисляющую сумму
переданных целых неотрицательных чисел в заданной системе счисления с
основанием [2..36]. Параметрами функции являются основание системы счисления, в
которой производится суммирование, количество переданных чисел, строковые
представления чисел в заданной системе счисления. Десятичное представление
переданных чисел может быть слишком велико и не поместиться во встроенные типы
данных; для решения возникшей проблемы также реализуйте функцию «сложения в
столбик» двух чисел в заданной системе счисления, для её использования при
реализации основной функции. Результирующее число не должно содержать ведущих
нулей. Продемонстрируйте работу функции.*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

enum err
{
    OK,
    CNT_ARGUMENTS,
    MEMORY_ERROR,
    WRONG_BASE,
    WRONG_POINTER,
    NOT_IN_THIS_BASE,
    OVERFLOW_CAPACITY
};

int strlength(char *str, int *length)
{
    if ((length == NULL) || (str == NULL))
    {
        return WRONG_POINTER;
    }
    char *str_end = str;
    while (*str_end++ != '\0')
    {
    }
    *length = str_end - str - 1;
    return 0;
}

int miss_nuls(char **str_res)
{
    if (str_res == NULL)
    {
        *str_res = NULL;
        return WRONG_POINTER;
    }
    while (**str_res == '0')
    {
        (*str_res)++;
    }
    if (**str_res == '\0')
    {
        (*str_res)--;
    }
    return 0;
}

int to_int(char c, int *num, int base)
{
    *num = base;
    if ((c >= 'a') && (c <= 'z'))
    {
        *num = c - 'a' + 10;
    }
    if ((c >= 'A') && (c <= 'Z'))
    {
        *num = c - 'A' + 10;
    }
    if ((c >= '0') && (c <= '9'))
    {
        *num = c - '0';
    }
    if (*num >= base)
    {
        return NOT_IN_THIS_BASE;
    }
    return 0;
}

int check_enogth_mem(char **buf, int *capacity, int max_length, int length1)
{
    char *buf_old_end = NULL, *new_buf = NULL;
    if (max_length + 1 >= *capacity)
    {
        // printf("ll");
        buf_old_end = *buf + *capacity - 1;
        if (*capacity << 1 > max_length + 1)
        {
            *capacity <<= 1;
        }
        else
        {
            *capacity += (max_length + 1);
        }
        if (buf_old_end - *buf + 1 > *capacity)
        {
            return OVERFLOW_CAPACITY;
        }
        // printf("%d", *capacity);
        if (!(new_buf = (char *)malloc(sizeof(char) * (*capacity))))
        {
            free(*buf);
            *buf = NULL;
            return MEMORY_ERROR;
        }
        for (int j = *capacity - 1; j >= *capacity - 1 - length1; --j)
        {
            new_buf[j] = *buf_old_end--;
        }
        free(*buf);
        *buf = new_buf;
    }
    return 0;
}

int sum_num_b(char **buf, char **str1, char *str2, int base, int *capacity)
{
    int i = 0, length1 = 0, length2 = 0, num1 = 0, num2 = 0, num = 0, ost = 0;
    char *buf_begin = NULL, *buf_old = NULL;
    enum err mistake = 0;
    if (miss_nuls(str1) || miss_nuls(&str2))
    {
        return WRONG_POINTER;
    }
    // printf("%s, %s", *str1, str2);
    if (strlength(*str1, &length1) || strlength(str2, &length2))
    {
        return WRONG_POINTER;
    }
    // printf("%d %d\n", length1, length2);
    if (mistake = check_enogth_mem(buf, capacity, max(length1, length2), length1))
    {
        return mistake;
    }
    *str1 = *buf + *capacity - 2;
    for (i = 0; i < max(length1, length2); ++i)
    {
        if (i >= length1)
        {
            num1 = 0;
        }
        else
        {
            if (mistake = to_int(**str1, &num1, base))
            {
                return mistake;
            }
        }
        if (i >= length2)
        {
            num2 = 0;
        }
        else
        {
            if (mistake = to_int(str2[length2 - i - 1], &num2, base))
            {
                return mistake;
            }
        }

        num = (ost + num1 + num2) % base;
        ost = (ost + num1 + num2) / base;

        **str1 = (num > 9) ? ('a' + num - 10) : ('0' + num);
        --(*str1);
    }
    if (ost != 0)
    {
        **str1 = (ost > 9) ? ('a' + ost - 10) : ('0' + ost);
    }
    else
    {
        (*str1)++;
    }
    return 0;
}

int sum_base(char **buf, char **str1, int base, int cnt, ...)
{
    va_list iterator;
    int i = 0, capacity = 2;
    char *str2 = NULL;
    enum err mistake = 0;
    if (cnt <= 0)
    {
        return CNT_ARGUMENTS;
    }
    if ((base < 2) || (base > 36))
    {
        return WRONG_BASE;
    }
    *buf = (char *)malloc(sizeof(char) * capacity);
    if (*buf == NULL)
    {
        return MEMORY_ERROR;
    }
    (*buf)[capacity - 1] = '\0';
    (*buf)[0] = '0';
    *str1 = *buf + capacity - 1;
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        str2 = va_arg(iterator, char *);
        mistake = sum_num_b(buf, str1, str2, base, &capacity);
        if (mistake)
        {
            *str1 = str2;
            return mistake;
        }
    }
    va_end(iterator);
    return 0;
}

int main()
{
    char *buf = NULL, *str_res = NULL;
    int base = 36;
    int length = 0;
    switch (sum_base(&buf, &str_res, base, 4, "08", "00000ZZZZZZZZZZZZZZZZZZZZZZZZZZ", "0000000000000", "1"))
    {
    case OK:
        if (miss_nuls(&str_res))
        {
            printf("Введен не верный указатель\n");
        }
        else
        {
            printf("%s\n", str_res);
        }
        break;
    case CNT_ARGUMENTS:
        printf("Колличество аргументов должно быть положительным\n");
        break;
    case MEMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    case WRONG_BASE:
        printf("Значение базы должно быть в пределах отрезка [2;36]\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель, либо переданное колличество аргументов превышает их фактическое колличество\n");
        break;
    case NOT_IN_THIS_BASE:
        printf("Число %s не может быть записанно в %d-ой СС\n", str_res, base);
        break;
    case OVERFLOW_CAPACITY:
        printf("Переполнение значения вместимости буфера\n");
        break;
    }
    free(buf);
    return 0;
}