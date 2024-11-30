/*
Реализуйте класс binary_int, который позволяет выполнять операции с целыми числами, используя только
побитовые операции и логические операции для выполнения арифметических задач. Класс должен
работать с числами в диапазоне значений типа int и представлять их только в виде двоичных значений.
Требования к реализации:
● Запрещено использовать стандартные арифметические операторы +, -, *, /, % для выполнения
сложения и вычитания. Все операции реализуются побитовыми манипуляциями с числовыми
значениями;

● Необходимо реализовать
*/
#include <iostream>
#include <tuple>

class binary_int
{
    int _b;

public:
    int get_b() const
    {
        return _b;
    }
    binary_int(int value) : _b(value)
    {
    }
    ~binary_int()
    {
    }
    binary_int &operator=(const binary_int &value)
    {
        _b = value._b;
        return *this;
    }

    binary_int &operator+=(const binary_int &num1)
    {
        int ost = num1._b;
        int new_ost = 0;
        while (ost != 0)
        {
            new_ost = (ost & _b);
            _b ^= ost;
            ost = new_ost << 1;
        }
        return *this;
    }
    binary_int operator+(const binary_int &num1) const
    {
        binary_int res = 0;
        res += num1;
        res += (*this);
        return res;
    }
    binary_int &operator++()
    {
        return (*this) += 1;
    }
    binary_int operator++(int)
    {
        binary_int new_num(*this);
        (*this) += 1;
        return new_num;
    }

    binary_int &operator-()
    {
        _b = ~_b;
        return ++(*this);
    }

    // не меняет операнд
    binary_int operator-() const
    {
        binary_int res(~_b);
        return res + 1;
    }
    binary_int operator-(const binary_int &num1) const
    {

        return (*this) + (-num1);
    }
    binary_int &operator-=(const binary_int &num1)
    {
        return (*this) += (-num1);
    }
    binary_int &operator--()
    {
        return (*this) -= 1;
    }

    binary_int operator--(int)
    {
        binary_int num(*this);
        (*this) -= 1;
        return num;
    }

    binary_int &operator*=(const binary_int &num1)
    {
        binary_int i = 1;
        binary_int x = (*this);
        if ((_b == 1) || (_b == 0))
        {
            return (*this);
        }
        while (i._b < num1._b)
        {
            (*this) += x;
            ++i;
        }
        return (*this);
    }
    binary_int operator*(const binary_int &num1) const
    {
        binary_int num2 = *this;
        return num2 *= num1;
    }

    binary_int &operator<<=(const binary_int &num1)
    {
        _b <<= num1._b;
        return *this;
    }
    binary_int &operator>>=(const binary_int &num1)
    {
        _b >>= num1._b;
        return *this;
    }
    binary_int operator<<(const binary_int &num1) const
    {
        return _b << num1._b;
    }
    binary_int operator>>(const binary_int &num1) const
    {
        return _b >> num1._b;
    }
    /* метод, возвращающий для объекта пару объектов типа binary_int, где в
первом объекте сохранены только половина старших битов вызывающего объекта, а во втором -
только половина младших битов вызывающего объекта; остальные биты необходимо обнулить.
*/
    std::tuple<binary_int, binary_int> half_bit() const
    {
        binary_int mask(1);
        mask <<= (sizeof(int) << 2);
        mask -= 1;
        int x = mask._b;
        while (x > 0)
        {
            std::cout << (x & 1) << ' ';
            x >>= 1;
        }
        return {_b & (~(mask._b)), _b & mask._b};
    }
};
// подумать что делать с передачей по ссылке в других заданиях сделать throw, подумать над преполнением
std::ostream &operator<<(std::ostream &out, const binary_int &num)
{
    std::cout << num.get_b();
    return out;
}

int main()
{
    binary_int num1(17);
    binary_int num2(7);
    binary_int num3 = num1 + num2;
    std::cout << "17 + 7 " << num3 << '\n';
    // num3++;
    std::cout << "++num1 " << (++num1) << '\n';
    std::cout << "-num3 " << (-num3) << '\n';
    num1 = 17;
    num2 = 7;
    num3 = num2 - num1;
    std::cout << "17- 7 " << num3.get_b() << '\n';
    --num3;
    std::cout << "--num3 " << (num3) << '\n';

    std::cout << "num3-- " << (num3--) << '\n';
    std::cout << "num3 " << (num3) << '\n';
    std::cout << "num3 " << (num3) << '\n';
    num3 = 1212;
    std::cout << "num3 " << num3 << '\n';
    num3 *= 2345;
    std::cout << "num3 *= 2345 " << num3 << '\n';
    std::tie(num1, num2) = num3.half_bit();
    std::cout << num1 << ' ' << num2 << std::endl;
    int x = num3.get_b();
    while (x > 0)
    {
        std::cout << (x & 1) << ' ';
        x >>= 1;
    }
    num3 <<= 1;
    std::cout << "num3 <<= 1 " << num3 << '\n';
    num1 = num3 << 3;
    std::cout << "num3 << 3 " << num1 << '\n';
    return 0;
}