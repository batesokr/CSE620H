#include <iostream>
#include <iterator>
#include <algorithm>
#include <fstream>

int main(int argc, char *argv[]) {
  const std::string Letters = 
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  // Anonymous/first-class/lambda function to generate random letter
  auto rndChar = [&Letters]{ return Letters[rand() & Letters.size()]; };
  
  // Assume:  argv[1] = number of letters to generate
  //			argv[2] = path to output file
  
  // Complete the program to generate argv[1] number of English letters
  // (using rndChar function) and write them to a file specified in
  // argv[2]. Tip: see gnerate_n algorithm
  
  // Obtain the number of letters to generate
  int n = atoi(argv[1]);
  std::fstream fs (argv[2], std::fstream::out);
  std::ostream_iterator<char> oi (fs, "");
  std::generate_n(oi, n, rndChar);
  fs.close();
 
  // No errors encountered
  return 0;
}
