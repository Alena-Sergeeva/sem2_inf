#include <stdio.h>
#include <stdlib.h>

enum err
{
    OK,
    MEMORY_ERROR
};
/*
 1. Заполнить массив фиксированного размера псевдослучайными числами в диапазоне
[a..b], где a, b задаются в качестве аргументов командной строки. Реализовать
функцию, выполняющую поиск максимального и минимального значений элементов
массива и меняющую местами максимальный и минимальный элементы в исходном
массиве за один проход по нему.
*/
void full_massive_rand_numb(int a, int b, int *massive, int size)
{
    for (int i = 0; i < size; ++i)
    {
        massive[i] = a + rand() % (b - a + 1);
    }
}

void find_change_max_min_eiem(int *a, int size)
{
    int max_i = 0, min_i = 0, max_num = -100000000, min_num = 1000000000, i = 0, temp = 0;
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
int creat_dinamic_massive(int **A, int **B, int *size_a, int *size_b)
{
    //*size_a = 10 + rand() % (10000 - 10 + 1);
    if (!(*A = (int *)malloc(sizeof(int) * (*size_a))))
    {
        return MEMORY_ERROR;
    }
    //*size_b = 10 + rand() % (10000 - 10 + 1);
    if (!(*B = malloc(sizeof(int) * (*size_b))))
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
    return (*(int *)a - *(int *)b);
}

void print_massive(int *a, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

int merge_A_B(int **C, int *size_c, int *A, int size_a, int *B, int size_b)
{
    *size_c = (size_a > size_b) ? size_a : size_b;
    print_massive(A, size_a);
    print_massive(B, size_b);
    *C = (int *)malloc(sizeof(int) * (*size_c));
    if (!(*C))
    {
        return MEMORY_ERROR;
    }

    qsort(A, size_a, sizeof(int), max);
    qsort(B, size_b, sizeof(int), max);
    print_massive(A, size_a);
    print_massive(B, size_b);
    int i = 0, j = 0, k = 0, min_ras = 1000000, pr_num = 0;
    while ((i < size_a) && (j < size_b) && (k < *size_c))
    {
        if (abs(A[i] - B[j]) < min_ras)
        {
            min_ras = abs(A[i] - B[j]);
            pr_num = B[j];
            ++j;
        }
        else
        {
            *(*C + k) = A[i] + pr_num;
            printf("%d %d %d\n", A[i], pr_num, *(*C + k));
            ++k;
            ++i;
            min_ras = 10000000;
        }
    }
    print_massive(*C, *size_c);
}

int main()
{
    int n = 10;
    int a[n];
    int size_a = 10, size_b = 10, size_c = 0;
    int *A = NULL, *B = NULL, *C = NULL;
    creat_dinamic_massive(&A, &B, &size_a, &size_b);
    merge_A_B(&C, &size_c, A, size_a, B, size_b);
    free(A);
    free(B);
    free(C);
    return 0;
}