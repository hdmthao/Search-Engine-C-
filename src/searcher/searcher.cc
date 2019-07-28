// Implement for searcher tree (actually a trie)

#include "searcher.h"
#include "../util/util.h"
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>

// Constructor for searcher node
SearcherNode::SearcherNode() : is_end_of_word(false) {
}

// Deconstructor for searcher node
SearcherNode::~SearcherNode() {
}

// Constructor for searcher
Searcher::Searcher() : root(nullptr), total_news(0), total_worlds(0) {
    root = new SearcherNode();
}

// Deconstuctor for searcher
Searcher::~Searcher() { 
}

bool Searcher::Init(int total) {
	total_news = total;
	nword.resize(total);
	return true;
}
// Insert a word to trie 
bool Searcher::Insert(const std::string &str, int &doc_id, int &position) {
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
    if (cur->posting_list.find(doc_id) == cur->posting_list.end()) {
        cur->posting_list.insert(std::pair<int, std::vector<int>> (doc_id, std::vector<int>()));
    }
    cur->posting_list[doc_id].push_back(position);
	nword[doc_id]++;
	total_worlds++;
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

SearcherNode* Searcher::GetNode(const std::string &word) {
	if (word.empty()) return nullptr;

	SearcherNode* cur = root;

	for (auto c: word) {
		if (cur->children.find(c) == cur->children.end()) {
			return nullptr;
		}
		cur = cur->children[c];
	}
	return cur;
}

// Main search
std::vector<std::pair<int, double>> Searcher::GetResultWithNormalSearch(const std::string &origin_query, int &total_result) {

	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);
	if (query.empty()) return result_list;
	query = util::string::RemoveStopWord(query);
	std::vector<std::string> word_list = util::string::Split(query);
	std::map<int, double> rank;
	std::ofstream fo("log_searcher.txt");
	// fo << total_worlds << " " << total_news << "\n";
	fo << query << "\n";
	float avgdll = total_worlds / total_news;
	for (auto word : word_list) {
		SearcherNode* cur_node = GetNode(word);
		if (cur_node == nullptr) continue;
		double idf_ = idf(total_news, cur_node->posting_list.size());
		for (auto it : cur_node->posting_list) {
			int doc_id = it.first;
			int D = nword[doc_id];
			rank[it.first] += BM25(idf_, (int)it.second.size(), avgdll, D);
		}
	}
	std::set<std::pair<int, double>, comp> set_result(
			rank.begin(), rank.end());
	int count = 0;
	total_result = set_result.size();
	for (auto doc : set_result) {
		count++;
		fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
		if (count == 5) break;
	}
	fo.close();
	return result_list;
}

double Searcher::idf(const int& N, const int& nqi) {
	return log10((N - nqi + 0.5 + 1) / (nqi + 0.5));
}

double Searcher::BM25(const double& idf, const int& tf, const double& avgdl, const int& D, const double& k1, const double& b){
	return idf * (tf * (k1 + 1)) / (tf + k1 * (1 - b + b * D/avgdl) + 1);
}

ResultInfo Searcher::HighlightResult(const std::string &s, const std::string &filename, int &id)
{
		std::ifstream fin("data/" + filename);

	ResultInfo *fil = new ResultInfo();
	fil->file_name = filename;
	fil->total_words = nword[id];
	std::string st = s;
	st = util::string::RemoveUnicode(st);
	st = util::string::Trim(s);
	st = util::string::RemoveMark(st);
	std::vector <std::string> words = util::string::Split(st);
	
	int max_word = 0;
	std::string line_max="";
	std::string line_se="";
	while (!fin.eof())
	{
		std::string str;

		int count = 0;
		getline(fin, str);
		str = util::string::RemoveUnicode(str);
		str = util::string::Trim(str);
		std::vector <std::string> word_str = util::string::Split(util::string::Trim(str));
				if (fil->title == "") {
			if (str.length() < 100)
				fil->title = str;			
			else {
				if (str.find(".") != -1 && str.find(".") < 120 )
					fil->title = str.substr(0, str.find("."));
				else {
					fil->title = str.substr(0, 100);
					std::string ti = fil->title;
					while (ti[ti.length()-1] != ' ')
						ti.pop_back();
					ti.pop_back();
					fil->title = ti + "...";
				}
			}
		}
			for (auto it = words.begin(); it != words.end(); it++) {
				if (util::string::ToLowerCase( str).find(util::string::ToLowerCase( *it)) != -1) {
				for (auto wo : word_str)
									if (util::string::ToLowerCase( wo).find(util::string::ToLowerCase(*it))!=-1) {
						fil->total_keywords++;
						++count;
					}
			}
		}
		if (count > max_word) {
			line_se = line_max;
			line_max = str;
			max_word = count;
		}
			}
	fin.close();
	if (line_max.length() < 80)
		fil->paragraph = line_max + "..." + line_se;
	else
		if (line_max.length() < 150)
			fil->paragraph = line_max;
		else {
			if (line_max.find(".") != -1 && line_max.find(".") < 150)
				fil->paragraph = line_max.substr(0, line_max.find("."));
			else {
				fil->paragraph = line_max.substr(0, 150);
				std::string pa = fil->paragraph;
				while (pa[pa.length() - 1] != ' ')
					pa.pop_back();
				pa.pop_back();
				fil->paragraph = pa + "...";
			}
		}
	std::vector <std::string> word_para = util::string::Split(util::string::Trim(fil->paragraph));
	std::string para = "";
	
	for (auto wo : word_para) {
		bool ok = false;
		for (auto it : words)
					if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase( it)) != -1)
				ok = true;
		if (ok)
			para += "<h>" + wo + "</> ";
		else
			para += wo + " ";
	}
	para.pop_back();
	fil->paragraph = para;
	return *fil;
}
