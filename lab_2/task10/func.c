#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

enum err
{
    OK,
    MEMMORY_ERROR,
    NULL_PTR
};

int decomposition(double eps, double a, double **resalt, int n, ...)
{
    int i = 0, n_1 = n;
    double *res = NULL;
    va_list iterator;
    va_start(iterator, n);
    if (resalt == NULL)
    {
        return NULL_PTR;
    }
    if (!(res = (double *)malloc(sizeof(double) * (n + 1))))
    {
        return MEMMORY_ERROR;
    }
    for (i = 0; i <= n; ++i)
    {
        res[i] = va_arg(iterator, double);
    }
    while (n > 0)
    {
        for (i = 1; i <= n; ++i)
        {
            res[i] += (res[i - 1] * a);
        }
        --n;
    }
    *resalt = res;

    return OK;
}

int main()
{
    double *result = NULL;
    int n = 4, i = 0;
    double a = 3.0;
    double eps = 0.0001;
    switch (decomposition(eps, a, &result, n, 2.0, -3.0, -6.0, 5.0, 6.0))
    {
    case OK:
        for (i = 0; i < n + 1; ++i)
        {
            printf("%f ", result[i]);
        }
        free(result);
        printf("\n");
        break;
    case MEMMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    case NULL_PTR:
        printf("Передан нулевой указатель\n");
        break;
    }

    return 0;
}