// Copyright 2014 raodm@miamioh.edu

#include "vector_operations.h"
#include <iostream>
#include <functional>
#include <iterator>
#include <algorithm>

// Convenience stream insertion operator to streamline main method
std::ostream& operator<<(std::ostream& os, const NumList& nl) {
    std::copy(nl.begin(), nl.end(), std::ostream_iterator<float>(os, " "));
    return os;
}

// The create list method to create a vector of given size with random numbers
NumList createList(const int size) {
    NumList retVal(size);
    int num = 1;
    auto generator = [&num](){ return num++; };
    std::generate_n(retVal.begin(), size, generator);
    return retVal;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Specify vector sizes for operation and operation.\n";
        return 1;
    }
    // Get size of vectors.
    const size_t VectSize = std::stoi(argv[1]);
    // Create two vectors with random values for testing.
    const NumList nl1 = createList(VectSize), nl2 = createList(VectSize);
    NumList result;
    switch (argv[2][0]) {
    case '+': result = nl1 + nl2; break;
    case '-': result = nl1 - nl2; break;
    case '*': result = nl1 * nl2; break;
    case '/': result = nl1 / nl2; break;
    default : std::cerr << "Invalid operation!\n";
    }
    std::cout << result << std::endl;
    return 0;
}
