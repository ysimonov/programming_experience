#include <iostream>

class Number
{
    int value = 0;

  public:
    // constructor
    Number(int x = 0) : value(x){};

    // getter function
    // writing inside of const function
    // will generate a compile error (prohibited)
    // no modification to member variables is possible
    // only mutable variables can be written into inside
    // of a const function
    int get_value() const
    {
        return value;
    }

    // setter function
    void set_value(int x)
    {
        value = x;
    }

    // unary operators
    // this is the prefix version of the unary increment operator
    Number &operator++();

    // this is the postfix version of th unary increment operator
    // the dummy type is always int, used to distingush between prefix and postfix
    Number operator++(int);

    Number &operator--();
    Number operator--(int);
};

// pre-increment
// changes the value of the value's reference
Number &Number::operator++()
{
    std::cout << "pre-increment: ";
    value += 1;

    // pointer dereference
    return *this;
}

// post-increment
Number Number::operator++(int)
{
    std::cout << "post-increment: ";
    Number temp = *this;
    value += 1;
    return temp;
}

// pre-decrement
Number &Number::operator--()
{
    std::cout << "pre-decrement: ";
    value -= 1;

    // pointer dereference
    return *this;
}

// post-decrement
Number Number::operator--(int)
{
    std::cout << "post-decrement: ";
    Number temp = *this;
    value -= 1;
    return temp;
}

std::ostream &operator<<(std::ostream &o, const Number &n)
{
    return o << (n.get_value());
}

int main()
{
    Number n(42);
    std::cout << "n = " << n << std::endl;
    std::cout << "++n = " << ++n << std::endl;
    std::cout << "n++ = " << n++ << std::endl;
    std::cout << "n = " << n << std::endl;

    return 0;
}