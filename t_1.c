#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

enum err{
    OK,
    NOT_NUM,
    WRONG_FLAG,
    NUM_TOO_BIG,
    ARGUMETS,
    NOT_SUIT,
    ONE
};


/*-h вывести в консоль натуральные числа в пределах 100 включительно, кратные x.
Если таковых нету – вывести соответствующее сообщение */
int multiples(long int x);
int prime_numb(long int x, int * pr_num);
int base_16(long int x, char * num_16);
int pw_table(long int x);
int sum_natur(long int x, long int *res);
int fact(long int x, long int *res);

void len(long int x, int *i);
int valid(int argc, char * argv[], long int * pt_x, char * fl);

int main(int argc, char * argv[])
{
    long int x = 0, res = 0;
    char fl = ' ';
    enum err f_mistake = 0;
    switch (valid(argc, argv, &x, &fl))
    {
        case OK:
            switch (fl)
            {
                case 'h':
                    f_mistake = multiples(x);
                    if (f_mistake == NOT_SUIT)
                    {
                        printf("x должно быть положительным, чтобы вывести кратные ему числа\n");
                    }

                    break;
                case 'p':
                    int pr_num = 0;
                    f_mistake = prime_numb(x, &pr_num);
                    if (f_mistake == NOT_SUIT)
                    {
                         printf("Число x неположительное, нельзя определить простое оно или составное\n");
                    }
                    if (f_mistake == ONE)
                    {
                         printf("Не является ни простым ни сотавным\n");
                    }
                    if (f_mistake == OK)
                    {
                        if (pr_num == 0)
                        {
                            printf("Число составное\n");
                        }
                        else
                        {
                            printf("Число простое\n");
                        }
                    }


                    break;
                case 's':
                    char num_16[sizeof(long int) * 8 + 1];
                    base_16(x, num_16);


                    for (int i = strlen(num_16)-1; i >= 0; --i)
                    {
                        printf("%c ", num_16[i]);
                    }
                    printf("\n");
                    break;
                case 'e':
                    if (pw_table(x) == NOT_SUIT)
                        printf("Число x неположительное\n");
                    break;
                case 'a':
                    if (sum_natur(x, &res) == NOT_SUIT)
                    {
                        printf("Число x меньше 1. Сумма натуральных чисел до x равна 1\n");
                    }
                    else
                    {
                        printf("%ld\n", res);
                    }
                    break;
                case 'f':
                    f_mistake = fact(x, &res);
                    if (f_mistake == NOT_SUIT)
                    {
                        printf("Нельзя вычислить факториал от отрицательного числа\n");
                    }
                    if (f_mistake == NUM_TOO_BIG)
                    {
                        printf("При вычислении произошло переполнение типа long int\n");
                    }
                    if (f_mistake == 0)
                    {
                        printf("%ld\n", res);
                    }
                    break;
            }
            break;
        case WRONG_FLAG:
            printf("Флаг введен не верно\n");
            break;
        case NOT_NUM:
            printf("Строка не вляется десятичным целым числом\n");
            break;
        case NUM_TOO_BIG:
            printf("переполнение типа long int\n");
            break;
        case ARGUMETS:
            printf("Введено неверное колличество аргументов\n");
            break;

    }

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
int valid(int argc, char * argv[], long int * pt_x, char * fl)
{
    if (argc != 3)
    {
        return ARGUMETS;
    }

    char * end_num = NULL;
    * pt_x = strtol(argv[1], &end_num, 10);
    int len_x = 0;
    len(* pt_x, &len_x);

    if ((*pt_x == LONG_MAX) || (*pt_x == LONG_MIN))
    {
        return NUM_TOO_BIG;
    }

    if ((len_x != strlen(argv[1])) || (*pt_x == 0 && strcmp(argv[1], "0") != 0))
    {
        return NOT_NUM;
    }

    if (!((argv[2][0] == '-' || argv[2][0] == '/') && strstr(" a h p s f e ", argv[2]+1) && (strlen(argv[2])== 2)))
    {
        return WRONG_FLAG;
    }
    if ((argv[2][1] == 'e') && (*pt_x > 10))
    {
        return NOT_SUIT;
    }
    *fl = argv[2][1];
    return 0;
}



int multiples(long int x)
{
    if (x <= 0)
    {
        return NOT_SUIT;
    }
    int i = 0;
    while (x * (i+1) <= 100)
    {
        ++i;
        printf("%ld ", x * i);
    }
    if (i == 0)
        printf("Чисел кратных x в пределах 100 включительно нет");
    printf("\n");
    return 0;
}

// -p определить, является ли число x простым; является ли x составным
int prime_numb(long int x, int * pr_num)
{
    if (x <= 0)
    {
        return NOT_SUIT;
    }
    if (x == 1)
    {
        return ONE;
    }
    int i=2;
    for (i; (i <= x / 2) && (x % i != 0); ++i){};
    (x % i == 0) ? ((*pr_num) = 0) : ((*pr_num) = 1);
    return 0;
}

/*разделить число x на отдельные цифры системы счисления с основанием 16 и
вывести отдельно каждую цифру числа, разделяя их пробелом, от старших
разрядов к младшим, без ведущих нулей в строковом представлении*/
int base_16(long int x, char * num_16)
{
    if (x == 0)
    {
        num_16[0] = '0';
        return 0;
    }
    int i = 0, y = 0;
    int n = abs(x);
    while (n > 0)
    {
        y = n % 16;
        num_16[i] = (y < 10) ? ('0' + y) : ('A' + y % 10);
        n /= 16;
        ++i;
    }
    num_16[i]='\0';
    return 0;
}

/*????-e вывести таблицу степеней (для всех показателей в диапазоне от 1 до x)
оснований от 1 до 10; для этого флага работает ограничение на вводимое число: x
должен быть не больше 10*/
int pw_table(long int x)
{
    if (x <= 0)
    {
        return NOT_SUIT;
    }
    int i, j;
    for (i= 1; i <=10; ++i)
    {
        long int res = 1;
        for (j = 1; j <= x; ++j)
        {
            res *= i;
            printf("%11ld ", res);
        }
        printf("\n");
    }
    return 0;
}

//-a вычислить сумму всех натуральных чисел от 1 до x и вывести полученное
//значение в консоль
int sum_natur(long int x, long int *res)
{
    if (x <= 0)
    {
        return NOT_SUIT;
    }
    (*res) = ((1 + x) * x)/2;
    return 0;
}

//-f вычислить факториал x и вывести полученное значение в консоль
int fact(long int x, long int *res)
{
    if (x < 0)
    {
        return NOT_SUIT;
    }
    (*res) = 1;
    int i = 2;
    while ((i <= x) && ((*res) * i > (*res)))
    {
        (*res) *= i;
        ++i;
    }
    if (i != x)
        return NUM_TOO_BIG;
    return 0;
}
