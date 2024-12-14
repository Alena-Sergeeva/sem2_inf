#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CNT_EXAMS 5

enum err
{
    OK,
    INVALID_INPUT,
    STOP_REALLOC,
    MEMMORY_ERROR,
    WRONG_POINTER,
    OVERFLOW_INT,
    WRONG_CHOISE,
    INPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_WRITTEN,
    FILE_EMPTY,
    SAME_NAME,
    ARGUMENTS_CNT,
    FOUND,
    NOT_FOUND,
    PRINTED,
    NOT_INTEGER,
    END,
    EXIT
};

/*
Экземпляр структуры типа Student содержит поля: id студента (целое неотрицательное
число), имя (непустая строка только из букв латинского алфавита), фамилия (непустая
строка только из букв латинского алфавита), группа (непустая строка) и оценки за 5
экзаменов (динамический массив элементов типа unsigned char).
*/
typedef struct Student
{
    int id;
    char *name;
    char *surname;
    char *group;
    unsigned char *marks;
    double arifm_mean_mark;
} Student;

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
    // printf("%c %c", file1[i], file2[j]);
    if ((file1[i] == file2[j]))
    {
        return SAME_NAME;
    }
    return OK;
}

int check_int_num(char *str, int *num)
{
    char *end_num = NULL;
    *num = 0;
    while (*str != '\0')
    {
        if (!isdigit(*str))
        {
            return NOT_INTEGER;
        }
        *num = (*num) * 10 + *str - '0';
        if (*num < 0)
        {
            return OVERFLOW_INT;
        }
        ++str;
    }
    return OK;
}

int check_name_or_surname(char *str, int size)
{
    char c;
    int i = 0;
    if ((str == NULL))
    {
        return WRONG_POINTER;
    }
    for (i = 0; i < size - 1; ++i)
    {
        if (!(((str[i] >= 'A') && (str[i] <= 'Z')) || ((str[i] >= 'a') && (str[i] <= 'z'))))
        {
            return INVALID_INPUT;
        }
    }
    return OK;
}

int check_capacity_and_realloc(Student **res, int i, int *capacity)
{
    Student *new_res = NULL;
    if ((res == NULL) || (capacity == NULL))
    {
        return WRONG_POINTER;
    }
    if (i >= *capacity)
    {
        if (*capacity << 1 < 0)
        {
            return OVERFLOW_INT;
        }
        *capacity <<= 1;
        if (!(new_res = (Student *)realloc(*res, sizeof(Student) * (*capacity))))
        {
            return MEMMORY_ERROR;
        }
        *res = new_res;
        new_res = NULL;
    }
    return OK;
}

int read_str_in_buf(FILE *fin, char **buf, int *bufsize, int *buf_end)
{
    char c = ' ', _c = ' ';
    char *new_buf = NULL;
    *buf_end = 0;
    c = fgetc(fin);
    while (!feof(fin))
    {
        if (!isspace(c))
        {
            (*buf)[(*buf_end)] = c;
            ++(*buf_end);
            if ((*buf_end >= *bufsize))
            {
                // длинее чем при чтении файла запроса быть не может, остановить пользователя
                if (fin == stdin)
                {
                    return STOP_REALLOC;
                }
                if (*bufsize << 1 < 0)
                {
                    return OVERFLOW_INT;
                }
                *bufsize <<= 1;
                if (!(new_buf = (char *)realloc(*buf, sizeof(char) * (*bufsize))))
                {
                    return MEMMORY_ERROR;
                }
                *buf = new_buf;
                new_buf = NULL;
            }
        }
        else
        {
            if (!isspace(_c))
            {
                (*buf)[*buf_end] = '\0';
                // указывает колличество символов в строке включая \0
                ++(*buf_end);
                return OK;
            }
        }
        _c = c;
        c = fgetc(fin);
    }
    if (feof(fin))
    {
        if (!isspace(_c))
        {
            (*buf)[*buf_end] = '\0';
            ++(*buf_end);
            return OK;
        }
        else
        {
            return END;
        }
    }
    return OK;
}

int read_file(char *file_name, Student **res, int *count, double *arif_mean_marks_all, int *max_str_capacity)
{
    FILE *fin = NULL;
    int capacity = 5, bufsize = 10;
    int i = 0;
    enum err mistake = 0;

    double sum_student = 0.0, sum_all = 0.0;
    int num = 0;

    int buf_end = 0;
    char *buf = NULL;

    if ((res == NULL) || (count == NULL) || (arif_mean_marks_all == NULL) || (max_str_capacity == NULL))
    {
        return WRONG_POINTER;
    }
    *count = 0;
    *arif_mean_marks_all = 0;
    *max_str_capacity = bufsize;

    if ((fin = fopen(file_name, "r")) == NULL)
    {
        return INPUT_FILE_NOT_OPEN;
    }
    if (!(buf = (char *)malloc(sizeof(char) * bufsize)))
    {
        return MEMMORY_ERROR;
    }
    if (!(*res = (Student *)malloc(sizeof(Student) * capacity)))
    {
        free(buf);
        fclose(fin);
        return MEMMORY_ERROR;
    }

    while (!feof(fin))
    {
        *count = i + 1;
        (*res)[i].name = NULL;
        (*res)[i].surname = NULL;
        (*res)[i].group = NULL;
        (*res)[i].marks = NULL;

        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (strncmp(buf, "ID:", strlen("ID:")) != 0))
        {
            if (mistake == END)
            {
                continue;
            }
            free(buf);
            fclose(fin);

            return mistake;
        }
        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || ((mistake = (check_int_num(buf, &num)))))
        {
            free(buf);
            fclose(fin);
            if (mistake == OK)
            {
                return INVALID_INPUT;
            }
            return mistake;
        }
        if (num < 0)
        {
            free(buf);
            fclose(fin);
            return INVALID_INPUT;
        }
        (*res)[i].id = num;

        // читаем имя
        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (strncmp(buf, "name:", strlen("name:")) != 0))
        {
            free(buf);
            fclose(fin);
            return mistake;
        }
        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (check_name_or_surname(buf, buf_end)))
        {
            free(buf);
            fclose(fin);
            if (mistake == OK)
            {
                return INVALID_INPUT;
            }
            return mistake;
        }

        if (!((*res)[i].name = (char *)malloc(sizeof(char) * buf_end)))
        {
            free(buf);
            fclose(fin);
            return MEMMORY_ERROR;
        }
        strncpy((*res)[i].name, buf, buf_end);

        // текущий плохой предыдущий хороший

        // читаем фамилию

        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (strncmp(buf, "surname:", strlen("surname:")) != 0))
        {
            free(buf);
            fclose(fin);
            return mistake;
        }
        if (mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end) || (mistake = check_name_or_surname(buf, buf_end)))
        {
            free(buf);
            fclose(fin);
            if (mistake == OK)
            {
                return INVALID_INPUT;
            }
            return mistake;
        }

        if (!((*res)[i].surname = (char *)malloc(sizeof(char) * (buf_end))))
        {
            free(buf);
            fclose(fin);
            return MEMMORY_ERROR;
        }
        strncpy((*res)[i].surname, buf, buf_end);

        // чиатем группу
        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (strncmp(buf, "group:", strlen("group:")) != 0))
        {
            free(buf);
            fclose(fin);
            return mistake;
        }
        if (mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end) || (strlen(buf) == 0))
        {
            free(buf);
            fclose(fin);
            if (mistake == OK)
            {
                return INVALID_INPUT;
            }
            return mistake;
        }
        if (!((*res)[i].group = (char *)malloc(sizeof(char) * (buf_end))))
        {
            free(buf);
            fclose(fin);
            return MEMMORY_ERROR;
        }
        strncpy((*res)[i].group, buf, buf_end);

        // читаем оценки
        if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (strncmp(buf, "marks:", strlen("marks:")) != 0))
        {
            free(buf);
            fclose(fin);
            return mistake;
        }
        if (!((*res)[i].marks = (unsigned char *)malloc(sizeof(unsigned char) * (CNT_EXAMS))))
        {
            fclose(fin);
            free(buf);
            return MEMMORY_ERROR;
        }

        for (int j = 0; j < CNT_EXAMS; ++j)
        {
            if ((mistake = read_str_in_buf(fin, &buf, &bufsize, &buf_end)) || (mistake = (check_int_num(buf, &num))))
            {
                free(buf);
                fclose(fin);
                return mistake;
            }
            if ((num < 0) || (num > __UINT8_MAX__))
            {
                fclose(fin);
                free(buf);
                return INVALID_INPUT;
            }
            (*res)[i].marks[j] = (unsigned char)num;
            sum_student += (*res)[i].marks[j];
        }

        (*res)[i].arifm_mean_mark = sum_student / CNT_EXAMS;

        sum_all += sum_student / CNT_EXAMS;

        if ((mistake = check_capacity_and_realloc(res, i + 1, &capacity)) != OK)
        {
            free(buf);
            fclose(fin);
            return mistake;
        }
        ++i;
        sum_student = 0.0;
    }
    fclose(fin);
    free(buf);
    if (i == 0)
    {
        return FILE_EMPTY;
    }
    *max_str_capacity = bufsize;
    *arif_mean_marks_all = sum_all / (i);
    *count = i;
    return OK;
}

int print_res_in_consol(const Student *res, int count)
{
    int i = 0, j = 0;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    for (i = 0; i < count; ++i)
    {
        printf("ID: %d\nname: %s\nsurname: %s\ngroup: %s\nmarks:", res[i].id, res[i].name, res[i].surname, res[i].group);
        for (j = 0; j < CNT_EXAMS; ++j)
        {
            printf("%hhu ", res[i].marks[j]);
        }
        printf("\n\n");
    }
    return OK;
}

int print_res_in_file(const char *file_name_out, const Student *res, int count)
{
    int i = 0, j = 0;
    FILE *fout = NULL;
    if ((file_name_out == NULL))
    {
        return WRONG_POINTER;
    }
    if (res == NULL)
    {
        return NOT_FOUND;
    }
    if (!(fout = fopen(file_name_out, "a")))
    {
        return OUTPUT_FILE_NOT_OPEN;
    }
    for (i = 0; i < count; ++i)
    {
        fprintf(fout, "name: %s\nsurname: %s\ngroup: %s\n%lf\n", res[i].name, res[i].surname, res[i].group, res[i].arifm_mean_mark);
        fprintf(fout, "\n");
    }
    fclose(fout);
    return OK;
}

int valid(
    int cnt, char *argv[],
    char **file_name_input, char **file_name_output)
{
    enum err mistake = 0;
    if ((cnt < 2) || (cnt > 3))
    {
        return ARGUMENTS_CNT;
    }
    if ((file_name_input == NULL) || (file_name_output == NULL) || !argv[1])
    {
        return WRONG_POINTER;
    }
    *file_name_output = NULL;
    *file_name_input = argv[1];
    if (cnt == 3)
    {
        *file_name_output = argv[2];
        if (mistake = check_same_file_name(argv[1], argv[2]))
        {
            return mistake;
        }
    }
    return OK;
}

void printf_options()
{
    printf("a. Вывести в файл данные найденного по id студента: ФИО, группу и среднюю оценку\n"
           "b. Вывести в файл фамилии и имена студентов, чей средний балл за все экзамены\n"
           "выше среднего балла за все экзамены по всем студентам\n"
           "c. Поиск студента по id:\n"
           "d. Поиск студента по фамилии:\n"
           "e. Поиск студента по имени:\n "
           "f. Поиск студента по группе:\n"
           "h. Сортировка масива по id\n"
           "i. Сортировка масива по фамилии\n"
           "j. Сортировка масива по имени\n"
           "k. Сортировка масива по группе\n"
           "z. Печать масива на текущий момент в консоль\nexit\n");
}

int found_id(const Student *st, const void *id)
{
    if (st->id == *((unsigned int *)(id)))
    {
        return FOUND;
    }
    return OK;
}

int found_name(const Student *st, const void *name)
{
    if (strcmp(st->name, ((char *)name)) == 0)
    {
        return FOUND;
    }
    return OK;
}

int found_surname(const Student *st, const void *surname)
{
    if (strcmp(st->surname, ((char *)surname)) == 0)
    {
        return FOUND;
    }
    return OK;
}

int found_group(const Student *st, const void *group)
{
    if ((strcmp(st->group, (char *)(group))) == 0)
    {
        return FOUND;
    }
    return OK;
}

int cmp_id(const void *stud_a, const void *stud_b)
{
    return ((Student *)stud_a)->id - ((Student *)stud_b)->id;
}

int cmp_name(const void *stud_a, const void *stud_b)
{
    return strcmp(((Student *)stud_a)->name, ((Student *)stud_b)->name);
}

int cmp_surname(const void *stud_a, const void *stud_b)
{
    return strcmp(((Student *)stud_a)->surname, ((Student *)stud_b)->surname);
}

int cmp_group(const void *stud_a, const void *stud_b)
{
    return strcmp(((Student *)stud_a)->group, ((Student *)stud_b)->group);
}

int find(Student const *res, int count, const void *id, Student **stud_ptr, int (*func)(const Student *, const void *))
{
    int i;
    if ((res == NULL) || (stud_ptr == NULL) || (id == NULL))
    {
        return WRONG_POINTER;
    }
    *stud_ptr = NULL;

    for (i = 0; i < count; ++i)
    {
        if (func((res + i), id) == FOUND)
        {
            *stud_ptr = (Student *)res + i;
            return FOUND;
        }
    }
    return NOT_FOUND;
}

int read_str_and_find(char **buf, int max_str_capacity, Student *res, int count, int (*func)(const Student *, const void *))
{
    int buf_end = 0;
    enum err mistake = 0;
    Student *student_ptr = NULL;
    if ((buf == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }
    printf("Введите строку: ");
    if (mistake = read_str_in_buf(stdin, buf, &max_str_capacity, &buf_end))
    {
        return mistake;
    }
    if (func != found_group)
    {
        if (mistake = check_name_or_surname(*buf, buf_end))
        {
            return mistake;
        }
    }
    if ((mistake = find(res, count, *buf, &student_ptr, func)) != FOUND)
    {
        return mistake;
    }
    while (mistake == FOUND)
    {
        print_res_in_consol(student_ptr, 1);
        mistake = find(student_ptr + 1, count - (student_ptr - res + 1), *buf, &student_ptr, func);
    }
    return 0;
}

int find_students(char *file_out, const Student *res, int count, double arif_mean_mark_all)
{
    FILE *fout = NULL;
    int i = 0;
    if ((res == NULL) || (file_out == NULL))
    {
        return WRONG_POINTER;
    }
    if (!(fout = fopen(file_out, "a")))
    {
        return OUTPUT_FILE_NOT_OPEN;
    }
    for (i = 0; i < count; ++i)
    {
        if (res[i].arifm_mean_mark > arif_mean_mark_all)
        {
            fprintf(fout, "%s %s\n", res[i].surname, res[i].name);
        }
    }

    if (i == 0)
    {
        return NOT_FOUND;
    }
    fprintf(fout, "\n");
    fclose(fout);
    return OK;
}

int clear_res(Student **res, int *count)
{
    int i;
    if ((res == NULL) || (count == NULL))
    {
        return WRONG_POINTER;
    }

    for (i = *count - 1; i >= 0; --i)
    {
        free((*res)[i].name);
        free((*res)[i].surname);
        free((*res)[i].group);
        free((*res)[i].marks);
    }
    *count = 0;
    free(*res);
    *res = NULL;
    return OK;
}

int try_speak(Student **res, int count, char **buf, int max_str_capacity, char *file_name_out, double arif_mean_all)
{
    int buf_end = 0;
    int num = 0;
    Student *student_ptr = NULL;
    enum err mistake = 0;
    if ((buf == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }

    printf_options();

    if ((mistake = read_str_in_buf(stdin, buf, &max_str_capacity, &buf_end)))
    {

        return mistake;
    }
    if (strcmp(*buf, "exit") == 0)
    {
        return EXIT;
    }

    if (!((buf_end == 2) && (strchr("abcdefghijkz", (*buf)[0]) != NULL)))
    {
        return WRONG_CHOISE;
    }

    switch ((*buf)[0])
    {
    case 'a':
        printf("Введите целое неотрицательное число: ");
        if ((mistake = read_str_in_buf(stdin, buf, &max_str_capacity, &buf_end)) || (mistake = check_int_num(*buf, &num)))
        {
            return mistake;
        }
        if (num < 0)
        {
            return INVALID_INPUT;
        }
        if ((mistake = find(*res, count, &num, &student_ptr, found_id)) != FOUND)
        {
            return mistake;
        }
        if ((mistake = print_res_in_file(file_name_out, student_ptr, 1)) == 0)
        {
            mistake = PRINTED;
        }
        return mistake;
    //"b. Вывести в файл фамилии и имена студентов, чей средний балл за все экзамены\n"
    //"выше среднего балла за все экзамены по всем студентам\n"
    case 'b':
        if ((mistake = find_students(file_name_out, *res, count, arif_mean_all)) == 0)
        {
            mistake = PRINTED;
        }
        return mistake;

    case 'c':
        printf("Введите целое неотрицательное число: ");
        if ((mistake = read_str_in_buf(stdin, buf, &max_str_capacity, &buf_end)) || (mistake = check_int_num(*buf, &num)))
        {
            return mistake;
        }
        if (num < 0)
        {
            return INVALID_INPUT;
        }
        if ((mistake = find(*res, count, &num, &student_ptr, found_id)) != FOUND)
        {
            return mistake;
        }
        while (mistake == FOUND)
        {
            print_res_in_consol(student_ptr, 1);
            mistake = find(student_ptr + 1, count - (student_ptr - *res + 1), &num, &student_ptr, found_id);
        }
        break;
    case 'e':
        return read_str_and_find(buf, max_str_capacity, *res, count, found_name);
        break;

    //"d. Поиск студента по фамилии: "
    case 'd':
        return read_str_and_find(buf, max_str_capacity, *res, count, found_surname);
        break;
    // f. Поиск студента по группе
    case 'f':
        return read_str_and_find(buf, max_str_capacity, *res, count, found_group);
        break;
        // h. Сортировка масиива по id

    case 'h':
        qsort(*res, count, sizeof(Student), cmp_id);
        break;

    //"i. Сортировка масиива по фамилии\n"
    case 'i':
        qsort(*res, count, sizeof(Student), cmp_surname);
        break;

    //"j. Сортировка масиива по имени\n"
    case 'j':
        qsort(*res, count, sizeof(Student), cmp_name);
        break;
    // k. Сортировка масиива по группе\n
    case 'k':
        qsort(*res, count, sizeof(Student), cmp_group);
        break;
    // z. Печать масиива на текущий момент в консоль\n
    case 'z':
        print_res_in_consol(*res, count);
        break;
    }
    return OK;
}

/*
Добавьте возможность вывода в трассировочный файл (путь к файлу передаётся как
аргумент командной строки) данные найденного по id студента: ФИО, группу и
среднюю оценку за экзамены. Также добавьте возможность вывести в трассировочный
файл фамилии и имена студентов, чей средний балл за все экзамены выше среднего
балла за все экзамены по всем считанным из файла студентам. Все вышеописанные
опции должны быть выполнимы из контекста интерактивного диалога с пользователем.
*/
int main(int argc, char *argv[])
{
    char *file_name_in = NULL, *file_name_out = NULL;
    Student *res = NULL;
    int count = 0, id = 0;
    enum err mistake = 0;
    double arif_mean_mark_all = 0.0;
    char fl = ' ', c = ' ';
    Student *student_ptr = NULL;

    switch (mistake = valid(argc, argv, &file_name_in, &file_name_out))
    {
    case ARGUMENTS_CNT:
        printf("неверное количество аргументов\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    case SAME_NAME:
        printf("имена файлов одинаковые\n");
        break;
    }
    if (mistake != OK)
    {
        return 0;
    }
    int max_str_capacity = 0;

    switch (mistake = read_file(file_name_in, &res, &count, &arif_mean_mark_all, &max_str_capacity))
    {
    case OK:
        printf("Файл прочитан\n");
        break;
    case INVALID_INPUT:
        printf("Неверные данные\n");
        break;
    case END:
        printf("Структура недописана\n");
        break;
    case WRONG_POINTER:
        printf("Передан нулевой указатель\n");
        break;
    case INPUT_FILE_NOT_OPEN:
        printf("Не удалось открыть входной файл\n");
        break;
    case MEMMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    case FILE_EMPTY:
        printf("Файл не содержит данных\n");
        break;
    }

    if (mistake != OK)
    {
        if (clear_res(&res, &count))
        {
            printf("передан нулевой указатель\n");
        }
        else
        {
            printf("\nпамять очищена\n");
        }
        return 0;
    }

    char *buf = NULL;
    if (!(buf = (char *)malloc(max_str_capacity)))
    {
        printf("Не удалось выделить память\n");
    }
    // printf("\n%lflll\n", arif_mean_mark_all);
    while ((mistake = try_speak(&res, count, &buf, max_str_capacity, file_name_out, arif_mean_mark_all)) != END)
    {
        if (mistake == EXIT)
        {
            printf("До свидания!\n");
            break;
        }
        switch (mistake)
        {
        case FOUND:
            print_res_in_consol(student_ptr, 1);
            break;
        case NOT_INTEGER:
            printf("это не целое число\nпопробуйте еще раз начиная с вввода опции\n");
            break;
        case OVERFLOW_INT:
            printf("Переполнение типа int\n");
            break;
        case PRINTED:
            printf("Результат записан в файл\n");
            break;
        case WRONG_POINTER:
            printf("передан нулевой указатель\n");
            break;
        case WRONG_CHOISE:
            printf("У вас нет такого выбора\n посмотрите список опций еще раз\n");
            break;
        case OUTPUT_FILE_NOT_OPEN:
            printf("Выходной файл не открылся\n");
            break;
        case INVALID_INPUT:
            printf("Некоректный ввод\n");
            break;
        case NOT_FOUND:
            printf("не найдено\n");
            break;
        }
    }
    // написать очистку массива, поля с оценками
    free(buf);
    if (clear_res(&res, &count))
    {
        printf("передан нулевой указатель");
    }
    else
    {
        printf("память очищена\n");
    }
    return 0;
}
