#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>

enum err
{
    OK,
    CNT_ARGUMENTS,
    NOT_NUM,
    NUM_TOO_BIG,
    WRONGE_EPS_VALUE,
    SUM_TOO_BIG,
    SERIES_DIVERGES
};
int valid(int, char *[], long double *, long double *, char *);

int sum_a(long double, long double, long double *);
int sum_b(long double, long double, long double *);
int sum_c(long double, long double, long double *);
int sum_d(long double, long double, long double *);

int main(int argc, char *argv[])
{
    long double eps = 0.0, x = 0.0, res = 0.0;
    char fl = ' ';
    enum err mistake = 0;
    switch (valid(argc, argv, &x, &eps, &fl))
    {
    case OK:
        printf("%Lf, %Lf\n", eps, x);
        mistake = sum_a(x, eps, &res);
        printf("a. %.15Lf\n", res);
        mistake = sum_b(x, eps, &res);
        printf("b. %.15Lf\n", res);
        mistake = sum_c(x, eps, &res);
        if (mistake == SERIES_DIVERGES)
        {
            printf("с. Ряд рассходится\n");
        }
        else
        {
            printf("c. %.15Lf\n", res);
        }
        mistake = sum_d(x, eps, &res);
        if (mistake == SUM_TOO_BIG)
        {
            printf("d. Необходимая точность не достигнута, произошло переполнение\n");
        }
        else
        {
            printf("d. %.15Lf\n", res);
        }
        break;

    case CNT_ARGUMENTS:
        printf("Неверное количество аргументов командной строки\n");
        break;

    case NOT_NUM:
        printf("Аргумент не является числом, либо произошла потяря значимости\n");
        break;

    case NUM_TOO_BIG:
        printf("Произошло переполнение типа long double\n");
        break;

    case WRONGE_EPS_VALUE:
        printf("eps -- должно быть положительным\n");
        break;
    }
    return 0;
}

int valid(int argc, char *argv[], long double *x, long double *eps, char *fl)
{
    if (argc != 3)
    {
        return CNT_ARGUMENTS;
    }
    char *pt_stop_symbol = NULL;
    *eps = strtold(argv[1], &pt_stop_symbol);
    if ((*eps == HUGE_VALL) || (*eps == (-HUGE_VALL)))
    {
        return NUM_TOO_BIG;
    }
    if ((strcmp(argv[1], "0") != 0) && (*eps == 0))
    {
        return NOT_NUM;
    }

    if ((*eps == 0) && (strlen(argv[1]) != 1))
    {
        return NOT_NUM;
    }
    if (!(*(pt_stop_symbol) == '\0'))
    {
        return NOT_NUM;
    }
    if (*eps <= 0)
    {
        return WRONGE_EPS_VALUE;
    }

    pt_stop_symbol = NULL;
    *x = strtold(argv[2], &pt_stop_symbol);
    if ((*x == HUGE_VALL) || (*x == (-HUGE_VALL)))
    {
        return NUM_TOO_BIG;
    }
    if ((strcmp(argv[2], "0") != 0) && (*x == 0))
    {
        return NOT_NUM;
    }

    if (!(*(pt_stop_symbol) == '\0'))
    {
        return NOT_NUM;
    }

    return 0;
}
// e^x
int sum_a(long double x, long double eps, long double *res)
{
    long double elem_n = 1;
    int n = 0;
    *res = 1;
    while ((fabs(elem_n) > eps))
    {
        elem_n *= (x / (n + 1));
        (*res) += elem_n;
        ++n;
        // printf("%Lf\n", *res);
    }
    (*res) += elem_n;
    if (isinf(elem_n))
        return SUM_TOO_BIG;

    return 0;
}
// cosx
int sum_b(long double x, long double eps, long double *res)
{
    long double elem_n = 1;
    int n = 0;
    *res = 1;
    while ((fabs(elem_n) > eps))
    {
        elem_n *= (((-1) * x * x) / ((2 * n + 2) * (2 * n + 1)));
        (*res) = (*res) + elem_n;
        ++n;
        // printf("%Lf\n", *res);
    }
    (*res) += elem_n;
    if (isinf(*res))
        return SUM_TOO_BIG;

    return 0;
}

// ряд сходится при только при |x|<1
int sum_c(long double x, long double eps, long double *res)
{
    long double elem_n = 1;
    int n = 0;
    if (fabs(x * x) >= 1)
    {
        return SERIES_DIVERGES;
    }
    while ((fabs(elem_n) > eps) && (!isinf(*res)) && (!isnanl(*res)))
    {
        elem_n *= ((9 * (n + 1) * (n + 1) * x * x) / ((3 * n + 2) * (3 * n + 1)));
        ++n;
        (*res) += elem_n;
    }
    (*res) += elem_n;
    if ((isinf(*res)) || (isnanl(*res)))
        return SUM_TOO_BIG;

    return 0;
}

// не понимаю
int sum_d(long double x, long double eps, long double *res)
{
    long double elem_n = ((-1) * (x * x)) / 2;
    int n = 1;

    while ((fabs(elem_n) > eps) && (!isinf(*res)) && (!isnanl(*res)))
    {
        elem_n *= ((x * x * (-1) * (2 * n + 1)) / (2 * n + 2));
        ++n;
        (*res) += elem_n;
        // printf("%Lf\n", *res);
    }
    (*res) += elem_n;

    if ((isinf(*res)) || (isnanl(*res)))
        return SUM_TOO_BIG;

    return 0;
}
