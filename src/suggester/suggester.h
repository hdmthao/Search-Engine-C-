#ifndef SUGGESTER_H
#define SUGGESTER_H

#include <vector>
#include <string>
#include <unordered_map>

class Query {
public:
    int rank;
    std::string query;

    bool operator < (const Query &b) const {
        return rank < b.rank;
    }

    Query(int rank, const std::string &query) : rank(rank), query(query) {};
    ~Query() {};
};


class SuggesterNode {
public:
    bool is_end_of_query;
    unsigned short int rank;
    unsigned int time;
    std::unordered_map<char, SuggesterNode*> children;

    int GetRealRank(unsigned int real_time);
    SuggesterNode();
    ~SuggesterNode();
};

class Suggester {
private:
    SuggesterNode* root;

    bool Start();
    void CleanSuggester(SuggesterNode* root);
    bool InsertQuery(const std::string &origin_query, unsigned short int rank, unsigned int time);
    bool CollectQueryForSuggest(SuggesterNode* cur, std::string &query, std::vector<Query> &suggest_list);
public:
    Suggester();
    ~Suggester();

    bool SaveQuery(const std::string &origin_query);
    std::vector<std::string> GetSuggest(const std::string &origin_query);
};

#endif