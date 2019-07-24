#ifndef SEARCHER_H
#define SEARCHER_H

#include <unordered_map>
#include "resultcomponet.h"
// it is actually a trie node with map table to save memory
class SearcherNode {
public:
    std::unordered_map<char, SearcherNode*> children;
    bool is_end_of_word;

    SearcherNode();
    ~SearcherNode();

};

// Main search
class Searcher {
private:
    SearcherNode* root;
    bool Insert(const std::string &);

public:
    Searcher();
    ~Searcher();
    
    bool AddData(const std::string &);
    bool IsExistWord(const std::string &str);
    ResultComponent HighlightResult(const std::string &origin_query, const std::string &file_name);
};

#endif
