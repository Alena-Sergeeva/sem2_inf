/*
3. Реализовать класс logical_values_array. В классе определить и реализовать:
    ● поле_value (типа unsigned int), которое хранит значение логической величины
● accessor для поля value
    ● конструктор, принимающий значение типа unsigned int (равное по умолчанию 0) и
инициализирующий переданным значением поле value
● методы, соответствующие всем стандартным логическим операциям: инверсия, конъюнкция,
дизъюнкция, импликация, коимпликация, сложение по модулю 2, эквивалентность, стрелка Пирса,
штрих Шеффера. Примечание: если одну операцию возможно выразить через другую, то
необходимо реализовывать одно через другое (например, эквивалентность можно реализовать
через сложение по модулю 2 и инверсию)
● статический метод equals, сравнивающий два значения типа logical_values_array по отношению
эквивалентности
● метод get_bit, который возвращает значение бита по его позиции (является параметром)

?????????????Как выделить память под char * ? как понять что я точно влезу

● метод, принимающий значение типа char *; по значению адреса в параметре должно быть записано
двоичное представление поля value в виде строки в стиле языка программирования C. Примечание:
конвертация должна быть основана на использовании битовых операций.
Продемонстрируйте работу реализованного функционала.
*/
#include <iostream>

enum err
{
    OK,
    WRONG_POINTER
};

int reverse(char *str, unsigned int length)
{
    if (str == NULL)
    {
        return WRONG_POINTER;
    }
    char c = '0';
    for (int j = 0; j < length / 2; ++j)
    {
        c = str[j];
        str[j] = str[length - j];
        str[length - j] = c;
    }
    return OK;
}

class logical_values_array
{
    unsigned int _value;

public:
    logical_values_array(unsigned int x = 0) : _value(x)
    {
    }
    ~logical_values_array()
    {
    }
    unsigned int assesor() const
    {
        return _value;
    }
    void inversion()
    {
        _value = ~_value;
    }
    static logical_values_array conjunction(logical_values_array const &p1, logical_values_array const &p2)
    {
        return p1._value & p2._value;
    }
    static logical_values_array disjunction(logical_values_array const &p1, logical_values_array const &p2)
    {
        return p1._value | p2._value;
    }
    // что надо возвращать в методе ?

    static logical_values_array implication(logical_values_array const &p1, logical_values_array const &p2)
    {
        return (~p1._value) | (p2._value);
    }
    static logical_values_array xor_(logical_values_array const &p1, logical_values_array const &p2)
    {
        return (p1._value | p2._value) & ((~p1._value) | (~p2._value));
    }
    static logical_values_array equivalence(logical_values_array const &p1, logical_values_array const &p2)
    {
        return (p1._value & p2._value) | ((~p1._value) & (~p2._value));
    }
    // NOT(x OR y) стрелка Пирса
    static logical_values_array nor(logical_values_array const &p1, logical_values_array const &p2)
    {
        return (~p1._value) & (~p2._value);
    }
    // NOT(x AND y) NAND штрих Шифера
    static logical_values_array nand(logical_values_array const &p1, logical_values_array const &p2)
    {
        return (~p1._value) | (~p2._value);
    }
    // левая компликация
    static logical_values_array complication(logical_values_array const &p1, logical_values_array const &p2)
    {
        return p1._value & (~p2._value);
    }
    // Считаем что пользователь нумерует биты с 0
    unsigned int get_bit(unsigned int i)
    {
        return _value & 1 << i;
    }

    int convert_U_int_to_str(char *str)
    {
        if (str == NULL)
        {
            return WRONG_POINTER;
        }
        unsigned int x = _value;
        int i = 0;
        if (x == 0)
        {
            *str = '0';
            *(str + 1) = '\0';
            return OK;
        }
        while (x > 0)
        {
            str[i] = '0';
            str[i] += x & 1;
            x >>= 1;
            ++i;
        }
        str[i] = '\0';
        if (reverse(str, i - 1))
        {
            return WRONG_POINTER;
        }
        return OK;
    }

    static bool eqv(logical_values_array const &p1, logical_values_array const &p2)
    {
        return p1._value == p2._value;
    }
};

int main()
{
    logical_values_array p1 = logical_values_array(4);
    logical_values_array p2 = logical_values_array(7);
    char *str = new char[sizeof(unsigned int) << 3 + 1];
    std::cout << p1.assesor() << " " << p2.assesor() << std::endl;
    std::cout << logical_values_array::conjunction(p1, p2).assesor() << " conjunction\n";
    std::cout << logical_values_array::disjunction(p1, p2).assesor() << " disjunction\n";
    std::cout << logical_values_array::xor_(p1, p2).assesor() << " xor\n";
    std::cout << logical_values_array::equivalence(p1, p2).assesor() << " equivalence\n";
    std::cout << logical_values_array::implication(p1, p2).assesor() << " implication\n";
    // p1.inversion();
    std::cout << p1.assesor() << " inversion\n";
    std::cout << logical_values_array::eqv(p1, p2) << " eqv\n";
    if (p1.convert_U_int_to_str(str))
    {
        std::cout << "Передан нулевой указатель\n";
    }
    else
    {
        std::cout << str << " " << p1.assesor() << " string \n";
    }
    if (p2.convert_U_int_to_str(str))
    {
        std::cout << "Передан нулевой указатель\n";
    }
    else
    {
        std::cout << str << " " << p2.assesor() << " invertion string\n";
    }
    p2.inversion();
    if (p2.convert_U_int_to_str(str))
    {
        std::cout << "Передан нулевой указатель\n";
    }
    else
    {
        std::cout << str << " " << p2.assesor() << " invertion string\n";
    }
    delete[] str;
    return 0;
}