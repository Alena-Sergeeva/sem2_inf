#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tgmath.h>
#include <limits.h>

#define eps __DBL_EPSILON__

enum err
{
    OK,
    TRUE,
    FALSE,
    WRONG_CNT,
    MEMMORY_ERROR,
    WRONG_DIAP,
    OVER_FLOW,
    WRONG_BASE,
    WRONG_POINTER
};

int gcd(long int a, long int b, long int *del)
{
    if (del == NULL)
    {
        return WRONG_POINTER;
    }
    while ((a != 0) && (b != 0))
    {
        if (a > b)
        {
            a = a % b;
        }
        else
        {
            b = b % a;
        }
    }
    *del = a + b;
    return OK;
}

int is_base_degree(long int num, int base, enum err *fl)
{
    long int num1 = base;
    if (fl == NULL)
    {
        return WRONG_POINTER;
    }
    while ((num % base == 0) && (num > 0))
    {
        num /= base;
        // printf("%ld ", num);
    }
    (1 == num) ? (*fl = TRUE) : (*fl = FALSE);
    return OK;
}

int is_end_dec(double num, int base, enum err *res)
{
    long int denominator = 1, numerator = 0, del = 0;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    while (fabs(num - ((double)numerator) / denominator) > eps) // то есть равны, разобраться с машинным eps
    {
        if ((denominator * 10 == LONG_MAX) || (denominator * 10 * num == LONG_MAX))
        {
            return OVER_FLOW;
        }
        denominator *= 10;
        numerator = (long int)floor(num * denominator);
    }
    if (gcd(numerator, denominator, &del))
    {
        return WRONG_POINTER;
    }
    if (is_base_degree(denominator / del, base, res))
    {
        return WRONG_POINTER;
    }
    return OK;
}
void print_res(enum err *res, int cnt)
{

    for (int i = 0; i < cnt; ++i)
    {
        switch (res[i])
        {
        case FALSE:
            printf("false\n");
            break;
        case TRUE:
            printf("true\n");
            break;
        case WRONG_DIAP:
            printf("число находится вне диапазона (0,1)\n");
            break;
        case OVER_FLOW:
            printf("Переподенеие типа\n");
            break;
        }
    }
    return;
}

int is_ending_in_this_base(enum err **res, int base, int cnt, ...)
{
    va_list iterator;
    int i;
    double num;
    enum err mistake;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    if (base < 2)
    {
        return WRONG_BASE;
    }
    if (cnt <= 0)
    {
        return WRONG_CNT;
    }
    if (!(*res = malloc(sizeof(enum err) * cnt)))
    {
        return MEMMORY_ERROR;
    }
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        num = va_arg(iterator, double);
        if ((num >= eps + 1.0) || (num <= eps) || (fabs(num - 1.0) <= eps)) // eps
        {
            (*res)[i] = WRONG_DIAP;
        }
        else
        {
            if (mistake = is_end_dec(num, base, *res + i))
            {
                (*res)[i] = mistake;
            }
        }
    }
    va_end(iterator);
    return OK;
}

int main()
{
    enum err mistake = 0;
    enum err *res = NULL;
    int cnt = 5;
    switch (is_ending_in_this_base(&res, 4, cnt, 1.6666666666666667, 0.125, 0.0625, 0.0, -1.9))
    {
    case OK:
        print_res(res, cnt);
        free(res);
        res = NULL;
        break;
    case WRONG_CNT:
        printf("Колличество аргументов должно быть положительным\n");
        break;
    case MEMMORY_ERROR:
        printf("Не удалось выделить память для хранения результатов\n");
        break;
    case WRONG_BASE:
        printf("Значение базы должно быть не меннее 2\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    }
    return 0;
}