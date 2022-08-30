#include <math.h>
#include <stdexcept>

double squareRoot(const double a)
{
    double b = sqrt(a);
    return b;
}

/*
 * function that calculates factorial of a number and
 * throws invalid argument error if negative number is given
 */
int factorial(int n)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }
    else if (n > 1)
    {
        int prod = 1;
        for (int i = 2; i <= n; i++)
        {
            prod *= i;
        }
        return prod;
    }
    else
    {
        throw std::invalid_argument("Cannot calculate the factorial of a negative number!");
    }
}