/**
 * Copyright 2014 Keith Batesole
 */

#include <iostream>
#include <string>
#include <cmath>

void perform_metrics(const double & num) {
  // Print the number performing the metrics on
  std::cout << "The number you entered is\t:\t" << num << std::endl;

  // Determine if number is positive
  std::cout << "Is the number positive?\t\t:\t"
            << (num > 0 ? "Yes" : "No")
            << std::endl;

  // Calculate the sine, cosine, and tangent
  std::cout << "The sine of the number is\t:\t" << (sin(num)) << std::endl;
  std::cout << "The cosine of the number is\t:\t" << (cos(num)) << std::endl;
  std::cout << "The tangent of the number is\t:\t" << (tan(num)) << std::endl;
}

int main() {
  // Obtain input from user
  double input;
  std::cout << "Enter a number: ";
  std::cin >> input;

  // Perform metrics on input
  perform_metrics(input);

  // No errors encountered
  return 0;
}
