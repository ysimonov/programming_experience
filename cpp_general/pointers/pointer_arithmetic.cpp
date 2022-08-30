#include <iostream>
#include <memory>
#include <vector>

#define print(x) std::cout << x << " "
#define println(x) std::cout << x << std::endl

void ModifyValue(int &val)
{
    val = 1000;
}

// Calculate sum by passing an address and access using a pointer as input argument
void Sum(int *array_ptr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array_ptr[i];
    }
    println("Sum of array elements using pointer to an array: " << sum);
}

// Calculate sum by passing an address and access using a pointer as input argument
void SumInputPtr1D(int *array_ptr, int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array_ptr[i];
    }
    println("Sum of array elements using pointer to an array: " << sum);
}

// Formal parameter as sized array
void SumInputSizedArr1D(int array_ptr[100], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array_ptr[i];
    }
    println("Sum of array elements using formal parameter as sized array: " << sum);
}

// Formal parameter as unsized array
void SumInputUnsizedArr1D(int array_ptr[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array_ptr[i];
    }
    println("Sum of array elements using formal parameter as sized array: " << sum);
}

int main()
{

    // *** Example Single Element ***
    int *number{nullptr};
    number = new int;

    // writing into dynamically allocated memory
    *number = 77;

    std::cout << "Allocated number: " << *number << std::endl;
    std::cout << "Memory Address: " << number << std::endl;

    delete number;
    number = nullptr;

    // *** Example 1D Array ***
    int size = 50;
    int *number_array_ptr = new int[size];
    for (int i = 0; i < size; i++)
        number_array_ptr[i] = i;

    println("\naddress of number_array_ptr[0]: " << &number_array_ptr[0]);
    println("address of the first element in number_array_ptr using number_array_ptr: " << number_array_ptr);
    println("address of number_array_ptr[3]: " << &number_array_ptr[3]);
    println("address of the fourth element in number_array_ptr using number_array_ptr + 4: " << number_array_ptr + 3);

    SumInputPtr1D(number_array_ptr, size);
    SumInputSizedArr1D(number_array_ptr, size);
    SumInputUnsizedArr1D(number_array_ptr, size);

    delete[] number_array_ptr;
    number_array_ptr = nullptr;

    // *** Example 2D Array ***
    int rows = 10;
    int cols = 10;

    // allocating rows
    int **number_matrix_ptr = new int *[rows];

    // allocating rows
    for (int i = 0; i < rows; i++)
        number_matrix_ptr[i] = new int[cols];

    // deallocating elements
    for (int i = 0; i < rows; i++)
        delete[] number_matrix_ptr[i];
    delete[] number_matrix_ptr;
    number_matrix_ptr = nullptr;

    // *** Example 1D Array using unique_ptr ***
    {
        std::unique_ptr<int[]> number_array_uptr(new int[15]);
    } // destructor gets called 15 times

    // *** Example 2D Array using unique ptr ***
    {
        // unique pointer is template based, so <T> should be specified
        std::unique_ptr<std::unique_ptr<int[]>[]> number_matrix_uptr(new std::unique_ptr<int[]>[rows]());

        println("\nprinting unique pointer (2D): ");

        for (int i = 0; i < rows; i++)
        {
            number_matrix_uptr[i] = std::make_unique<int[]>(cols);
            for (int j = 0; j < cols; j++)
            {
                number_matrix_uptr[i][j] = i * j;
                print(number_matrix_uptr[i][j]);
            }
            println("");
        }
        println("");
    }

    // *** Example 1D Array using vector ***
    // vector not only allocates space,
    // but also initializes all elements within memory
    // this is different to pointer, that only allocates space
    std::vector<int> number_array_vec(size);

    // *** Example 2D Array using vector ***
    // allocate rows elements of vector of vectors
    std::vector<std::vector<int>> number_matrix_vec(rows);

    println("printing vector (2D): ");

    for (int i = 0; i < rows; i++)
    {
        std::vector<int> temp(cols);

        // transfer memory from temp to number_matrix_vec
        number_matrix_vec[i] = std::move(temp);

        // assign elements
        for (int j = 0; j < cols; j++)
        {
            number_matrix_vec[i][j] = i * j;
            print(number_matrix_vec[i][j]);
        }
        println("");
    }
    println("");

    // *** Declaring and using references ***
    int int_value{45};
    double double_value{33.65};

    int &ref_to_int_value_1{int_value};  // assignment via initialization
    int &ref_to_int_value_2 = int_value; // assignment via assignment
    double &ref_to_double_value_1{double_value};

    println("int value: " << int_value);
    println("double value: " << double_value);
    println("ref to int value 1: " << ref_to_int_value_1);
    println("ref to int value 2: " << ref_to_int_value_2);
    println("ref to double value 1: " << ref_to_double_value_1);
    println("memory address of int value: " << &int_value);
    println("memory address of double value: " << &double_value);
    println("memory address of ref to int value 1: " << &ref_to_int_value_1);
    println("memory address of ref to int value 2: " << &ref_to_int_value_2);
    println("memory address of ref to double value 1: " << &ref_to_double_value_1);
    println("sizeof(int): " << sizeof(int));
    println("sizeof(int&): " << sizeof(int &));
    println("sizeof(ref to int value 1): " << sizeof(ref_to_int_value_1));

    ModifyValue(ref_to_int_value_1);

    println("int value (after modifying): " << int_value);
    println("ref to int value 1 (after modifying): " << ref_to_int_value_1);
    println("ref to int value 2 (after modifying): " << ref_to_int_value_2);

    return EXIT_SUCCESS;
}