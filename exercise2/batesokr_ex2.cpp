#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <set>

int countVowels(const std::string& str) {
  // Returns the number of vowels (one of the characters 'aeiouAEIOU')
  // in the supplied string
  const std::set<char> vowels {'a', 'e', 'i', 'o', 'u'};
  int count = 0;
  for(size_t i = 0; (i < str.length()); i++) {
    count += (vowels.find(tolower(str[i])) != vowels.end()) ? 1 : 0;
  }
  return count;
}

void countVowels(char *fileName, std::ifstream& dataFile) {
  // Load the whole file into memory!
  std::istream_iterator<char> data(dataFile), eof;
  std::string str(data, eof);
  std::cout << "Vowels in file " << fileName << ": "
            << countVowels(str)  << std::endl;
}

int main(int argc, char *argv[]) {
  if(argc == 3){
    if (std::string(argv[1]) == "--str") {
      std::cout << "Vowel count: " << countVowels(argv[2]) << std::endl;
    } else {
      std::ifstream dataFile(argv[2]);
      if (dataFile.good()) {
        countVowels(argv[2], dataFile);
      } else {
        std::cout << "Unable to read file: " << argv[2] << std::endl;
        return 1;
      }
    }
  }
  return 0;
}
