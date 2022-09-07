#include <iostream>

class MultiplyBy {
    int mult = 1;
    MultiplyBy();

   public:
    MultiplyBy(int n) : mult(n){};

    // this is the function operator
    int operator()(int n) const {
        return mult * n;
    };
};

int main() {
    const MultiplyBy times4(4);
    const MultiplyBy times10(10);
    const MultiplyBy times15(15);

    std::cout << "times4(5) is " << times4(5) << std::endl;
    std::cout << "times4(15) is " << times4(15) << std::endl;
    std::cout << "times15(15) is " << times15(15) << std::endl;

    return 0;
}