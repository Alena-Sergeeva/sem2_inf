#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
тип структуры String, содержащую в себе поля для указателя на
динамический массив символов типа char и количества символов (длины строки) типа
int
*/
enum err
{
    OK,
    WRONG_POINTER,
    MEMMORY_ERROR,
    INT_OVERFLOW
};

typedef struct String
{
    char *array;
    int length;
} String;

int check_overflow_int(unsigned long s)
{
    if (s > __INT_MAX__)
    {
        return INT_OVERFLOW;
    }
    return OK;
}

// ● создания экземпляра типа String на основе значения типа char *
//(Я так решила) Создает экземляр типа String в динамической памяти
// считаем что str это строка, завершающаяся '\0', иначе не поянтно где ее конец
int create_string(char *str, String **new_string)
{
    if (new_string == NULL)
    {
        return WRONG_POINTER;
    }
    if (!(*new_string = (String *)malloc(sizeof(String))))
    {
        return MEMMORY_ERROR;
    }
    if (str == NULL)
    {
        (*new_string)->array = NULL;
        (*new_string)->length = 0;
    }
    else
    {
        // unsigned long strlen(str)
        if (check_overflow_int(strlen(str)))
        {
            return INT_OVERFLOW;
        }
        (*new_string)->length = strlen(str);
        if (!((*new_string)->array = (char *)malloc(sizeof(char) * ((*new_string)->length + 1))))
        {
            free(new_string);
            new_string = NULL;
            return MEMMORY_ERROR;
        }
        strcpy((*new_string)->array, str);
    }
    return OK;
}

// запись в статический String
int create_string_static(char *str, String *new_string)
{
    if (new_string == NULL)
    {
        return WRONG_POINTER;
    }
    if (str == NULL)
    {
        new_string->array = NULL;
        new_string->length = 0;
    }
    else
    {
        if (check_overflow_int(strlen(str)))
        {
            return INT_OVERFLOW;
        }
        new_string->length = strlen(str);
        if (!(new_string->array = (char *)malloc(sizeof(char) * ((new_string->length + 1)))))
        {
            return MEMMORY_ERROR;
        }
        strcpy(new_string->array, str);
    }
    return OK;
}

// ● удаления внутреннего содержимого экземпляра типа String
int delete_inside_str(String *str)
{
    if (str == NULL)
    {
        return WRONG_POINTER;
    }
    free(str->array);
    str->array = NULL;
    str->length = 0;
    return OK;
}
/*
● отношения эквивалентности между двумя экземплярами типа String
(лексографический компаратор)
работает как обычный компаратор, в случае равенства вернет 0
*/
int str_cmp(String *str1, String *str2, int *fl)
{
    if ((fl == NULL) || (str1 == NULL) || (str2 == NULL) || (str1->array == NULL) || (str2->array == NULL))
    {
        return WRONG_POINTER;
    }
    *fl = strcmp(str1->array, str2->array);
    return OK;
}

/*
● отношения порядка между двумя экземплярами типа String (первично по длине
строки, вторично по лексографическому компаратору)
*/
int str_cmp_length_lecs(String *str1, String *str2, int *res)
{
    if ((str1 == NULL) || (str2 == NULL) || (res == NULL))
    {
        return WRONG_POINTER;
    }
    if (str1->length == str2->length)
    {
        if (str_cmp(str1, str2, res))
        {
            return WRONG_POINTER;
        }
    }
    else
    {
        *res = str1->length - str2->length;
    }
    return OK;
}

/*
● копирования содержимого экземпляра типа String в существующий экземпляр
типа String
*/

int string_cpy(String *ptrstr, String *deststr)
{
    if ((deststr == NULL) || (ptrstr == NULL))
    {
        return WRONG_POINTER;
    }
    free(deststr->array);
    if (!(deststr->array = (char *)malloc(sizeof(char) * (ptrstr->length + 1))))
    {
        return MEMMORY_ERROR;
    }
    strcpy(deststr->array, ptrstr->array);
    deststr->length = ptrstr->length;
    return OK;
}
/*
● копирования содержимого экземпляра типа String в новый экземпляр типа String,
размещённый в динамической памяти
*/
int create_din_string_cpy(String *ptrstr, String **str_new)
{
    if ((ptrstr == NULL) || (str_new == NULL))
    {
        return WRONG_POINTER;
    }
    if (!(*str_new = (String *)malloc(sizeof(String))))
    {
        return MEMMORY_ERROR;
    }
    (*str_new)->length = ptrstr->length;
    if (ptrstr->length == 0)
    {
        (*str_new)->array = NULL;
    }
    // Если значение size равно 0,
    // функция malloc выделяет элемент нулевой длины в куче и возвращает допустимый указатель на этот элемент.
    if (!((*str_new)->array = (char *)malloc(sizeof(char) * (ptrstr->length + 1))))
    {
        free(*str_new);
        *str_new = NULL;
        return MEMMORY_ERROR;
    }
    // Блоки памяти точно не пересекаются, т. к. подаютя структуры String, а не произвольные массивы типа char
    strcpy((*str_new)->array, ptrstr->array);
    return OK;
}
/*
● конкатенации к содержимому первого экземпляра типа String содержимого
    второго экземпляра типа String.
*/
int cat_string(String *deststr, String *str)
{
    char *buf = NULL;
    if ((str == NULL) || (deststr == NULL))
    {
        return WRONG_POINTER;
    }

    // Over_flow check
    if (check_overflow_int(deststr->length + str->length + 1))
    {
        return INT_OVERFLOW;
    }
    if (!(buf = (char *)malloc(sizeof(char) * (deststr->length + str->length + 1))))
    {
        return MEMMORY_ERROR;
    }
    memcpy(buf, deststr->array, sizeof(char) * deststr->length);
    memcpy((buf + deststr->length), str->array, sizeof(char) * (str->length + 1));
    free(deststr->array);
    deststr->array = buf;
    deststr->length = deststr->length + str->length;
    buf = NULL;
    return OK;
}

void clear(String **str)
{
    if (str == NULL)
    {
        return;
    }
    free((*str)->array);
    free(*str);
    str = NULL;
    return;
}

void print_mistake(enum err mistake)
{
    switch (mistake)
    {
    case OK:
        printf("OK\n");
        break;
    case WRONG_POINTER:
        printf("WRONG_POINTER\n");
        break;
    case MEMMORY_ERROR:
        printf("MEMMORY_ERROR\n");
        break;
    }
    return;
}

void print_S(String *str)
{
    printf("string - %s, length - %d\n", str->array, str->length);
}

int main()
{
    char *str = "ell";
    char *str0 = "End)";
    char *test = "yup";
    String *str1 = NULL;
    String *str3 = NULL;
    int res = 0;
    enum err mistake = 0;
    /*
    непонятно как чистить такого вида строку (чистить через delete_inside),
    а как отличать от динамически хранящийся строк (((
    лучше создавать строки через creat_string динамически
    String str2 = {"asdfiii", 7};
    */
    String str2;
    mistake = create_string_static(test, &str2);

    printf("создания экземпляра типа String на основе значения типа char (статический)*\n");
    print_mistake(mistake);
    if (mistake == OK)
    {
        print_S(&str2);
    }

    mistake = create_string(str, &str1);
    printf("создания экземпляра типа String на основе значения типа char *\n");
    print_mistake(mistake);
    if (mistake == OK)
    {
        print_S(str1);
    }

    mistake = str_cmp(&str2, str1, &res);
    // print_S(&str2);
    // print_S(str1);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("лексиграфическая разница: %d\n", res);
    }

    mistake = str_cmp_length_lecs(str1, &str2, &res);
    print_mistake(mistake);
    if (mistake == OK)
    {

        printf("Сравнение в начале по длине потом лексиграфически: %d\n", res);
    }

    printf("\nкопирования содержимого экземпляра типа String в существующий экземпляр типа String\n");
    mistake = string_cpy(&str2, str1);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("str2: ");
        print_S(&str2);
        printf("str1: ");
        print_S(str1);
    }

    printf("\nкопирования содержимого экземпляра типа String в новый экземпляр типа String\n");
    mistake = create_din_string_cpy(str1, &str3);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("str1: ");
        print_S(&str2);
        printf("str3: ");
        print_S(str1);
    }

    printf("\nконкатенации к содержимому первого экземпляра типа String содержимого 2-ого экземпляра типа String\n");
    mistake = cat_string(str1, str3);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("str1: ");
        print_S(str1);
        printf("str3: ");
        print_S(str3);
    }

    printf("\nудаления внутреннего содержимого экземпляра типа String\n");
    // как понтять String статически создано или динамически, чистить array как?
    mistake = delete_inside_str(str3);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("%d; указатель нулевой:%d\n", str3->length, str3->array == NULL);
    }

    mistake = delete_inside_str(&str2);
    print_mistake(mistake);
    if (mistake == OK)
    {
        printf("%d; указатель нулевой:%d\n", str2.length, str2.array == NULL);
    }

    clear(&str1);
    clear(&str3);

    mistake = create_string(str0, &str1);
    printf("создания экземпляра типа String на основе значения типа char *\n");
    print_mistake(mistake);
    if (mistake == OK)
    {
        print_S(str1);
    }

    clear(&str1);

    return 0;
}