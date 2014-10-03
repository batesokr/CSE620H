#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <iterator>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Specify <File> <toggle_char> ...\n";
        return 1;
    }
    // Read the file.
    std::ifstream dataFile(argv[1]);
    dataFile >> std::noskipws;
    std::istream_iterator<char> dataIter(dataFile), eof;
    std::string data(dataIter, eof);
    // Replace specified charachters
    for(int curr = 2; (curr < argc); curr++) {
        const char low = argv[curr][0];
        const char up  = toupper(low);
        auto  toggle = [low,up](char c) {
            return (c == low ? up : c == up ? low : c);
        };
        std::transform(data.begin(), data.end(), data.begin(), toggle);
    }
    // Print the modifed data.
    std::cout << data;
    return 0;
}
