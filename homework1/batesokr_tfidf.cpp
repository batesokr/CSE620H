/**
 * Keith Batesole (2014) batesokr@miamioh.edu
 * 
 * Homework assignment 1 for CSE 620H, Fall 2014, Miami University 
 */

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <utility> // std::pair
#include <vector>
#include <set>
#include <sstream>

// Globals
static int num_docs = 0;
static std::unordered_map<std::string, int> corpus_index;
static std::unordered_map<std::string, int> search_index;
static std::set<std::string> sig_words;
static std::set<char> ignore_chars {'.', ',', ';', ':', '\'', '"', '!', '?', '(', ')', '{', '}', '[', ']',
        '&', '_', '/', '\\', '|', '`', '~', '<', '>' };
static std::set<char> invalid_chars {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

bool clean_word(std::string& word){
  // Transform word to lowercase    
  std::transform(word.begin(), word.end(), word.begin(), tolower);
  
  // Remove any punctuation
   word.erase(std::remove_if(word.begin(), word.end(), [](const char& c){
  	return ignore_chars.find(c) != ignore_chars.end();
      }), word.end());
   
   // Only add to index if it has no invalid characters and does not contain 'http'
   if(
     std::find_if(word.begin(), word.end(), [](const char& c){
	 return invalid_chars.find(c) != invalid_chars.end();
      }) == word.end()
     && word.find("http") != 0 
     && (word.length() > 0)
      ){
     // word is clean
     return true;
   }else{
     return false;
   }
}

void read_document(const std::string& doc, std::unordered_map<std::string, int>& index){
  // Go through each word in the document, clean it, then add it to the index
  std::fstream fs (doc, std::fstream::in);
  std::istream_iterator<std::string> fs_iterator (fs);
  std::for_each(fs_iterator, std::istream_iterator<std::string>(), [&index](std::string word){
      if(clean_word(word)){
	index[word] = index[word] + 1;
      }
    });
  fs.close();
  num_docs++;
}

void create_corpus_index(const char* corpus_file){
  // Create the index to be used for analysis
  std::cout << "Loading corpus using files listed in " << corpus_file << std::endl;
  std::fstream corpus (corpus_file, std::fstream::in);
  std::istream_iterator<std::string> corpus_iterator(corpus);
  std::for_each(corpus_iterator, std::istream_iterator<std::string>(), [](const std::string& doc){
      read_document(doc, corpus_index);
    });
  std::cout << "Loaded corpus of " << corpus_index.size() 
	    << " words from " << num_docs << " file(s)." 
	    << std::endl;
}

void create_search_index(const char* search_file){
  // Create the index of the search words
  read_document(search_file, search_index);
  num_docs--;
}

void print_signficant_words(unsigned int n){
  // Start the analysis
  std::cout << "------[ Starting analysis ]------" << std::endl;

  // Create and sort the word frequencies
  std::vector< std::pair<std::string, float> > words (search_index.size());
  std::transform(search_index.begin(), search_index.end(), words.begin(), 
		 [](const std::pair<std::string, int>& pair){
		   float second = (float)pair.second * log10((float)num_docs/(float)corpus_index[pair.first]);
		   return std::pair<std::string, float> (pair.first, second);
    });
  std::sort(words.begin(), words.end(), 
	    [](const std::pair<std::string,float>& pair1, const std::pair<std::string,float>& pair2){
	      return pair1.second > pair2.second;
    });

  // Print the top-n
  std::cout << "Top 10 significant words..." << std::endl;
  n = (words.size() < n ? words.size() : n);
  std::for_each(words.begin(), words.begin()+n, [](const std::pair<std::string, float>& word){
      std::cout << word.first << ": " << word.second << std::endl;
      sig_words.insert(word.first);
    });
}

void print_sentences(const char* search_file){
  // Print the significant sentences
  std::cout << "Lines with one ore more significant words:" << std::endl;
  std::fstream fs (search_file, std::fstream::in);
  std::string line;
  int num_sigs;
  for(int line_num = 1; std::getline(fs, line); line_num++){
    std::istringstream line_ss (line);
    num_sigs = std::count_if(std::istream_iterator<std::string>(line_ss), std::istream_iterator<std::string>(),
			     [](std::string word){
			       return (clean_word(word) ? sig_words.find(word) != sig_words.end() 
				       : false);
			     });
    if(num_sigs > 1){
      std::cout << line_num << "[" << num_sigs << "]:\t" << line << std::endl;
    }
  }
  fs.close();  
}

void print_statistics(const char* search_file, int n){
  print_signficant_words(n);
  print_sentences(search_file);
}

/** Command arguments are as follows
 *  - argv[1]: The corpus file
 *  - argv[2]: The search file
 *  - argv[3]: The number of significant terms to display
 */
int main(int argc, char * argv[]){
  // Create an index from the corpus
  create_corpus_index(argv[1]);

  // Create an index from the search file
  create_search_index(argv[2]);

  // Print statistics
  print_statistics(argv[2], (atoi(argv[3])));

  // No errors encountered
  return 0;
}
