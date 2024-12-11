#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/*
2. Реализуйте функцию с переменным числом аргументов, находяющую значение
многочлена степени n в заданной точке. Входными параметрами являются точка
(вещественного типа), в которой определяется значение многочлена, степень
многочлена (целочисленного типа), и его коэффициенты (вещественного типа, от
старшей степени до свободного коэффициента в порядке передачи параметров
функции слева направо).

*/
#define EPS 1e-6

#define max(a, b) (((a) > (b)) ? (a) : (b))

enum err
{
    OK,
    WRONG_POINTER,
    WRONG_ARG_CNT,
    EMPTY_STR,
    WRONG_BASE,
    OVERFLOW,
    WRONG_STR,
    YES,
    NO
};

typedef struct point
{
    double x;
    double y;
} point;

/*
Реализуйте функцию с переменным числом аргументов, принимающую координаты
(вещественного типа, пространство двумерное) вершин многоугольника и
определяющую, является ли этот многоугольник выпуклым.
*/
// выпуклый многоугольник
int convex_polygon(char *fl, int cnt, ...)
{
    va_list iter;
    int i = 0, j = 0;
    double k = 0.0;
    int cnt_low_nul = 0;
    if (cnt < 0)
    {
        return WRONG_ARG_CNT;
    }
    point arr[cnt];
    if (fl == NULL)
    {
        return WRONG_POINTER;
    }
    *fl = 1;
    va_start(iter, cnt);
    for (i = 0; i < cnt; ++i)
    {
        arr[i].x = va_arg(iter, double);
        arr[i].y = va_arg(iter, double);
    }
    va_end(iter);
    for (i = 1; i < cnt - 1; ++i)
    {
        if (fabs(arr[i - 1].x - arr[i].x) <= EPS)
        {
            k = 0.0;
        }
        else
        {
            k = (arr[i].y - arr[i - 1].y) / (arr[i - 1].x - arr[i].x);
        }
        cnt_low_nul = 0;
        printf("%d; k = %lf; %lf %lf %lf\n", i, k, fabs(arr[i - 1].x - arr[i].x), arr[i - 1].x, arr[i].x);
        j = i + 1;
        while (j != i - 1)
        {
            /*
            if ((fabs(arr[i - 1].y + k * (arr[j].x - arr[i - 1].x) - arr[j].y) <= EPS))
            {
                *fl = 0;
                return OK;
            }
            */
            // точка лежит на продолжении грани многоугольника, значит он тосно не выпуклый
            printf("j: %d ; %lf\n", j, (arr[i - 1].y + k * (arr[j].x - arr[i - 1].x) - arr[j].y));
            if (((arr[i - 1].y + k * (arr[j].x - arr[i - 1].x) - arr[j].y) < 0) || ((fabs(arr[i - 1].y + k * (arr[j].x - arr[i - 1].x) - arr[j].y) <= EPS)))
            {

                ++cnt_low_nul;
            }
            j = (j + 1) % cnt;
        }
        printf(" %d\n", cnt_low_nul);
        if (cnt_low_nul % (cnt - 2) != 0)
        {
            printf(" %d kkk\n", cnt_low_nul);
            *fl = 0;
            return OK;
        }
    }
    return OK;
}

/*
Реализуйте функцию с переменным числом аргументов, находяющую значение
многочлена степени n в заданной точке. Входными параметрами являются точка
(вещественного типа), в которой определяется значение многочлена, степень
многочлена (целочисленного типа), и его коэффициенты (вещественного типа, от
старшей степени до свободного коэффициента в порядке передачи параметров
функции слева направо).
*/
int count_y(double *res, double x, int n, ...)
{
    va_list iterator;
    int i = 0;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    *res = 0.0;
    va_start(iterator, n);
    for (i = 0; i <= n - 1; ++i)
    {
        *res = (*res + va_arg(iterator, double)) * x;
        printf("%lf ", *res);
    }
    *res += va_arg(iterator, double);
    va_end(iterator);
    printf("%lf\n", *res);
    return 0;
}

/*
Реализуйте функцию с переменным числом аргументов, находящую среди
переданных строковых представлений целых неотрицательных чисел, заданных в
системе счисления с основанием base, передаваемым как параметр функции, чисел,
являющихся в системе счисления с основанием base числами Капрекара.
Продемонстрируйте работу реализованных функций.
*/
int to_int_10(char *str, int base, long int *num)
{
    long int res = 0;
    if ((str == NULL) || (num == NULL))
    {
        return WRONG_POINTER;
    }
    if (*str == '\0')
    {
        return EMPTY_STR;
    }
    while (*str != '\0')
    {
        if (!isalnum(*str) || toupper(*str) - 'A' >= base)
        {
            return WRONG_STR;
        }
        res = res * base + (isalpha(*str) ? (toupper(*str) - 'A' + 10) : (*str - '0'));
        if (res < 0)
        {
            return OVERFLOW;
        }
        ++str;
    }
    *num = res;
    return OK;
}

void to_base(long int num, int base_to, char *str, char **str_begin)
{
    int y;
    *str_begin = str + (sizeof(long int) << 3);
    **str_begin = '\0';
    while (num > 0)
    {
        --*str_begin;
        y = num % base_to;
        **str_begin = ((y > 9) ? ('a' + y - 10) : ('0' + y));
        num /= base_to;
    }

    return;
}

int sum_num_b(char **sum_s1_s2, char *str1, int length1, char *str2, int length2, int base)
{
    int i = 0, num1 = 0, num2 = 0, num = 0, ost = 0;
    enum err mistake = 0;
    if (sum_s1_s2 == NULL)
    {
        return WRONG_POINTER;
    }
    --length1;
    --length2;
    for (i = 0; i <= max(length1, length2); ++i)
    {
        num1 = (length1 - i < 0) ? 0 : (isalpha(str1[length1 - i]) ? toupper(str1[length1 - i]) + 10 - 'A' : str1[length1 - i] - '0');
        if (length2 - i < 0)
        {
            num2 = 0;
        }
        else
        {
            num2 = isalpha(str2[length2 - i]) ? toupper(str2[length2 - i]) + 10 - 'A' : str2[length2 - i] - '0';
        }
        // printf("%d str1[i] %c; %d str2[j] %c\n", num1, str1[i], num2, str2[i]);
        num = (ost + num1 + num2) % base;
        ost = (ost + num1 + num2) / base;

        **sum_s1_s2 = (num > 9) ? ('A' + num - 10) : ('0' + num);
        --(*sum_s1_s2);
    }
    if (ost != 0)
    {
        **sum_s1_s2 = (ost > 9) ? ('A' + ost - 10) : ('0' + ost);
    }
    else
    {
        (*sum_s1_s2)++;
    }
    // a xnj ,eltn tckb "0" ?????
    while (**sum_s1_s2 == '0')
    {
        ++*sum_s1_s2;
    };
    printf("res: %s\n", *sum_s1_s2);
    return OK;
}

int Kaprekars_num(enum err *res, int base, int cnt, ...)
{
    int i = 0;
    char *str = NULL;
    long int num = 0, length = 1, qrt = 0;
    enum err mistake = 0;
    va_list iterator;
    char str_base[sizeof(long int) << 3 + 1];
    char *str_begin = str_base + (sizeof(long int) << 3);

    if (cnt < 0)
    {
        return WRONG_ARG_CNT;
    }
    if ((base < 2) || (base > 36))
    {
        return WRONG_BASE;
    }
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        length = 1;
        // str = va_arg(iterator, char *);
        upper(&str, va_arg(iterator, char *));
        if (mistake = to_int_10(str, base, &num))
        {
            res[i] = mistake;
        }
        if (num * num == __INT64_MAX__)
        {
            res[i] = OVERFLOW;
        }
        qrt = num * num;
        to_base(qrt, base, str_base, &str_begin);

        printf("qrt : %ld; num : %ld; %s\n", qrt, num, str_begin);

        int len_str_base = strlen(str_begin);

        char sum_s1_s2[sizeof(long int) << 3 + 1];
        char *sum_ptr = sum_s1_s2 + (sizeof(long int) << 3);
        *sum_ptr = '\0';
        if (qrt == num)
        {
            res[i] = YES;
            continue;
        }
        while (len_str_base != length)
        {
            sum_ptr = sum_s1_s2 + (sizeof(long int) << 3) - 1;
            sum_num_b(&sum_ptr, str_begin, length, str_begin + length, len_str_base - length, base);
            printf("string %s\n", sum_ptr);
            if (strcmp(str, sum_ptr) == 0)
            {
                res[i] = YES;
                break;
            }
            ++length;
        }
        if (len_str_base == length)
        {
            res[i] = NO;
        }
    }
    va_end(iterator);
    return OK;
}

void print_res(enum err *res, int cnt)
{
    for (int i = 0; i < cnt; ++i)
    {
        switch (res[i])
        {
        case NO:
            printf("NO ");
            break;
        case YES:
            printf("YES ");
            break;
        case WRONG_STR:
            printf("Строка не удовлетворяет условию ");
            break;
        case EMPTY_STR:
            printf("Строка не удовлетворяет условию ");
            break;
        case OVERFLOW:
            printf("Переполнени long int ");
            break;
        }
    }
    printf("\n");
}

int main()
{
    char fl = '0';
    enum err *res = NULL;
    int cnt = 0;
    // я обхожу точки по кругу , то есть звездочку задать не получитя-- нет не по кругу
    //  а в указанном порядке
    //  квадрат
    printf("%d\n", convex_polygon(&fl, 4, 0., 0., 1., 0., 1., 1., 0., 1.));
    printf("result - %d\n", fl);
    // ступенька
    printf("%d\n", convex_polygon(&fl, 6, 1., 0., 3., 0., 3., 1., 2.0, 1.0, 2., 3., 1.0, 3.));
    printf("result - %d\n", fl);
    // подумать что делать с прямой линией, параллельными прямыми(как бужто их не возможно задать), точкой
    printf("%d\n", convex_polygon(&fl, 3, 0., 0., 1., 1., 2., 2.));
    printf("result - %d\n", fl);

    // треугольник
    printf("%d\n", convex_polygon(&fl, 3, 0., 1., 7., 1., 5., 4.));
    printf("result - %d\n", fl);

    // ломанная
    printf("%d\n", convex_polygon(&fl, 4, 0., 0., 1., 1., 2., 1., 2., 6.));
    printf("result - %d\n", fl);

    cnt = 4;
    if (!(res = (enum err *)malloc(sizeof(enum err) * cnt)))
    {
        printf("Не удалось выделить память\n");
    }
    Kaprekars_num(res, 10, cnt, "1", "9", "45", "17");
    print_res(res, cnt);
    free(res);
    res = NULL;

    cnt = 5;
    if (!(res = (enum err *)malloc(sizeof(enum err) * cnt)))
    {
        printf("Не удалось выделить память\n");
    }
    // сделать чтобы маленькие буквы становились большими
    Kaprekars_num(res, 16, cnt, "1", "6", "A", "f", "33");
    print_res(res, cnt);
    free(res);
    res = NULL;

    double c_y = 0.0;
    count_y(&c_y, 2.0, 2, 2.0, 3.0, 4.0);
    printf("%lf\n", c_y);
    return 0;
}