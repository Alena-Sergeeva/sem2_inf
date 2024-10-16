#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tgmath.h>
#include <limits.h>

enum err
{
    OK,
    CNT_ARGUMENTS,
    NOT_NUM,
    NUM_TOO_BIG,
    WRONGE_EPS_VALUE,
};

int valid(int, char *[], double *);
double a_func(double x);
double b_func(double x);
double c_func(double x);
double d_func(double x);
int integral(double (*fun)(double x), double eps, double *res);

int main(int argc, char *argv[])
{
    double eps = 0.0, res = 0.0;
    switch (valid(argc, argv, &eps))
    {
    case OK:
        if (eps < 0.0000001)
        {
            printf("Программа будет работать слишком долго\n");
            return 0;
        }

        integral(a_func, eps, &res);
        printf("a. %lf\n", res);

        integral(b_func, eps, &res);
        printf("b. %lf\n", res);

        integral(c_func, eps, &res);
        printf("c. %lf\n", res);

        integral(d_func, eps, &res);
        printf("d. %lf\n", res);
        break;
    case CNT_ARGUMENTS:
        printf("Неверное количество аргументов командной строки\n");
        break;

    case NOT_NUM:
        printf("Аргумент не является числом, либо произошла потяря значимости\n");
        break;

    case NUM_TOO_BIG:
        printf("Произошло переполнение типа double\n");
        break;

    case WRONGE_EPS_VALUE:
        printf("eps -- должно быть положительным\n");
        break;
    }
    return 0;
}

int isnal(char *str)
{
    while ((*str != '\0') && !((*str == 'E') || (*str == 'e')))
    {
        if ((*str != '0') && !((*(str) == '+') || (*(str) == '-') || *str == '.'))
        {
            return 0;
        }
        ++str;
    }
    return 1;
}

int valid(int argc, char *argv[], double *eps)
{
    enum err mistake = 0;
    if (argc != 2)
    {
        return CNT_ARGUMENTS;
    }
    char *pt_stop_symbol = NULL;
    *eps = strtod(argv[1], &pt_stop_symbol);
    if ((fabs(*eps) == HUGE_VAL) || (*eps == (-HUGE_VAL)))
    {
        return NUM_TOO_BIG;
    }

    if ((*eps == 0.0) && !(isnal(argv[1])))
    {
        return NUM_TOO_BIG;
    }
    if (!(*(pt_stop_symbol) == '\0'))
    {
        return NOT_NUM;
    }
    if (*eps <= 0)
    {
        return WRONGE_EPS_VALUE;
    }
    return 0;
}

double a_func(double x)
{
    return (logl(1.0 + x)) / x;
}
double b_func(double x)
{
    return exp((x * x) / (-2));
}
double c_func(double x)
{
    return logl(1 / (1 - x));
}
double d_func(double x)
{
    return pow(x, x);
}
int integral(double (*func)(double x), double eps, double *res)
{
    double res_temp = 0.0, res_prev = 0.0;
    int n = 4;
    do
    {
        double y_i_1 = 0.0, y_i_2, i;
        res_prev = res_temp;
        res_temp = 0.0;
        for (i = 1.0 / n; i < 1.0; i += (1.0 / n))
        {
            y_i_2 = func(i);
            res_temp += ((y_i_1 + y_i_2) / (2.0 * n)); // трапеция
            y_i_1 = y_i_2;
        }

        n <<= 1;
    } while (fabs(res_temp - res_prev) > eps);
    *res = res_temp;
    return 0;
}
// a. 0.8224670334241133
// b. 0.85562439
// c. 0.8224670334241133
// d. 0.7834305107
