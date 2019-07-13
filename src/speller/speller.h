#ifndef SPELLER_H
#define SPELLER_H

#include "../trie/trie.h"
#include "../searcher/searcher.h"
#include <string.h>


class Speller {
private:
    Trie* dict; 
    Searcher* searcher; 
    bool Insert(std::string &s); // insert s to dict if it not exist in dict
    bool Search(std::string &s); // return true if there is exist a word in database
	bool AutoCorrect(const std::string &origin, std::string &fix); // auto correct a word

public:
    Speller(Searcher* searcher);
    ~Speller();
	bool Check(const std::string &origin, std::string &fix); // auto correct a sentence
};


#endif
