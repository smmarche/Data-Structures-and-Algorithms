# Custom Vector Implementation and Interactive Menu Program

This repository contains a custom implementation of a dynamic array (vector) class called `Vector`, along with an interactive menu-driven program that demonstrates the usage of this vector class. The vector class provides dynamic memory allocation and basic functionality similar to the standard library's `std::vector`.

## Vector Class (Vector.h)

The `Vector` class is a dynamically resizable array that provides the following functionalities:

- Creating vectors
- Adding elements
- Removing elements
- Copying vectors
- Clearing vectors
- Printing vector contents

### Usage

To use the `Vector` class, you can include the `Vector.h` header file in your code and then create instances of the `Vector` class with specified data types. The implementation provides an iterator for iterating through the elements of the vector.

```cpp
#include "Vector.h"

int main() {
    // Create instances of Vector
    Vector<int> intVector;
    Vector<double> doubleVector;

    // Perform operations on vectors
    intVector.push_back(42);
    doubleVector.push_back(3.14);

    // ... more operations ...

    return 0;
}

Interactive Menu Program (main.cpp)

The repository also includes an interactive menu-driven program (main.cpp) that demonstrates the use of the Vector class. The program allows you to perform various operations on named vectors, including adding elements, removing elements, copying vectors, clearing vectors, and printing vector contents.
Running the Interactive Program
Compile the program using a C++ compiler (e.g., g++):
g++ -o main main.cpp Vector.h
Run the compiled executable:
./main
Follow the on-screen instructions to perform different vector operations.

