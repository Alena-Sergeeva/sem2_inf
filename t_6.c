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

int a_integ(double eps, double *res);
// int S_a_integ(long double eps, long double *res);
int b_integ(double eps, double *res);
int c_integ(double eps, double *res);
int d_integ(double eps, double *res);

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

        a_integ(eps, &res);
        printf("a. %lf\n", res);

        b_integ(eps, &res);
        printf("b. %lf\n", res);

        c_integ(eps, &res);
        printf("c. %lf\n", res);

        d_integ(eps, &res);
        printf("d. %lf\n", res);
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
    // printf("%lf", *eps);
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
// 0.8224670334241133

// Метод Симпсона
// 0.00001 0.822458
// 0.000001 0.822466
/* почемуто точность ниже на единицу
int S_a_integ(long double eps, long double *res)
{
    long double res_temp = 0.0, res_prev = 0.0;
    int n = 2;
    long double y_i_1 = 0, y_i_2, y_i_3;
    do
    {
        res_prev = res_temp;
        res_temp = 0.0;
        long double i;
        for (i = 1; i > eps; i -= (1.0 / (n)))
        {
            y_i_1 = logl(1.0 + i) / i;
            y_i_2 = logl(1.0 + (i + 1.0 / (2.0 * n))) / (i + 1.0 / (2.0 * n));
            y_i_3 = logl(1.0 + i + 1.0 / n) / (i + 1.0 / n);
            res_temp += ((1.0 / (2 * n) / 3.0) * (y_i_1 + 4 * y_i_2 + y_i_3)); // парабола
        }
        printf("n = %d: %Lf\n", n, fabs(res_temp - res_prev));
        n *= 2;
    } while (fabs(res_temp - res_prev) > eps);
    printf("n = %d: %Lf", n, res_temp);
    *res = res_temp;
    return 0;
}*/

// 0.00001 0.822461
int a_integ(double eps, double *res)
{
    double res_temp = 0.0, res_prev = 0.0;
    int n = 4;
    do
    {
        double y_i_1 = (logl(1.0 + 1.0 / n)) / (1.0 / n), y_i_2, i;
        res_prev = res_temp;
        res_temp = 0.0;
        for (i = 2.0 / n; i < 1.0; i += (1.0 / n))
        {
            y_i_2 = (logl(1.0 + i)) / i;
            res_temp += ((y_i_1 + y_i_2) / (2 * n)); // трапеция
            y_i_1 = y_i_2;
        }
        y_i_2 = (logl(1.0 + 1.0)) / 1.0;
        res_temp += (((y_i_1 + y_i_2) / 2) * (1 - i));
        n *= 2;
    } while (fabs(res_temp - res_prev) > eps);
    // printf("n = %d: %Lf", n, res_temp);
    *res = res_temp;
    return 0;
}

// 0.85562439
int b_integ(double eps, double *res)
{
    double res_temp = 0.0, res_prev = 0.0;
    int n = 2;
    double y_i_1 = 1, y_i_2, i;
    do
    {
        res_prev = res_temp;
        res_temp = 0.0;
        for (i = 1.0 / n; i < 1.0; i += (1.0 / n))
        {
            y_i_2 = exp((i * i) / (-2));
            res_temp += ((y_i_1 + y_i_2) / (2 * n)); // трапеция
            y_i_1 = y_i_2;
        }
        y_i_2 = exp((1.0 + 1.0) / (-2));
        res_temp += (((y_i_1 + y_i_2) / 2) * (1 - i));
        // printf("n = %d: %Lf, %Lf\n", n, fabs(res_temp - res_prev), eps);
        n *= 2;
    } while (fabs(res_temp - res_prev) > eps);
    // printf("n = %d: %Lf", n, res_temp);
    *res = res_temp;
    return 0;
}

// 1
int c_integ(double eps, double *res)
{
    double res_temp = 0.0, res_prev = 0.0;
    int n = 2;
    double y_i_1 = 0, y_i_2, i;
    do
    {
        res_prev = res_temp;
        res_temp = 0.0;
        for (i = 1.0 / n; i < 1.0; i += (1.0 / n))
        {
            y_i_2 = logl(1 / (1 - i));
            res_temp += ((y_i_1 + y_i_2) / (2 * n)); // трапеция
            y_i_1 = y_i_2;
        }
        // printf("n = %d: %Lf, %Lf\n", n, fabs(res_temp - res_prev), eps);
        n *= 2;
    } while (fabs(res_temp - res_prev) > eps);
    // printf("n = %d: %Lf", n, res_temp);
    *res = res_temp;
    return 0;
}

// 0.7834305107
int d_integ(double eps, double *res)
{
    double res_temp = 0.0, res_prev = 0.0;
    int n = 2;
    double y_i_1 = 1, y_i_2, i; // 0^0
    do
    {
        res_prev = res_temp;
        res_temp = 0.0;
        for (i = 1.0 / n; i < 1.0; i += (1.0 / n))
        {
            y_i_2 = pow(i, i);
            res_temp += ((y_i_1 + y_i_2) / (2 * n)); // трапеция
            y_i_1 = y_i_2;
        }
        y_i_2 = 1.0;
        res_temp += (((y_i_1 + y_i_2) / 2) * (1 - i));
        n *= 2;
    } while (fabs(res_temp - res_prev) > eps);
    //   printf("\nn = %d: %Lf", n, res_temp);
    *res = res_temp;
    return 0;
}