#ifndef TRIENODE_H
#define TRIENODE_H

#include "../config.h"

#include <unordered_map>

class TrieNode {
public:
    bool is_end_of_word;
    TrieNode();
    ~TrieNode();

};

#endif
