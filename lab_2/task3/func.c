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

int full_suf_array(int *suf, char *str)
{
    int i = 1, j = 0;
    (suf)[0] = 0;
    while (str[i] != '\0')
    {
        if (str[i] != str[j])
        {
            if (j == 0)
            {
                ++i;
                (suf)[i] = 0;
            }
            else
            {
                j = (suf)[j - 1];
            }
        }
        else
        {
            (suf)[i] = j + 1;
            ++i;
            ++j;
        }
    }
    return 0;
}
// вернет указатель на первое вхождение строки
int find_str(char *buf, char *exampl, int *suf, int *res)
{
    int i = 0, j = 0;
    if ((buf == NULL) || (exampl == NULL) || (suf == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }
    *res = (-1);
    while ((buf[i] != '\n') && (buf[i] != '\0') && (exampl[j] != '\0'))
    {
        if (buf[i] == exampl[j])
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
                j = suf[j - 1];
            }
        }
    }
    if (exampl[j] == '\0')
    {
        *res = i - j + 1;
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
        printf("%d %d\n", (*head)->line, (*head)->index);
        temp_pointer = (*head)->next;
        free(*head);
        *head = temp_pointer;
    }
    // printf("%d %d\n", (*head)->line, (*head) -> index);
    return;
}

int read_file(char *file_in, char *str, int *suf)
{
    FILE *fin = NULL;
    int i = 1, index = 0, ost = 0;
    size_t str_length = strlen(str);
    char buf[BUFSIZE];
    char *buf_begin = buf;
    enum err mistake = 0;
    Result *head = NULL;
    Result *top = NULL;
    if (create_result_list(&head))
    {
        return MEMMORY_ERROR;
    }
    top = head;
    if ((fin = fopen(file_in, "r")) == NULL)
    {
        return FILE_NOT_OPEN;
    }
    while (fgets(buf, 256, fin) != NULL)
    {
        buf_begin = buf;
        if (mistake = find_str(buf, str, suf, &index))
        {
            return mistake;
        }
        while (index != (-1))
        {
            if (mistake = add_elem(head, &top, i, index + (buf_begin - buf + 1)))
            {
                print_and_clear(&head, &top, file_in, 1);
            }
            buf_begin += (index - 1 + str_length - 1);
            if (mistake = find_str(buf_begin, str, suf, &index))
            {
                return mistake;
            }
        }
        ++i;
    }
    print_and_clear(&head, &top, file_in, 0);
    free(head);
    head = NULL;
    top = head;
    fclose(fin);
    return 0;
}

int find_str_in_files(char *str, int cnt, ...)
{
    va_list iterator;
    int str_length = strlen(str);
    int suf[str_length], i = 0;
    char *file_in;
    enum err mistake;
    if (str == NULL)
    {
        return WRONG_POINTER;
    }
    if (cnt <= 0)
    {
        WRONG_CNT;
    }
    full_suf_array(suf, str);
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
    return 0;
}

int main()
{
    enum err mistake = find_str_in_files("pppp", 2, "test1.txt", "test2.txt");
    return 0;
}