#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum err{
    OK,
    FEW_ARGUMENTS,
    NOT_NUM,
    WRONG_FLAG,
    NUM_TOO_BIG,
    WRONGE_EPS_VALUE,
    SUM_TOO_BIG
};
int valid (int, char * [], long double *, long double *, char *);

int sum_a(long double, long double, long double *);
int sum_b(long double, long double, long double *);
int sum_c(long double, long double, long double *);
int sum_d(long double, long double, long double *);

int main (int argc, char * argv[])
{
    long double eps = 0.0, x = 0.0, res = 0.0;
    char fl = ' ';
    enum err mistake = 0;
    switch (valid(argc, argv, &x, &eps, &fl))
    {
        case OK:
            //printf("%Lf, %Lf\n", x, eps);
            switch (fl)
            {
                case 'a':
                    mistake = sum_a(x, eps, &res);
                    break;
                case 'b':
                    mistake = sum_b(x, eps, &res);
                    break;
                case 'c':
                    mistake = sum_c(x, eps, &res);
                    break;
                case 'd':
                    mistake = sum_d(x, eps, &res);
                    break;
            }
            if (mistake == SUM_TOO_BIG)
            {
                printf("Необходимая точность не достигнута, произошло переполнение\n");
            }
            else
            {
                printf("%LE\n", res);
            }
            break;

        case FEW_ARGUMENTS:
            printf("Недостаточно аргументов командной строки\n");
            break;

        case WRONG_FLAG:
            printf("Неверный флаг\n");
            break;

        case NOT_NUM:
            printf("Аргумент не является числом\n");
            break;

        /*case NUM_TOO_BIG:
            printf("Произошло переполнение типа long double\n");
            break;
        */
        case WRONGE_EPS_VALUE:
            printf("eps -- должно быть неотрицательным\n");
            break;

    }
    return 0;
}

int valid (int argc, char * argv[], long double *x, long double * eps, char *fl)
{
    if (argc < 4)
    {
        return FEW_ARGUMENTS;
    }
    if (!(('-' == argv[1][0]) || ('/' == argv[1][0])) || !(strstr(" a b c d ", argv[1]+1)))
    {
        return WRONG_FLAG;
    }
    * fl = argv[1][1];

    char * pt_stop_symbol = NULL;

    * x = strtold(argv[2], &pt_stop_symbol);
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
        printf("%s\n", pt_stop_symbol);

        return NOT_NUM;
    }

    * eps = strtold(argv[3], &pt_stop_symbol);

    if ((*eps == HUGE_VALL) || (*eps == (-HUGE_VALL)))
    {
        return NUM_TOO_BIG;
    }
    if ((strcmp(argv[3], "0") != 0) && (*eps == 0))
    {
        return NOT_NUM;
    }
    if (!(*(pt_stop_symbol) == '\0'))
    {

        printf("%s\n", pt_stop_symbol);

        return NOT_NUM;
    }
    if (*eps < 0)
    {
        return WRONGE_EPS_VALUE;
    }

    return 0;
}

int sum_a(long double x, long double eps, long double *res)
{
    long double elem_n = x, elem_pr = 1;
    int n = 1;

    //long double LONG_DOUBLE = pow(2, sizeof(long double) * 8);

    //printf("%LE\n", LONG_DOUBLE);

    while ((abs(elem_n - elem_pr) > eps))
    {
        elem_pr = elem_n;
        elem_n *= (x / (n+1));
        ++n;
    }

    /*
    if (elem_n > LONG_DOUBLE / (x / (n+1)))
        return SUM_TOO_BIG;
    */
    * res = elem_n;
    return 0;
}

int sum_b(long double x, long double eps, long double *res)
{
    long double elem_n = (((-1) * x * x))/ 2, elem_pr = 1;
    int n = 1;
    /*
    long double LONG_DOUBLE = pow(2, sizeof(long double) * 8);
    printf("%LE\n", LONG_DOUBLE);
    */
    while ((abs(elem_n - elem_pr) > eps))
    {
        elem_pr = elem_n;
        elem_n *= (((-1) * x * x)/((2 * n + 2) * (2 * n + 1)));
        ++n;
    }
    /*
    printf("%LE\n", elem_n);
    printf("%LE\n", LONG_DOUBLE / (((-1) * x * x)/((2 * n + 2) * (2 * n + 1))));
    if (abs(elem_n) > abs(LONG_DOUBLE / (((-1) * x * x)/((2 * n + 2) * (2 * n + 1)))))
        return SUM_TOO_BIG;
    */

    * res = elem_n;
    return 0;
}

int sum_c(long double x, long double eps, long double *res)
{
    long double elem_n = (9*x*x)/2, elem_pr = 1;
    int n = 1;

    //long double LONG_DOUBLE = pow(2, sizeof(long double) * 8);

    while (abs(elem_n - elem_pr) > eps)
    {
        elem_pr = elem_n;
        elem_n *= ((9 * (n + 1) * (n + 1) * x * x)/((3 * n + 2)* (3 * n + 1)));
        ++n;
    }
    /*
    if (elem_n > LONG_DOUBLE / ((9 * (n + 1) * (n + 1) * x * x)/((3 * n + 2)* (3 * n + 1))))
        return SUM_TOO_BIG;
    */

    * res = elem_n;
    return 0;
}

int sum_d(long double x, long double eps, long double *res)
{
    long double elem_n = (3 * x * x * x * x) / 8, elem_pr = ((-1) * (x * x)) / 2;
    int n = 2;
    long double x_pw_2 = x * x * (-1);

    //long double LONG_DOUBLE = pow(2, sizeof(long double) * 8);
    while (abs(elem_n - elem_pr) > eps)
    {
        elem_pr = elem_n;
        elem_n *= ((x_pw_2 * (n + 0.5))/ (n+1)) ;
        ++n;
    }

    //if (elem_n > LONG_DOUBLE / ((x_pw_2 * (n + 0.5))/ (n+1)))
    //    return SUM_TOO_BIG;

    * res = elem_n;

    return 0;
}

