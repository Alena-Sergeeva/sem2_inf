#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <tgmath.h>

#define eps __DBL_EPSILON__

enum err
{
    OK,

    WRONG_CNT,
    MEMMORY_ERROR,
    WRONG_DIAP = -1,
    OVER_FLOW = -2,
    WRONG_BASE
};

int gcd(long int a, long int b, long int *del)
{
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
    return 0;
}

int is_base_degree(long int num, int base, short int *fl)
{
    long int num1 = base;
    while ((num % base == 0) && (num > 0))
    {
        num /= base;
        // printf("%ld ", num);
    }
    (1 == num) ? (*fl = 1) : (*fl = 0);
    return 0;
}
int is_end_dec(double num, int base, short int *res)
{
    long int denominator = 1, numerator = 0, del = 0;
    // isnan isinf?

    while (fabs(num - ((double)numerator) / denominator) > eps) // то есть равны, разобраться с машинным eps
    {
        denominator *= 10;
        numerator = (long int)floor(num * denominator);
        //   printf("%ld ", (long int)floor(num * denominator));
        // printf(" %lf ", ((double)numerator) / denominator);
    }
    //   printf("\n%ld, %ld\n", numerator, denominator);
    gcd(numerator, denominator, &del);
    // printf("delitel : %ld\n", del);

    is_base_degree(denominator / del, base, res);
    return 0;
}
void print_res(short int *res, int cnt)
{

    for (int i = 0; i < cnt; ++i)
    {
        switch (res[i])
        {
        case 0:
            printf("false\n");
            break;
        case 1:
            printf("true\n");
            break;
        case WRONG_DIAP:
            printf("число находится вне диапазона (0,1)\n");
            break;
        case OVER_FLOW:
            printf("\n");
            break;
        }
    }
    return;
}

int is_ending_in_this_base(int base, int cnt, ...)
{
    va_list iterator;
    int i;
    short int *res;
    double num;
    enum err mistake;
    if (!(res = malloc(sizeof(short int) * cnt)))
    {
        return MEMMORY_ERROR;
    }
    if (base < 2)
    {
        return WRONG_BASE;
    }
    if (cnt <= 0)
    {
        return WRONG_CNT;
    }
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        num = va_arg(iterator, double);
        if ((num >= eps + 1.0) || (num <= eps) || (fabs(num - 1.0) <= eps)) // eps
        {
            res[i] = WRONG_DIAP;
        }
        else
        {
            if (mistake = is_end_dec(num, base, res + i))
            {
                res[i] = mistake;
            }
        }
    }
    print_res(res, cnt);
    free(res);
    res = NULL;
    va_end(iterator);
    return 0;
}

int main()
{
    enum err mistake = 0;
    switch (mistake = is_ending_in_this_base(4, 5, 1.6666666666666667, 0.125, 0.0625, 0.0, -1.9))
    {
    case WRONG_CNT:
        printf("Колличество аргументов должно быть положительным\n");
        break;
    case MEMMORY_ERROR:
        printf("Не удалось выделить память для хранения результатов\n");
        break;
    case WRONG_BASE:
        printf("Значение базы должно быть не меннее 2\n");
        break;
    }
    return 0;
}