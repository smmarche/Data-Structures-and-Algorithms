This is a basic implementation of an unordered map (hash map) in C++, similar to std::unordered_map. The implementation includes custom hash functions and prime number calculation for hash table sizing.
Files

    hash_functions.h and hash_functions.cpp: Contains custom hash functions used for key hashing in the hash map.
    primes.h and primes.cpp: Provides functions to calculate the next greater prime number.
    unorderedmap.h: The main implementation of the UnorderedMap class template.
    main.cpp: An example program demonstrating the usage of the UnorderedMap class.
    data_files: A directory containing sample data files used in the example program.

Usage

    Compile the Code: Use your preferred C++ compiler to compile main.cpp along with hash_functions.cpp and primes.cpp. For example:
g++ main.cpp hash_functions.cpp primes.cpp -o unordered_map_example
    Run the Executable: Run the compiled executable to see the example usage of the UnorderedMap class.

Example Output

The example program in main.cpp demonstrates how to use the UnorderedMap class and provides visualizations of the hash table's bucket sizes.
Note

    This is a simplified implementation for educational purposes and may not cover all edge cases or optimizations found in standard library implementations.
