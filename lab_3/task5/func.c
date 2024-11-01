#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CNT_EXAMS 5

enum err
{
    OK,
    INVALID_INPUT,
    MEMMORY_ERROR,
    WRONG_POINTER,
    OVERFLOW_INT,
    INPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_WRITTEN,
    FILE_EMPTY,
    SAME_NAME,
    ARGUMENTS_CNT,
    FOUND
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
    char name[60];
    char surname[60];
    char group[30];
    unsigned char *marks;
    double arifm_mean_mark;
} Student;

/* Через аргументы
командной строки программе на вход подаётся путь к файлу, содержащему записи о
студентах. При старте программа считывает поданный файл в динамический массив
структур типа Student. В программе должен быть реализован поиск всех студентов по:
● id;
● фамилии;
● имени;
● группе,
сортировка (для сортировки необходимо передавать компаратор для объектов структур)
студента(-ов) по:
● id;
● фамилии;
● имени;
● группе.
Добавьте возможность вывода в трассировочный файл (путь к файлу передаётся как
аргумент командной строки) данные найденного по id студента: ФИО, группу и
среднюю оценку за экзамены. Также добавьте возможность вывести в трассировочный
файл фамилии и имена студентов, чей средний балл за все экзамены выше среднего
балла за все экзамены по всем считанным из файла студентам. Все вышеописанные
опции должны быть выполнимы из контекста интерактивного диалога с пользователем.
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
            return INVALID_INPUT;
        }
        ++str;
    }
    return OK;
}

int clear_student(Student **res, int count)
{
    int i = 0;
    if (res == NULL)
    {
        return WRONG_POINTER;
    }
    for (i = 0; i < count; ++i)
    {
        free((*res)[i].marks);
        (*res)[i].marks = NULL;
    }
    free(*res);
    *res = NULL;
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

int read_file(char *file_name, Student **res, int *count, double *arif_mean_marks_all)
{
    FILE *fin = NULL;
    int capacity = 5, i = 0;
    enum err mistake = 0;
    int cnt_arg = 0;
    double sum_student = 0.0, sum_all = 0.0;
    if ((res == NULL) || (count == NULL) || (arif_mean_marks_all == NULL))
    {
        return WRONG_POINTER;
    }
    *count = 0;
    *arif_mean_marks_all = 0;
    if ((fin = fopen(file_name, "r")) == NULL)
    {
        return INPUT_FILE_NOT_OPEN;
    }
    if (!(*res = (Student *)malloc(sizeof(Student) * capacity)))
    {
        fclose(fin);
        return MEMMORY_ERROR;
    }
    while ((cnt_arg = fscanf(fin, "%d\n%59s\n%59s\n%29s\n", &((*res)[i].id), (*res)[i].name, (*res)[i].surname, (*res)[i].group)) == 4)
    {
        if ((*res)[i].id < 0)
        {

            fclose(fin);
            return INVALID_INPUT;
        }
        // printf("%u, %s, %s, %s\n", ((*res)[i].id), (*res)[i].name, (*res)[i].surname, ((*res)[i].group));
        //   printf("%s", (*res)[i].name);
        if (mistake = check_name_or_surname((*res)[i].name))
        {
            // printf("%s\n", (*res)[i].name);
            fclose(fin);
            return mistake;
        }
        if (mistake = check_name_or_surname((*res)[i].surname))
        {
            fclose(fin);
            return mistake;
        }
        if (isspace(*((*res)[i].group)))
        {
            fclose(fin);
            return INVALID_INPUT;
        }
        if (!((*res)[i].marks = (unsigned char *)malloc(sizeof(unsigned char) * (5 + 1))))
        {
            fclose(fin);
            return MEMMORY_ERROR;
        }
        for (int j = 0; j < CNT_EXAMS; ++j)
        {
            fscanf(fin, "%hhu", ((*res)[i].marks + j));
            sum_student += (*res)[i].marks[j];
            printf("%d\n", __UINT8_MAX__);
            printf("%d\n", (1 << ((sizeof(unsigned char))) - 1));
        }
        (*res)[i].arifm_mean_mark = sum_student / CNT_EXAMS;
        sum_all += sum_student;
        if (mistake = check_capacity_and_realloc(res, i + 1, &capacity))
        {
            fclose(fin);
            return mistake;
        }
        ++i;
    }
    fclose(fin);
    *arif_mean_marks_all = sum_all / i;
    if (cnt_arg > -1)
    {
        return INVALID_INPUT;
    }
    if (i == 0)
    {
        return FILE_EMPTY;
    }
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
        printf("ID: %u\nname: %s\nsurname: %s\ngroup: %s\nmarks:", res[i].id, res[i].name, res[i].surname, res[i].group);
        for (j = 0; j < CNT_EXAMS; ++j)
        {
            printf("%hhu ", res[i].marks[j]);
        }
        printf("\n");
    }
    return OK;
}
// попроьуй печать через stdin, модно ли его закрывать?
int print_res_in_file(const char *file_name_out, const Student *res, int count)
{
    int i = 0, j = 0;
    FILE *fout = NULL;
    if ((file_name_out == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }
    if (!(fout = fopen(file_name_out, "a")))
    {
        return OUTPUT_FILE_NOT_OPEN;
    }
    for (i = 0; i < count; ++i)
    {
        fprintf(fout, "ID: %u\nname: %s\nsurname: %s\ngroup: %s\nmarks:", res[i].id, res[i].name, res[i].surname, res[i].group);
        for (j = 0; j < CNT_EXAMS; ++j)
        {
            fprintf(fout, " %hhu", res[i].marks[j]);
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
    return OK;
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
    // printf("%c %c", file1[i], file2[j]);
    if ((file1[i] == file2[j]))
    {
        return SAME_NAME;
    }
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
        printf("lll");
        return WRONG_POINTER;
    }
    *file_name_output = NULL;
    *file_name_input = argv[1];
    if (cnt == 3)
    {
        *file_name_output = argv[2];
        if (mistake = check_same_file_name(argv[1], argv[3]))
        {
            return mistake;
        }
    }
    return OK;
}

void printf_options()
{
    printf("a. Вывести в файл данные найденного по id студента: ФИО, группу и среднюю оценку\n");
    printf("b. Вывести в файл фамилии и имена студентов, чей средний балл за все экзамены\nвыше среднего балла за все экзамены по всем студентам\n");
    printf("c. Поиск студента по id: ");
    printf("d. Поиск студента по фамилии: ");
    printf("e. Поиск студента по имени: ");
    printf("f. Поиск студента по группе: ");
    printf("h. Сортировка масиива по id\n");
    printf("i. Сортировка масиива по фамилии\n");
    printf("j. Сортировка масиива по имени\n");
    printf("k. Сортировка масиива по группе\n");
    printf("z. Печать масиива на текущий момент в консоль\n");
}

int found_name(Student *st, void *name)
{
    if (strcmp(st->name, ((char *)name)) == 0)
    {
        return FOUND;
    }
    return OK;
}

int found_id(Student *st, void *id)
{
    if (st->id == *((unsigned int *)(id)))
    {
        return FOUND;
    }
    return OK;
}

int find(Student **res, int count, void *id, Student **stud_ptr, int (*func)(Student *, void *))
{
    if ((res == NULL) || (*res == NULL) || (stud_ptr == NULL))
    {
        printf("yyy");
        return WRONG_POINTER;
    }
    *stud_ptr = NULL;
    int i;
    for (i = 0; i < count; ++i)
    {
        if (func((*res + i), id) == FOUND)
        {
            *stud_ptr = *res + i;
            return OK;
        }
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
    char fl = ' ';
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
    printf_options();
    switch (mistake = read_file(file_name_in, &res, &count, &arif_mean_mark_all))
    {
    case INVALID_INPUT:
        printf("Неверные данные\n");
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
        return 0;
    }
    while (scanf("%c\n", &fl) == 1)
    {
        if (strchr("abcdefghijkz", fl) == NULL)
        {
            printf("Вы ввели бред\n");
        }
        else
        {
            switch (fl)
            {
            // Вывести в файл данные найденного по id студента: ФИО, группу и среднюю оценку
            case 'a':
                // check 1234.123 122akak?
                if (scanf("%d", &id) != 1)
                {
                    printf("пупупу, я хочу неотрицательное число");
                }
                find(&res, count, &id, &student_ptr, found_id);
                if (student_ptr == NULL)
                {
                    printf("не найдено\n");
                }
                else
                {
                    print_res_in_consol(student_ptr, 1);
                }
                break;
                /*case 'b':

                                case c:
                                case d:
                                case e:
                                case f:
                                case g:
                                case h:
                                case i:
                                case j:
                                case k:
                                case z:*/
            }
            id = 0;
            student_ptr = NULL;
        }
    }
}