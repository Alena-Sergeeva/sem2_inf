#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define EPS 1e-6

#define max(a, b) (((a) > (b)) ? (a) : (b))

enum err
{
    OK,
    WRONG_POINTER,
    WRONG_ARG_CNT,
    EMPTY_STR,
    NE_CHOT,
    WRONG_BASE,
    OVERFLOW,
    WRONG_STR,
    POINT,
    YES,
    NO
};

typedef struct Vector
{
    double i;
    double j;
} Vector;

/*
Реализуйте функцию с переменным числом аргументов, принимающую координаты
(вещественного типа, пространство двумерное) вершин многоугольника и
определяющую, является ли этот многоугольник выпуклым.
*/
// выпуклый многоугольник
/*
Если определитель положителен, то тройка векторов имеет ту же ориентацию, что и система координат.
Если определитель отрицателен, то тройка векторов имеет ориентацию, противоположную ориентации системы координат.
Если определитель равен нулю, то векторы компланарны (линейно зависимы).
*/
int convex_polygon(char *fl, int cnt, ...)
{
    va_list iter;
    int k = 0, j = 0, cnt_neg = 0;
    double x_pr = 0.0, y_pr = 0.0, x = 0.0, y = 0.0, x1, y1, det = 0.0;
    int cnt_low_nul = 0;
    if (cnt < 0)
    {
        return WRONG_ARG_CNT;
    }
    if (cnt == 1)
    {
        return POINT;
    }
    Vector vector_arr[cnt];
    if (fl == NULL)
    {
        return WRONG_POINTER;
    }
    *fl = 0;
    va_start(iter, cnt);
    x1 = x_pr = va_arg(iter, double);
    y1 = y_pr = va_arg(iter, double);
    for (k = 0; k < cnt - 1; ++k)
    {
        x = va_arg(iter, double);
        y = va_arg(iter, double);
        vector_arr[k].i = x - x_pr;
        vector_arr[k].j = y - y_pr;
        x_pr = x;
        y_pr = y;
    }
    vector_arr[k].i = x1 - x_pr;
    vector_arr[k].j = y1 - y_pr;
    va_end(iter);
    for (k = 0; k < cnt - 1; ++k)
    {
        det = vector_arr[k].i * vector_arr[k + 1].j - vector_arr[k + 1].i * vector_arr[k].j;
        if (fabs(det) < EPS)
        {
            return 0;
        }
        if (det < 0)
        {
            cnt_neg += 1;
        }
    }
    det = vector_arr[k].i * vector_arr[0].j - vector_arr[0].i * vector_arr[k].j;
    if (det < 0)
    {
        cnt_neg += 1;
    }
    if ((cnt_neg == cnt) || (cnt_neg == 0))
    {
        *fl = 1;
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
    for (i = 0; i <= n; ++i)
    {
        *res = *res * x + va_arg(iterator, double);
    }
    va_end(iterator);
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
    long int num_1 = 0;
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
        num_1 = isalpha(*str) ? (toupper(*str) - 'A' + 10) : (*str - '0');
        if (!isalnum(*str) || (num_1 >= base))
        {
            return WRONG_STR;
        }
        res = res * base + num_1;
        if (res < 0)
        {
            return OVERFLOW;
        }
        ++str;
    }
    *num = res;
    return OK;
}

int to_base(long int num, int base_to, char **str_begin)
{
    if ((str_begin == NULL) || (*str_begin == NULL))
    {
        return WRONG_POINTER;
    }
    int y;
    **str_begin = '\0';
    while (num > 0)
    {
        --*str_begin;
        y = num % base_to;
        **str_begin = ((y > 9) ? (y - 10 + 'A') : ('0' + y));
        num /= base_to;
    }
    return 0;
}

int sum_num_b(char **sum_s1_s2, char *str1, int length1, char *str2, int length2, int base)
{
    int i = 0, num1 = 0, num2 = 0, num = 0, ost = 0;
    enum err mistake = 0;
    if ((sum_s1_s2 == NULL) || (*sum_s1_s2 == NULL) || (str1 == NULL) || (str2 == NULL))
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

    while (**sum_s1_s2 == '0')
    {
        ++*sum_s1_s2;
    }
    return OK;
}

int equal(char *str1, char *str2, char *fl)
{
    if ((str1 == NULL) || (str2 == NULL) || (fl == NULL))
    {
        return WRONG_POINTER;
    }
    *fl = 0;
    while ((*str1 != '\0') && (*str2 != '\0'))
    {
        if (toupper(*str1) != *str2)
        {
            return 0;
        }
        ++str1;
        ++str2;
    }
    if ((*str1 == '\0') && (*str2 == '\0'))
    {
        *fl = 1;
    }
    return 0;
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
    char fl;

    char sum_s1_s2[sizeof(long int) << 3 + 1];
    char *sum_ptr = sum_s1_s2 + (sizeof(long int) << 3);
    *sum_ptr = '\0';

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
        str = va_arg(iterator, char *);
        if (mistake = to_int_10(str, base, &num))
        {
            res[i] = mistake;
            continue;
        }
        if (num == 1)
        {
            res[i] = YES;
            continue;
        }

        if (num > __INT64_MAX__ / num)
        {
            res[i] = OVERFLOW;
            continue;
        }
        qrt = num * num;
        if (mistake = to_base(qrt, base, &str_begin))
        {
            res[i] = mistake;
            continue;
        }

        int len_str_base = strlen(str_begin);

        while (len_str_base != length)
        {
            sum_ptr = sum_s1_s2 + (sizeof(long int) << 3) - 1;
            mistake = sum_num_b(&sum_ptr, str_begin, length, str_begin + length, len_str_base - length, base);
            if ((mistake != 0) || ((equal(str, sum_ptr, &fl) != 0)))
            {
                res[i] = mistake;
                continue;
            }
            if (fl)
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

void print_mistake(enum err res)
{
    switch (res)
    {
    case OK:
        printf("result : ");
        break;
    case NO:
        printf("NO\n");
        break;
    case YES:
        printf("YES\n");
        break;
    case WRONG_STR:
        printf("Строка не удовлетворяет условию\n");
        break;
    case EMPTY_STR:
        printf("Строка не удовлетворяет условию\n");
        break;
    case OVERFLOW:
        printf("Переполнени long int\n");
        break;
    case WRONG_ARG_CNT:
        printf("Число неявных параметров функции должно быть положительным\n");
        break;
    case POINT:
        printf("Это точка\n");
        break;
    case NE_CHOT:
        printf("Число координат должно быть чётным\n");
        break;
    }
}

void print_Kapric_res(enum err *res, int cnt)
{
    for (int i = 0; i < cnt; ++i)
    {
        print_mistake(res[i]);
    }
}

int main()
{
    char fl = '0';
    enum err *res = NULL;
    int cnt = 0;
    // я обхожу точки по кругу , то есть звездочку задать не получитя-- нет не по кругу
    //  а в указанном порядке

    //  квадрат
    print_mistake(convex_polygon(&fl, 4, 0., 0., 1., 0., 1., 1., 0., 1.));
    printf("%d\n", fl);
    // ступенька
    print_mistake(convex_polygon(&fl, 6, 1., 0., 3., 0., 3., 1., 2.0, 1.0, 2., 3., 1.0, 3.));
    printf("%d\n", fl);
    // линия
    print_mistake(convex_polygon(&fl, 3, 0., 0., 1., 1., 2., 2.));
    printf("%d\n", fl);

    // треугольник
    print_mistake(convex_polygon(&fl, 3, 0., 1., 7., 1., 5., 4.));
    printf("%d\n", fl);

    // ломанная
    print_mistake(convex_polygon(&fl, 4, 0., 0., 1., 1., 2., 1., 2., 6.));
    printf("%d\n", fl);

    print_mistake(convex_polygon(&fl, 5, 0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 3.0, 3.0, 5.0, 0.0));
    printf("%d\n", fl);
    cnt = 4;
    if (!(res = (enum err *)malloc(sizeof(enum err) * cnt)))
    {
        printf("Не удалось выделить память\n");
    }
    Kaprekars_num(res, 10, cnt, "1", "9", "45", "17");
    print_Kapric_res(res, cnt);
    free(res);
    res = NULL;

    cnt = 7;
    if (!(res = (enum err *)malloc(sizeof(enum err) * cnt)))
    {
        printf("Не удалось выделить память\n");
    }
    // сделать чтобы маленькие буквы становились большими
    Kaprekars_num(res, 10, cnt, "1", "999", "2223", "2728", "A", "922337203685477580", "33");
    print_Kapric_res(res, cnt);
    free(res);
    res = NULL;

    double c_y = 0.0;
    print_mistake(count_y(&c_y, 2.0, 2, 2.0, 3.0, 4.0));
    printf("%lf\n", c_y);
    return 0;
}