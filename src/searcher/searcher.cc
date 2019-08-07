// Implement for searcher tree (actually a trie)

#include "searcher.h"
#include "../util/util.h"
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

using util::string::isNum_string;

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
std::vector<std::pair<int, double>> Searcher::GetResultWithExcludeSearch(const std::string &origin_query, const std::string &exclude_word) {
	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);
	// std::ofstream fo("log_searcher.txt");
		
	query = util::string::RemoveStopWord(query);
	if (query.empty()) {
		std::string tmp_query = util::string::Trim(origin_query);
		tmp_query = util::string::RemoveMark(tmp_query);
		if (!tmp_query.empty()) query = tmp_query;
	}
	std::vector<std::string> word_list = util::string::Split(query);
	std::map<int, double> rank;
	// fo << total_worlds << " " << total_news << "\n";
	// fo << query << "\n";
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
	std::vector<SearcherNode> list;
	std::vector<int> list_doc;
	SearcherNode* cur_node = GetNode(exclude_word);
	if (cur_node != nullptr) {
		list.push_back(*cur_node);
		list_doc = GetChung(list);
	}
	std::set<int> list_id;
	for (auto id : list_doc) list_id.insert(id);
	std::set<std::pair<int, double>, comp> set_result;

	for (auto doc: rank) {
		int id = doc.first;
		double score = doc.second;
		if (list_id.find(id) != list_id.end()) continue;
		set_result.insert({id, score});
	}
	for (auto doc : set_result) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}
	return result_list;
}


std::vector<std::pair<int, double>> Searcher::GetResultWithAndSearch(const std::string &origin_query) {
	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);

	query = util::string::RemoveStopWord(query);
	std::vector<std::string> word_list = util::string::Split(query);
	std::map<int, double> rank;

	float avgdll = total_worlds / total_news;
	std::vector<SearcherNode> list;

	for (auto word : word_list) {
		SearcherNode* cur_node = GetNode(word);
		if (cur_node == nullptr) continue;
		list.push_back(*cur_node);
	}
	std::vector<int> list_doc = GetChung(list);

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
	std::set<std::pair<int, double>, comp> set_result;

	for (auto id: list_doc) {
		set_result.insert({id, rank[id]});
	}
	for (auto doc : set_result) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}

	return result_list;
}

std::vector<std::pair<int, double>> Searcher::GetResultWithExaclytWord(const std::string &origin_query) {
	std::vector<std::pair<int, double>> result_list;

	std::string query = util::string::Normalize(origin_query);

	std::vector<std::string> word_list = util::string::Split(query);
	std::map<int, double> rank;

	float avgdll = total_worlds / total_news;
	std::vector<SearcherNode> list;

	for (auto word : word_list) {
		SearcherNode* cur_node = GetNode(word);
		if (cur_node == nullptr) continue;
		list.push_back(*cur_node);
	}
	std::vector<std::pair<int, int>> list_doc = GetExact(list);

	for (auto doc : list_doc) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}

	return result_list;	
}

std::vector<std::pair<int, double>> Searcher::GetResultWithWildcard(const std::string &origin_query) {
	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);

	std::vector<std::string> word_list = util::string::Split(query);
	std::map<int, double> rank;
	float avgdll = total_worlds / total_news;
	std::vector<SearcherNode> list;
	
	for (auto word : word_list) {
		if (word == "*") {
			SearcherNode* cur_node = new SearcherNode();
			cur_node->posting_list[-1].push_back(-1);
			list.push_back(*cur_node);
		} else {
			SearcherNode* cur_node = GetNode(word);
			if (cur_node == nullptr) continue;
			list.push_back(*cur_node);
		}
	}

	std::vector<std::pair<int, int>> list_doc = GetWildcard(list);

	for (auto doc : list_doc) {
		result_list.push_back({doc.first, doc.second});
	}

	return result_list;
}


std::vector<std::pair<int, double>> Searcher::GetResultWithIncludeSearch(const std::string &origin_query, const std::string &include_word) {

	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);
	std::ofstream fo("log_searcher.txt");
		
	query = util::string::RemoveStopWord(query);
	query = " " + include_word;
	if (query.empty()) {
		std::string tmp_query = util::string::Trim(origin_query);
		tmp_query = util::string::RemoveMark(tmp_query);
		if (!tmp_query.empty()) query = tmp_query;
	}
	std::vector<std::string> word_list = util::string::Split(query);
	// for (auto word : word_list) {
	// 	fo << word << "\n";
	// }
	fo.close();
	std::map<int, double> rank;
	// fo << total_worlds << " " << total_news << "\n";
	// fo << query << "\n";
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
	for (auto doc : set_result) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}
	// fo.close();
	return result_list;
}
std::vector<std::pair<int, double>> Searcher::GetResultWithNormalSearch(const std::string &origin_query) {

	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);
	std::ofstream fo("log_searcher.txt");
		
	query = util::string::RemoveStopWord(query);
	if (query.empty()) {
		std::string tmp_query = util::string::Trim(origin_query);
		tmp_query = util::string::RemoveMark(tmp_query);
		if (!tmp_query.empty()) query = tmp_query;
	}
	std::vector<std::string> word_list = util::string::Split(query);
	// for (auto word : word_list) {
	// 	fo << word << "\n";
	// }
	fo.close();
	std::map<int, double> rank;
	// fo << total_worlds << " " << total_news << "\n";
	// fo << query << "\n";
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
	for (auto doc : set_result) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}
	// fo.close();
	return result_list;
}

std::vector<std::pair<int, double>> Searcher::GetResultWithTitleSearch(const std::string &origin_query, std::unordered_map<int, std::string> &doc_map) {

	std::vector<std::pair<int, double>> result_list;
	std::string query = util::string::Normalize(origin_query);
	std::ofstream fo("log_searcher.txt");
		
	// query = util::string::RemoveStopWord(query);
	if (query.empty()) {
		std::string tmp_query = util::string::Trim(origin_query);
		tmp_query = util::string::RemoveMark(tmp_query);
		if (!tmp_query.empty()) query = tmp_query;
	}
	std::vector<std::string> word_list = util::string::Split(query);
	// for (auto word : word_list) {
	// 	fo << word << "\n";
	// }
	fo.close();
	std::map<int, double> rank;
	// fo << total_worlds << " " << total_news << "\n";
	// fo << query << "\n";
	float avgdll = total_worlds / total_news;

	for (auto word : word_list) {
		SearcherNode* cur_node = GetNode(word);
		if (cur_node == nullptr) continue;
		double idf_ = idf(total_news, cur_node->posting_list.size());
		for (auto it : cur_node->posting_list) {
			int doc_id = it.first;
			if (!CheckTitle(doc_map[doc_id], word)) continue;
			int D = nword[doc_id];
			rank[it.first] += BM25(idf_, (int)it.second.size(), avgdll, D);
		}
	}
	std::set<std::pair<int, double>, comp> set_result(
			rank.begin(), rank.end());
	for (auto doc : set_result) {
		// fo << doc.first << " " << doc.second << "\n";
		result_list.push_back({doc.first, doc.second});
	}
	// fo.close();
	return result_list;
}

bool Searcher::CheckTitle(std::string &file_name, std::string &word) {
	std::ifstream fi("data/" + file_name);
	std::string title;
	while (!fi.eof()) {
		getline(fi, title);
		std::vector<std::string> line = util::string::Split(title);

		int count = 0;
		for (auto w : line) {
			count += w.length();
			if (count >= 120) break;
			if (util::string::ToLowerCase(w) == word) return true;
		}
		break;
	}
	return false;
}

double Searcher::idf(const int& N, const int& nqi) {
	return log10((N - nqi + 0.5 + 1) / (nqi + 0.5));
}

double Searcher::BM25(const double& idf, const int& tf, const double& avgdl, const int& D, const double& k1, const double& b){
	return idf * (tf * (k1 + 1)) / (tf + k1 * (1 - b + b * D/avgdl) + 1);
}

ResultInfo Searcher::HighlightResult(const std::string &s, const std::string &filename)
{
	std::ifstream fin("data/" + filename);
	std::ofstream fo("log_highlight.txt");
	ResultInfo *fil = new ResultInfo();
	// fil->file_name = filename;
	// fil->total_words = nword[id];
	// fil->paragraph = "blabalbalbal";
	// fo << filename << "\n";
	// fo.close();
	// fil->keywords_notFound.push_back("x");
	std::string st = s;
	st = util::string::RemoveUnicode(st);
	st = util::string::RemoveStopWord(st);
	st = util::string::Trim(st);
	std::vector <std::string> words = util::string::Split(st);
	
	long long max_word = 0;
	std::string line_max="";
	std::string line_se="";
	while (!fin.eof())
	{
		std::string str;

		long long count = 0;
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
			if (util::string::ToLowerCase(str).find(util::string::ToLowerCase(*it)) != -1) {
				for (auto wo : word_str)
					if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase(*it)) != -1) {
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
	// fixed
	if (line_max.length() < 200) {
		if (line_se!="" )
			fil->paragraph = line_se + "..." + line_max;
		else
			fil->paragraph =line_max;
	}
	//fixed
	else
		if (line_max.length() < 300)  //fixed
			fil->paragraph = line_max;
		else {
			if (line_max.find(".") != -1 && line_max.find(".") < 300 && line_max.find(".")>200)  //fixed
				fil->paragraph = line_max.substr(0, line_max.find("."));
			else {
				fil->paragraph = line_max.substr(0, 300);  //fixed
				std::string pa = fil->paragraph;
				while (pa[pa.length() - 1] != ' ')
					pa.pop_back();
				pa.pop_back();
				fil->paragraph = pa + "...";
			}
		}
	bool fikey = false;
	for (auto wo:words)
		if (util::string::ToLowerCase(fil->paragraph).find(util::string::ToLowerCase(wo)) != -1)
		{
			fikey = true;
			break;
		}
	if (!fikey)
	{
		for (auto wo : words)
			if (util::string::ToLowerCase(line_max).find(util::string::ToLowerCase(wo)) != -1) {
				int des = util::string::ToLowerCase(line_max).find(util::string::ToLowerCase(wo));
				fil->paragraph = line_max.substr(des-80, line_max.length());  //fixed
				des = fil->paragraph.find(" ");
				fil->paragraph = fil->paragraph.substr(des+1, 300);  //fixed
				std::string pa = fil->paragraph;
				while (pa[pa.length() - 1] != ' ')
					pa.pop_back();
				pa.pop_back();
				fil->paragraph = "..."+pa + "...";
				break;
			}
	}
	// // //fixed
	std::ifstream fi("data/" + filename);
	std::string line = "";
	while (fil->paragraph.length() < 150 &&(line.find(line_max) == -1) && !fi.eof())
	{
		getline(fi, line);
		line = util::string::RemoveUnicode(line);
	}

	while (!fi.eof() && fil->paragraph.length() < 150)
	{
		getline(fi, line);
		line = util::string::RemoveUnicode(line);
		line = util::string::Trim(line);
		fil->paragraph += " "+line;
	}
	fi.close();
	if (fil->paragraph.length() > 300){
		fil->paragraph = fil->paragraph.substr(0, 300);
	std::string pa = fil->paragraph;
	while (pa[pa.length() - 1] != ' ')
		pa.pop_back();
	pa.pop_back();
	fil->paragraph = pa + "...";
	}
	// //fixed

	std::vector <std::string> word_para = util::string::Split(util::string::Trim(fil->paragraph));
	// std::string para = "";
	
	// for (auto wo : word_para) {
	// 	bool ok = false;
	// 	int des = -1;
	// 	int lens = 0;
	// 	for (auto it : words)
	// 		if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it)) != -1 && des == -1)  {
	// 			ok = true;
	// 		if(des==-1)
	// 			des = util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it));
	// 			lens += it.length();
		
	// 		}
	// 	if (ok) {
	// 		wo.insert(des, "<>");
	// 		wo.insert(des + lens+2, "</>");
	// 		para += wo + " "; 
	// 	}
	// 	else
	// 		para += wo + " ";
	// }
	// para.pop_back();
	// fil->paragraph = para;

	std::string para = "";
	
	for (auto wo : word_para) {
		bool ok = false;
		int des = -1;
		int lens = 0;
		for (auto it : words)
			if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it)) != -1)  {
				if (!isNum_string(wo) && des != util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it))) {
					ok = true;
					//if(des==-1)
					des = util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it));
					lens = it.length();//+
					(wo).insert(des, "<>");
					(wo).insert(des + lens + 2, "</>");
				}
				else if (isNum_string(wo) && wo == it) {
					des = util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it));
					lens = it.length();//+
					(wo).insert(des, "<>");
					(wo).insert(des + lens + 2, "</>");
					ok = false;
				}
		
			}
		if (ok) {
			// wo.insert(des, "<>");
			// wo.insert(des + lens+2, "</>");
			para += wo + " "; 
		}
		else
			para += wo + " ";
	}
	para.pop_back();
	fil->paragraph = para;

	std::vector <std::string> word_tit = util::string::Split(util::string::Trim(fil->title));
	std::string tit = "";

	for (auto wo : word_tit) {
		bool ok = false;
		int des = -1;
		int lens = 0;
		for (auto it : words)
			if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it)) != -1 && des == -1) {
				ok = true;
				//if(des==-1) 
				des = util::string::ToLowerCase(wo).find(util::string::ToLowerCase(it));
				lens= it.length();
				fo << des << " -> " << lens << "\n";
				wo.insert(des, "<>");
				wo.insert(des + lens + 2 , "</>");
			}
		// if (ok) {
			
		// 	tit += wo + " ";
		// }
		// else
		tit += wo + " ";
	}
	tit.pop_back();
	fil->title = tit;
	// //add
	for (auto wo : words)
	{
		std:: ifstream inf("data/" + filename);
		std::string str;
		str = util::string::RemoveUnicode(str);
		bool ok = false;
		while (!inf.eof()) {
			getline(inf, str);
			if (util::string::ToLowerCase(str).find(util::string::ToLowerCase(wo)) != -1) {
				ok = true;
				break;
			}
		}
		inf.close();
		if (!ok)
			fil->keywords_notFound.push_back(wo);
	}
	fo << fil->title << "\n";
	fo << fil->paragraph << "\n";
	fo << fil->file_name << "\n";
	fo.close();
	return *fil;
}


std::vector <std::string> Searcher::GetSynonym(const std::string &origin_word) {
	std::vector<std::string> result;
	result.push_back(origin_word);
	std::ifstream inp;
	inp.open(("synonym_data/" + origin_word));
	if (inp.is_open()) {
		std::string synonym_new;
		while (getline(inp, synonym_new)) {
			if (synonym_new != origin_word)
				result.push_back(synonym_new);
		}
	}
	inp.close();
	// note : chuan hoa lai data
	//      : loai bo stopword
	return result;
}


bool Searcher::cmp(std::pair<int, int> x, std::pair<int, int> y) {
	return x.second > y.second;
}


std::vector< std::pair<int, int> > Searcher::GetExact(std::vector<SearcherNode> &wordList){
	std::vector< std::pair<int, int> > result;
	std::unordered_map<int, std::vector<int>>::iterator it, it1;
	for (it = wordList.begin()->posting_list.begin(); it != wordList.begin()->posting_list.end(); it++) {
		bool flag = 0;// find for orther word;
		for (int i = 0; i < wordList.size(); i++) {
			if (wordList[i].posting_list.find(it->first) == wordList[i].posting_list.end()){
				flag = 1;
				break;
			}
		}
		if (flag)
			continue;
		result.push_back(std::make_pair(it->first, 0));
		for (int i = 0; i < it->second.size(); i++) {
			flag = 1;
			for (int j = 1; j < wordList.size(); j++) {
				std::vector<int>::iterator pos;
				pos = std::lower_bound(wordList[j].posting_list[it->first].begin(), wordList[j].posting_list[it->first].end(), it->second[i] + j);
				if (pos == wordList[j].posting_list[it->first].end() || wordList[j].posting_list[it->first][pos-wordList[j].posting_list[it->first].begin()] != it->second[i] + j) {
					flag = 0;
					break;
				}
			}
			result.back().second += flag;
		}
		if (result.back().second == 0)
			result.pop_back();
	}
	sort(result.begin(), result.end(), cmp);
	return result;
}

std::vector<int> Searcher::GetChung(std::vector<SearcherNode> &wordList) {
	std::vector<int> result;
	std::unordered_map<int, std::vector<int>>::iterator it;

	for (it = wordList.begin()->posting_list.begin(); it != wordList.begin()->posting_list.end(); it++) {
		bool flag = 0;// find for orther word;
		for (int i = 0; i < wordList.size(); i++) {
			if (wordList[i].posting_list.find(it->first) == wordList[i].posting_list.end()){
				flag = 1;
				break;
			}
		}
		if (flag)
			continue;
		result.push_back(it->first);
	}
	return result;
}

std::string Searcher::GetSynonym_string(const std::string &originword) {
	std::vector<std::string> result = GetSynonym(originword);
	std::string st = "";
	for (auto re : result)
		st += re + " ";
	st.pop_back();
	return st;
}


std::vector< std::pair<int, int> > Searcher::GetWildcard(std::vector<SearcherNode> &wordList){
	std::vector< std::pair<int, int> > result;
	std::unordered_map<int, std::vector<int>>::iterator it, it1;
	for (it = wordList.begin()->posting_list.begin(); it != wordList.begin()->posting_list.end(); it++) {
		bool flag = 0;// find for orther word;
		for (int i = 0; i < wordList.size(); i++) {
			if (wordList[i].posting_list.find(-1) == wordList[i].posting_list.end() && wordList[i].posting_list.find(it->first) == wordList[i].posting_list.end()){
				flag = 1;
				break;
			}
		}
		if (flag)
			continue;
		result.push_back(std::make_pair(it->first, 0));
		for (int i = 0; i < it->second.size(); i++) {
			flag = 1;
			for (int j = 1; j < wordList.size(); j++) {
				if (wordList[j].posting_list.find(-1) != wordList[j].posting_list.end())
					continue;
				std::vector<int>::iterator pos;
				pos = std::lower_bound(wordList[j].posting_list[it->first].begin(), wordList[j].posting_list[it->first].end(), it->second[i] + j);
				if (pos == wordList[j].posting_list[it->first].end() || wordList[j].posting_list[it->first][pos- wordList[j].posting_list[it->first].begin()] != it->second[i] + j) {
					flag = 0;
					break;
				}
			}
			result.back().second += flag;
		}
		if (result.back().second == 0)
			result.pop_back();
	}
	sort(result.begin(), result.end(), cmp);
	return result;
}