#include <stdio.h>
#include <string.h>

/*
 Вывод программы должен транслироваться в выходной файл. Программа распознает следующие флаги:
● -d необходимо исключить символы арабских цифр из входного файла;
● -i для каждой строки входного файла в выходной файл необходимо записать
сколько раз в этой строке встречаются символы букв латинского алфавита;
● -s для каждой строки входного файла в выходной файл необходимо записать
сколько раз в этой строке встречаются символы, отличные от символов букв
латинского алфавита, символов арабских цифр и символа пробела;
● -a необходимо заменить символы, отличные от символов цифр, ASCII кодом,
записанным в системе счисления с основанием 16.

*/
enum err
{
    OK,
    WRONG_FLAG,
    FILE_INPUT_NOT_OPEN,
    FILE_OUTPUT_NOT_OPEN,
    TOO_LONG_FILE_NAME,
    FEW_ARGUMENTS

};

//-d необходимо исключить символы арабских цифр из входного файла;
int put_away_num(const char * f_in, const char * f_out)
{
    char c;
    FILE * fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE * fout = fopen(f_out, "w+");
    if (!fout)
    {
        fclose(fin);
        return FILE_OUTPUT_NOT_OPEN;
    }
    while ((c = fgetc(fin)) != EOF)
    {
        if (!((c <= '9') && (c >= '0')))
        {
            fputc(c, fout);
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

//-i для каждой строки входного файла в выходной файл необходимо записать
//сколько раз в этой строке встречаются символы букв латинского алфавита

//Что считается концом строки в файле -- ноль как и везде !
int cnt_latin_letter(const char * f_in, const char * f_out)
{
    FILE * fin = fopen(f_in, "r");
    FILE * fout = fopen(f_out, "w+");
    char c = ' ';
    if (!fin)
    {
        fclose(fin);
        return FILE_INPUT_NOT_OPEN;
    }
    if (!fout)
    {
        fclose(fout);
        return FILE_OUTPUT_NOT_OPEN;
    }
    char buf[256];
    while(fgets(buf, 256, fin) != NULL)
    {
        int cnt = 0;
        for (int i = 0; i < strlen(buf); ++i)
        {
            if (((buf[i] >= 'A') && (buf[i] <= 'Z')) || ((buf[i] >= 'a') && (buf[i] <= 'z')))
            {
                ++cnt;
            }
        }
        fprintf(fout, "%d\n", cnt);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

/*
-s для строки входного файла в выходной файл необходимо записать
сколько раз в этой строке встречаются символы, отличные от символов букв
латинского алфавита, символов арабских цифр и символа пробела;
*/
int cnt_symb(const char * f_in, const char * f_out)
{
    FILE * fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE * fout = fopen(f_out, "w+");
    if (!fout)
    {
        fclose(fin);
        return FILE_OUTPUT_NOT_OPEN;
    }
    char buf[256];

    while(fgets(buf, 256, fin) != NULL)
    {
        int cnt = 0;
        printf("%s\n", buf);
        for (int i = 0; i < strlen(buf); ++i)
        {
            if (!(((buf[i] >= 'A') && (buf[i] <= 'Z')) || ((buf[i] >= 'a') && (buf[i] <= 'z')) || ((buf[i] <= '9') && (buf[i]>='0')) || (buf[i] == ' ')))
            {
                printf("%d\n", buf[i]);
                ++cnt;
                // Считаются также символы переноса строки, табуляции, переноса каретки и другие
            }
        }
        fprintf(fout, "%d\n", cnt);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}

//-a необходимо заменить символы, отличные от символов цифр, ASCII кодом,
//записанным в системе счисления с основанием 16.
int replace_not_num_to_base16(const char * f_in, const char * f_out)
{
    FILE * fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE * fout = fopen(f_out, "w+");
    if (!fout)
    {
        fclose(fin);
        return FILE_OUTPUT_NOT_OPEN;
    }
    char number_16[sizeof(int) * 8 + 1];
    char c;
    while ((c = fgetc(fin)) != EOF)
    {
        if (!((c >= '0') && (c <= '9')))
        {
            int n = c, i = 0;
            while (n > 0)
            {
                int y = n % 16;
                number_16[i] = (y > 9) ? ('A' + y % 10) : ('0' + y);
                n /= 16;
                ++i;
            }
            for (--i; i >= 0; --i)
            {
                fputc(number_16[i], fout);
            }
        }
        else
        {
            fputc(c, fout);
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}


/*Флаг начинается с символа ‘-’ или ‘/’.
Если флаг содержит в качестве второго символа опциональный символ ‘n’ (то есть
“-nd”, “/nd”, “-ni”, “/ni”, “-ns”, “/ns”, “-na”, “/na”), то путь к выходному файлу является
третьим аргументом командной строки; иначе имя выходного файла генерируется
приписыванием к имени входного файла префикса “out_”.
*/
int creat_file_name(const char * pt_fl_in, char * pt_fl_out)
{
    int len_fl_in = strlen(pt_fl_in);
    //printf("%s\n", pt_fl_in);
    int len_fl_name = 0;
    char pr[] = "out_";
    int len_pr = strlen(pr);

    int j = 0;
    while ((len_fl_in - len_fl_name -1 >= 0) && (pt_fl_in[len_fl_in - 1 - len_fl_name] != '/'))
    {
        ++len_fl_name;
    }
    if (len_pr + len_fl_name > 255)
        return TOO_LONG_FILE_NAME;
    //printf("%d", len_fl_name);
    for (j = 0; j < len_pr; ++j)
    {
        pt_fl_out[j] = pr[j];
    }
    for (j = 0; j < len_fl_name + len_pr+ 1; ++j)
    {
        pt_fl_out[len_pr + j] = pt_fl_in[len_fl_in - len_fl_name  + j];
    }
    return 0;
}

int valid(int argc, char * argv[], char * * pt_fl_in, char *pt_fl_out)
{
    if (argc < 3)
    {
        return FEW_ARGUMENTS;
    }
    if ((argv[1][0] != '-') && (argv[1][0] != '/'))
    {
        return WRONG_FLAG;
    }
    if (!((argv[1][1] == 'n') && strstr(" d i s a ", argv[1] + 2)) && !(strstr(" d i s a ", argv[1] + 1)))
    {
        return WRONG_FLAG;
    }

    if ((argv[1][1] == 'n') && strstr(" d i s a ", argv[1] + 2))
    {
        if (argc < 4)
            return FEW_ARGUMENTS;//не введен входной файл
        *(pt_fl_in) = argv[3];
        strcpy(pt_fl_out, argv[2]);
        //????? не понимаю каким аргументом по счету подается входной файл
        return OK;
    }
    *( pt_fl_in) = argv[2];
    return creat_file_name(*pt_fl_in, pt_fl_out);
}

int main(int argc, char * argv[])
{
    char * pt_fl_in = NULL;
    char pt_fl_out[256];
    enum err file_error;
    switch (valid(argc, argv, &pt_fl_in, pt_fl_out))
    {
        case OK:
            printf("%s-входной файл\n%s-выходной файл\n", pt_fl_in, pt_fl_out);
            switch (argv[1][strlen(argv[1])-1])
            {
                case 'd':
                    file_error = put_away_num(pt_fl_in, pt_fl_out);
                    break;

                case 'i':
                    file_error = cnt_latin_letter(pt_fl_in, pt_fl_out);
                    break;

                case 's':
                    file_error = cnt_symb(pt_fl_in, pt_fl_out);
                    break;

                case 'a':
                    file_error = replace_not_num_to_base16(pt_fl_in, pt_fl_out);
                    break;

            }
            if (file_error == FILE_INPUT_NOT_OPEN)
            {
                printf("Входной файл не открылся или был не найден\n");
            }
            if (file_error == FILE_OUTPUT_NOT_OPEN)
            {
                printf("Выходной файл не открылся, либо \n");
            }
            if (file_error == OK)
            {
                printf("Программа отработала успешно\n");
            }
            break;

        case FEW_ARGUMENTS:
            printf("Недостаточно аргументов командной строки\n");
            break;

        case WRONG_FLAG:
            printf("Неверный флаг\n");
            break;

        case TOO_LONG_FILE_NAME:
            printf("При генерации имени выходного файла была превышена его длина\n");
            break;
    }

    return 0;
}
