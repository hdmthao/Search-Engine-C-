#include "speller.h"
#include "../util/util.h"
#include <iostream>

Speller::Speller(Searcher* searcher) : searcher(searcher), dict(nullptr) {
    dict = new Trie();
}

Speller::~Speller() {
    SAFE_DELETE(dict);
}


bool Speller::Insert(std::string &s) {
    if (dict->IsExistWord(s)) {
        return false; // this world already exist
    }

    return dict->Insert(s);
}

bool Speller::Search(std::string &s) {
    return searcher->IsExistWord(s);
}