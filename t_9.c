#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define N 10

enum err
{
    OK,
    MEMORY_ERROR,
    NOT_NUM,
    OVERFLOW_LONG_INT,
    SIDES,
    ARGUMENTS
};

void len(long int x, int *i)
{
    if ((x < 0) || (x == 0))
    {
        ++(*i);
        x = abs(x);
    }
    while (x > 0)
    {
        x /= 10;
        ++(*i);
    }
}

int check_long_int(char *str, long int *x)
{
    char *end_num = NULL;
    *x = strtol(str, &end_num, 10);
    int len_x = 0;
    len(*x, &len_x);
    if ((*x == LONG_MAX) || (*x == LONG_MIN))
    {
        return OVERFLOW_LONG_INT;
    }
    if ((len_x != strlen(str)) || (*x == 0 && strcmp(str, "0") != 0))
    {
        return NOT_NUM;
    }
    return 0;
}
int valid(int argc, char *argv[], long int *a, long int *b)
{
    enum err mistake = 0;
    if (argc != 3)
    {
        return ARGUMENTS;
    }
    if ((mistake = check_long_int(argv[1], a)) != OK)
    {
        return mistake;
    }
    if ((mistake = check_long_int(argv[2], b)) != OK)
    {
        return mistake;
    }
    if (*a > *b)
    {
        return SIDES;
    }
    return 0;
}

/*
 1. Заполнить массив фиксированного размера псевдослучайными числами в диапазоне
[a..b], где a, b задаются в качестве аргументов командной строки. Реализовать
функцию, выполняющую поиск максимального и минимального значений элементов
массива и меняющую местами максимальный и минимальный элементы в исходном
массиве за один проход по нему.
*/
void full_massive_rand_numb(long int a, long int b, long int *massive, int size)
{
    for (int i = 0; i < size; ++i)
    {
        massive[i] = a + rand() % (b - a + 1);
    }
}

void find_change_max_min_elem(long int *a, int size)
{
    int max_i = 0, min_i = 0, i = 0;
    long int max_num = LONG_MIN, min_num = LONG_MAX, temp = 0;
    for (i; i < size; ++i)
    {
        if (a[i] > max_num)
        {
            max_i = i;
            max_num = a[i];
        }
        if (a[i] < min_num)
        {
            min_i = i;
            min_num = a[i];
        }
    }
    temp = a[max_i];
    a[max_i] = a[min_i];
    a[min_i] = temp;
    return;
}

/*
2. Заполнить динамические массивы А и В псевдослучайного размера в диапазоне
[10..10000] псевдослучайными числами в диапазоне [-1000..1000]. Сформировать из
них динамический массив С, где i-й элемент массива C есть i-й элемент массива А
(A[i]), к которому добавлено значение ближайшего к A[i] по значению элемента из
массива В (если ближайших по значению элементов несколько, допустимо добавление
любого из них).
*/
int creat_dinamic_massive(long int **A, long int **B, int *size_a, int *size_b)
{
    *size_a = 10 + rand() % (10000 - 10 + 1);
    //*size_a = 5 + rand() % (10 - 5 + 1);
    if (!(*A = (long int *)malloc(sizeof(long int) * (*size_a))))
    {
        return MEMORY_ERROR;
    }
    *size_b = 10 + rand() % (10000 - 10 + 1);
    //*size_b = 5 + rand() % (10 - 5 + 1);
    if (!(*B = (long int *)malloc(sizeof(long int) * (*size_b))))
    {
        free(*A);
        return MEMORY_ERROR;
    }
    full_massive_rand_numb(-1000, 1000, *A, *size_a);
    full_massive_rand_numb(-1000, 1000, *B, *size_b);
    return 0;
}
int max(const void *a, const void *b)
{
    return (*(long int *)a - *(long int *)b);
}

void print_massive(long int *a, long int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("%ld ", a[i]);
    }
    printf("\n");
}

int merge_A_B(long int **C, int *size_c, long int *A, int size_a, long int *B, int size_b, int min_ras)
{
    *size_c = size_a;
    *C = (long int *)malloc(sizeof(long int) * (*size_c));
    if (!(*C))
    {
        return MEMORY_ERROR;
    }

    qsort(A, size_a, sizeof(long int), max);
    qsort(B, size_b, sizeof(long int), max);
    int i = 0, j = 0, k = 0, min_ras_1 = min_ras, close_num = 0;
    while ((i < size_a) && (k < *size_c))
    {
        if (abs(A[i] - B[j]) < min_ras_1)
        {
            min_ras_1 = abs(A[i] - B[j]);
            close_num = B[j];
            ++j;
        }
        else
        {
            *(*C + k) = A[i] + close_num;
            ++k;
            ++i;
            --j;
            min_ras_1 = min_ras;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int n = 10;
    long int m[n];
    long int a, b;
    int size_a = 0, size_b = 0, size_c = 0;
    long int *A = NULL, *B = NULL, *C = NULL;
    enum err mistake1 = 0, mistake2 = 0;
    switch (valid(argc, argv, &a, &b))
    {
    case OK:
        printf("Первая функция:\nисходный массив: ");
        full_massive_rand_numb(a, b, m, N);
        print_massive(m, N);
        find_change_max_min_elem(m, N);
        printf("Массив после обработки: ");
        print_massive(m, N);

        if (creat_dinamic_massive(&A, &B, &size_a, &size_b) == MEMORY_ERROR)
        {
            printf("Не удалось выделить мапять");
        }
        else
        {
            printf("Вторая функция:\nисходный массив A: ");
            print_massive(A, size_a);
            printf("исходный массив B: ");
            print_massive(B, size_b);
            if (merge_A_B(&C, &size_c, A, size_a, B, size_b, 2000) == MEMORY_ERROR)
            {
                printf("Не удалось выделить память\n");
            }
            else
            {
                printf("Конечный массив C: ");
                print_massive(C, size_c);
            }
        }
        break;
    case ARGUMENTS:
        printf("Неверное колличество аргументов\n");
        break;
    case SIDES:
        printf("Левая граница должна быть меньше правой\n");
        break;
    case OVERFLOW_LONG_INT:
        printf("Значение границы превышает тип int\n");
        break;
    case NOT_NUM:
        printf("Значение границы должно быть целым числом\n");
        break;
    }

    free(A);
    free(B);
    free(C);
    return 0;
}