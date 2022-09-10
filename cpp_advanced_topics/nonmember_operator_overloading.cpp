#include <iostream>
#include <string>

// Overloading operators with nonmember functions

// Simple demonstration class to operate on rational numbers
class Rational {
    int n_ = 0;
    int d_ = 1;

   public:
    // A constructor with default value (default constructor)
    Rational(int numerator = 0, int denomenator = 1)
        : n_(numerator), d_(denomenator){};

    // Copy constructor
    // Simply copies the values from another rational object
    Rational(const Rational &rhs) : n_(rhs.n_), d_(rhs.d_) {
        std::cout << "Copy constructor was called" << std::endl;
    };
    Rational(Rational &&) noexcept;
    ~Rational();

    void reset();

    // Getter functions
    inline int numerator() const { return n_; };
    inline int denomenator() const { return d_; };

    // This handles strings
    operator std::string() const;

    // Copy operator
    Rational &operator=(const Rational &);

    // Move operator
    Rational &operator=(Rational &&) noexcept;
};

// Resets values to the original state
void Rational::reset() {
    n_ = 0;
    d_ = 1;
}

// Move constructor
Rational::Rational(Rational &&rhs) noexcept {
    std::cout << "Move constructor was called" << std::endl;
    n_ = std::move(rhs.n_);
    d_ = std::move(rhs.d_);
    rhs.reset();
}

// Object destructor
Rational::~Rational() { reset(); }

// this is an example of the non-member operator overloading
Rational operator+(const Rational &lhs, const Rational &rhs) {
    return Rational((lhs.numerator() * rhs.denomenator()) +
                        (lhs.denomenator() * rhs.numerator()),
                    lhs.denomenator() * rhs.denomenator());
}

Rational operator-(const Rational &lhs, const Rational &rhs) {
    return Rational((lhs.numerator() * rhs.denomenator()) -
                        (lhs.denomenator() * rhs.numerator()),
                    lhs.denomenator() * rhs.denomenator());
}

Rational operator*(const Rational &lhs, const Rational &rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denomenator() * rhs.denomenator());
}

Rational operator/(const Rational &lhs, const Rational &rhs) {
    return Rational(lhs.numerator() * rhs.denomenator(),
                    lhs.denomenator() * rhs.numerator());
}

Rational::operator std::string() const {
    if (d_ == 1) {
        return std::to_string(n_);
    } else {
        return std::to_string(n_) + "/" + std::to_string(d_);
    }
}

// overloading the left shift operator
std::ostream &operator<<(std::ostream &o, const Rational &r) {
    return o << std::string(r);
}

// Returns a reference to itself
// This is accomplished by using *this
Rational &Rational::operator=(const Rational &rhs) {
    // check if copying from the same object or not
    // this is the pointer to this object, and
    // &rhs is the reference to the object on rhs
    std::cout << "Assignment operator was called " << std::endl;
    if (this != &rhs) {
        n_ = rhs.numerator();
        d_ = rhs.denomenator();
    }
    return *this;
}

// Moves reference from rhs to lhs
Rational &Rational::operator=(Rational &&rhs) noexcept {
    std::cout << "Move assignment operator was called " << std::endl;
    if (this != &rhs) {
        n_ = std::move(rhs.n_);
        d_ = std::move(rhs.d_);
        rhs.reset();
    }
    return *this;
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

    // this is possible with the member operator overloading
    std::cout << a << " + " << b << " = " << a + b << std::endl;
    std::cout << a << " - " << b << " = " << a - b << std::endl;
    std::cout << a << " * " << b << " = " << a * b << std::endl;
    std::cout << a << " / " << b << " = " << a / b << std::endl;

    // this wouldn't be possible with the member operator overloading
    std::cout << 14 << " + " << b << " = " << 14 + b << std::endl;
    std::cout << 14 << " - " << b << " = " << 14 - b << std::endl;
    std::cout << 14 << " * " << b << " = " << 14 * b << std::endl;
    std::cout << 14 << " / " << b << " = " << 14 / b << std::endl;

    // for this part to work we need a conversion operator overload
    std::string s = "Rational value is: ";
    s += b;
    std::cout << s << std::endl;

    // copy
    Rational f = c;

    // move
    Rational g = std::move(c);

    std::cout << "Value of c: " << c << std::endl;
    std::cout << "Value of f after copying c: " << f << std::endl;
    std::cout << "Value of g after moving ref of c into f: " << g << std::endl;
    std::cout << "Value of c after moving ref into g: " << c << std::endl;

    c = std::move(g);

    return 0;
}