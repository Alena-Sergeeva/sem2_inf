#include <stdio.h>
#include <math.h>

#define BUFSIZE 256

enum err
{
    OK,
    WRONG_DIAP,
    NO_SOLUTIONS,
    WRONG_EPS,
    NOT_GOOD_FUNC_ONTHIS_DIAP,
    SOLUTIONS_ON_BOARD
};

/*Реализуйте функцию, которая находит корень уравнения одной переменной методом
дихотомии. Аргументами функции являются границы интервала, на котором находится
корень; точность (эпсилон), с которой корень необходимо найти, а также указатель на
функцию, связанной с уравнением от одной переменной.*/
double func0(double x);
double func1(double x);
double func2(double x);
double func3(double x);
double func4(double x);
double func5(double x);
double func6(double x);
double func7(double x);
double func8(double x);
double func9(double x);

int find_solutions(double a, double b, double eps, double *res, double (*func)(double x))
{
    double c = (b + a) / 2;

    if (a >= b)
    {
        return WRONG_DIAP;
    }
    if (((isnan(fabs(func(a)))) || (isinf(func(a)))) || (isnan(fabs((func(b)))) || isinf(func(b))))
    {
        return NOT_GOOD_FUNC_ONTHIS_DIAP;
    }
    // printf("%lf %lf \n", func(a), func(b));
    if (func(a) * func(b) >= 0)
    {
        return NO_SOLUTIONS;
    }
    if (eps <= 0)
    {
        return WRONG_EPS;
    }
    while (b - a >= 2 * eps)
    {
        printf("%.15lf %.15lf \n", func(a), func(b));
        if (func(c) * func(a) < 0)
        {
            b = c;
        }
        else
        {
            if (func(c) * func(a) == 0)
            {
                printf("%.15lf %.15lf \n", func(c), func(a));
                *res = (func(c) == 0) ? c : a;
            }
            a = c;
        }

        c = (b + a) / 2;
    }
    *res = c;
    return 0;
}

int main()
{
    double res = 0;
    switch (find_solutions(-0.5, 22.0, 0.1e-10, &res, func7))
    {
    case OK:
        printf("%.15lf\n", res);
        break;
    case WRONG_DIAP:
        printf("Правая граница должна быть больше левой\n");
        break;
    case WRONG_EPS:
        printf("Точность должна быть положительной\n");
        break;
    case NO_SOLUTIONS:
        printf("Уравнение не имеет решений в заданном интервале\n");
        break;
    case NOT_GOOD_FUNC_ONTHIS_DIAP:
        printf("Значение из интервала выходит за область определения функции\nлибо значение функции на интервале слишком велики и не помещаются в double\n");
        break;
    }
    return 0;
}

double func0(double x)
{
    return x * x - 2;
}

//(0.1, 1)
// при 1 возвращает 0, что делать
double func1(double x)
{

    return x + log(x) - 0.5;
}

//(0, 1) 0.5671439
double func2(double x)
{
    return x * exp(x) - 1;
}

//(-2, 2) -0.23612579
double func3(double x)
{
    return 2 * x + cos(x) - 0.5;
}

//(0, 1) не пересекает ось x
double func4(double x)
{
    return x * x - 1 - log(x + 1) + 10;
}

// не имеет решений
double func5(double x)
{
    return 1 / x;
}

//(0; 2) 1
double func6(double x)
{
    return sqrt(x) - 1;
}
// не имеет решений
double func8(double x)
{
    return sqrt(x) + 1;
}

//(-0.5; 2)
double func7(double x)
{
    return 1 / (x + 1) - 1;
}
