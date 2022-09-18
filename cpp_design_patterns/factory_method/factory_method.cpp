
#include <iostream>
#include <memory>
#include <string>

class Coffee {
   protected:
    std::string type_;

   public:
    Coffee() {}

    std::string getType() const { return type_; }
    void setType(const std::string& type) { type_ = type; }
};

class Espresso : public Coffee {
   public:
    Espresso() : Coffee() {
        setType("Espresso");
        std::cout << std::endl << "Making a cup of Espresso" << std::endl;
        std::cout << "Grind and brew one scoop of Espresso beans." << std::endl;
    }
};

class Cappuccino : public Coffee {
   public:
    Cappuccino() : Coffee() {
        setType("Cappucino");
        std::cout << std::endl << "Making a cup of Cappucino" << std::endl;
        std::cout << "Grind and brew one scoop of espresso beans." << std::endl;
        std::cout << "Heat and foam milk." << std::endl;
    }
};

class CoffeeMachine {
   private:
    std::unique_ptr<Coffee> coffee_ = nullptr;

   public:
    std::unique_ptr<Coffee> getCoffee() {
        int choice;

        std::cout << "Select type of coffee to make: " << std::endl;
        std::cout << "1: Espresso" << std::endl;
        std::cout << "2: Cappucino" << std::endl;
        std::cout << "Selection: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                return std::make_unique<Espresso>();
            case 2:
                return std::make_unique<Cappuccino>();
            default:
                std::cout << "Invalid selection!" << std::endl;
                return nullptr;
        }
    }
};

int main() {
    CoffeeMachine coffee_machine;
    auto cup = coffee_machine.getCoffee();

    if (cup) {
        std::cout << std::endl
                  << "You had asked for a(n) " << cup->getType() << std::endl;
    }

    return EXIT_SUCCESS;
}