#include "trie.h"
#include "../util/util.h"


TrieNode::TrieNode() : is_end_of_word(false) {
}


TrieNode::~TrieNode() {

}


Trie::Trie() : root(nullptr) {
    root = new TrieNode();
}


Trie::~Trie() {
    CleanTree(root); 
}

bool Trie::Insert(const std::string &s) {
    // Format insert word for sure
    std::string word = util::string::Normalize(s);

    if (root == nullptr) {
        root = new TrieNode();
    }

    TrieNode* cur = root;
    for (auto c : word) {
        if (cur->children.find(c) == cur->children.end()) {
            cur->children[c] = new TrieNode();
        }

        cur = cur->children[c];
    }

    cur->is_end_of_word = true;
    return true;
}


bool Trie::IsExistWord(const std::string &s) {
    // normalize find word for sure
    std::string word = util::string::Normalize(s);
    
    if (word.empty() || root == nullptr) return false;

    TrieNode* cur = root;
    
    for (auto c : word) {
        if (cur->children.find(c) == cur->children.end()) return false;
        cur = cur->children[c];
    }

    return cur->is_end_of_word;
}


bool Trie::CleanTree(TrieNode *root) {
    if (root == nullptr) return true;

    for (auto child : root->children) {
        CleanTree(child.second);
    }

    SAFE_DELETE(root);
    return true;
}
