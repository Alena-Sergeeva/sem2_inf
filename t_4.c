#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum err
{
    OK,
    WRONG_FLAG,
    FILE_INPUT_NOT_OPEN,
    FILE_OUTPUT_NOT_OPEN,
    TOO_LONG_FILE_NAME,
    FEW_ARGUMENTS,
    OUTPUT_INPUT_SAME,
    FILE_OUTPUT_NOT_CREAT,
    ONLY_DIR
};

//-d необходимо исключить символы арабских цифр из входного файла;
int put_away_num(const char *, const char *);

//-i для каждой строки входного файла посчитать количество символов букв латинского алфавита
int cnt_latin_letter(const char *, const char *);

//-s для строки посчитать количество символов, отличных от букв латинского алфавита, арабских цифр и пробела;
int cnt_symb(const char *, const char *);

//-a необходимо заменить символы, отличные от цифр, ASCII кодом, записать в 16-ой СИ.
int replace_not_num_to_base16(const char *, const char *);

/*Флаг начинается с символа ‘-’ или ‘/’.
Если флаг содержит в качестве второго символа опциональный символ ‘n’ (то есть
“-nd”, “/nd”, “-ni”, “/ni”, “-ns”, “/ns”, “-na”, “/na”), то путь к выходному файлу является
третьим аргументом командной строки; иначе имя выходного файла генерируется
приписыванием к имени входного файла префикса “out_”.
*/
int creat_file_name(char *, char **);

int valid(int argc, char *argv[], char **pt_fl_in, char **pt_fl_out);

int main(int argc, char *argv[])
{
    char *pt_fl_in = NULL, *pt_fl_out = NULL;
    enum err file_error;
    switch (valid(argc, argv, &pt_fl_in, &pt_fl_out))
    {
    case OK:
        printf("%s-входной файл\n%s-выходной файл\n", pt_fl_in, pt_fl_out);
        switch (argv[1][strlen(argv[1]) - 1])
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
            free(pt_fl_out);
        }
        if (file_error == FILE_INPUT_NOT_OPEN)
        {
            printf("Входной файл не открылся или был не найден\n");
        }
        if (file_error == FILE_OUTPUT_NOT_OPEN)
        {
            printf("Выходной файл не открылся, либо не удалось сосздать\n");
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

    case OUTPUT_INPUT_SAME:
        printf("Относительные имена файлов совпадают\n");
        break;
    case FILE_OUTPUT_NOT_CREAT:
        printf("Не удалось выделить память для входного файла\n");
        break;
    case ONLY_DIR:
        printf("Один из аргументов явлется директорией, ожидалось имя файла\n");
        break;
    }
    return 0;
}

void find_file_name(char *begin_file_path, char **pt_begin_fl_name)
{
    *pt_begin_fl_name = begin_file_path + strlen(begin_file_path);
    while ((begin_file_path != *pt_begin_fl_name) && (*(--(*pt_begin_fl_name)) != '/'))
    {
    }
    if (**pt_begin_fl_name == '/')
    {
        ++(*pt_begin_fl_name);
    }
    return;
}

int creat_file_name(char *pt_fl_in, char **pt_fl_out)
{
    char *fl_name_start = NULL;
    if (strlen("out_") + strlen(pt_fl_in) > 255)
        return TOO_LONG_FILE_NAME;

    find_file_name(pt_fl_in, &fl_name_start);
    char *p = *pt_fl_out;
    strncpy(p, pt_fl_in, fl_name_start - pt_fl_in);
    p += fl_name_start - pt_fl_in;
    strcpy(p, "out_");
    p += strlen("out_");
    strcpy(p, fl_name_start);
    return 0;
}

int valid(int argc, char *argv[], char **pt_fl_in, char **pt_fl_out)
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
            return FEW_ARGUMENTS;
        *(pt_fl_in) = argv[3];
        *pt_fl_out = argv[2];

        char *pt_fl_name_out = NULL, *pt_fl_name_in = NULL;
        find_file_name(argv[2], &pt_fl_name_out);
        find_file_name(argv[3], &pt_fl_name_in);
        // printf("\n%s-file name out \n %s - file name\n", pt_fl_name_out, pt_fl_name_in);
        if (strcmp(pt_fl_name_in, pt_fl_name_out) == 0)
        {
            return OUTPUT_INPUT_SAME;
        }

        //????? не понимаю каким аргументом по счету подается входной файл
        return OK;
    }
    *(pt_fl_in) = argv[2];
    if (*(argv[2] + strlen(argv[2]) - 1) == '/')
    {
        return ONLY_DIR;
    }
    if (!(*pt_fl_out = (char *)malloc(sizeof(char) * 256)))
    {
        return FILE_OUTPUT_NOT_CREAT;
    }
    return creat_file_name(*pt_fl_in, pt_fl_out);
}

int put_away_num(const char *f_in, const char *f_out)
{
    char c;
    FILE *fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE *fout = fopen(f_out, "w+");
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

int cnt_latin_letter(const char *f_in, const char *f_out)
{
    FILE *fin = fopen(f_in, "r");
    FILE *fout = fopen(f_out, "w+");
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
    while (fgets(buf, 256, fin) != NULL)
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
enum ASCII_name
{
    Vertical_tab = 11,
    Horisontal_tab = 9,
    Line_feed = 10,
    Carrige_return = 13,
    Space = 32
};
*/
// s
int cnt_symb(const char *f_in, const char *f_out)
{
    FILE *fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE *fout = fopen(f_out, "w+");
    if (!fout)
    {
        fclose(fin);
        return FILE_OUTPUT_NOT_OPEN;
    }
    char buf[256];
    while (fgets(buf, 256, fin) != NULL)
    {
        int cnt = 0;
        // printf("текущая строка: %s\n", buf);
        for (int i = 0; i < strlen(buf); ++i)
        { // Считаются также символы переноса строки, табуляции, переноса каретки и другие
            if (!(isalnum(buf[i]) || (buf[i] == ' ')))
            {
                ++cnt;
            }
            // printf("%d\n", (buf[i]));
        }
        fprintf(fout, "%d\n", cnt);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
// a
int replace_not_num_to_base16(const char *f_in, const char *f_out)
{
    FILE *fin = fopen(f_in, "r");
    if (fin == NULL)
    {
        return FILE_INPUT_NOT_OPEN;
    }
    FILE *fout = fopen(f_out, "w+");
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