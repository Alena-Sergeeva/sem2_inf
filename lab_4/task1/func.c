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
    NO_DEFINES
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

typedef struct Elem
{
    int length;
    Node *head_list;
} Elem;

typedef struct Hash_table
{
    int hashsize;
    Elem **array;
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
    if (!((*table)->array = (Elem **)malloc(sizeof(Elem *) * size)))
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
int clear_array(Elem **array, int size)
{
    int i = 0;
    if (array == NULL)
    {
        return WRONG_POINTER;
    }
    for (i = 0; i < size; ++i)
    {
        free(array[i]);
    }
    *array = NULL;
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
            if (mistake = clear_list(&((*table)->array[i]->head_list)))
            {
                return mistake;
            }
        }
        free((*table)->array[i]);
        (*table)->array[i] == NULL; //????????как будто что-то не освободила
    }
    free((*table)->array);
    (*table)->array = NULL;
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
            temp_node = table->array[i]->head_list;
            printf("length - %d; ", table->array[i]->length);
            while (temp_node != NULL)
            {
                printf("key - %d; def_name - %s; value - %s\n", i, temp_node->def_name, temp_node->value);
                temp_node = temp_node->next;
            }
        }
    }
    return OK;
}

// Cоздаем ноду списка
int create_node_and_full(char *def_name, char *value, unsigned int hash_func_value, Node **res)
{
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    if (!(*res = (Node *)malloc(sizeof(Node))))
    {
        return MEMMORY_ERROR;
    }
    // if ((*res)->def_name = (char *) malloc(si))
    (*res)->def_name = def_name;
    (*res)->hash_func_value = hash_func_value;
    (*res)->next = NULL;
    (*res)->value = value;
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
        *key = *key * 62 + str[i] - (isdigit(*str) ? ('0') : (toupper(str[i]) - 'A'));
        if (('a' < str[i]) && (str[i] < 'z'))
        {
            *key += (26 + 10);
        }
        ++i;
    }
    return OK;
}

int try_found_new_min(Elem **array, unsigned int size, unsigned int *min_i)
{
    int i = 0;
    if (min_i == NULL)
    {
        return WRONG_POINTER;
    }
    *min_i = size;
    for (i; i < size; ++i)
    {
        if (array[i] != NULL)
        {
            if ((*min_i == size) || (array[i]->length < array[*min_i]->length))
            {
                *min_i = i;
            }
        }
    }
    return OK;
}

int chek_min_max(Hash_table *table, unsigned int *max_i, unsigned int *min_i, unsigned int index)
{
    if (!table || !max_i || !min_i)
    {
        return WRONG_POINTER;
    }
    if ((*max_i == table->hashsize) || ((*max_i != table->hashsize) && (table->array[index]->length > table->array[*max_i]->length)))
    {
        *max_i = index;
    }
    if (index == *min_i)
    {
        if (try_found_new_min(table->array, table->hashsize, min_i))
        {
            return WRONG_POINTER;
        }
        return OK;
    }
    if (*min_i == table->hashsize)
    {
        *min_i = index;
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

int rebild_hash_table(Hash_table **table, unsigned int *max_i, unsigned int *min_i)
{
    int new_hashsize = 0;
    char fl = 0;
    enum err mistake = OK;
    int i = 0, index = 0;
    Hash_table *table_new = NULL;
    Node *temp_node = NULL, *res = NULL;
    printf("lllll\n");
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
        new_hashsize += 2;
        if (is_prime(new_hashsize, &fl))
        {
            return WRONG_POINTER;
        }
    }
    *min_i = new_hashsize;
    *max_i = new_hashsize;
    printf("%d\n", new_hashsize);

    if (mistake = creat_and_intil_hash_table(&table_new, new_hashsize))
    {
        return mistake;
    }
    for (i = 0; i < (*table)->hashsize; ++i)
    {
        if ((*table)->array[i] != NULL)
        {
            temp_node = (*table)->array[i]->head_list;
            while (temp_node != NULL)
            {
                index = temp_node->hash_func_value % new_hashsize;
                if ((*table)->array[index] == NULL)
                {
                    if (!((*table)->array[index] = (Elem *)malloc(sizeof(Elem))))
                    {
                        clear_hach_table(&table_new);
                        return MEMMORY_ERROR;
                    }
                    (*table)->array[index]->head_list = NULL;
                    (*table)->array[index]->length = 0;
                }

                (*table)->array[index]->length += 1;
                if (chek_min_max(table_new, max_i, min_i, index))
                {
                    clear_hach_table(&table_new);
                    return WRONG_POINTER;
                }

                res = (*table)->array[index]->head_list;
                (*table)->array[index]->head_list = temp_node;
                (*table)->array[index]->head_list->next = res;
                temp_node = temp_node->next;
            }
        }
    }
    if (clear_array((*table)->array, (*table)->hashsize))
    {
        return WRONG_POINTER;
    }
    free(*table);
    *table = table_new;
    return OK;
}

int check_need_rebild(unsigned int max_col, unsigned int min_col, char *fl)
{
    if (!fl)
    {
        return WRONG_POINTER;
    }
    *fl = 0;
    printf("max_col %d; min_col %d\n", max_col, min_col);
    if (max_col / min_col >= 2)
    {
        *fl = 1;
        printf("hhhh\n");
    }

    return OK;
}

int add_elem_to_table(Hash_table *table, char *def_name, char *value, unsigned int *min_i, unsigned int *max_i)
{
    unsigned int key = 0, index;
    enum err mistake = OK;
    Node *res = NULL;
    char fl = 0;
    if ((table == NULL) || (def_name == NULL) || (value == NULL) || !max_i || !min_i)
    {
        return WRONG_POINTER;
    }
    if (mistake = hash_func(def_name, &key))
    {
        return mistake;
    }
    // printf("key - %d\n", key);
    index = key % table->hashsize;
    if (mistake = create_node_and_full(def_name, value, key, &res))
    {
        return mistake;
    }
    if (table->array[index] == NULL)
    {
        if (!(table->array[index] = (Elem *)malloc(sizeof(Elem))))
        {
            free(res);
            res = NULL;
            return MEMMORY_ERROR;
        }
        table->array[index]->head_list = NULL;
        table->array[index]->length = 1;
    }
    else
    {
        table->array[index]->length += 1;
    }
    res->next = table->array[index]->head_list;
    table->array[index]->head_list = res;

    if (chek_min_max(table, max_i, min_i, index))
    {
        return WRONG_POINTER;
    }

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
        if (!(new_buf = (char *)malloc(sizeof(char) * (*capacity))))
        {
            return MEMMORY_ERROR;
        }
        *buf = new_buf;
        new_buf = NULL;
    }
    return OK;
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
    *end_c = c;
    (*buf)[*buf_end] = '\0';
    ++(*buf_end);
    // printf("%s\n", *buf);
    return OK;
}

int read_diretive(FILE *fin, FILE *fout, char **buf, int *capacity, char *end_c, Hash_table **table)
{
    enum err mistake = OK;
    char *def_name = NULL;
    char *value = NULL;
    int buf_end = 0;
    char fl = 0;
    unsigned int max_i = (*table)->hashsize, min_i = (*table)->hashsize;
    if ((fin == NULL) || (fout == NULL) || (buf == NULL) || (capacity == NULL) || (table == NULL) || (*table == NULL) || (end_c == NULL))
    {
        return WRONG_POINTER;
    }
    while (!feof(fin))
    {
        if (mistake = read_lecsem(fin, fout, buf, capacity, &buf_end, end_c, split_func_isspace))
        {
            return mistake;
        }
        if (strncmp(*buf, "#define", buf_end) == 0)
        {
            fprintf(fout, "%s%c", *buf, *end_c);
            if (mistake = read_lecsem(fin, fout, buf, capacity, &buf_end, end_c, split_func_isspace))
            {
                return mistake;
            }
            if (!(def_name = (char *)malloc(sizeof(char) * (buf_end))))
            {
                return MEMMORY_ERROR;
            }
            strncpy(def_name, *buf, buf_end);
            fprintf(fout, "%s%c", def_name, *end_c);
            if (mistake = read_lecsem(fin, fout, buf, capacity, &buf_end, end_c, split_func_for_value))
            {
                free(def_name);
                return mistake;
            }
            if (!(value = (char *)malloc(sizeof(char) * (buf_end))))
            {
                free(def_name);
                return MEMMORY_ERROR;
            }
            strncpy(value, *buf, buf_end);
            fprintf(fout, "%s%c", value, *end_c);

            if ((max_i != (*table)->hashsize) && (min_i != (*table)->hashsize))
            {
                if (check_need_rebild((*table)->array[max_i]->length, (*table)->array[min_i]->length, &fl))
                {
                    return WRONG_POINTER;
                }
            }
            if (fl == 1)
            {
                if (mistake = rebild_hash_table(table, &max_i, &min_i))
                {
                    return mistake;
                }
            }
            if (mistake = add_elem_to_table(*table, def_name, value, &min_i, &max_i))
            {
                free(def_name);
                free(value);
                return mistake;
            }
        }
        else
        {
            // printf("lll");
            break;
            // как будто лишняя проверка
            /*if ((!feof(fin)) && (def_name == NULL))
            {

                return NO_DEFINES;
            }
            */
        }
    }
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
    temp_node = table->array[key]->head_list;
    while (temp_node != NULL)
    {
        if (strncmp(str, temp_node->def_name, str_end) == 0)
        {
            *value = temp_node->value;
            printf("%s", *value);
        }
        temp_node = temp_node->next;
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
    char end_c = ' ';
    Hash_table *table = NULL;
    if ((!(fin = fopen(file_in, "r"))) || (!(fout = fopen(file_out, "a"))))
    {
        fclose(fin);
        fclose(fout);
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
        printf("jjj\n");
        return mistake;
    }

    if (mistake = read_diretive(fin, fout, &buf, &capacity, &end_c, &table))
    {
        close_free(fin, fout, buf, &table);
        return mistake;
    }

    print_hash_table(table);

    while (!feof(fin))
    {
        // если буфер пустой значит были только дефайны новой дексемы не найжено
        if (strlen(buf) == 0)
        {
            // printf("hhhh\n");
            return OK;
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
        fprintf(fout, "%s%c", value, end_c);

        if (mistake = read_lecsem(fin, fout, &buf, &capacity, &buf_end, &end_c, split_func_isspace))
        {
            close_free(fin, fout, buf, &table);
            return mistake;
        }
        // printf("%s\n", buf);
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
    fprintf(fout, "%s", buf);
    close_free(fin, fout, buf, &table);
    return OK;
}

int main(int argc, char *argv[])
{
    switch (read_file(argv[1], "temparary.txt"))
    {
    case OK:
        printf("Ураа)\n");
        break;
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
    }
}