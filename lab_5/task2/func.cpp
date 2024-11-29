#include <fstream>
#include <iostream>
#include <vector>
#include <cstddef>

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
        // добавить проверку на вектор нулевой длины
        int key_length = _key.size();
        if (key_length == 0)
        {
            throw std::runtime_error("Длина ключа должна быть больше 0\n");
        }
        for (unsigned int i = 0; i <= N; ++i)
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
        // кинуть исключение
        std::ifstream f_in(file_in, std::ios_base::binary | std::ios_base::in);
        if (!f_in.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для чтения\n");
        }
        std::ofstream f_out(file_out, std::ios_base::binary | std::ios_base::out);
        /* Файл сосздается лишняя провека
        if (!f_out.is_open())
        {
            // f_in.close();
            throw std::runtime_error("Не удалось открыть файл для записи\n");
        }
        */
        std::vector<std::byte> S;
        this->_init(S);
        unsigned char c;
        while ((c = f_in.get()) && (!f_in.eof()))
        {
            std::cout << (unsigned char)c << ' ';
            c = static_cast<unsigned char>(static_cast<std::byte>(c) ^ this->pseudo_random_generation_algorithm(S));
            f_out.put(c);
        }

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
        first.encode("text2.bin", "text12.bin", 0);
        // second.encode("text2.bin", "text3.bin", 0);
        first.encode("text3.bin", "text4.bin", 0);
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
