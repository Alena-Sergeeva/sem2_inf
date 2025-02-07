#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
/*
Реализуйте функции overfprintf и oversprintf, поведение которых схоже с поведением
стандартных функций fprintf и sprintf, то есть эти функции имеют одинаковый
прототип и логику работы, но в ваших функциях помимо стандартных флагов
определены следующим образом дополнительные флаги:


● %CV – аналогично флагу %Cv, при этом символы букв во входном строковом
представлении целого числа должны быть записаны в верхнем регистре;
● %to – печать в поток вывода результата перевода целого числа, записанного в
строковом представлении в системе счисления с заданным основанием в
систему счисления с основанием 10 (при обработке флага первым параметром
функции, “снимаемым” со стека, является строка, описываемая значением типа
char *, вторым - основание исходной системы счисления в диапазоне [2..36] (при
основании системы счисления, не входящем в диапазон, значение основания
системы счисления устанавливается равным 10)); символы букв во входном
строковом представлении целого числа должны быть записаны в нижнем
регистре;
● %TO - аналогично флагу %to, при этом символы букв во входном строковом
представлении целого числа должны быть записаны в верхнем регистре;
● %mi – печать дампа памяти (байты значения, записанные в системе счисления с
основанием 2, в порядке нахождения в памяти “слева направо”; строковые
представления байтов должны сепарироваться одним символом пробела)
значения знакового целого 4-байтного числа;
● %mu – печать дампа памяти (байты значения, записанные в системе счисления с
основанием 2, в порядке нахождения в памяти “слева направо”; строковые
представления байтов должны сепарироваться одним символом пробела)
значения беззнакового целого 4-байтного числа;
● %md – печать дампа памяти (байты значения, записанные в системе счисления с
основанием 2, в порядке нахождения в памяти “слева направо”; строковые
представления байтов должны сепарироваться одним символом пробела),
значения вещественной переменной типа double;
● %mf – печать дампа памяти (байты значения, записанные в системе счисления с
основанием 2, в порядке нахождения в памяти “слева направо”; строковые
представления байтов должны сепарироваться одним символом пробела),
значения вещественной переменной типа float.
Продемонстрируйте работу реализованных функций
*/
/*
#define dump(type, ptr_num, steam)                                    \
    {                                                                 \
        for (int i = 0; i < sizeof(type); ++i)                        \
        {                                                             \
            cv_print(steam, *((unsigned char *)(ptr_num) + i), 2, 0); \
            fputc(' ', steam);                                        \
        }                                                             \
    }
*/
enum err
{
    OK = 0,
    NULL_PTR = -1,
    WRONG_STEAM = -2,
    WRONG_RO_NUM = -3,
    MEMORY_ERROR = -4,
    WRONG_STR = -5,
    WRONG_FORMAT = -6
};

typedef struct
{
    int numb;
    const char *letters;
} Rom_num;

int print_str(FILE *stream, const char *str)
{
    char *str_ptr = NULL;
    if ((str == NULL) || (stream == NULL))
    {
        return NULL_PTR;
    }
    str_ptr = (char *)str - 1;
    while (*(++str_ptr) != '\0')
    {
        // printf("%c", *str_ptr);
        fputc(*str_ptr, stream);
    }
    // так как \0 не печатаем
    return (str_ptr - str);
}
/*
int from_rom_char_to_int(const char с)
{
    switch (c)
    {
    case 'I':
        return 1;
        break;
    case 'V':
        return 5;
        break;
    case 'X':
        return 10;
        break;
    case 'L':
        return 50;
        break;
    case 'C':
        return 100;
        break;
    case 'D':
        return 500;
        break;
    case 'M':
        return 1000;
        break;
    }
    return WRONG_RO_NUM;
}
*/
//%Ro – печать в поток вывода целого числа типа int, записанного римскими цифрами;
/*Это не то надо было в обратную сторону...
int from_rom_to_int(const char *str, int *num)
{
    int _c = 0;
    int cnt_same = 1, ch_in_int = 0, sum = 0;
    if ((str == NULL) || (num == NULL))
    {
        return NULL_PTR;
    }
    ch_in_int = from_rom_char_to_int(*str);
    if (ch_in_int == WRONG_RO_NUM)
    {
        return WRONG_RO_NUM;
    }
    _c = ch_in_int;
    *num = 0;
    while (*(++str) != '\0')
    {
        ch_in_int = from_rom_char_to_int(*str);
        if (ch_in_int == WRONG_RO_NUM)
        {
            return WRONG_RO_NUM;
        }
        if (_c == ch_in_int)
        {
            ++cnt_same;
            if (cnt_same > 3)
            {
                return WRONG_RO_NUM;
            }
            if (strchr("VLD", *str) != NULL)
            {
                return WRONG_RO_NUM;
            }
            *num += _c;
        }
        else
        {
            cnt_same = 1;
            if (_c < ch_in_int)
            {
                if ((_c == 5) || (_c == 50) || (_c == 500))
                {
                    return WRONG_RO_NUM;
                }
                sum = ch_in_int - _c;
                // printf("%d %d %d\n", _c, ch_in_int, sum);
                if ((sum == 4) || (sum == 9) || (sum == 40) || (sum == 90) || (sum == 400) || (sum == 900))
                {
                    *num -= _c;
                }
                else
                {
                    return WRONG_RO_NUM;
                }
            }
            else
            {
                *num += _c;
            }
        }
        _c = ch_in_int;
    }
    *num += _c;
    // printf("%d, cnt - %d\n", *num, cnt_same);
    return OK;
}
*/

int from_int_to_rom_char(int num, char *str)
{
    int i = 0;
    // 3999 MMMCMXCIX
    if ((num > 3999) || (num <= 0))
    {
        return WRONG_RO_NUM;
    }
    int length = 0;
    Rom_num rom_num[13] = {{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}};
    while (num > 0)
    {
        i = -1;
        while (num - rom_num[++i].numb < 0)
        {
        }
        num -= rom_num[i].numb;
        str[length++] = rom_num[i].letters[0];
        if (rom_num[i].letters[1] != '\0')
        {
            str[length++] = rom_num[i].letters[1];
        }
    }
    str[length] = '\0';
    return 0;
}
/*
%Zr – печать в поток вывода цекендорфова представления целого числа типа
unsigned int (коэффициенты 0 и 1 при числах Фибоначчи должны быть записаны
от младшего к старшему слева направо с дополнительной единицей в конце
записи, репрезентирующей окончание записи);
*/
int Zeckendorf(unsigned int num, char **Zec_str)
{
    int str_size = 1;
    int ch_f_prev = 1, ch_f = 1, tmp = 0, i = 0;
    if (Zec_str == NULL)
    {
        NULL_PTR;
    }
    while (ch_f <= num)
    {
        tmp = ch_f;
        ch_f = ch_f + ch_f_prev;
        ch_f_prev = tmp;
        ++str_size;
    }
    // printf("%d\n%d %d\n", num, str_size, ch_f);

    if (!(*Zec_str = (char *)malloc(sizeof(char) * str_size)))
    {
        return MEMORY_ERROR;
    }
    (*Zec_str)[str_size - 1] = '\0';
    for (i = str_size - 2; i >= 0; --i)
    {
        if (ch_f_prev <= num)
        {
            (*Zec_str)[i] = '1';
            num -= ch_f_prev;
        }
        else
        {
            (*Zec_str)[i] = '0';
        }
        // printf("%d : %c; ", i, (*Zec_str)[i]);
        tmp = ch_f_prev;
        ch_f_prev = ch_f - ch_f_prev;
        ch_f = tmp;
    }
    // printf("%s\n", *Zec_str);
    return OK;
}

/*
%Cv печать целого числа типа int в системе счисления с заданным основанием
(при обработке флага первым параметром функции, “снимаемым” со стека,
является целое число типа int, вторым - основание целевой системы счисления в
диапазоне [2..36] (при основании системы счисления, не входящем в диапазон,
значение основания системы счисления устанавливается равным 10)); символы
букв в результирующем строковом представлении целого числа должны быть
записаны в нижнем регистре;
*/
int cv_print(char *buf, int num, int base, int registr)
{
    int ost = 0;
    int neg = 0;
    char str[(sizeof(int) << 3) + 1];
    char *ptr_end = NULL;
    if (buf == NULL)
    {
        return NULL_PTR;
    }
    if ((base < 2) || (base > 36))
    {
        base = 10;
    }

    ptr_end = str + (sizeof(int) << 3); // + << один приоритет, либо у плюса больше
    *ptr_end = '\0';
    if (num == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return OK;
    }
    if (num < 0)
    {
        neg = 1;
        num *= (-1);
    }
    while (num > 0)
    {
        --ptr_end;
        ost = num % base;
        num /= base;
        *ptr_end = ost + ((ost < 10) ? '0' : (-10 + ((registr) ? 'A' : 'a')));
    }
    if (neg)
    {
        *--ptr_end = '-';
    }
    memcpy(buf, ptr_end, sizeof(char) * (ptr_end - str + 1));
    return OK;
}
/*
 %to – печать в поток вывода результата перевода целого числа, записанного в
строковом представлении в системе счисления с заданным основанием в
систему счисления с основанием 10 (при обработке флага первым параметром
функции, “снимаемым” со стека, является строка, описываемая значением типа
char *, вторым - основание исходной системы счисления в диапазоне [2..36] (при
основании системы счисления, не входящем в диапазон, значение основания
системы счисления устанавливается равным 10)); символы букв во входном
строковом представлении целого числа должны быть записаны в нижнем
регистре;
*/
int to_print(char *str, int base, int low, int *num)
{
    int res = 0, numb = 0;
    if ((str == NULL) || (num == NULL))
    {
        return NULL_PTR;
    }
    if ((base < 2) || (base > 32))
    {
        base = 10;
    }
    while (*str)
    {
        if ((isalnum(*str) == 0) || (isupper(*str) && low) || (islower(*str) && low == 0))
        {
            return WRONG_STR;
        }
        numb = isdigit(*str) ? (*str) - '0' : toupper(*str) - 'A' + 10;
        if (numb >= base)
        {
            return WRONG_STR;
        }
        res = res * base + numb;
        ++str;
    }
    *num = res;
    return OK;
}

// а как понять что принимать int или long int

int dump(void *ptr_num, size_t size_of_type, char *buf)
{
    unsigned char *ptr = (unsigned char *)(ptr_num);
    char *ptr_buf = NULL;
    if ((buf == NULL) || (ptr_num == NULL))
    {
        return NULL_PTR;
    }
    // добавляем размер на пробелы и последний \0
    ptr_buf = buf;
    for (int i = 0; i < size_of_type; ++i)
    {
        cv_print(ptr_buf, ptr[i], 2, 0);
        ptr_buf += strlen(ptr_buf);
        *(ptr_buf++) = ' ';
    }
    *(--ptr_buf) = '\0';
    // printf("check:%s\n", buf);
    return 0;
}
// int sprintf(char *string, char *format, arg1, arg2, ...)

int oversprintf(char *string, const char *format, ...)
{
    char *ptr = NULL;
    char *buf = NULL;
    char format_str[3];
    int num = 0;
    int j = 0;
    float num_float = 0.0;
    double num_double = 0.0;
    enum err mistake = 0;
    va_list iterator;
    if (format == NULL)
    {
        return NULL_PTR;
    }
    if (string == NULL)
    {
        return WRONG_STEAM;
    }
    ptr = (char *)format;
    va_start(iterator, format);
    format_str[0] = '%';

    while (*ptr != '\0')
    {

        if (*ptr != '%')
        {
            string[j++] = *ptr;
            ++ptr;
        }
        else
        {
            if (*(ptr + 1) == '\0')
            {
                return WRONG_FORMAT;
            }
            ++ptr;
            if ('d' == *ptr || ('i' == *ptr) || ('o' == *ptr) || ('c' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                j += sprintf(string + j, format_str, va_arg(iterator, int));
                ptr += 1;
                continue;
            }
            if (('x' == *ptr) || ('X' == *ptr) || ('u' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                j += sprintf(string + j, format_str, va_arg(iterator, unsigned int));
                ptr += 1;
                continue;
            }
            if (('E' == *ptr) || ('e' == *ptr) || ('g' == *ptr) || ('G' == *ptr) || ('f' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                j += sprintf(string + j, format_str, va_arg(iterator, double));
                ptr += 1;
                continue;
            }
            if ('s' == *ptr)
            {
                j += sprintf(string + j, "%s", va_arg(iterator, char *));
                ptr += 1;
                continue;
            }
            if ('p' == *ptr)
            {
                j += sprintf(string + j, "%p", va_arg(iterator, void *));
                ptr += 1;
                continue;
            }
            if (*(ptr + 1) == '\0')
            {
                return WRONG_FORMAT;
            }

            if ((*ptr == 'R') && (*(ptr + 1) == 'o'))
            {
                //// 3999 MMMCMXCIX
                char str[10];
                if ((mistake = from_int_to_rom_char(va_arg(iterator, int), str)) != 0)
                {
                    va_end(iterator);
                    return mistake;
                }
                j += sprintf(string + j, "%s", str);
                ptr += 2;
                continue;
            }
            if (((*ptr == 'Z') && (*(ptr + 1) == 'r')))
            {
                mistake = Zeckendorf(va_arg(iterator, unsigned int), &buf);
                if (mistake != 0)
                {
                    free(buf);
                    va_end(iterator);
                    return mistake;
                }
                j += sprintf(string + j, "%s", buf);
                free(buf);
                ptr += 2;
                continue;
            }
            if (((*ptr == 'C') && ((*(ptr + 1) == 'v') || (*(ptr + 1) == 'V'))))
            {
                // заменить на статику
                if (!(buf = (char *)malloc(sizeof(char) * (sizeof(int) << 3 + 1))))
                {
                    return MEMORY_ERROR;
                }

                num = va_arg(iterator, int);
                mistake = cv_print(buf, num, va_arg(iterator, int), (*(ptr + 1) == 'V'));
                if (mistake != 0)
                {
                    free(buf);
                    va_end(iterator);
                    return mistake;
                }
                j += sprintf(string + j, "%s", buf);
                free(buf);
                ptr += 2;
                continue;
            }
            if (((*ptr == 't') && (*(ptr + 1) == 'o')) || ((*ptr == 'T') && (*(ptr + 1) == 'O')))
            {
                buf = va_arg(iterator, char *);
                mistake = to_print(buf, va_arg(iterator, int), (*ptr == 'T'), &num);
                if (mistake != 0)
                {
                    va_end(iterator);
                    return mistake;
                }
                j += sprintf(string + j, "%d", num);
                buf = NULL;
                ptr += 2;
                continue;
            }
            if ((*ptr == 'm') && (strchr("iufd", *(ptr + 1))))
            {
                if (!(buf = (char *)malloc((sizeof(int) << 3 + sizeof(int)) * sizeof(char))))
                {
                    return MEMORY_ERROR;
                }
                if (*(ptr + 1) == 'i')
                {
                    num = va_arg(iterator, int);
                    mistake = dump(&num, sizeof(int), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    j += sprintf(string + j, "%s", buf);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'u')
                {
                    num = va_arg(iterator, unsigned int);
                    mistake = dump(&num, sizeof(unsigned int), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    j += sprintf(string + j, "%s", buf);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'd')
                {
                    num_double = va_arg(iterator, double);
                    mistake = dump(&num_double, sizeof(double), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    // printf("pupu:%s %f\n", buf, num_double);
                    j += sprintf(string + j, "%s", buf);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'f')
                {
                    num_float = va_arg(iterator, double);
                    mistake = dump(&num_float, sizeof(float), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    // printf("pupu:%s\n", buf);
                    j += sprintf(string + j, "%s", buf);
                    ptr += 2;
                    free(buf);
                    continue;
                }
            }
            if (*(ptr) == '%')
            {
                string[j++] = '%';
                ++ptr;
                continue;
            }
            va_end(iterator);
            return WRONG_FORMAT;
        }
    }
    string[j] = '\0';
    va_end(iterator);
    return 0;
}

int overfprintf(FILE *stream, const char *format, ...)
{
    char *ptr = NULL;
    char *buf = NULL;
    char format_str[3];
    int num = 0;
    float num_float = 0.0;
    double num_double = 0.0;
    enum err mistake = 0;
    va_list iterator;
    if (format == NULL)
    {
        return NULL_PTR;
    }
    if (stream == NULL)
    {
        return WRONG_STEAM;
    }
    ptr = (char *)format;
    va_start(iterator, format);
    format_str[0] = '%';
    while (*ptr != '\0')
    {
        if ((*ptr != '%') || (*(ptr + 1) == '\0'))
        {
            fputc(*ptr, stream);
            ++ptr;
        }
        else
        {
            if (*ptr == '\0')
            {
                return OK;
            }
            ++ptr;
            if ('d' == *ptr || ('i' == *ptr) || ('o' == *ptr) || ('c' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                fprintf(stream, format_str, va_arg(iterator, int));
                ptr += 1;
                continue;
            }
            if (('x' == *ptr) || ('X' == *ptr) || ('u' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                fprintf(stream, format_str, va_arg(iterator, unsigned int));
                ptr += 1;
                continue;
            }
            if (('E' == *ptr) || ('e' == *ptr) || ('g' == *ptr) || ('G' == *ptr) || ('f' == *ptr))
            {
                format_str[1] = *ptr;
                format_str[2] = '\0';
                fprintf(stream, format_str, va_arg(iterator, double));
                ptr += 1;
                continue;
            }
            if ('s' == *ptr)
            {
                fputs(va_arg(iterator, char *), stream);
                ptr += 1;
                continue;
            }
            if ('p' == *ptr)
            {
                fprintf(stream, "%p", va_arg(iterator, void *));
                ptr += 1;
                continue;
            }
            if (*(ptr + 1) == '\0')
            {
                return WRONG_FORMAT;
            }

            if ((*ptr == 'R') && (*(ptr + 1) == 'o'))
            {
                //// 3999 MMMCMXCIX
                char str[10];
                if ((mistake = from_int_to_rom_char(va_arg(iterator, int), str)) != 0)
                {
                    va_end(iterator);
                    return mistake;
                }
                fputs(str, stream);
                ptr += 2;
                continue;
            }
            if (((*ptr == 'Z') && (*(ptr + 1) == 'r')))
            {
                mistake = Zeckendorf(va_arg(iterator, unsigned int), &buf);
                if (mistake != 0)
                {
                    free(buf);
                    va_end(iterator);
                    return mistake;
                }
                fputs(buf, stream);
                free(buf);
                ptr += 2;
                continue;
            }
            if (((*ptr == 'C') && ((*(ptr + 1) == 'v') || (*(ptr + 1) == 'V'))))
            {
                if (!(buf = (char *)malloc(sizeof(char) * (sizeof(int) << 3 + 1))))
                {
                    return MEMORY_ERROR;
                }
                num = va_arg(iterator, int);
                // видимо аргументы в функцию укладываются справа налево в сv_printf,наверное
                // она начинает проходится по аргументам с конца, по этому в базу ложится число
                // проверить в интернете
                mistake = cv_print(buf, num, va_arg(iterator, int), (*(ptr + 1) == 'V'));
                if (mistake != 0)
                {
                    free(buf);
                    va_end(iterator);
                    return mistake;
                }
                fputs(buf, stream);
                free(buf);
                ptr += 2;
                continue;
            }
            if (((*ptr == 't') && (*(ptr + 1) == 'o')) || ((*ptr == 'T') && (*(ptr + 1) == 'O')))
            {
                buf = va_arg(iterator, char *);
                mistake = to_print(buf, va_arg(iterator, int), (*ptr == 'T'), &num);
                if (mistake != 0)
                {
                    free(buf);
                    va_end(iterator);
                    return mistake;
                }
                fprintf(stream, "%d", num);
                ptr += 2;
                continue;
            }
            if ((*ptr == 'm') && (strchr("iufd", *(ptr + 1))))
            {
                if (!(buf = (char *)malloc((sizeof(int) << 3 + sizeof(int)) * sizeof(char))))
                {
                    return MEMORY_ERROR;
                }
                if (*(ptr + 1) == 'i')
                {
                    num = va_arg(iterator, int);
                    mistake = dump(&num, sizeof(int), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    fputs(buf, stream);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'u')
                {
                    num = va_arg(iterator, unsigned int);
                    mistake = dump(&num, sizeof(unsigned int), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    fputs(buf, stream);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'd')
                {
                    num_double = va_arg(iterator, double);
                    mistake = dump(&num_double, sizeof(double), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    fputs(buf, stream);
                    ptr += 2;
                    free(buf);
                    continue;
                }
                if (*(ptr + 1) == 'f')
                {
                    num_float = va_arg(iterator, double);
                    mistake = dump(&num_float, sizeof(float), buf);
                    if (mistake != 0)
                    {
                        free(buf);
                        va_end(iterator);
                        return mistake;
                    }
                    fputs(buf, stream);
                    ptr += 2;
                    free(buf);
                    continue;
                }
            }
            if (*(ptr) == '%')
            {
                fputc('%', stream);
                ++ptr;
                continue;
            }
            va_end(iterator);
            return WRONG_FORMAT;
        }
    }
    va_end(iterator);
    return 0;
}
void print_mistake(int mistake)
{
    switch (mistake)
    {
    case NULL_PTR:
        printf("Передан нулевой указатель\n");
        break;
    case WRONG_RO_NUM:
        printf("Римское число превышает 3999\n");
        break;
    case MEMORY_ERROR:
        printf("Не удалось выделить память\n");
        break;
    case WRONG_STR:
        printf("Строка не подходит для данной системы исчисления, содержит недопустимые символы\n");
        break;
    case WRONG_FORMAT:
        printf("Нарушен формат спецификатора\n");
        break;
    }
}

int main()
{
    int num = 0;
    char *buf = NULL;
    float num1 = -4.25;
    double num2 = -4.25;
    enum err mist = 0;
    // int от младшего к старшему (Little-endian)
    print_mistake(overfprintf(stdout, "%"));
    print_mistake(overfprintf(stdout, "%%hsg %Ro dschsd %Zr %ch doump_int:%mi\n doump_unint:%mu\n dump_double:%md\n dump_float:%mf\n", 12, 19, '-', -4, 4, -4.25, 0.75));
    print_mistake(overfprintf(stdout, "string -- %s\n char:%c; %d, %u\n", "this is string", 'A', 4, -1));

    char str[200000];
    print_mistake(mist = oversprintf(str, "string -- %s\n char:%c; %d, %u\n", "this is string", 'A', 4, -1));
    if (mist == 0)
    {
        printf("%s", str);
    }
    print_mistake(mist = oversprintf(str, "test Ro:%Ro;\n test Zr:%Zr;\n\n", 143, 19));
    if (mist == 0)
    {
        printf("%s", str);
    }
    print_mistake(mist = oversprintf(str, "test Cv:%Cv;\n test Cv:%Cv;\n", 143, 2, 19, 16));
    if (mist == 0)
    {
        printf("%s", str);
    }
    print_mistake(mist = oversprintf(str, "test to (AB, base=16):%to;\n test TO (13, base=4):%TO;\n\n", "AB", 16, "13", 4));
    if (mist == 0)
    {
        printf("%s", str);
    }
    print_mistake(mist = oversprintf(str, "doump_int:%mi\n doump_unint:%mu\n dump_double:%md\n dump_float:%mf\n", -4, 4, -4.25, 0.75));
    if (mist == 0)
    {
        printf("%s", str);
    }
    print_mistake(mist = oversprintf(str, "hdggi%TO", "GJG", 11));
    if (mist == 0)
    {
        printf("%s", str);
    }
    return 0;
}
