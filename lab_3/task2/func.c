/*На вход программе через аргументы командной строки подается путь ко входному
файлу, флаг (флаг начинается с символа ‘-’ или ‘/’, второй символ - ‘a’ или ‘d’) и путь к
выходному файлу. В файле в каждой строчке содержится информация о сотруднике
(для этой информации определите тип структуры Employee): id (целое неотрицательное
число), имя (непустая строка только из букв латинского алфавита), фамилия (непустая
строка только из букв латинского алфавита), заработная плата (неотрицательное
вещественное число). Программа должна считать записи из файла в динамический
массив структур и в выходной файл вывести данные, отсортированные (с флагом
‘-a’/‘/a’ - по возрастанию, с флагом ‘-d’/‘/d’ - по убыванию)  Для сортировки коллекции экземпляров структур
используйте стандартную функцию qsort, своя реализация каких-либо алгоритмов
сортировки не допускается.*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE 60

enum err
{
    OK,
    SAME_PATH,
    FILE_EMPTY,
    WRONG_EMPLOYER,
    WRONG_NAME,
    WRONG_SALARY,
    WRONG_ID,
    WRONG_FLAG,
    MEMMORY_ERROR,
    ARGUMETS_CNT,
    INPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_OPEN,
    WRONG_POINTER,
    TOO_LONG_NAME,
    WRONG_SURNAME,
    OVERFLOW_INT
};

typedef struct Employe
{
    int id;
    char name[BUFSIZE];
    char surname[BUFSIZE];
    int salary;
} Employe;

int check_capacity_and_realloc(Employe **res, int i, int *capacity)
{
    Employe *new_res = NULL;
    if ((res == NULL) || (capacity == NULL))
    {
        return WRONG_POINTER;
    }
    if (i >= *capacity)
    {
        if (*capacity << 1 > __INT_MAX__)
        {
            return OVERFLOW_INT;
        }
        *capacity <<= 1;
        if (!(new_res = (Employe *)realloc(*res, sizeof(Employe) * (*capacity))))
        {
            return MEMMORY_ERROR;
        }
        *res = new_res;
        new_res = NULL;
    }
    return OK;
}
/*
int check_unsigned_int(int num, unsigned int *x)
{
    unsigned int x_new = 0;
    char c = '0';
    if ((fin == NULL) || (x == NULL))
    {
        return WRONG_POINTER;
    }
    while (isdigit(c = fgetc(*fin)))
    {
        x_new = x_new * 10 + c - '0';
    }
    if ((c == '\n') || (feof(*fin)))
    {
        *x = x_new;
        return OK;
    }
    else
    {
        return WRONG_ID;
    }
}
*/
int check_name_or_surname(char *str)
{
    char c;
    int i = 0;
    if ((str == NULL))
    {
        return WRONG_POINTER;
    }
    while (*str != '\0')
    {
        if (!(((*str >= 'A') && (*str <= 'Z')) || ((*str >= 'a') && (*str <= 'z'))))
        {
            return WRONG_NAME;
        }
        ++str;
    }
    return OK;
}

int read_file(char *file_name, Employe **res, int *count)
{
    FILE *fin = NULL;
    int capacity = 5, i = 0;
    enum err mistake = 0;
    int cnt_arg = 0;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    fin = fopen(file_name, "r");
    if (fin == NULL)
    {
        return INPUT_FILE_NOT_OPEN;
    }
    if (!(*res = (Employe *)malloc(sizeof(Employe) * capacity)))
    {
        fclose(fin);
        return MEMMORY_ERROR;
    }
    while ((cnt_arg = fscanf(fin, "%d\n%59s\n%59s\n%d\n", &((*res)[i].id), (*res)[i].name, (*res)[i].surname, &((*res)[i].salary))) == 4)
    {
        if ((*res)[i].id < 0)
        {
            fclose(fin);
            return WRONG_ID;
        }
        // printf("%u, %s, %s, %u %d %d\n", ((*res)[i].id), (*res)[i].name, (*res)[i].surname, ((*res)[i].salary), i, cnt_arg);
        //  printf("%s", (*res)[i].name);
        if (mistake = check_name_or_surname((*res)[i].name))
        {
            fclose(fin);
            return mistake;
        }
        if (mistake = check_name_or_surname((*res)[i].surname))
        {
            fclose(fin);
            if (mistake == WRONG_NAME)
            {
                return WRONG_SURNAME;
            }
            return mistake;
        }
        if ((*res)[i].salary < 0)
        {
            fclose(fin);
            return WRONG_SALARY;
        }
        if (mistake = check_capacity_and_realloc(res, i + 1, &capacity))
        {
            fclose(fin);
            return mistake;
        }
        ++i;
    }
    fclose(fin);
    if (cnt_arg > -1)
    {
        return WRONG_EMPLOYER;
    }
    if (i == 0)
    {
        return FILE_EMPTY;
    }
    *count = i;

    return OK;
}

int print_res_in_file(const char *file_name_out, const Employe *res, int count)
{
    int i;
    FILE *fout = NULL;
    if ((file_name_out == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }
    if (!(fout = fopen(file_name_out, "w")))
    {
        return OUTPUT_FILE_NOT_OPEN;
    }
    for (i = 0; i < count; ++i)
    {
        fprintf(fout, "ID: %u\nname: %s\nsurname: %s\nsalary: %u\n", res[i].id, res[i].name, res[i].surname, res[i].salary);
    }
    fclose(fout);
    return OK;
}

/*первично - по зарплате,
далее (если зарплаты равны) - по фамилии,
далее (если зарплаты и фамилии равны) -
по именам, наконец, по id.*/
// по возрастанию
int cmp_up(const void *a, const void *b)
{
    int res = 0;
    if (((Employe *)a)->salary != ((Employe *)b)->salary)
    {
        return ((Employe *)a)->salary - ((Employe *)b)->salary;
    }
    else
    {
        if ((res = strcmp(((Employe *)a)->surname, ((Employe *)b)->surname)) != 0)
        {
            return res;
        }
        else
        {
            if ((res = strcmp(((Employe *)a)->name, ((Employe *)b)->name)) != 0)
            {
                printf("%d\n", res);
                return res;
            }
            else
            {
                return ((Employe *)a)->id - ((Employe *)b)->id;
            }
        }
    }
}

int cmp_down(const void *b, const void *a)
{
    int res = 0;
    if (((Employe *)a)->salary != ((Employe *)b)->salary)
    {
        return ((Employe *)a)->salary - ((Employe *)b)->salary;
    }
    else
    {
        if ((res = strcmp(((Employe *)a)->surname, ((Employe *)b)->surname)) != 0)
        {
            return res;
        }
        else
        {
            if ((res = strcmp(((Employe *)a)->name, ((Employe *)b)->name)) != 0)
            {
                // printf("%d\n", res);
                return res;
            }
            else
            {
                return ((Employe *)a)->id - ((Employe *)b)->id;
            }
        }
    }
}

int check_same_file_name(char *file1, char *file2)
{
    int i = 0, j = 0;
    if ((file1 == NULL) || (file2 == NULL))
    {
        return WRONG_POINTER;
    }
    i = strlen(file1) - 1;
    j = strlen(file2) - 1;
    while ((i > 0) && (j > 0) && (file1[i] != '/') && ('/' != file2[j]))
    {
        if (file1[i] != file2[j])
        {
            return OK;
        }
        --i;
        --j;
    }
    if ((file1[i] == '/'))
    {
        ++i;
    }
    if ((file2[j] == '/'))
    {
        ++j;
    }
    printf("%c %c", file1[i], file2[j]);
    if ((file1[i] == file2[j]))
    {
        return SAME_PATH;
    }
    return OK;
}

// тсортированные (с флагом ‘-a’/‘/a’ - по возрастанию, с флагом ‘-d’/‘/d’ - по убыванию)
int valid(
    int cnt, char *argv[],
    char **file_name_input, char **file_name_output, char *fl)
{
    enum err mistake = 0;
    if (cnt != 4)
    {
        return ARGUMETS_CNT;
    }
    if (!((argv[2][0] == '-' || argv[2][0] == '/') && strstr("ad", argv[2] + 1) && (*(argv[2] + 2)) == '\0'))
    {
        return WRONG_FLAG;
    }
    *fl = argv[2][1];
    if (mistake = check_same_file_name(argv[1], argv[3]))
    {
        return mistake;
    }
    *file_name_input = argv[1];
    *file_name_output = argv[3];
    return OK;
}
int main(int argc, char *argv[])
{
    char *file_name_in = NULL, *file_name_out = NULL;
    Employe *res = NULL;
    int count = 0;
    enum err mistake = 0;
    char fl = ' ';
    switch (mistake = valid(argc, argv, &file_name_in, &file_name_out, &fl))
    {
    case ARGUMETS_CNT:
        printf("неверное количество аргументов\n");
        break;
    case WRONG_FLAG:
        printf("неверный флаг\n");
        break;
    case SAME_PATH:
        printf("имена файлов одинаковые\n");
        break;
    }
    if (mistake != 0)
    {
        return OK;
    }
    switch (read_file(file_name_in, &res, &count))
    {
    case OK:
        if (fl == 'a')
        {
            qsort(res, count, sizeof(Employe), cmp_up);
        }
        else
        {
            qsort(res, count, sizeof(Employe), cmp_down);
        }
        switch (print_res_in_file(file_name_out, res, count))
        {
        case OUTPUT_FILE_NOT_OPEN:
            printf("не удалось открыть выходной файл\n");
            break;
        case WRONG_POINTER:
            printf("передан нулевой указатель\n");
            break;
        }
        break;
    case WRONG_EMPLOYER:
        printf("данные работника записаны некоректно\n");
        break;
    case WRONG_NAME:
        printf("имя должно содержать только латинские буквы");
        break;
    case WRONG_SALARY:
        printf("Зарплата должна быть целым неотрицательным числом, либо произошло переполнение типа int\n");
        break;
    case WRONG_ID:
        printf("ID должно быть целым неотрицательным числом, либо произошло переполнение типа int\n");
        break;
    case FILE_EMPTY:
        printf("Файл не содержит данных\n");
        break;
    case MEMMORY_ERROR:
        printf("не удалось выделить память\n");
        break;
    case INPUT_FILE_NOT_OPEN:
        printf("не удалось открыть входной файл\n");
        break;
    case WRONG_POINTER:
        printf("передан нулевой указатель\n");
        break;
    case WRONG_SURNAME:
        printf("фамилия должна содержать только латинские буквы\n");
        break;
    case OVERFLOW_INT:
        printf("Переполнение типа int\n");
        break;
    }
    free(res);
    res == NULL;
    return 0;
}