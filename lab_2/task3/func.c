/* функцию с переменным числом аргументов, принимающую в качестве
входных параметров подстроку и пути к файлам. Необходимо чтобы для каждого
файла функция производила поиск всех вхождений переданной подстроки в
содержимом этого файла. При реализации запрещается использование функций strstr и
strncmp из заголовочного файла string.h. Продемонстрируйте работу функции, также
организуйте наглядный вывод результатов работы функции: для каждого файла, путь к
которому передан как параметр Вашей функции, для каждого вхождения подстроки в
содержимое файла необходимо вывести номер строки (индексируется с 1) и номер
символа в строке файла, начиная с которого найдено вхождение подстроки. В
подстроку могут входить любые символы (обратите внимание, что в подстроку также
могут входить символы пробела, табуляций, переноса строки, в неограниченном
количестве
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#define BUFSIZE 256

enum err
{
    OK,
    FILE_NOT_OPEN,
    MEMMORY_ERROR,
    WRONG_POINTER,
    WRONG_CNT
};

typedef struct Result
{
    struct Result *next;
    int line;
    int index;
} Result;

int full_suf_array(int **suf, char *str)
{
    int i = 1, j = 0;
    if (((*suf) == NULL) || (str == NULL))
    {
        return WRONG_POINTER;
    }
    (*suf)[0] = 0;
    while (str[i] != '\0')
    {
        if (str[i] != str[j])
        {
            if (j == 0)
            {
                (*suf)[i] = 0;
                ++i;
            }
            else
            {
                j = (*suf)[j - 1];
            }
        }
        else
        {
            (*suf)[i] = j + 1;
            ++i;
            ++j;
        }
    }
    return 0;
}

int add_elem(Result *head, Result **top, int line1, int index1)
{
    Result *top_new = NULL;
    if (*top == NULL)
    {
        return WRONG_POINTER;
    }
    (*top)->line = line1;
    (*top)->index = index1;
    if (!(top_new = (Result *)malloc(sizeof(Result))))
    {
        return MEMMORY_ERROR;
    }
    (*top)->next = top_new;
    *top = top_new;
    return 0;
}

int create_result_list(Result **res)
{
    if (!(*res = (Result *)malloc(sizeof(Result))))
    {
        return MEMMORY_ERROR;
    }
    return 0;
}

void print_and_clear(Result **head, Result **top, char *file_name, int fl_err)
{
    Result *temp_pointer = NULL;
    printf("%s\n", file_name);
    if (fl_err)
    {
        printf("не удалось выделить памяти на все элементы, выведу те которые удалось записать\n");
    }
    if (*head == *top)
    {
        printf("Вхождение подстроки в файл не было найдено\n");
        free(*head);
        *top = NULL;
        *head = NULL;
        return;
    }
    while (*head != *top)
    {
        printf("в %d строке на %d позиции\n", (*head)->line, (*head)->index);
        temp_pointer = (*head)->next;
        free(*head);
        *head = temp_pointer;
    }
    free(*head);
    *top = NULL;
    *head = NULL;
    return;
}

int read_file(char *file_in, char *str, int *suf)
{
    FILE *fin = NULL;
    int line = 1, i = 0, j = 0, index = 0;
    char c = ' ';
    Result *head = NULL;
    Result *top = NULL;
    if ((str == NULL) || (suf == NULL))
    {
        return WRONG_POINTER;
    }
    if ((fin = fopen(file_in, "r")) == NULL)
    {
        return FILE_NOT_OPEN;
    }
    if (create_result_list(&head))
    {
        return MEMMORY_ERROR;
    }
    top = head;
    while (!feof(fin))
    {
        c = fgetc(fin);
        if (c == str[j])
        {
            ++i;
            ++j;
        }
        else
        {
            if (j == 0)
            {
                ++i;
            }
            else
            {
                while ((c != str[j]) && (j != 0))
                {
                    j = suf[j - 1];
                }
                ++i;
            }
        }
        if (str[j] == '\0')
        {
            index = i - j + 1;
            j = 0;
            if (add_elem(head, &top, line, index))
            {
                print_and_clear(&head, &top, file_in, 1);
                fclose(fin);
                return MEMMORY_ERROR;
            }
        }
        if (c == '\n')
        {
            i = 0;
            j = 0;
            ++line;
        }
    }
    print_and_clear(&head, &top, file_in, 0);
    fclose(fin);
    return 0;
}

int find_str_in_files(char *str, int cnt, ...)
{
    va_list iterator;
    int *suf = NULL;
    int i = 0;
    char *file_in = NULL;
    enum err mistake = 0;
    if (str == NULL)
    {
        return WRONG_POINTER;
    }
    if (cnt <= 0)
    {
        return WRONG_CNT;
    }
    if (!(suf = (int *)malloc(sizeof(int) * strlen(str))))
    {
        return MEMMORY_ERROR;
    }
    if (full_suf_array(&suf, str))
    {
        return WRONG_POINTER;
    }
    va_start(iterator, cnt);
    for (i = 0; i < cnt; ++i)
    {
        file_in = va_arg(iterator, char *);
        if (mistake = read_file(file_in, str, suf))
        {
            return mistake;
        }
    }
    va_end(iterator);
    free(suf);
    return 0;
}

int main()
{
    switch (find_str_in_files("liillillil", 2, "test1.txt", "test2.txt"))
    {
    case MEMMORY_ERROR:
        printf("Не удалось выделить памят\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    case WRONG_CNT:
        printf("Колличество аргументов должно быть положительным\n");
        break;
    case FILE_NOT_OPEN:
        printf("Файл не удалось открыть\n");
        break;
    }
    return 0;
}