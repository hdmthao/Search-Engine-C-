#ifndef SEARCHER_H
#define SEARCHER_H

#include <unordered_map>
#include <set>
#include <functional>
#include <vector>
#include <string>
#include "../component/component.h"

struct comp {
	bool operator()(const std::pair<int, double>& l, const std::pair<int, double>& r) const {
            if (l.second != r.second)
    		return l.second >  r.second;
            return l.first < r.first;
	}
};


// it is actually a trie node with map table to save memory
class SearcherNode {
public:
    std::unordered_map<char, SearcherNode*> children;
    std::unordered_map<int, std::vector<int>> posting_list;
    bool is_end_of_word;

    SearcherNode();
    ~SearcherNode();
};

// Main search
class Searcher {
private:
    SearcherNode* root;
    int total_news;
    long long total_worlds;
    std::vector<int> nword;
    SearcherNode* GetNode(const std::string &);
    double idf(const int& N, const int& nqi);
    double BM25(const double& idf, const int& tf, const double& avgdl, const int& D, const double& k1 = 1.2, const double& b = 0.75);
    bool CheckTitle(std::string &file_name, std::string &word);
public:
    Searcher();
    ~Searcher();
    
    static bool cmp(std::pair<int, int> x, std::pair<int, int> y);
    bool Init(int total);
    bool Insert(const std::string &, int&, int&);
    bool IsExistWord(const std::string &str);
    std::vector<std::pair<int, double>> GetResultWithNormalSearch(const std::string &);
    std::vector<std::pair<int, double>> GetResultWithAndSearch(const std::string &);
    std::vector<std::pair<int, double>> GetResultWithExcludeSearch(const std::string &, const std::string &);
    std::vector<std::pair<int, double>> GetResultWithIncludeSearch(const std::string &, const std::string &);
    std::vector<std::pair<int, double>> GetResultWithTitleSearch(const std::string &, std::unordered_map<int, std::string> &doc_map);
    std::vector<std::pair<int, double>> GetResultWithExaclytWord(const std::string &);
    std::vector<std::pair<int, double>> GetResultWithWildcard(const std::string &);
    ResultInfo HighlightResult(const std::string &origin_query, const std::string &file_name);
    std::vector <std::string> GetSynonym(const std::string &origin_word);
    std::string GetSynonym_string(const std::string &originword);
    std::vector<std::pair<int, int>> GetExact(std::vector<SearcherNode> &wordList);
    std::vector<int> GetChung(std::vector<SearcherNode> &wordList);
    std::vector< std::pair<int, int> > GetWildcard(std::vector<SearcherNode> &wordList);
};

#endif
