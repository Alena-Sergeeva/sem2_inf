#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

enum err{
    OK,
    NOT_NUM,
    WRONG_FLAG,
    OVERFLOW_L_INT,
    OVERFLOW_DUB,
    CNT_ARGUMENTS,
    WRONGE_EPS_VALUE,
    ONE_SOLVE,
    NOT_SOLVE,
    NOT_quadratic_equation,
    ANY,
    NEGATIV_SIDE,
    NOT_NUM_LOOSE_MEAN,
    NOT_NUM_L_INT
};

int solve_quadratic_equation(double eps, double a, double b, double c, double *res1, double *res2);
void print_res(int mistake, double res1, double res2);

int multiples(long int num1, long int num2, int * f);
int is_right_triangle(double eps, double a, double b, double c, int *f);

int is_db_correct(char * argv, double * a);
int is_li_correct(char * argv, long int * a);
int valid (int argc, char * argv[], double *eps, double * a, double * b, double * c, long int * num_1, long int * num_2);

int main(int argc, char * argv[])
{
    /*int k = 0;
    is_right_triangle(0.001, 4.0001, 5, 3, &k);
    printf("%d\n", k);
    multiples(-2, 4, &k);
    printf("%d\n", k);
    double res1 = 0.0, res2 = 0.0;
    int mistake = solve_quadratic_equation(0.0001, -3, 4, -2, &res1, &res2);
    printf("%lf %lf\n", res1, res2);
    mistake = solve_quadratic_equation(0.0001, 0, 0, 0.00001, &res1, &res2);
    printf("%lf %lf %d\n", res1, res2, mistake);*/
    double eps, a, b, c;
    long int num1, num2;
    enum err mistake = 0;
    double res1 = 0.0, res2 = 0.0;
    int f = 0;
    switch (valid(argc, argv, &eps, &a, &b, &c, &num1, &num2))
    {
        case OK:
            printf("%lf %lf %lf %lf %ld %ld\n", eps, a, b, c, num1, num2);
            switch (argv[1][1])
            {
                case 'q':
                    mistake = solve_quadratic_equation(eps, a, b, c, &res1, &res2);
                    print_res(mistake, res1, res2);
                    mistake = solve_quadratic_equation(eps, c, a, b, &res1, &res2);
                    print_res(mistake, res1, res2);
                    mistake = solve_quadratic_equation(eps, b, c, a, &res1, &res2);
                    print_res(mistake, res1, res2);
                    break;
                case 'm':
                    multiples(num1, num2, &f);
                    if (f == 0)
                    {
                        printf("Первое число не кратно второму\n");
                    }
                    else
                    {
                        printf("Первое число кратно второму\n");
                    }
                    break;
                case 't':
                    if (is_right_triangle(eps, a, b, c, &f) == NEGATIV_SIDE)
                    {
                        printf("Стороны треугольника должны быть больше 0\n");
                    }
                    else
                    {
                        if (f == 0)
                        {
                            printf("Не является прямым трегольником\n");
                        }
                        else
                        {
                            printf("Прямой треугольник\n");
                        }
                    }
                    break;

                }
                break;

            case NOT_NUM_L_INT:
                printf("Аргумент не является десятичным числлом\n");
                break;

            case NOT_NUM:
                printf("Аргумент не является числом\n");
                break;

            case OVERFLOW_L_INT:
                printf("Переполнение типа long int\n");
                break;

            case OVERFLOW_DUB:
                printf("Переполнение типа double\n");
                break;

            case NOT_NUM_LOOSE_MEAN:
                printf("Аргумент не является числом или произошла потеря значимости\n");
                break;

            case WRONG_FLAG:
                printf("Неверный флаг\n");
                break;

            case CNT_ARGUMENTS:
                printf("Неверное количество аргументов\n");
                break;

            case WRONGE_EPS_VALUE:
                printf("eps - должно быть положительным числом\n");
                break;

    }

    return 0;
}

void print_res(int mistake, double res1, double res2)
{
    if (mistake == OK)
    {
        printf("%lf %lf\n", res1, res2);
    }
    if (mistake == ANY)
    {
        printf("x -- любое\n");
    }
    if (mistake == ONE_SOLVE)
    {
        printf("%lf\n", res1);
    }
    if (mistake == NOT_SOLVE)
    {
        printf("Уравнение не имеет действительного решения\n");
    }
    if (mistake == NOT_quadratic_equation)
    {
        printf("%lf не является квадратным уравнением\n", res1);
    }
}

int solve_quadratic_equation(double eps, double a, double b, double c, double *res1, double *res2)
{
    double d = b * b - 4 * a * c;
    //printf("%lf\n", b * b - 4 * a * c);
    //if (d < eps)

    if (d < 0)
    {
        return NOT_SOLVE;
    }
    if (fabs(a) < eps)
    {
        //bx + c = 0
        if ((fabs(c) < eps) && (fabs(b) < eps))
            return ANY;
        if ((fabs(b) < eps) && !(fabs(c) < eps))
            return NOT_SOLVE;
        (fabs(c) < eps) ? (*res1 = 0.0) : (*res1 = (c*(-1 ))/ b);
        return NOT_quadratic_equation;
    }
    *res1 = ((-1) * b + pow(d, 0.5))/ (2 * a);
    *res2 = ((-1) * b - pow(d, 0.5))/ (2 * a);
    if (fabs(*res1 - *res2) < eps)
    {
        return ONE_SOLVE;
    }
    return 0;
}


/*
 -m необходимо задать два ненулевых целых числа, после чего определить, кратно
ли первое число второму
*/
int multiples(long int num1, long int num2, int *f)
{
    (abs(num1) % num2 == 0) ? (*f = 1) : (*f = 0);
    return 0;
}
/*
-t первый пар аметр (вещественное число) задаёт точность сравнения вещественных
чисел (эпсилон); необходимо проверить, могут ли оставшиеся три (вещественные
числа) параметра являться длинами сторон прямоугольного треугольника.
*/
int is_right_triangle(double eps, double a, double b, double c, int *f)
{
    if ((fabs(a) < eps) || (fabs(b) < eps) || (fabs(c) < eps))
        return NEGATIV_SIDE;
    double max_side = max(max(a, b), max(b, c));
    double min_side = min(min(a, b), min(b, c));
    double sr_side = a + b + c - min_side - max_side;

    //printf("%lf %lf %lf\n", max_side, sr_side, min_side);

    (fabs(min_side * min_side + sr_side * sr_side - max_side * max_side) < eps) ? (*f = 1) : (*f = 0);

    //printf("%lf\n", min_side * min_side + sr_side * sr_side - max_side * max_side);
    return 0;
}

void len(long int x, int *i)
{
    if ((x < 0) || (x == 0))
    {
        ++ (*i);
        x = abs(x);
    }
    while (x > 0){
        x /= 10;
        ++(*i);
    }
}

int is_li_correct(char * argv, long int * x)
{
    char * end_num = NULL;
    *x = strtol(argv, &end_num, 10);
    int len_x = 0;
    len(*x, &len_x);

    if ((*x == LONG_MAX) || (*x == LONG_MIN))
    {
        return OVERFLOW_L_INT;
    }
    if ((len_x != strlen(argv)) || (*x == 0 && strcmp(argv, "0") != 0))
    {
        return NOT_NUM_L_INT;
    }
    return OK;
}

int is_db_correct(char * argv, double * a)
{
    char * pt_stop_symbol = NULL;
    * a = strtold(argv, &pt_stop_symbol);
    if ((*a == HUGE_VALL) || (*a == (-HUGE_VALL)))
    {
        return OVERFLOW_DUB;
    }
    if ((strcmp(argv, "0") != 0) && (*a == 0))
    {
        return NOT_NUM_LOOSE_MEAN;
    }

    if ((*a == 0) && (strlen(argv) != 1))
    {
        return NOT_NUM;
    }
    if (!(*(pt_stop_symbol) == '\0'))
    {
        return NOT_NUM;
    }
    return OK;
}

int valid (int argc, char * argv[], double *eps, double * a, double * b, double * c, long int * num_1, long int * num_2)
{
    if (argc < 4)
    {
        return CNT_ARGUMENTS;
    }
    if (!((argv[1][0] == '-' || argv[1][0] == '/') && strstr(" q m t ", argv[1]+1) && (strlen(argv[1])== 2)))
    {
        return WRONG_FLAG;
    }
    enum err mistake = 0;
    if ((argv[1][1] == 'q') || (argv[1][1] == 't'))
    {
        if (argc != 6)
        {
            return CNT_ARGUMENTS;
        }
        mistake = is_db_correct(argv[2], eps);
        if (mistake != OK)
        {
            return mistake;
        }
        if (eps < 0)
        {
            return WRONGE_EPS_VALUE;
        }
        mistake = is_db_correct(argv[3], a);
        if (mistake != OK)
        {
            return mistake;
        }
        mistake = is_db_correct(argv[4], b);
        if (mistake != OK)
        {
            return mistake;
        }
        mistake = is_db_correct(argv[5], c);
        if (mistake != OK)
        {
            return mistake;
        }
        return OK;
    }
    if (argv[1][1] == 'm')
    {
        if (argc != 4)
        {
            return CNT_ARGUMENTS;
        }
        mistake = is_li_correct(argv[2], num_1);
        if (mistake != OK)
        {
            return mistake;
        }
        mistake = is_li_correct(argv[3], num_2);
        if (mistake != OK)
        {
            return mistake;
        }
        return OK;
    }
}
