// Copyright 2014 raodm@miamioh.edu

#include "vector_operations.h"
#include <algorithm>
#include <functional>

// Unorthodox overload of operator+ for vectors.
NumList operator+(const NumList& nl1, const NumList& nl2) {
    NumList retVal(nl1);
    std::transform(nl1.begin(), nl1.end(), nl2.begin(), retVal.begin(),
                   std::plus<Type>());
    return retVal;
}

// Unorthodox overload of operator- for vectors.
NumList operator-(const NumList& nl1, const NumList& nl2) {
    NumList retVal(nl1);
    std::transform(nl1.begin(), nl1.end(), nl2.begin(), retVal.begin(),
                   std::minus<Type>());
    return retVal;
}

// Unorthodox overload of operator* for vectors.
NumList operator*(const NumList& nl1, const NumList& nl2) {
    NumList retVal(nl1);
    std::transform(nl1.begin(), nl1.end(), nl2.begin(), retVal.begin(),
                   std::multiplies<Type>());
    return retVal;
}

// Unorthodox overload of operator* for vectors.
NumList operator/(const NumList& nl1, const NumList& nl2) {
    NumList retVal(nl1);
    std::transform(nl1.begin(), nl1.end(), nl2.begin(), retVal.begin(),
                   std::divides<Type>());
    return retVal;
}
