#include <eigen3/Eigen/Dense>
#include <iostream>

using namespace Eigen;

int main() {
    Matrix<double, 3, 3> a;
    a << 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f;

    MatrixXd b = MatrixXd::Random(3, 3);

    std::cout << "Matrix a: \n";
    std::cout << a << "\n\n";

    std::cout << "Matrix b: \n";
    std::cout << b << "\n\n";

    auto sum = a + b;
    auto diff = a - b;
    auto prod = a * b;
    auto inv = a.inverse();
    auto pinv = a.completeOrthogonalDecomposition().pseudoInverse();
    auto diag = b * b.inverse();

    std::cout << "Matrix a + b: \n";
    std::cout << sum << "\n\n";

    std::cout << "Matrix a - b: \n";
    std::cout << diff << "\n\n";

    std::cout << "Matrix a * b: \n";
    std::cout << prod << "\n\n";

    std::cout << "Inverse of matrix a: \n";
    std::cout << inv << "\n\n";

    std::cout << "Pseudoinverse of matrix a: \n";
    std::cout << pinv << "\n\n";

    std::cout << "Inverse of matrix b times b: \n";
    std::cout << diag << "\n\n";

    return EXIT_SUCCESS;
}