/**
 * Copyright 2014 Keith Batesole
 */

#include <string>
#include <iostream>

void perform_line_metrics(const std::string & line) {
  // Print length, first and last character, and first and last word
  std::cout << "Length of line\t:\t" << line.length() << std::endl;
  std::cout << "First character\t:\t" << line.front() << std::endl;
  std::cout << "Last character\t:\t" << line.back() << std::endl;
  std::cout << "First word\t:\t"
            << line.substr(0, line.find_first_of(" "))
            << std::endl;

  int last_pos = line.find_last_of(" ");
  std::cout << "Last word\t:\t"
            << line.substr(last_pos+1, line.length())
            << std::endl;
}

int main() {
  // Get a line from the standard input
  std::cout << "Enter a line: ";
  std::string input;
  std::getline(std::cin, input);

  // Perform metrics on the input
  perform_line_metrics(input);

  // No errors
  return 0;
}
