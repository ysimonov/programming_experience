#include <iostream>
#include <memory>
#include <vector>
#include <algorithm> // generate

void foo(std::unique_ptr<int> ptr)
{
    std::cout << "Received unique pointer into 'foo' function with a value of " << *ptr.get() << std::endl;
}

std::unique_ptr<int> get_ptr()
{   
    std::unique_ptr<int> ptr = std::make_unique<int>(59);
    return ptr;
}

void examples() 
{

    // create dynamic int with a value of 20 owned by a unique ptr
    {
        std::unique_ptr<int> ptr1 = std::make_unique<int>(20);
        std::cout << "Value of a pointer is " << *ptr1 << std::endl;
    }
    // outside of scope, destructor is called


    // create a unique pointer to an array of 15 ints
    std::unique_ptr<int[]> ptr_arr1 = std::make_unique<int[]>(15);

    // modify third element
    ptr_arr1[2] = 10;

    // note that dynamic arrays don't store size, so it is required to keep
    // information about the number of elements in the container in order to access


    //! Example of transferring ownership of the contents of a smart pointer
    //! to another smart pointer
    std::unique_ptr<int> ptr2 = std::make_unique<int>();

    // use dereference operator to change the value of the pointer
    *ptr2 = 1;

    std::cout << "\nMoving ownership from ptr2 to ptr3\n";
    std::cout << "The value of ptr2 before chaning ownership: " << *ptr2 << std::endl;
    std::cout << "Memory address of ptr2 before changing ownership: " << &ptr2 << std::endl;

    // Move ownership from ptr2 to ptr3. After this operation ptr2 will no longer
    // maintain the ownership of memory associated with it.
    std::unique_ptr<int> ptr3 = std::move(ptr2);

    std::cout << "Memory address of ptr2 after changing ownership: " << &ptr2 << std::endl;
    std::cout << "Value of ptr3 after changing ownership: " << *ptr3 << std::endl;
    std::cout << "Memory address of ptr3 after changing ownership: " << &ptr3 << "\n\n";

    // note than ownership is now transfered to function foo
    foo(std::move(ptr3));

    // the caller owns the resulting unique pointer
    std::unique_ptr<int> ptr4 = get_ptr();

    
    // unlike auto_ptr, unique_ptr can be instantiated with a vector allocation
    const size_t size_vec_ptr = 20;
    std::vector<std::unique_ptr<int>> vec_ptr(size_vec_ptr);

    // note that if empty allocator is called std::make_unique<int>(),
    // all values will be set to 0
    std::generate(vec_ptr.begin(), vec_ptr.end(), [](){ return std::make_unique<int>(15); });

    std::cout << "Allocated vector of 20 element unique ptr, all having values of 15" << std::endl;
    for (auto&& val : vec_ptr) 
    {
        std::cout << *val << " ";
    }
    std::cout << std::endl;


    //! std::shared_ptr is the class template that defines a smart pointer
    //! that is able to share ownership of an object with other shared pointers.
    //! this is different to unique_ptr, that only holds UNIQUE ownership.
    //! sharing is implemented through reference counting. Similar to python,
    //! when the reference reaches zero, the object is automatically destroyed.

    std::shared_ptr<double> shared_ptr1 = std::make_shared<double>();
    std::cout << "\nCreated a shared pointer shared_ptr1: " << *shared_ptr1 << 
        " at the memory location of " << &shared_ptr1 << std::endl;

    // creating other pointer using copy constructor
    std::shared_ptr<double> shared_ptr2(shared_ptr1);

    std::cout << "Created shared_ptr2 using copy constructor (memory): " << &shared_ptr2 << " (value): " << *shared_ptr2 << std::endl;

    // create using assignment
    std::shared_ptr<double> shared_ptr3;
    shared_ptr3 = shared_ptr1;

    std::cout << "Created shared_ptr3 using assignment (memory): " << &shared_ptr3 << " (value): " << *shared_ptr3 << std::endl;
    std::cout << "Number of instances managing the shared pointer: " << shared_ptr1.use_count() << std::endl;


    //! there is no direct way of allocating arrays using make shared
    //! the way to do so is use new and std::default_delete
    //! allocated memory will be correctly cleaned up using delete[]
    std::shared_ptr<int[]> shared_arr1(new int[10], std::default_delete<int[]>());

    // dereferencing can be done using array index operator
    shared_arr1[0] = 42;

    std::cout << "\nAllocated shared pointer array of size 10" << std::endl;
    std::cout << "Assigned first value of the array to " << *(shared_arr1.get()) << std::endl;

    std::shared_ptr<int[]> shared_arr2(shared_arr1);
    std::cout << "First element of shared_arr2: " << shared_arr2[0] << std::endl;

    // delete shared_arr2
    // reset doesn't produce an exception
    shared_arr1.reset();

    if (shared_arr1 == nullptr) 
    {
        std::cout << "Shared pointer array shared_arr1 was deleted by calling reset()\n";
    }
    
    //! by default shared pointer increments reference count and doesn't transfer the ownership
    //! the ownership can be transferred with std::move
    std::shared_ptr<int[]> shared_arr3 = std::move(shared_arr2);

    if (shared_arr2 == nullptr)
    {
        std::cout << "Ownership of shared_arr2 was transfered to shared_arr3!\n";
        std::cout << "First element of shared_arr3: " << shared_arr3[0] << std::endl;

    }

    shared_arr2.reset();

}

int main() 
{
    examples();
    return 0;
}