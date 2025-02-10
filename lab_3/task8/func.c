/*
A) Реализуйте приложение для сбора статистических данных по заданному тексту.
Результатом работы программы является информация о том, сколько раз каждое слово
из файла встречается в данном файле (путь к файлу - первый аргумент командной
строки). Слова в файле разделяются сепараторами (каждый сепаратор - символ),
которые подаются как второй и последующие аргументы командной строки. В
интерактивном диалоге с пользователем реализуйте выполнение дополнительных
опций: вывод информации о том сколько раз заданное слово встречалось в файле (ввод
слова реализуйте с консоли); вывод первых n наиболее часто встречающихся слов в
файле (значение n вводится с консоли); поиск и вывод в контексте вызывающего кода в
консоль самого длинного и самого короткого слова (если таковых несколько,
необходимо вывести в консоль любое из них). Размещение информации о считанных
словах реализуйте посредством двоичного дерева поиска.
B) Для построенного дерева в пункте А реализуйте и продемонстрируйте работу
функции поиска глубины данного дерева.
C) Для построенного дерева в пункте A реализуйте функции сохранения построенного
дерева в файл и восстановления бинарного дерева из файла. При этом восстановленное
дерево должно иметь точно такую же структуру и вид, как и до сохранения.
Продемонстрируйте работу реализованных функций.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum traverce_rule
{
    prefix,
    postfics,
    infixs
} traverce_rule;

enum err
{
    OK,
    MEMMORY_ERROR,
    FILE_INPUT_NOT_OPEN,
    OVERFLOW_INT,
    WRONG_POINTER
};

typedef struct Node
{
    char *data;
    int key;
    struct Node *pleft;
    struct Node *pright;
} Node;

typedef struct Tree
{
    int (*cmp)(const void *, const void *);
    Node *root;
} Tree;

int check_capacity_and_realloc(char **buf, int length, int *capacity)
{
    char *new_buf = NULL;
    if ((buf == NULL) || (capacity == NULL))
    {
        return WRONG_POINTER;
    }
    if (length >= *capacity)
    {
        if (*capacity << 1 <= 0)
        {
            return OVERFLOW_INT;
        }
        *capacity <<= 1;
        if (!(new_buf = (char *)realloc(*buf, sizeof(char) * (*capacity))))
        {
            return MEMMORY_ERROR;
        }
        *buf = new_buf;
        new_buf = NULL;
    }
    return OK;
}

/*
void sum(tkey k, tvalue v, size_t deep)
{
    printf("%d ; deep %d\n", k + v, deep);
}

void traverse_with_action_node(Node *root, traverce_rule rule, void (*action)(tkey, tvalue, size_t), int deep)
{
    if (root == NULL)
    {
        return;
    }
    deep += 1;
    switch (rule)
    {
    case postfics:
        traverse_with_action_node(root->pleft, rule, action, deep);
        traverse_with_action_node(root->pright, rule, action, deep);
        action(root->key, root->data, deep);
        break;
    case prefix:
        action(root->key, root->data, deep);
        traverse_with_action_node(root->pleft, rule, action, deep);
        traverse_with_action_node(root->pright, rule, action, deep);

        break;
    case infixs:
        traverse_with_action_node(root->pleft, rule, action, deep);
        action(root->key, root->data, deep);
        traverse_with_action_node(root->pright, rule, action, deep);

        break;
    }
}

void traverse_with_action(Tree *tree, traverce_rule rule, void (*action)(tkey, tvalue, size_t))
{
    if (tree == NULL)
    {
        return;
    }
    traverse_with_action_node(tree->root, rule, action, -1);
    return;
}
*/

int creat_node(Node **new_n, int length, char *buf)
{
    if (!(*new_n = (Node *)malloc(sizeof(Node))))
    {
        return MEMMORY_ERROR;
    }
    if (!((*new_n)->data = (char *)malloc(sizeof(char) * (length))))
    {
        free(*new_n);
        return MEMMORY_ERROR;
    }
    (*new_n)->key = 1;
    (*new_n)->pleft = NULL;
    (*new_n)->pright = NULL;
    strcpy((*new_n)->data, buf);
    return 0;
}

void print(const Node *root, int cnt)
{
    if (root == NULL)
    {
        cnt = cnt + 1;
        return;
    }
    cnt = cnt + 1;
    print(root->pright, cnt);
    for (int i = 0; i < cnt; ++i)
    {
        printf("         ");
    }
    printf("(%d) %s\n", root->key, root->data);
    print((root)->pleft, cnt);
    cnt -= 1;
}

int search(Node *root, char *data, Node **res)
{
    int cmp = strcmp(data, root->data);
    while (root != NULL)
    {
        if (cmp == 0)
        {
            break;
        }
        if (cmp < 0)
            root = root->pleft;
        else
            root = root->pright;
    }
    *res = root;
    return 0;
}

int insert_w(Node **root, int length, char *buf)
{
    Node *ptr = *root;
    int cmp = 0;
    if (*root == NULL)
    {
        return creat_node(root, length, buf);
    }
    while (ptr != NULL)
    {
        cmp = strcmp(ptr->data, buf);
        if (cmp == 0)
        {
            ptr->key += 1;
        }
        if (cmp >= 0)
        {
            if (ptr->pleft == NULL)
            {
                return creat_node(&(ptr->pleft), length, buf);
            }
            ptr = ptr->pleft;
        }
        else
        {
            if (ptr->pright == NULL)
            {
                return creat_node(&(ptr->pright), length, buf);
            }
            ptr = ptr->pright;
        }
    }
}
// делаем дерево компаратор по строкам
// если встречаем новую лексему ищем по дереву

// первые n самых встречающихся слов, просто запихнуть все слова в массив и
// отсортировать по cnt

void clear(Node **root)
{
    if (*root == NULL)
        return;
    free((*root)->data);
    clear(&((*root)->pleft));
    clear(&((*root)->pright));
    free(*root);
}

int cmp1(const void *k1, const void *k2)
{
    return (int *)k1 - (int *)k2;
}

int read_file(char *file_in)
{
    int cnt = 0;
    char *buf = NULL;
    char c = ' ', _c = ' ';
    int buf_capasity = 6;
    int buf_length = 0;
    Node *root = NULL;
    FILE *fin = NULL;
    enum err mistake = 0;

    if (!(fin = fopen(file_in, "r")))
    {
        return FILE_INPUT_NOT_OPEN;
    }

    if (!(buf = (char *)malloc(sizeof(char) * buf_capasity)))
    {
        fclose(fin);
        return MEMMORY_ERROR;
    }

    while (!feof(fin))
    {
        c = fgetc(fin);
        if ((!isspace(_c)) && (isspace(c) || c == EOF))
        {
            buf[buf_length++] = '\0';
            if (mistake = insert_w(&root, buf_length, buf))
            {
                fclose(fin);
                free(buf);
                clear(&root);
                return mistake;
            }
            buf_length = 0;
        }
        else
        {
            if (!isspace(c))
            {
                buf[buf_length++] = c;
                if (mistake = check_capacity_and_realloc(&buf, buf_length, &buf_capasity))
                {
                    fclose(fin);
                    free(buf);
                    clear(&root);
                    return mistake;
                }
            }
        }
        _c = c;
    }

    print(root, 0);

    clear(&root);
    fclose(fin);
    free(buf);

    return 0;
}

int main()
{
    printf("%d\n", read_file("test1.txt"));

    return 0;
}
/*
int main()
{
    Tree t = {cmp1, NULL};
    Node *root = NULL;

    insert(&root, 3);
    insert(&root, 10);
    print(root, 0);
    printf("\n");

    insert(&root, 2);
    insert(&root, -7);
    insert(&root, 15);
    insert(&root, 9);
    insert(&root, -1);

    t.root = root;
    print(root, 0);
    traverse_with_action(&t, infixs, sum);

    clear(&root);
    return 0;
}
*/