#include <iostream>
#include <memory>
#include <string>

class Door {
   public:
    Door() {}
    virtual void open() = 0;
    virtual ~Door() {}
};

class GasCarDoor : public Door {
   public:
    GasCarDoor() { std::cout << "Making a door for a gas car\n"; }
    void open() { std::cout << "click\n"; }
};

class ElectricCarDoor : public Door {
   public:
    ElectricCarDoor() { std::cout << "Making a door for an electric car\n"; }
    void open() { std::cout << "shhhhhh\n"; }
};

class Engine {
   protected:
    std::string sound_;

   public:
    Engine() { sound_ = ""; };

    virtual void run() = 0;
    virtual ~Engine() {}
};

class GasEngine : public Engine {
   public:
    GasEngine() {
        sound_ = "vroom";
        std::cout << "Making a gas engine.\n";
    }

    void run() { std::cout << sound_ << std::endl; }
};

class ElectricEngine : public Engine {
   public:
    ElectricEngine() {
        sound_ = "SHHHH";
        std::cout << "Making an electric engine.\n";
    }

    void run() { std::cout << sound_ << std::endl; }
};

class CarFactory {
   public:
    virtual std::unique_ptr<Door> BuildDoor() = 0;
    virtual std::unique_ptr<Engine> BuildEngine() = 0;
    virtual ~CarFactory() {}
};

class GasCarFactory : public CarFactory {
   public:
    std::unique_ptr<Door> BuildDoor() { return std::make_unique<GasCarDoor>(); }
    std::unique_ptr<Engine> BuildEngine() {
        return std::make_unique<GasEngine>();
    }
};

class ElectricCarFactory : public CarFactory {
   public:
    std::unique_ptr<Door> BuildDoor() {
        return std::make_unique<ElectricCarDoor>();
    }
    std::unique_ptr<Engine> BuildEngine() {
        return std::make_unique<ElectricEngine>();
    }
};

int main() {
    int choice = -1;
    std::unique_ptr<CarFactory> car_plant = nullptr;

    std::cout << "Select a car type: \n";
    std::cout << "1: Gasoline\n";
    std::cout << "2: Electric\n";
    std::cout << "Selection: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            car_plant = std::make_unique<GasCarFactory>();
            break;
        case 2:
            car_plant = std::make_unique<ElectricCarFactory>();
            break;
        default:
            std::cout << "Invalid Selection!\n";
            break;
    }

    if (car_plant) {
        auto my_door = car_plant->BuildDoor();
        auto my_engine = car_plant->BuildEngine();

        my_door->open();
        my_engine->run();
    }

    return EXIT_SUCCESS;
}