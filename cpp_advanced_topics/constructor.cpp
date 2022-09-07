#include <cstdio>
#include <string>

// static strings
const std::string unk = "unknown";
const std::string clone_prefix = "clone-";

// object interface
class Animal {
    std::string _type = "";
    std::string _name = "";
    std::string _sound = "";

   public:
    Animal();  // default constructor
    Animal(const std::string &type, const std::string &name, const std::string &sound);
    Animal(const Animal &);             // copy constructor
    Animal &operator=(const Animal &);  // copy operator
    ~Animal();                          // destructor

    void print() const;
};

// this is definition of the default constructor (explicit)
Animal::Animal() : _type(unk), _name(unk), _sound(unk) {
    puts("default constructor");
}

// constructor with three arguments
// given through initialization list
Animal::Animal(const std::string &type, const std::string &name, const std::string &sound)
    : _type(type), _name(name), _sound(sound) {
    puts("constructor with arguments");
}

Animal::Animal(const Animal &rhs) {
    puts("copy constructor");
    _name = clone_prefix + rhs._name;
    _type = rhs._type;
    _sound = rhs._sound;
}

// destructor doesn't need to do anything because there is not allocated memory
Animal::~Animal() {
    printf("destructor: %s the %s\n", _name.c_str(), _type.c_str());
}

void Animal::print() const {
    printf("%s the %s says %s\n", _name.c_str(), _type.c_str(), _sound.c_str());
}

Animal &Animal::operator=(const Animal &rhs) {
    puts("copy operator");
    if (this != &rhs) {
        _name = clone_prefix + rhs._name;
        _type = rhs._type;
        _sound = rhs._sound;
    }
    return *this;
}

int main() {
    // default constructor
    Animal a;
    a.print();

    const Animal b("cat", "fluffy", "meow");
    b.print();

    // copy constructor
    const Animal c = b;
    c.print();

    a = c;
    a.print();

    // destructor is call for every object, a, b and c
    // when end of scope is reached

    return 0;
}