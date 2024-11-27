#include <iostream>
#include <vector>
#include <fstream>
/*
encoder. В классе определить и реализовать:
● Конструктор, принимающий ключ шифрования (массив байтов типа std::vector<std::byte>)
● Метод encode, который принимает путь ко входному файлу (типа std::string), выходному файлу
(типа std::string) и флаг, отвечающий за то, выполнять шифрование или дешифрование (типа bool) и
выполняет процесс шифрования/дешифрования файла
● mutator для значения ключа Метод, который используется для изменения значения переменной экземпляра.
*/
#define N 128

class encoder
{
    std::vector<std::byte> _S;
    int x;
    int y;
    // void _init(std::vector <std::byte> key)

    void swap(int &i, int &j)
    {
        std::byte temp = _S[i];
        _S[i] = _S[j];
        _S[j] = temp;
    }
    void _init(std::vector<std::byte> key)
    {
        int key_length = key.size();
        for (int i = 0; i < N; ++i)
        {
            _S.push_back((std::byte)(i));
        }
        int j = 0;
        std::byte temp;
        for (int i = 0; i < N; ++i)
        {
            j = (j + (unsigned char)_S[i] + (unsigned char)key[i % key_length]) % N;
            this->swap(i, j);
        }
        for (int i = 0; i < N; ++i)
        {
            std::cout << (char)_S[i] << ' ';
        }
    }
    std::byte pseudo_random_generation_algorithm(char c)
    {
        x = (x + 1) % N;
        y = (y + (unsigned char)_S[x]) % N;
        this->swap(x, y);
        return _S[((unsigned char)(_S[x]) + (unsigned char)_S[y]) % N];
    }

public:
    // Конструктор, принимающий ключ шифрования (массив байтов типа std::vector<std::byte>)
    encoder(std::vector<std::byte> key)
    {
        x = 0;
        y = 0;
        this->_init(key);
    }
    void encode(std::string file_in, std::string file_out, bool need_deciph)
    {
        // кинуть исключение
        std::ifstream f_in(file_in);
        if (!f_in.is_open())
        {
            throw "Не удалось открыть файл для чтения\n";
        }
        std::ofstream f_out(file_out);
        if (!f_in.is_open())
        {
            f_in.close();
            throw "Не удалось открыть файл для записи\n";
        }
        char c = ' ';
        // if (need_deciph == 0)
        //{
        while (f_in.get(c))
        {
            f_out.put(c ^ (char)pseudo_random_generation_algorithm(c));
        }
        //}
        /*   else
           {
               while (f_in.get(c))
               {
                   f_out.put(c ^ (char)pseudo_random_generation_algorithm(c));
               }
           }
   */
        f_out.close();
        f_in.close();
    }
    void mutator(std::vector<std::byte> key)
    {
        this->_init(key);
    }
};

int main()
{
    // std::vector<std::byte> a = {31, 101, 112, 101};
    std::vector<std::byte> a;
    a.push_back((std::byte)31);
    a.push_back((std::byte)37);
    a.push_back((std::byte)82);
    a.push_back((std::byte)66);
    encoder first(a);
    try
    {
        first.encode("text1.txt", "text2.txt", 0);
    }
    catch (const char *err)
    {
        std::cout << err;
    }
    try
    {
        first.encode("text2.txt", "text1.txt", 1);
    }
    catch (const char *err)
    {
        std::cout << err;
    }

    return 0;
}
/*
sdhsuhfow djfl 82hdns
ksjsiw2mms? kdk
    kiks
*/