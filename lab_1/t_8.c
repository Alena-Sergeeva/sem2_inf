#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define max(a, b) (((a) > (b)) ? (a) : (b))

enum err
{
    OK,
    INPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_OPEN,
    ARGUMENTS,
    WRONG_FLAG,
    NUM_TOO_BIG
};

void to_num(int *c_num, char c)
{
    *c_num = c + (isalpha(c) ? (-'A' + 10) : (-'0'));
    return;
}

int to_base_10(char *num, char *pt_num_end, char c_max, long long int *res)
{
    int base = 0, c_num = 0;
    to_num(&base, c_max);
    ++base;
    while ((*res >= 0) && (num <= pt_num_end))
    {
        to_num(&c_num, *num++);
        *res = *res * base + c_num;
    }
    if (num <= pt_num_end)
    {
        return NUM_TOO_BIG;
    }
    return 0;
}

void print_res(FILE *fout, enum err mistake, char *num, char c_max, long long int res, int fl_neg)
{
    int base;
    to_num(&base, c_max);
    // fprintf каждый раз обращается к дескриптору, каждый раз открывает файл, дорого
    // Нужно создать буфер и туда класть данные, когда накопится уже буфер закинуть в файл
    if (mistake == NUM_TOO_BIG)
    {
        (fl_neg ? fprintf(fout, "-%s %d переполнение типа long long int\n", num, ++base) : fprintf(fout, "%s %d переполнение типа long long int\n", num, ++base));
        return;
    }
    (fl_neg ? fprintf(fout, "-%s %d -%lld\n", num, ++base, res) : fprintf(fout, "%s %d %lld\n", num, ++base, res));
    return;
}

int read_file(char *file_in, char *file_out)
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    if ((fin = fopen(file_in, "r")) == NULL)
    {
        return INPUT_FILE_NOT_OPEN;
    }
    if ((fout = fopen(file_out, "w")) == NULL)
    {
        fclose(fin);
        return OUTPUT_FILE_NOT_OPEN;
    }

    //???максимальная длина 2-го числа которое влезет в long long int
    // int 64 использовать вместо лонг лонг , потому что современные ОС могут прировнять его к int
    int max_len_str = sizeof(__int64_t) * 8, fl_neg = 0;
    long long int res = 0;
    enum err mistake = 0;
    char num[sizeof(char) * max_len_str + 1];
    char *pt_num_end = num - 1;
    char c = ' ', c_max = '0';

    while ((c = fgetc(fin)) != EOF)
    {
        if (!(isspace(c)))
        {
            if (c == '-')
            {
                fl_neg = 1;
            }
            else
            {
                c = (isalpha(c) ? toupper(c) : c);
                c_max = max(c, c_max);
                if (pt_num_end - num <= max_len_str)
                {
                    *(++pt_num_end) = c;
                }
                else
                {
                    print_res(fout, NUM_TOO_BIG, num, c_max, res, fl_neg);
                    while (!isspace(fgetc(fin)))
                    {
                    }
                }
            }
        }
        if ((isspace(c)) && (pt_num_end != num - 1))
        {
            res = 0;
            *(pt_num_end + 1) = '\0';
            mistake = to_base_10(num, pt_num_end, c_max, &res);
            print_res(fout, mistake, num, c_max, res, fl_neg);
            pt_num_end = num - 1;
            c_max = '0';
            fl_neg = 0;
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

int valid(int argc, char *argv[], char **f_in, char **f_out)
{
    if (argc != 3)
    {
        return ARGUMENTS;
    }
    *f_in = argv[1];
    *f_out = argv[2];
    return OK;
}

int main(int argc, char *argv[])
{
    enum err mistake;
    char *f_in, *f_out;
    switch (valid(argc, argv, &f_in, &f_out))
    {
    case OK:
        mistake = read_file(f_in, f_out);
        switch (mistake)
        {
        case OK:
            printf("Программа закончила работу успешно\n");
            break;
        case INPUT_FILE_NOT_OPEN:
            printf("Входной файл не удалось открыть, либо его название неверное\n");
            break;
        case OUTPUT_FILE_NOT_OPEN:
            printf("Выходной файл не удалось открыть, либо его название неверное\n");
            break;
        }
        break;

    case ARGUMENTS:
        printf("Введено неверное количество аргументов командной строки\n");
        break;

        return 0;
    }
}