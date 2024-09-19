#include <stdio.h>
#include <string.h>

/*
 -r записать в файл, путь к которому подаётся последним аргументом командной
строки, лексемы из файлов file1 и file2, пути к которым подаются как третий и
четвёртый аргументы командной строки соответственно, где на нечётных
позициях находятся лексемы из file1, а на чётных – из file2. Лексемы во входных
файлах разделяются произвольным количеством символов пробела, табуляций и
переносов строк. Если количество лексем в файлах различается, после
достижения конца одного из входных файлов, необходимо переписать в
выходной файл оставшиеся лексемы из другого из входных файлов. Лексемы в
выходном файле должны быть разделены одним символом пробела.
*/
enum err
{
    OK,
    FILE_NOT_OPEN,
    FEW_ARGUMENTS,
    TO_MUCH_ARGUMENTS,
    WRONG_FLAG
};

void skeep_white_space (FILE * file, char * c)
{
    while ((*c == ' ') || (*c == '\t') || (*c == '\n') || (*c =='\r') )
    {
        * c = fgetc(file);
    }
    return;
}

//????????????Файлы нужно открывать в функции под задание или в main

int f_r(const char * file1, const char * file2, const char * file3)
{
    FILE * file1_in = fopen(file1, "r");
    FILE * file2_in = fopen(file2, "r");
    FILE * file_out = fopen(file3, "w");
    if (!(file1_in) || !(file2_in) || !(file_out))
        return FILE_NOT_OPEN;
    char c = ' ', c_pr = ' ', a = ' ';
    FILE * file_temp = file1_in;
    int white_space = 0;
    skeep_white_space(file1_in, &c);
    fputc(c, file_out);
    while ((c = fgetc(file_temp)) != EOF)
    {
        white_space = 0;
        if ((c == ' ') || (c == '\t') || (c  == '\n') || (c=='\r'))
        {
            fputc(' ', file_out);
            skeep_white_space(file_temp, &c);
            white_space = 1;
            a = c_pr;
            c_pr = c;
            file_temp = ((file_temp == file2_in) ? file1_in : file2_in);
            c = a;
            skeep_white_space(file_temp, &c);
            if ((c != ' ') && (c != EOF))
                fputc(c, file_out);
        }
        else
            fputc(c, file_out);
    }

    if (!(white_space))
        fputc(' ', file_out);

    file_temp = (file_temp == file1_in) ? file2_in : file1_in;
    c = c_pr; // Оно EOF если колво слов в файлах одинаковое, первая буква из более длинного файла
    fputc(c, file_out);
    while ((c=fgetc(file_temp)) != EOF)
    {
        if ((c == ' ') || (c == '\t') || (c == '\n') || (c =='\r'))
        {
            fputc(' ', file_out);
            skeep_white_space(file_temp, &c);
        }
        if (c != EOF)
            fputc(c, file_out);
    }

    fclose(file1_in);
    fclose(file2_in);
    fclose(file_out);

    return 0;
}

/*
-a записать в файл, путь к которому подаётся последним аргументом командной
строки, файл, путь к которому подаётся третьим аргументом командной строки,
таким образом, чтобы:
○ в каждой десятой лексеме сначала все символы букв латинского алфавита
были преобразованы в эквивалентные символы строчных букв латинского
алфавита, а затем все символы были преобразованы в эквивалентные им
ASCII-коды, записанные в системе счисления с основанием 4;
○ в каждой второй (и одновременно не десятой) лексеме все символы букв
латинского алфавита были преобразованы в эквивалентные символы
строчных букв латинского алфавита;
○ в каждой пятой (и одновременно не десятой) лексеме все символы были
преобразованы в эквивалентные им ASCII-коды, записанные в системе
счисления с основанием 8.
*/
void write_to_base (int n, int q, FILE *file_out)
{
    if (n == 0)
        return;
    write_to_base(n / q, q, file_out);
    fputc('0' + n % q, file_out);
}

int f_a(char * file1, char * file2)
{
    FILE * file_in = fopen(file1, "r");
    FILE * file_out = fopen(file2, "w");
    if (!(file_in) || !(file_out))
        return FILE_NOT_OPEN;
    char c = ' ';
    skeep_white_space(file_in, &c);
    int i = 1;
    while (c != EOF)
    {
        if ((c != ' ') && ( c != '\t') && (c != '\n') && (c != '\r'))
        {
            if (!((i % 10 == 0) || (i % 2 == 0) || (i % 5 == 0)))
                fputc(c, file_out);
            else
            {
                if (i % 10 == 0)
                {
                    if (((c <= 'Z') && (c >= 'A')) || ((c <= 'z') && (c >='a')))
                    {
                        if ((c <= 'Z') && (c >= 'A'))
                            c = c - 'A' + 'a';
                        int n = c;
                        write_to_base(c, 4, file_out);
                    }
                    else
                        fputc(c, file_out);
                }
                else
                {
                    if (i % 2 == 0)
                    {
                        if ((c <= 'Z') && (c >= 'A'))
                            c = c - 'A' + 'a';
                        fputc(c, file_out);
                    }
                    if (i % 5 == 0)
                    {
                        write_to_base(c, 8, file_out);
                        //printf("%c = %d в ASCII преобразовании\n", c, (int)c);//???? что делать с unicode  символами
                    }
                }
            }
            c = fgetc(file_in);
        }
        else
        {
            ++i;
            skeep_white_space(file_in, &c);
            fputc(' ', file_out);
        }
    }
    fclose(file_in);
    fclose(file_out);

    return 0;
}

int valid(int argc, char * argv[])
{
    if (argc < 4)
        return FEW_ARGUMENTS;
    printf("%ld", strlen(argv[1]));
    if (!(((strlen(argv[1]) == 2) && ((argv[1][0] == '-') || (argv[1][0] == '/')) && ((argv[1][1] == 'a') || (argv[1][1] == 'r')))))
        return WRONG_FLAG;


    if (((argv[1][1] == 'a') && (argc < 4)) || ((argv[1][1] == 'r') && (argc < 5)))
        return FEW_ARGUMENTS;
    if (((argv[1][1] == 'a') && (argc > 4)) || ((argv[1][1] == 'r') && (argc > 5)))
        return TO_MUCH_ARGUMENTS;
    return OK;
}

int main(int argc, char * argv[])
{
    enum err d;
    switch (valid(argc, argv))
    {
        case OK:
            if (argv[1][1] == 'a')
                d = f_a(argv[2], argv[3]);
            if (argv[1][1] == 'r')
                d = f_r(argv[2], argv[3], argv[4]);
            if (d == OK)
                printf("Программа закончила работу успешно\n");
            else
                printf("Файл не удалось открыть либо его название неверное\n");
            break;

        case FEW_ARGUMENTS:
            printf("Введено недостаточно аргументов командной строки\n");
            break;

        case TO_MUCH_ARGUMENTS:
            printf("Введено слишком много аргументов командной строки\n");
            break;

        case WRONG_FLAG:
            printf("Введен неверный флаг\n");
            break;
    }

    return 0;
}
