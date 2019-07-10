#ifndef SPELLER_H
#define SPELLER_H

#include "../trie/trienode.h"


class Speller {
private:
    Trie* dict;  

public:
    Speller();
    ~Speller();

};

#endif
