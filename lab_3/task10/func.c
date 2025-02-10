#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum err
{
    OK = 0,
    WRONG_POINTER,
    OVERFLOW_INT,
    SAME_NAME,
    MEMMORY_ERROR,
    FILE_END,
    INPUT_FILE_NOT_OPEN,
    OUTPUT_FILE_NOT_OPEN
};

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

int print_tree(FILE *fin, FILE *fout)
{
    if ((fin == NULL) || (fout == NULL))
    {
        return WRONG_POINTER;
    }
    char c = fgetc(fin);
    if (c == '\n' || c == EOF)
        fprintf(fout, "|_\n");

    int count = 0;
    while (c != '\n' && c != EOF)
    {
        if (c == '(')
        {
            count++;
        }
        else
        {
            if (c == ')')
            {
                count--;
            }
            else
            {
                if (c == ' ' || c == ',')
                {
                    c = fgetc(fin);
                    continue;
                }
                else
                {
                    for (int i = 0; i < count * 3; ++i)
                    {
                        fprintf(fout, " ");
                    }
                    fprintf(fout, "|_%c\n", c);
                }
            }
        }
        c = fgetc(fin);
    }
    return OK;
}

int main(int argc, char *argv[])
{
    enum err mistake = 0;
    FILE *fin = NULL;
    FILE *fout = NULL;
    int count = 0;
    if (argc != 3)
    {
        printf("Не верное колличество аргументов командной строки\n");
        return 0;
    }
    if (mistake = check_same_file_name(argv[1], argv[2]))
    {
        if (mistake == WRONG_POINTER)
        {
            printf("Передан нулевой указатель\n");
        }
        if (mistake == SAME_NAME)
        {
            printf("Одинаковые имена файлов\n");
        }
        return 0;
    }

    if (!(fin = fopen(argv[1], "r")))
    {
        printf("Не удалось открыть входной файл\n");
        return 0;
    }
    if (!(fout = fopen(argv[2], "w")))
    {
        fclose(fin);
        printf("Не удалось открыть выходной файл\n");
        return 0;
    }

    while (!feof(fin))
    {
        if (print_tree(fin, fout))
        {
            printf("Передан нулевой указатель\n");
        }
        fprintf(fout, "\n");
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
