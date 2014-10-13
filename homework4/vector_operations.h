#ifndef VECTOR_OPERATIONS_H
#define VECTOR_OPERATIONS_H

// Copyright 2014 raodm@miamioh.edu

#include <vector>

// Short cut for a vector of floating pointer numbers.
typedef float Type;
typedef std::vector<Type> NumList;

/** Adds each value at index 'i' in first vector with value at index
    'i' in second vector.

    Note that the two vector must be of the same length/size.
    
    \param[in] nl1 The first list of values.

    \param[in] nl2 The second list of values.

    \return A list of numbers where, for all i (0 <= i <= nl1.size()),
    result[i] = nl1[i] + nl2[i].
*/
NumList operator+(const NumList& nl1, const NumList& nl2);

/** Subtracts each value at index 'i' in second vector from value at index
    'i' in second vector.

    Note that the two vector must be of the same length/size.
    
    \param[in] nl1 The first list of values.

    \param[in] nl2 The second list of values.

    \return A list of numbers where, for all i (0 <= i <= nl1.size()),
    result[i] = nl1[i] - nl2[i].
*/
NumList operator-(const NumList& nl1, const NumList& nl2);

/** Multiplies each value at index 'i' in first vector with value at
    index 'i' in second vector.

    Note that the two vector must be of the same length/size.
    
    \param[in] nl1 The first list of values.

    \param[in] nl2 The second list of values.

    \return A list of numbers where, for all i (0 <= i <= nl1.size()),
    result[i] = nl1[i] * nl2[i].
*/
NumList operator*(const NumList& nl1, const NumList& nl2);

/** Divides each value at index 'i' in first vector by value at index
    'i' in second vector.

    Note that the two vector must be of the same length/size.
    
    \param[in] nl1 The first list of values.

    \param[in] nl2 The second list of values.

    \return A list of numbers where, for all i (0 <= i <= nl1.size()),
    result[i] = nl1[i] / nl2[i].
*/
NumList operator/(const NumList& nl1, const NumList& nl2);

#endif
