#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
/*-h вывести в консоль натуральные числа в пределах 100 включительно, кратные x.
Если таковых нету – вывести соответствующее сообщение */
void f_h(long int x)
{
    int i = 2;
    while (x <= 100)
    {
        printf("%ld ", x);
        x *= i;
        ++i;
    }
    if (i == 2)
        printf("Чисел кратных x в пределах 100 включительно нет");
    printf("\n");
}

// -p определить, является ли число x простым; является ли x составным
void f_p(long int x)
{
    int i=2;
    if (x < 0)
    {
        printf("Отрицательное число не может быть простым");
        return;
    }
    for (i; (i <= x / 2) && (x % i != 0); ++i){};
    /*if (x % i == 0)
        printf("x составное число")
    else printf("x простое число");*/
    ((x % i == 0) || (x == 1)) ? printf("x составное число\n") : printf("x простое число\n");
}

/*разделить число x на отдельные цифры системы счисления с основанием 16 и
вывести отдельно каждую цифру числа, разделяя их пробелом, от старших
разрядов к младшим, без ведущих нулей в строковом представлении*/
void ch(long int x)
{
    if (x == 0)
        return;
    ch(x / 16);
    int y = x % 16;

    printf("%c ", (y < 10) ? ('0' + y) : ('A' + y % 10));
}

void f_s(long int x)
{
    (x == 0) ? (printf("0")) : ch(abs(x));
    printf("\n");
}

/*????-e вывести таблицу степеней (для всех показателей в диапазоне от 1 до x)
оснований от 1 до 10; для этого флага работает ограничение на вводимое число: x
должен быть не больше 10*/


/*
void f_e(long int x)
{

}
*/

//-a вычислить сумму всех натуральных чисел от 1 до x и вывести полученное
//значение в консоль


// что делать с отрицательными числами ???????????

void f_a(long int x)
{
    printf("%ld", (1 + x) / 2 * x);
}

//-f вычислить факториал x и вывести полученное значение в консоль
void f_f(long int x)
{
    long long int f = 1;
    for (int i = 2; i <= x; ++i)
    {
        f *= i;
    }
    printf("%lld\n", f);
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
int valid(int argc, char * argv[], long int * pt_x)
{
    if (argc != 3)
    {
        printf("Введено неверное количество аргументов\n");
        return 0;
    }
    //нужно ли проверять корректность введения названия программы argv[0]
    * pt_x = strtol(argv[1],'\0', 10);
    int len_x = 0;
    len(* pt_x, &len_x);
    if ((len_x != strlen(argv[1])) || (*pt_x == 0 && strcmp(argv[1], "0") != 0))
    {
        printf("Строка не вляется десятичным целым числом\n");
        return 0;
    }

    //printf("%ld %d\n", *pt_x, len_x);

    if ((*pt_x == LONG_MAX) || (*pt_x == LONG_MIN))
    {
        printf("переполнение типа long int\n");
        return 0;
    }

    if (!((argv[2][0] == '-' || argv[2][0] == '/') && strstr("a h p s f e", argv[2]+1)))
    {
        printf("Флаг введен не верно\n");
        printf("%s", argv[2]+1);
        return 0;
    }
    if ((strcmp(argv[2], "-e") == 0) && (*pt_x > 10))
    {
        printf("x должен быть не больше 10\n");
        return 0;
    }
}


int main(int argc, char * argv[])
{
    long int x = 0;

    if (!valid(argc, argv, &x))
        return 1;

    if (&x == NULL)
        return 1;
    if (strcmp(argv[2], "-h") == 0)
        f_h(x);
    if (strcmp(argv[2], "-p") == 0)
        f_p(x);
    if (strcmp(argv[2], "-s") == 0)
        f_s(x);
    /*if (strcmp(argv[2], "-e") == 0)
        f_e(x);*/
    if (strcmp(argv[2], "-a") == 0)
        f_a(x);
    if (strcmp(argv[2], "-f") == 0)
        f_f(x);

    return 0;
}
