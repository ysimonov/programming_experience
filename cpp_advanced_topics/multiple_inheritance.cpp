#include <iostream>
#include <string>

// Fur class
class Fur
{
    std::string _quality;
    Fur(){};

  protected:
    Fur(const std::string &f) : _quality(f){};

  public:
    const std::string &quality() const
    {
        return _quality;
    }
};

// this is a base / parent / super class
class Animal
{

    // these are private members of the Animal class
    std::string _name;
    std::string _type;
    std::string _sound;

    // the private constructor prevents construction of base class
    Animal(){};

  protected:
    // everything within protected scope is accessible by
    // deriven / child / subclass class
    Animal(const std::string &n, const std::string &t, const std::string s) : _name(n), _type(t), _sound(s){};

  public:
    void speak() const;

    // getter functions to access private
    // class variables
    const std::string &name() const
    {
        return _name;
    };

    const std::string &type() const
    {
        return _type;
    }

    const std::string &sound() const
    {
        return _sound;
    }
};

void Animal::speak() const
{
    std::cout << _name << " the " << _type << " says " << _sound << std::endl;
}

// Dog class is derived from Animal class
class Dog : public Animal
{
    int _walked;

  public:
    // the Dog constructor also calls Animal constructor and passes
    // Animal's name, type and sound as constructor arguments
    // it also sets the private variable _walked to 0.

    Dog(std::string n) : Animal(n, "dog", "woof"), _walked(0){};

    int walk()
    {
        return ++_walked;
    }
};

// Cat class is derived from Animal class
// the Cat class inherits from both Animal and Fur classes
// therefore, constructors are call for each parent class
class Cat : public Animal, public Fur
{
    int _petted;

  public:
    Cat(std::string n) : Animal(n, "cat", "meow"), Fur("silky"), _petted(0){};

    int pet()
    {
        return ++_petted;
    }
};

// Pig class is derived from Animal class
class Pig : public Animal
{
    int _fed;

  public:
    Pig(std::string n) : Animal(n, "pig", "oink"), _fed(0){};

    int feed()
    {
        return ++_fed;
    }
};

int main()
{
    Dog d("Rover");
    Cat c("Fluffy");
    Pig p("Arnold");

    d.speak();
    c.speak();
    p.speak();

    std::cout << "The " << d.type() << " has been walked " << d.walk() << " times " << std::endl;
    std::cout << "The " << c.type() << " has been petted " << c.pet() << " times " << std::endl;
    std::cout << "The " << p.type() << " has been fed " << p.feed() << " times " << std::endl;

    std::cout << "The " << c.type() << " grooms her " << c.quality() << " fur " << std::endl;

    return 0;
}