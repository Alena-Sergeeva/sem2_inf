#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
enum err
{
    OK,
    NO_ARGUMENT,
    WRONG_POINTER,
    OVER_FLOW
};

int check_double_overflow(double x)
{
    if (isnan(x * x) || isinf(x * x) || (x * x == 0.0))
    {
        return OVER_FLOW;
    }
    return 0;
}
/*среднее геометрическое переданных ей чисел вещественного типа. Количество (значение типа int)
переданных вещественных чисел задаётся в качестве последнего обязательного параметра функции*/
int geometric_mean(double *res, int cnt, ...)
{
    int i = 0;
    va_list iterator;
    double mult = 1.0;
    if (cnt <= 0)
    {
        return NO_ARGUMENT;
    }
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    *res = 0;
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        if (check_double_overflow(mult *= va_arg(iterator, double)))
        {
            return OVER_FLOW;
        }
    }
    va_end(iterator);
    if (check_double_overflow(*res = pow(mult, 1.0 / cnt)))
    {
        return OVER_FLOW;
    }
    return 0;
}

/*рекурсивная функция возведения вещественного числа в целую
степень. При реализации используйте алгоритм быстрого возведения в степень.*/
int power(double num, int exponent, double *res)
{
    if (exponent == 1)
    {
        *res = num;
        return 0;
    }
    power(num, exponent >> 1, res);
    if (check_double_overflow((*res) * (*res)))
    {
        return OVER_FLOW;
    }
    *res *= (*res);
    if (exponent & 1)
    {
        if (check_double_overflow(num * (*res)))
        {
            return OVER_FLOW;
        }
        (*res) *= num;
    }
    return 0;
}

int power_int(double num, int exponent, double *res)
{
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    if (exponent < 0)
    {
        num = 1 / num;
    }
    *res = 1.0;
    return power(num, abs(exponent), res);
}

void print_res(enum err mistake, double res)
{
    switch (mistake)
    {
    case OK:
        printf("%.10lf\n", res);
        break;
    case NO_ARGUMENT:
        printf("Число аргументов неположительное\n");
        break;
    case OVER_FLOW:
        printf("Переполнение типа double\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    }
}
int main()
{
    double res = 1.0;
    enum err mistake = 0;
    printf("Среднее геметрическое: ");
    mistake = geometric_mean(&res, 4, 1.0, 2.0, 3.0, 4.0);
    print_res(mistake, res);
    printf("Возведение в степень: ");
    res = 1.0;
    mistake = power_int(2, -8, &res);
    print_res(mistake, res);
}