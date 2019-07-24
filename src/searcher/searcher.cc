// Implement for searcher tree (actually a trie)

#include "searcher.h"
#include "../util/util.h"
#include <iostream>

// Constructor for searcher node
SearcherNode::SearcherNode() : is_end_of_word(false) {
}

// Deconstructor for searcher node
SearcherNode::~SearcherNode() {
}

// Constructor for searcher
Searcher::Searcher() : root(nullptr) {
    root = new SearcherNode();
}

// Deconstuctor for searcher
Searcher::~Searcher() { 
}

// Insert a word to trie 
bool Searcher::Insert(const std::string &str) {
    // Standarized word 
    std::string word = util::string::Normalize(str);
    
    // word is empty so we cannot insert
    if (word.empty()) return false;

    // insert word to trie
    if (root == nullptr) {
        root = new SearcherNode();
    }
    
    SearcherNode* cur = root;

    for (auto c: word) {
        if (cur->children.find(c) == cur->children.end()) {
            cur->children[c] = new SearcherNode();   
        }
        cur = cur->children[c];
    }
    cur->is_end_of_word = true;

    return true;
}

bool Searcher::AddData(const std::string &str) {
    std::vector<std::string> word_list = util::string::Split(str);

    for (auto word : word_list) {
        Insert(word);
    }

    return true;
}

// Search a word in trie
bool Searcher::IsExistWord(const std::string &str) {
    // Standarized word
    std::string word = util::string::Normalize(str);
    // std::cout << word << "\n";
    // word is empty so we cannot search
    if (word.empty()) return false;

    // search word 
    SearcherNode* cur = root;

    for (auto c: word) {
        if (cur->children.find(c) == cur->children.end()) {
            return false;
        }
        cur = cur->children[c];

    }
 
    return cur->is_end_of_word;
}
