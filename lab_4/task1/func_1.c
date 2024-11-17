#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASHSIZE 128

enum err
{
    OK,
    MEMMORY_ERROR,
    WRONG_POINTER,
    WRONG_DEF_NAME,
    FILE_NOT_OPEN,
    INT_OVERFLOW,
    NO_DEFINES,
    SAME_MACROS
};

/*
Реализуйте приложение для организации макрозамен в тексте. На вход приложению
через аргументы командной строки подаётся путь к текстовому файлу, содержащему в
начале набор директив #define, а далее обычный текст. Синтаксис директивы
соответствует стандарту языка C:
#define <def_name> <value>
Аргументов у директивы нет, директива не может быть встроена в другую директиву.
Ваше приложение должно обработать содержимое текстового файла, выполнив замены
последовательностей символов <def_name> на <value>. Количество директив
произвольно, некорректных директив нет, объём текста во входном файле произволен.
В имени <def_name> допускается использование символов латинского алфавита
(прописные и строчные буквы не отождествляются) и символов арабских цифр;
значение <value> произвольно и завершается символом переноса строки или символом
конца файла. Для хранения имен макросов и макроподстановок используйте
хеш-таблицу размера HASHSIZE (начальное значение равно 128). Для вычисления
. Хеш-значение для <def_name> в рамках хеш-таблицы вычисляйте как
остаток от деления эквивалентного для <def_name> числа в системе счисления с
основанием 10 на значение HASHSIZE. Для разрешения коллизий используйте метод
цепочек. В ситуациях, когда после модификации таблицы длины самой короткой и
самой длинной цепочек в хеш-таблице различаются в 2 раза и более, пересобирайте
хеш-таблицу с испо льзованием другого значения HASHSIZE (логику модификации
значения HASHSIZE продумайте самостоятельно) до достижения примерно
равномерного распределения объектов структур по таблице. Оптимизируйте расчёт
хэш-значений при пересборке таблицы при помощи кэширования.
*/

typedef struct Node
{
    char *def_name;
    char *value;
    unsigned int hash_func_value;
    struct Node *next;
} Node;

typedef struct Hash_table
{
    int hashsize;
    Node **array;
} Hash_table;

// создаем таблицу, иницилизируем ее массив нулами
int creat_and_intil_hash_table(Hash_table **table, unsigned int size)
{
    int i = 0;
    if (table == NULL)
    {
        return WRONG_POINTER;
    }
    if (!(*table = (Hash_table *)malloc(sizeof(Hash_table))))
    {
        return MEMMORY_ERROR;
    }
    (*table)->hashsize = size;
    if (!((*table)->array = (Node **)malloc(sizeof(Node *) * size)))
    {
        free(*table);
        *table = NULL;
        return MEMMORY_ERROR;
    }
    for (i = 0; i < size; ++i)
    {
        (*table)->array[i] = NULL;
    }
    return OK;
}

int clear_list(Node **head_node)
{
    Node *temp_node = NULL;
    if (head_node == NULL)
    {
        return WRONG_POINTER;
    }
    while (*head_node != NULL)
    {

        free((*head_node)->def_name);
        free((*head_node)->value);
        temp_node = (*head_node)->next;
        free(*head_node);
        *head_node = temp_node;
    }
    free(*head_node);
    *head_node = NULL;
    return OK;
}

int clear_hach_table(Hash_table **table)
{
    int i = 0;
    enum err mistake = OK;
    if (table == NULL)
    {
        return WRONG_POINTER;
    }
    if (*table == NULL)
    {
        return OK;
    }
    for (i = 0; i < (*table)->hashsize; ++i)
    {
        if ((*table)->array[i] != NULL)
        {
            if (mistake = clear_list((*table)->array + i))
            {
                free((*table)->array);
                free(*table);
                *table = NULL;
                return mistake;
            }
        }
        //????????как будто что-то не освободила
    }
    free((*table)->array);
    free(*table);
    *table = NULL;
    return OK;
}

int print_hash_table(Hash_table *table)
{
    int i = 0;
    Node *temp_node = NULL;
    enum err mistake = OK;
    if (table == NULL)
    {
        return WRONG_POINTER;
    }
    printf("hashsize: %d\n", table->hashsize);
    for (i = 0; i < table->hashsize; ++i)
    {
        if (table->array[i] != NULL)
        {
            temp_node = table->array[i];
            while (temp_node != NULL)
            {
                printf("key - %d; def_name - %s; value - %s hash_value -%d\n", i, temp_node->def_name, temp_node->value, temp_node->hash_func_value);
                temp_node = temp_node->next;
            }
        }
    }
    return OK;
}
// Cоздаем ноду списка
int create_node_and_full(char *def_name, int size1, char *value, int size2, unsigned int hash_func_value, Node **res)
{
    if ((res == NULL) || (def_name == NULL) || (value == NULL))
    {
        return WRONG_POINTER;
    }

    if (!(*res = (Node *)malloc(sizeof(Node))))
    {
        return MEMMORY_ERROR;
    }
    if (!((*res)->def_name = (char *)malloc(sizeof(char) * (size1))))
    {
        free(res);
        return MEMMORY_ERROR;
    }
    if (!((*res)->value = (char *)malloc(sizeof(char) * (size2))))
    {
        free((*res)->def_name);
        free(res);
        return MEMMORY_ERROR;
    }
    /*
    printf("%d %s\n", size1, def_name);
    for (int k = 0; k < size1; ++k)
    {
        printf("%d ", def_name[k]);
    }
    printf("\n");
    printf("%d %s\n", size2, value);
    for (int k = 0; k < size2; ++k)
    {
        printf("%d ", value[k]);
    }
    printf("\n");
    */
    strncpy((*res)->def_name, def_name, size1);
    strncpy((*res)->value, value, size2);
    (*res)->hash_func_value = hash_func_value;
    (*res)->next = NULL;
    return OK;
}

/*
хеш-функции - <def_name> как число, записанное в системе счисления
с основанием 62 (алфавит этой системы счисления состоит из символов {0, …,9, A, …,
Z, a, …, z})
*/
int hash_func(char *str, unsigned int *key)
{
    int i = 0;
    if ((key == NULL) || (str == NULL))
    {
        return WRONG_POINTER;
    }
    *key = 0;
    while (str[i] != '\0')
    {
        if (!isalnum(str[i]))
        {
            return WRONG_DEF_NAME;
        }
        if (isdigit(*str))
        {
            *key = *key * 62 + str[i] - '0';
        }
        else
        {
            if (('A' <= str[i]) && (str[i] <= 'Z'))
            {
                *key = *key * 62 + str[i] - 'A' + 10;
            }
            else
            {
                *key = *key * 62 + str[i] - 'a' + 10 + 26;
            }
        }
        ++i;
    }
    return OK;
}

int is_prime(unsigned int x, char *fl)
{
    int i = 3;
    if (fl == NULL)
    {
        return WRONG_POINTER;
    }
    for (i; (i * i <= x) && (x % i != 0); ++i)
    {
    };
    (x % i == 0) ? ((*fl) = 0) : ((*fl) = 1);
    return OK;
}

int rebild_hash_table(Hash_table **table)
{
    int new_hashsize = 0;
    char fl = 0;
    enum err mistake = OK;
    int i = 0, index = 0;
    Hash_table *table_new = NULL;
    Node *head_node = NULL, *res = NULL;
    if (table == NULL)
    {
        return WRONG_POINTER;
    }
    if ((*table)->hashsize << 1 < __INT_MAX__)
    {
        new_hashsize = (*table)->hashsize << 1;
    }
    else
    {
        return INT_OVERFLOW;
    }
    new_hashsize += 1;
    if (is_prime(new_hashsize, &fl))
    {
        return WRONG_POINTER;
    }

    while (!fl)
    {
        if (new_hashsize + 2 < __INT_MAX__)
        {
            new_hashsize += 2;
        }
        else
        {
            return INT_OVERFLOW;
        }
        if (is_prime(new_hashsize, &fl))
        {
            return WRONG_POINTER;
        }
    }

    if (mistake = creat_and_intil_hash_table(&table_new, new_hashsize))
    {
        return mistake;
    }
    for (i = 0; i < (*table)->hashsize; ++i)
    {
        if ((*table)->array[i] != NULL)
        {
            head_node = (*table)->array[i];
            while (head_node != NULL)
            {
                index = head_node->hash_func_value % new_hashsize;
                if (mistake = create_node_and_full(head_node->def_name, (strlen(head_node->def_name) + 1), head_node->value, (strlen(head_node->value) + 1), index, &res))
                {
                    return mistake;
                }
                res->next = table_new->array[index];
                table_new->array[index] = res;
                res = head_node->next;
                head_node = res;
            }
        }
    }
    clear_hach_table(table);
    *table = table_new;
    return OK;
}
int list_length(Node *head, int *length)
{
    if (length == NULL)
    {
        return WRONG_POINTER;
    }
    *length = 0;
    if (head == NULL)
    {

        return OK;
    }
    while (head != NULL)
    {
        head = head->next;
        ++(*length);
    }
    return OK;
}

int check_need_rebild(Hash_table *table, char *fl)
{
    int i = 0;
    int max_length = 0, min_length = __INT_MAX__;
    int length_i = 0;
    if (!fl)
    {
        return WRONG_POINTER;
    }
    *fl = 0;
    for (i = 0; i < table->hashsize; ++i)
    {
        if (table->array[i] != NULL)
        {
            if (list_length(table->array[i], &length_i))
            {
                return WRONG_POINTER;
            }
            if (max_length < length_i)
            {
                max_length = length_i;
            }
            if (min_length > length_i)
            {
                min_length = length_i;
            }
        }
    }
    // printf("max_length %d; min_length %d\n", max_length, min_length);
    if (max_length / min_length >= 2)
    {
        *fl = 1;
    }
    return OK;
}

int add_elem_to_table(Hash_table *table, char *def_name, int size1, char *value, int size2)
{
    unsigned int key = 0, index;
    enum err mistake = OK;
    Node *res = NULL;
    char fl = 0;
    if ((table == NULL) || (def_name == NULL) || (value == NULL))
    {
        return WRONG_POINTER;
    }
    if (mistake = hash_func(def_name, &key))
    {
        return mistake;
    }
    printf("def_name - %s value - %s hash_size - %d\n", def_name, value, key);
    index = key % table->hashsize;
    if (mistake = create_node_and_full(def_name, size1, value, size2, key, &res))
    {
        return mistake;
    }
    res->next = table->array[index];
    table->array[index] = res;

    return OK;
}

int check_buf_size(char **buf, int buf_end, int *capacity)
{
    char *new_buf = NULL;
    if ((buf == NULL) || (capacity == NULL))
    {
        return WRONG_POINTER;
    }

    if (buf_end >= *capacity)
    {
        if ((*capacity << 1 < __INT_MAX__))
        {
            *capacity <<= 1;
        }
        else
        {
            return INT_OVERFLOW;
        }
        if (!(new_buf = (char *)realloc(*buf, sizeof(char) * (*capacity))))
        {
            return MEMMORY_ERROR;
        }
        *buf = new_buf;
        new_buf = NULL;
    }
    return OK;
}

int split_func_wathespace(char c)
{
    return c == ' ';
}
int split_func_issymbol(char c)
{
    return (!isalnum(c));
}
int split_func_for_value(char c)
{
    return ((c == '\n') || (c == '\r'));
}
int split_func_isspace(char c)
{
    return isspace(c);
}

int read_lecsem(FILE *fin, FILE *fout, char **buf, int *capacity, int *buf_end, char *end_c, int (*split_func)(char c))
{
    char c = ' ';
    enum err mistake = 0;
    if ((fin == NULL) || (fout == NULL) || (buf == NULL) || (capacity == NULL) || (buf_end == NULL))
    {
        return WRONG_POINTER;
    }
    while ((!feof(fin)) && isspace(c = fgetc(fin)))
    {
        fprintf(fout, "%c", c);
    }
    *buf_end = 0;
    while ((!feof(fin)) && !(split_func(c)))
    {
        if (mistake = check_buf_size(buf, *buf_end, capacity))
        {
            free(*buf);
            *buf = NULL;
            return mistake;
        }
        (*buf)[*buf_end] = c;
        ++(*buf_end);
        c = fgetc(fin);
    }
    if (mistake = check_buf_size(buf, *buf_end, capacity))
    {
        free(*buf);
        *buf = NULL;
        return mistake;
    }

    (*buf)[*buf_end] = '\0';
    ++(*buf_end);
    *end_c = c;
    return OK;
}

int find_lecsem_in_table(char *str, int str_end, Hash_table *table, char **value)
{
    enum err mistake = OK;
    unsigned int key = 0;
    Node *temp_node = NULL;
    if ((str == NULL) || (table == NULL) || (value == NULL))
    {
        return WRONG_POINTER;
    }
    *value = NULL;
    mistake = hash_func(str, &key);
    key %= table->hashsize;
    if ((mistake == WRONG_DEF_NAME) || (table->array[key] == NULL))
    {
        return OK;
    }
    if (mistake)
    {
        return mistake;
    }
    temp_node = table->array[key];
    while (temp_node != NULL)
    {
        if (strncmp(str, temp_node->def_name, str_end) == 0)
        {
            *value = temp_node->value;
        }
        temp_node = temp_node->next;
    }
    return OK;
}

int read_diretive(FILE *fin, FILE *fout, char **buf, int *capacity, char *end_c, Hash_table **table)
{
    enum err mistake = OK;
    char *def_name = NULL;
    int buf_end = 0, buf_value_end = 0, value_capacity = 10;
    char fl = 0;
    char *buf_value = NULL;
    char *temp_value = NULL;
    if ((fin == NULL) || (fout == NULL) || (buf == NULL) || (capacity == NULL) || (table == NULL) || (*table == NULL) || (end_c == NULL))
    {
        return WRONG_POINTER;
    }
    if (!(buf_value = (char *)malloc(sizeof(char) * value_capacity)))
    {
        return MEMMORY_ERROR;
    }
    while (!feof(fin))
    {
        if (mistake = read_lecsem(fin, fout, buf, capacity, &buf_end, end_c, split_func_isspace))
        {
            free(buf_value);
            return mistake;
        }
        if (strncmp(*buf, "#define", buf_end) == 0)
        {
            fprintf(fout, "%s%c", *buf, *end_c);
            if (mistake = read_lecsem(fin, fout, buf, capacity, &buf_end, end_c, split_func_wathespace))
            {
                free(buf_value);
                return mistake;
            }
            fprintf(fout, "%s%c", *buf, *end_c);

            if (mistake = read_lecsem(fin, fout, &buf_value, &value_capacity, &buf_value_end, end_c, split_func_for_value))
            {
                free(buf_value);
                return mistake;
            }
            fprintf(fout, "%s%c", buf_value, *end_c);
            if (mistake = find_lecsem_in_table(*buf, buf_end, *table, &temp_value))
            {
                free(buf_value);
                return mistake;
            }
            if (temp_value == NULL)
            {
                if (mistake = add_elem_to_table(*table, *buf, buf_end, buf_value, buf_value_end))
                {
                    free(buf_value);
                    return mistake;
                }
                fl = 1;
            }
            else
            {
                free(buf_value);
                return SAME_MACROS;
            }
        }
        else
        {
            break;
        }
    }
    free(buf_value);
    if (fl == 0)
    {
        return NO_DEFINES;
    }
    return OK;
}

void close_free(FILE *fin, FILE *fout, char *buf, Hash_table **table)
{
    fclose(fin);
    fclose(fout);
    free(buf);
    clear_hach_table(table);
}

int read_file(char *file_in, char *file_out)
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    char *buf = NULL, *value = NULL;
    int capacity = 20, buf_end = 0;
    enum err mistake = OK;
    char end_c = ' ', fl = 0;
    Hash_table *table = NULL;

    if ((!(fin = fopen(file_in, "r"))) || (!(fout = fopen(file_out, "w"))))
    {
        fclose(fin);
        fclose(fout); // NULL?
        return FILE_NOT_OPEN;
    }
    if (!(buf = (char *)malloc(sizeof(char) * capacity)))
    {
        fclose(fin);
        fclose(fout);
        return MEMMORY_ERROR;
    }
    if (mistake = creat_and_intil_hash_table(&table, HASHSIZE))
    {
        fclose(fin);
        fclose(fout);
        free(buf);
        return mistake;
    }

    if (mistake = read_diretive(fin, fout, &buf, &capacity, &end_c, &table))
    {
        close_free(fin, fout, buf, &table);
        if (mistake == NO_DEFINES)
        {
            return OK;
        }
        return mistake;
    }

    print_hash_table(table);

    if (check_need_rebild(table, &fl))
    {
        close_free(fin, fout, buf, &table);
        return WRONG_POINTER;
    }

    if (fl == 1)
    {
        if (mistake = rebild_hash_table(&table))
        {
            close_free(fin, fout, buf, &table);
            return mistake;
        }
    }
    print_hash_table(table);

    while (!feof(fin))
    {
        if (mistake = find_lecsem_in_table(buf, buf_end, table, &value))
        {
            close_free(fin, fout, buf, &table);
            return mistake;
        }
        if (value == NULL)
        {
            value = buf;
        }
        fprintf(fout, "%s%c", value, end_c);

        if (mistake = read_lecsem(fin, fout, &buf, &capacity, &buf_end, &end_c, split_func_issymbol))
        {
            close_free(fin, fout, buf, &table);
            return mistake;
        }
    }
    if (mistake = find_lecsem_in_table(buf, buf_end, table, &value))
    {
        close_free(fin, fout, buf, &table);
        return mistake;
    }
    if (value == NULL)
    {
        value = buf;
    }
    fprintf(fout, "%s", value);
    close_free(fin, fout, buf, &table);
    return OK;
}

int copy_in_input_file(char *input_file, char *temparry_file)
{
    FILE *fin = NULL;
    FILE *temp_f = NULL;
    char c = ' ';
    if (!(fin = fopen(input_file, "w")))
    {
        return FILE_NOT_OPEN;
    }
    if (!(temp_f = fopen(temparry_file, "r")))
    {
        fclose(fin);
        return FILE_NOT_OPEN;
    }
    c = fgetc(temp_f);
    while (!feof(temp_f))
    {
        fputc(c, fin);
        c = fgetc(temp_f);
    }
    fclose(fin);
    fclose(temp_f);
    return OK;
}

void print_mistake(enum err mistake)
{
    switch (mistake)
    {
    case MEMMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    //????????????????
    case WRONG_DEF_NAME:
        printf("Макрос написан неправильно\n");
        break;
    case INT_OVERFLOW:
        printf("Переполнение типа int\n");
        break;
    case FILE_NOT_OPEN:
        printf("Не удалось открыть файл\n");
        break;
    // надо просто вернуть входной файл как было
    case NO_DEFINES:
        printf("Макросов не найдено, файл без изменений\n");
        break;
    case SAME_MACROS:
        printf("Встречено два одинаковых макроса\n");
        break;
    }
}
int main(int argc, char *argv[])
{
    enum err mistake = 0;
    if (mistake = read_file(argv[1], "temparary.txt"))
    {
        print_mistake(mistake);
        return 0;
    }
    if (copy_in_input_file(argv[1], "temparary.txt"))
    {
        printf("Не удалось открыть файл\n");
    }
    else
    {
        printf("Откройте входной файл\n");
    }
    return 0;
}