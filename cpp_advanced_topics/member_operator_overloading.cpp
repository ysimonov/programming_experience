#include <iostream>

// Overloading operators with member functions

// Simple demonstration class to operate on rational numbers
class Rational {
    int _n = 0;
    int _d = 1;

   public:
    // A constructor with default value (default constructor)
    Rational(int numerator = 0, int denomenator = 1) : _n(numerator), _d(denomenator){};

    // Copy constructor
    // Simply copies the values from another rational object
    Rational(const Rational &rhs) : _n(rhs._n), _d(rhs._d){};
    ~Rational();

    // Getter functions
    int numerator() const {
        return _n;
    };
    int denomenator() const {
        return _d;
    };

    // Operator overloads
    Rational &operator=(const Rational &);

    // Member operators of rational class for arithmetic operations
    // Member overloads are defined as:
    // Class Name - operator keyword - operator symbol -
    // const Class name on the left side of operator -
    // & operand on the right hand side
    // const indicates that functions are const safe, so they
    // return a separate new object, and not the reference to
    // existing object
    Rational operator+(const Rational &) const;
    Rational operator-(const Rational &) const;
    Rational operator*(const Rational &) const;
    Rational operator/(const Rational &) const;
};

// Returns a reference to itself
// This is accomplished by using *this
Rational &Rational::operator=(const Rational &rhs) {
    // check if copying from the same object or not
    // this is the pointer to this object, and
    // &rhs is the reference to the object on rhs
    if (this != &rhs) {
        _n = rhs.numerator();
        _d = rhs.denomenator();
    }
    return *this;
}

Rational Rational::operator+(const Rational &rhs) const {
    return Rational((_n * rhs._d) + (_d * rhs._n), _d * rhs._d);
}

Rational Rational::operator-(const Rational &rhs) const {
    return Rational((_n * rhs._d) - (_d * rhs._n), _d * rhs._d);
}

Rational Rational::operator*(const Rational &rhs) const {
    return Rational(_n * rhs._n, _d * rhs._d);
}

Rational Rational::operator/(const Rational &rhs) const {
    return Rational(_n * rhs._d, _d * rhs._n);
}

Rational::~Rational() {
    _n = 0;
    _d = 1;
}

// overloading the left shift operator
std::ostream &operator<<(std::ostream &o, const Rational &r) {
    if (r.denomenator() == 1) {
        return o << r.numerator();
    } else {
        return o << r.numerator() << '/' << r.denomenator();
    }
}

int main() {
    // constructing with just a numerator
    Rational a = 7;  // 7 / 1
    std::cout << "a is: " << a << std::endl;

    // constructing with a numerator and a denominator
    Rational b(5, 3);  // 5 / 3
    std::cout << "b is: " << b << std::endl;

    // copy constructor
    Rational c = b;
    std::cout << "c is: " << c << std::endl;

    // calling a default constructor
    Rational d;
    std::cout << "d is: " << d << std::endl;

    // using an assignment operator
    d = c;
    std::cout << "d is: " << d << std::endl;

    Rational &e = d;  // reference

    // assignment to self
    d = e;
    std::cout << "e is: " << e << std::endl;

    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << a << " - " << b << " = " << a - b << std::endl;
    std::cout << a << " * " << b << " = " << a * b << std::endl;
    std::cout << a << " / " << b << " = " << a / b << std::endl;

    return 0;
}