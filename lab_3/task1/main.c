#include <stdio.h>
#include <stdlib.h>

enum err
{
    OK,
    WRONG_POINTER,
    MEMMORY_ERROR,
    EMPTY_OR_WRONG_POINTER
};

typedef struct list_item
{
    struct list_item *next;
    char data;
} List_item;

int print_st(List_item *head)
{
    List_item *ptr = NULL;
    if (head == NULL)
    {
        return EMPTY_OR_WRONG_POINTER;
    }
    while (head != NULL)
    {
        printf("%c", head->data);
        ptr = head;
        head = head->next;
        free(ptr);
    }
    printf("\n");
    return OK;
}

int push(List_item **head, char c)
{
    List_item *ptr = NULL;
    if (head == NULL)
    {
        return WRONG_POINTER;
    }
    ptr = *head;
    if (!(*head = (List_item *)malloc(sizeof(List_item))))
    {
        return MEMMORY_ERROR;
    }
    (*head)->next = ptr;
    (*head)->data = c;
    return OK;
}

int sum(int num1, int num2, int *res)
{
    int ost = 1;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    *res = num1;
    while (ost != 0)
    {
        ost = (num1 & num2);
        num1 = num1 ^ num2;
        num2 = ost << 1;
    }
    *res = num1;
    return OK;
}

int negative(int num, int *res)
{
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    return sum(~num, 1, res);
}

int abs_b(int *num, int *fl)
{
    int res = 0;
    if ((fl == NULL) || (num == NULL))
    {
        return WRONG_POINTER;
    }
    if (*num < 0)
    {
        *fl = 1;
        if (negative(*num, &res))
        {
            return WRONG_POINTER;
        }
        *num = res;
    }
    return OK;
}

int to_base_2_r(int num, int r, List_item **res, char *nums)
{
    enum err mistake = 0;
    int base = 1 << r;
    int fl = 0;
    int proc_b = 0;
    int num_n = 0;
    if (abs_b(&num, &fl))
    {
        return WRONG_POINTER;
    }
    if (sum((base), -1, &proc_b))
    {
        return WRONG_POINTER;
    }
    // printf("%d %d\n", proc_b, base);
    do
    {
        if (mistake = push(res, nums[num & proc_b]))
        {
            return mistake;
        }
        num >>= r;
    } while (num != 0);
    if (fl == 1)
    {
        push(res, '-');
    }
    return OK;
}

int main()
{
    List_item *res = NULL;
    char *nums = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    switch (to_base_2_r(570, 1, &res, nums))
    {
    case OK:
        if (print_st(res) == EMPTY_OR_WRONG_POINTER)
        {
            printf("Попытка распечатать пустой результат или передан нулевой указатель\n");
        }
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    case MEMMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    }
    return 0;
}