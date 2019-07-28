#include "suggester.h"
#include "../config.h"
#include "../util/util.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <math.h>

SuggesterNode::SuggesterNode() : is_end_of_query(false), rank(0), time(0) {
}

SuggesterNode::~SuggesterNode() {
}


int SuggesterNode::GetRealRank(unsigned int real_time) {
    int diff = real_time - time;
    int real_rank = rank;
    if (diff <= 3600) { // the last hour, so rank * 4
        real_rank *= 4;
    } else if (diff <= 24 * 3600) { // the last day, so rank * 2
        real_rank *= 2;
    } else if (diff <= 7 * 24 * 3600) { // the last week, so rank / 2
        real_rank /= 2; 
    } else { // the last month
        real_rank /= 4;
    }

    return real_rank;
}


Suggester::Suggester() : root(nullptr) {
    root = new SuggesterNode();
    Start();
}

Suggester::~Suggester() {
    CleanSuggester(root);
}

bool Suggester::Start() {
    std::ifstream fi(config::path::HISTORY);
    
    if (!fi.is_open()) {
        std::cout << "Cannot open history file\n";
        return false;
    }
    
    std::string query = "";
    std::string rank_s = "";
    std::string time_s = "";
    while (getline(fi, query, ',')) {
        getline(fi, rank_s,',');
        getline(fi, time_s,',');
        unsigned short int rank = std::stoul(rank_s);
        unsigned int time = std::stoul(time_s);
        // std::cout << query << " " << rank << " " << time << "\n";
        InsertQuery(query, rank, time);
    }
    fi.close();

    return true;
}

bool Suggester::SaveQuery(const std::string &origin_query) {
    // save query to history filse

    if (config::privacy::private_mode) {
        // current mode is private so don't save anything
        return false;
    }

    unsigned short int rank = 1;
    unsigned int real_time = util::time::timer::GetCurrentTime();
    InsertQuery(origin_query, rank, real_time);

    return true;
}


bool Suggester::InsertQuery(const std::string &origin_query, unsigned short int rank, unsigned int time) {
    // Insert query to history tree, if there already in tree so up the rank,
    // Normalize query for sure
    std::string query = util::string::Normalize(origin_query);

    if (root == nullptr) {
        root = new SuggesterNode();
    }

    SuggesterNode* cur = root;
    for (auto c : query) {
        if (cur->children.find(c) == cur->children.end()) {
            cur->children[c] = new SuggesterNode();
        }
        cur = cur->children[c];
    }

    if (cur->is_end_of_query) {
        cur->rank += rank;
        cur->time = time;
    } else {
        cur->is_end_of_query = true;
        cur->rank = rank;
        cur->time = time;
    }

    return true;
}

std::vector<std::string> Suggester::GetSuggest(const std::string &origin_query) {
    // return at least 3 suggester for this query
    
    std::string query = util::string::Trim(origin_query);
    // vector to save all posible query
    std::vector<std::string> suggest_list;

    if (root == nullptr) {
        // history is empty, just return empty list
        return suggest_list;
    }
    // search prefix of query
    SuggesterNode* cur = root;
    for (auto c: query) {
        if (cur->children.find(c) == cur->children.end()) {
            // not exist this query, just return empty list
            return suggest_list;
        }
        cur = cur->children[c];
    }

    // check this query have a prefix in history tree
    bool is_query = cur->is_end_of_query;

    // check this node don't have any child node
    bool is_last = cur->children.empty();

    // if have only one prefix for this query, so just return it
    if (is_query && is_last) {
        suggest_list.push_back(query);
        return suggest_list;
    }

    // if there are many posible history for this query, so collect all off them
    if (!is_last) {
        std::vector<Query> query_list;
        // std::string query_origin = query;
        CollectQueryForSuggest(cur, query, query_list);
        std::sort(query_list.begin(), query_list.end());
        unsigned int query_list_size = query_list.size();
        for (unsigned int i = 0; i < std::min(config::search::max_suggest, query_list_size); ++i) {
            suggest_list.push_back(query_list[i].query);     
        }
    }
    return suggest_list;
}

bool Suggester::CollectQueryForSuggest(SuggesterNode* cur, std::string &query, std::vector<Query> &query_list) {

    unsigned int real_time = util::time::timer::GetCurrentTime();

    if (cur->is_end_of_query) {
        int rank = cur->GetRealRank(real_time); // calculate rank for this query
        query_list.push_back(Query(rank, query));
    }

    if (cur->children.empty()) {
        return false;
    }

    for (auto child : cur->children) {
        query.push_back(child.first);
        CollectQueryForSuggest(child.second, query, query_list);
        query.pop_back();
    }

    return true;
}


void Suggester::CleanSuggester(SuggesterNode* root) {
    if (root == nullptr) {
        return;
    }

    for(auto child : root->children) {
        CleanSuggester(child.second);
    }

    SAFE_DELETE(root);
}