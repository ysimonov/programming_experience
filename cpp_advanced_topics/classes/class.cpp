#include <cstdio>

// by convention class definition is placed in a header file
// class methods are placed in .cpp file

class c1
{
    // every member of a class is private by default
    // this is the principle of data encapsulation
    int i = 0;

// these are public methods
// available from object interface
public:
    void set_value( const int & value );

    // const qualifier indicates that this 
    // method is const safe, it can be used in const context
    int get_value() const;
};

// :: is a scope resolution operator
// this means that function set_value is a member of a class c1
void c1::set_value( const int & value)
{
    i = value;
}

int c1::get_value() const
{
    return i;
}

int main()
{
    const int i = 47;
    c1 o1;
    o1.set_value(i);
    printf("value is %d\n", o1.get_value());
    return 0;
}