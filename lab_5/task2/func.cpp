#include <fstream>
#include <iostream>
#include <vector>
#include <cstddef>
#include <string>

#define N 256

class encoder
{
    std::vector<std::byte> _key;
    static void swap(std::vector<std::byte> &S, const unsigned int &i, const unsigned int &j)
    {
        std::byte temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }
    void _init(std::vector<std::byte> &S) const
    {
        int key_length = _key.size();
        if (key_length == 0)
        {
            throw std::runtime_error("Длина ключа должна быть больше 0\n");
        }
        for (unsigned int i = 0; i < N; ++i)
        {
            S.push_back(static_cast<std::byte>(i));
        }
        unsigned int j = 0;
        std::byte temp;
        for (unsigned int i = 0; i < N; ++i)
        {
            j = (j + static_cast<unsigned int>(S[i]) + static_cast<unsigned int>(_key[i % key_length])) % N;
            swap(S, i, j);
        }
    }
    std::byte pseudo_random_generation_algorithm(std::vector<std::byte> &S) const
    {
        unsigned int x = 0, y = 0;
        x = (x + 1) % N;
        y = (y + static_cast<unsigned int>(S[x])) % N;
        swap(S, x, y);
        unsigned int t = (static_cast<unsigned int>(S[x]) + static_cast<unsigned int>(S[y])) % N;
        return S[t];
    }

public:
    // Конструктор, принимающий ключ шифрования (массив байтов типа std::vector<std::byte>)
    encoder(std::vector<std::byte> key)
    {
        _key = key;
    }
    ~encoder() = default;
    void encode(std::string file_in, std::string file_out, bool need_deciph)
    {
        if (file_in.compare(file_out) == 0)
        {
            throw std::runtime_error("Имена файлов одинаковые\n");
        }
        std::ifstream f_in(file_in, std::ios_base::in);
        if (!f_in.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для чтения\n");
        }
        std::ofstream f_out(file_out, std::ios_base::out);
        std::vector<std::byte> S;
        this->_init(S);
        std::byte c{0};
        while (f_in.read((char *)&c, sizeof(std::byte)))
        {
            c ^= this->pseudo_random_generation_algorithm(S);
            f_out.write((char *)&c, sizeof(std::byte));
        }

        need_deciph == 1 ? std::cout << "Файл декодирован\n" : std::cout << "Файл закодирован\n";
        f_out.close();
        f_in.close();
    }

    void mutator(const std::vector<std::byte> &key)
    {
        if (key.size() == 0)
        {
            throw std::runtime_error("Длина ключа должна быть больше 0\n");
        }
        _key = key;
    }
};

int main()
{
    std::vector<std::byte> a = {std::byte(31), std::byte(101), std::byte(112), std::byte(101)};
    std::vector<std::byte> b = {};
    encoder first(a);
    encoder second(b);
    try
    {
        first.encode("/home/ali_/sem_2_inf/sem2_inf/lab_5/task2/images.jpg", "text1.bin", 0);
        // second.encode("text2.bin", "text3.bin", 0);
        first.encode("text1.bin", "test3.jpg", 1);
    }
    catch (const std::runtime_error &e)
    {
        std::cout << e.what();
    }

    return 0;
}
/*
sdhsuhfow djfl 82hdns
ksjsiw2mms? kdk
    kiks
*/
