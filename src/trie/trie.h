#ifndef TRIE_H
#define TRIE_H

#include "../config.h"

#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool is_end_of_word;
    TrieNode();
    ~TrieNode();

};


class Trie {
private:
    TrieNode* root;
public:
    Trie();
    ~Trie();
    
    bool Insert(const std::string &);
    bool IsExistWord(const std::string &);
    bool CleanTree(TrieNode* );
};
#endif
