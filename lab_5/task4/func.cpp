/*
 Реализовать класс комплексного числа. В классе определить и реализовать:
● поля, соответствующие действительной и мнимой части комплексного числа (типа double)
● конструктор, который принимает значения действительной и мнимой части (оба параметра по
умолчанию равны 0)
● методы, производящие операции сложения, вычитания, умножения и деления комплексных чисел
● метод, возвращающий модуль комплексного числа
● метод, возвращающий аргумент комплексного числа
Продемонстрируйте работу реализованного функционала.
*/
#include <iostream>
#include <cmath>

#define EPS 1e-9

class complex_num
{
    double _real_prt;
    double _im_prt;
    void check_double_over_flow() const
    {
        if ((std::isinf(_real_prt)) || (std::isinf(_im_prt)))
        {
            throw "переполнение double\n";
        }
    }

public:
    complex_num(double real_prt = 0, double im_prt = 0) : _real_prt(real_prt), _im_prt(im_prt)
    {
    }
    ~complex_num()
    {
    }
    complex_num &operator=(complex_num const &num)
    {
        _real_prt = num._real_prt;
        _im_prt = num._im_prt;
        return *this;
    }
    static complex_num sum(complex_num const &num1, complex_num const &num2)
    {
        complex_num num;
        num._real_prt = num1._real_prt + num2._real_prt;
        num._im_prt = num1._im_prt + num2._im_prt;
        return num;
    }
    static complex_num subtr(complex_num const &num1, complex_num const &num2)
    {
        complex_num num;
        num._real_prt = num1._real_prt - num2._real_prt;
        num._im_prt = num1._im_prt - num2._im_prt;
        return num;
    }
    static complex_num mult(complex_num const &num1, complex_num const &num2)
    {
        complex_num num;
        num._real_prt = num1._real_prt * num2._real_prt - num1._im_prt * num2._im_prt;
        num._im_prt = num1._real_prt * num2._real_prt + num1._im_prt * num2._im_prt;
        return num;
    }
    static complex_num division(complex_num const &num1, complex_num const &num2)
    {
        if ((fabs(num2._real_prt) < EPS) && (fabs(num2._im_prt) < EPS))
        {
            throw "Деление на 0\n";
        }
        // знаменатель
        double denominator = num1._im_prt * num1._im_prt + num2._im_prt * num2._im_prt;

            if (fabs(denominator) < EPS)
        {
            throw "потеря точности\n";
        }
        if (std::isinf(denominator))
        {
            throw "переполение double\n";
        }

        complex_num num;
        num._real_prt = num1._real_prt * num2._real_prt / denominator + num1._im_prt * num2._im_prt / denominator;
        num._im_prt = num1._real_prt * num2._real_prt / denominator - num1._im_prt * num2._im_prt / denominator;
        return num;
    }
    double abs_() const
    {
        if (std::isinf(sqrt(_real_prt * _real_prt + _im_prt * _im_prt)))
        {
            throw "переполение double\n";
        }
        // что делать с потерей точности
        return sqrt(_real_prt * _real_prt + _im_prt * _im_prt);
    }

    double arg() const
    {
        if ((_real_prt == 0) && (_im_prt == 0))
        {
            throw "Аргумент не определен\n";
        }
        if (fabs(_real_prt) < EPS)
        {
            return (_im_prt < 0) ? -M_PI_2 : M_PI_2;
        }
        double argtn = atan(_im_prt / _real_prt);
        if (_real_prt > 0)
        {
            return argtn;
        }
        return (_im_prt < 0) ? -M_PI + argtn : M_PI + argtn;
    }
    void print_complex_num() const
    {
        (*this).check_double_over_flow();
        std::cout << _real_prt << " + " << _im_prt << "i\n";
    }
};

int main()
{
    complex_num a1(1.0, 2.0);
    complex_num a2(1.7E-309, 1);

    complex_num res = complex_num::sum(a1, a2);
    std::cout << "Сумма: ";
    res.print_complex_num();

    res = complex_num::subtr(a1, a2);
    std::cout << "Разность: ";
    res.print_complex_num();

    std::cout << "Деление: ";
    try
    {
        res = complex_num::division(a1, a2);
        res.print_complex_num();
        std::cout << "Умножение: ";
        res = complex_num::mult(a1, a2);
        res.print_complex_num();
        std::cout << "Модуль: ";
        std::cout << a2.abs_() << '\n';
        std::cout << "Аргумент: ";
        std::cout << a2.arg() << '\n';
    }
    catch (const char *err)
    {
        std::cout << err;
    }

    return 0;
}