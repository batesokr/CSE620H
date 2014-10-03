#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <set>
#include <sstream>

int main(int argc, char** argv){
  // Read the file
  std::ifstream dataFile(argv[1]);
  dataFile >> std::noskipws;

  // Toggle characters
  std::transform(std::istream_iterator<char>(dataFile), std::istream_iterator<char>(), 
		 std::ostream_iterator<char>(std::cout), [&argc, &argv](char c){
      if(std::count_if(argv+2, argv+argc, [&c](const char* cc){return c == *cc;}) % 2 != 0) 
	return (char)toupper(c);
      else if(std::count_if(argv+2, argv+argc, [&c](const char* cc){return (char)tolower(c) == *cc;}) % 2 != 0)
	return (char)tolower(c);
      else
	return c;
    });

  // No errors encountered
  return argc;
}
