#include "engine.h"
#include "../util/util.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using util::string::isNum;
using util::string::range_string;

// constructor engine
Engine::Engine() : searcher(nullptr) {
    
}

// deconstructor for engine
Engine::~Engine() {

}

// Init engine
bool Engine::Init() {
    StartSearcher();
    StartSpeller();
    StartSuggester();
    return true;
}

// free memory
bool Engine::Exit() {
    StopSuggester();
    StopSpeller();
    StopSearcher();
    return true;
}

bool Engine::Loading(LoadUI* loadui) {
    std::vector<std::string> file_list;
    util::time::timer::SetTime();
    file_list = GetListNewsFromIndexFile(config::path::NEWS);
    searcher->Init((int)file_list.size());
    int rate = file_list.size() / 100;
    
    for (int i = 1; i <= file_list.size(); ++i) {
        int id = i - 1;
        doc_map[id] = file_list[i - 1];
        BuildSearcher(file_list[i - 1], id);
        if (i >= rate && i % rate == 0) {
            loadui->Draw(i, util::time::timer::GetTimeInterval());
        }
    }
    
    return true;
}

//Searcher
bool Engine::StartSearcher() {
    searcher = new Searcher();
    return true;
}

bool Engine::BuildSearcher(const std::string &path_to_file, int &doc_id) {
    std::string path = "data/" + path_to_file;
    std::ifstream fi(path);
    if (!fi.is_open()) {
        std::cout << "Can not read " << path_to_file << "\n";
        return false;
    }
    // std::ofstream fo("log.txt");
    std::string s;
    int position = 1;
    std::vector<std::string> word_list;
    while (fi >> s) {
        // fo <<  util::string::Normalize(s) << " " << count_index << "\n";
        s = util::string::Normalize(s);
        word_list = util::string::Split(s);
        for (int i =0; i < word_list.size(); ++i) {
            // fo << word_list[i] << " " << position << "\n";
            searcher->Insert(word_list[i], doc_id, position);
            position++;
        }
    }

    // fo.close();
    fi.close();

    return true;
}

SearchResult *Engine::Search(const std::string &query) {
    std::ofstream fo("log_engine.txt", std::ios::app);
    // fo << query << "\n";
    // fo.close();
    util::time::timer::SetTime();
    SearchTypeInfo *op = Migo_OP(query);
    SearchResult* result = new SearchResult();
    std::vector<std::pair<int, double>> result_id_list;
    result->query = query;
    result->highlight_query = query;
    fo << (int)op->type << "\n";
    fo << op->query << "\n";
    if (op->type == SearchType::Normal) result->hint = speller->GetHint(query);

    if (op->type == SearchType::Normal) {
        result_id_list = searcher->GetResultWithNormalSearch(query);
    } else if (op->type == SearchType::And) {
        std::string tmp_query = op->word_1 + " " + op->word_2;
        result_id_list = searcher->GetResultWithAndSearch(tmp_query);
    } else if (op->type == SearchType::Or) {
        result_id_list = searcher->GetResultWithNormalSearch(query);
    } else if (op->type == SearchType::Exclude) {
        result_id_list = searcher->GetResultWithExcludeSearch(op->query, op->word_exclude);
    } else if (op->type == SearchType::Intitle) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithTitleSearch(op->query, doc_map);
    } else if (op->type == SearchType::Include) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithIncludeSearch(op->query, op->word_include);
    } else if (op->type == SearchType::Filetype) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithNormalSearch(op->query);
    } else if (op->type == SearchType::Price) {
        result_id_list = searcher->GetResultWithNormalSearch(query);
    } else if (op->type == SearchType::Hashtag) {
        result_id_list = searcher->GetResultWithNormalSearch(query);
    } else if (op->type == SearchType::Exact) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithExaclytWord(op->query);
    } else if (op->type == SearchType::Wildcard) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithWildcard(op->query);
    } else if (op->type == SearchType::Range_num) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithNormalSearch(op->query);
    } else if (op->type == SearchType::Range_usd) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithNormalSearch(op->query);
    } else if (op->type == SearchType::Range_pc) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithNormalSearch(op->query);
    } else if (op->type == SearchType::Synonym) {
        result->highlight_query = op->query;
        result_id_list = searcher->GetResultWithNormalSearch(op->query);
    }
    result->total_result = result_id_list.size();
    if (result->total_result == 0 && !result->hint.empty()) {
        result_id_list = searcher->GetResultWithNormalSearch(result->hint);
        result->total_result = result_id_list.size();
        if (result->total_result > 0) {
            result->is_search_insted = true;
            result->highlight_query = result->hint; 
        }
    }
    int count = 0, limit_highlight = 5;
    for (auto news : result_id_list) {
        std::string file_name = doc_map[news.first];
        ResultInfo info;
        if (count < limit_highlight)
            info = searcher->HighlightResult(result->highlight_query, file_name);
        info.file_name = file_name;
        info.score = news.second;
        result->result_list.push_back(info);
        count++;
    }

    if (!config::privacy::private_mode) suggester->SaveQuery(query);
    result->time_estimation = util::time::timer::GetTimeInterval();
    // fo << result->result_list[4].paragraph << "\n";
    // fo  << result->time_estimation << "\n";
    fo.close();
    return result;
}

bool Engine::StopSearcher() {
    SAFE_DELETE(searcher);
    return true;
}

// Speller
bool Engine::StartSpeller() {
    speller = new Speller(searcher);
    return true;
}

bool Engine::StopSpeller() {
    SAFE_DELETE(speller);
    return true;
}

// Sugester
bool Engine::StartSuggester() {
    suggester = new Suggester();
    return true;
}

std::vector<std::string> Engine::GetSuggest(const std::string &origin_query) {
    // Return atleast 3 history query, query suggest base Frecency
    std::vector<std::string> suggest_list;
    suggest_list = suggester->GetSuggest(origin_query);

    return suggest_list;
}

bool Engine::StopSuggester() {
    SAFE_DELETE(suggester);
    return true;
}

std::vector<std::string> Engine::GetListNewsFromIndexFile(const std::string &path) {
    std::vector<std::string> file_list;
    std::ifstream fi(path);
    if (!fi.is_open()) {
        std::cout << "Not exist data file\n";
        return file_list; // just empty
    }
    std::string file_name;
    while (getline(fi, file_name)) {
        file_list.push_back(file_name);
    }
    fi.close();
    return file_list;
}

std::vector<std::string> Engine::GetNews(std::string &file_name, int &size) {
    std::vector<std::string> news;
    // news.push_back("test test test test test test test test");
    std::ifstream fi("data/" + file_name);
    if (!fi.is_open()) {
        news.push_back("Opps! Page Not Found...");
        return news;
    }
    std::string pre_line = "";
    while (!fi.eof()) {
        std::string line;
        getline(fi, line);
        if (line == "" && pre_line == "") continue; 
        line = util::string::RemoveUnicode(line);
        std::vector<std::string> list_line = util::string::DivideToLine(line, size);
        for (int i = 0; i < list_line.size(); ++i) {
            news.push_back(list_line[i]);
        }
        pre_line = news.back();
    }
    if (!news.empty() && news.back() == "") news.pop_back();
    if (news.empty()) {
        news.push_back("Sorry! This file is empty...");
    } 
    return news;
}

std::vector<std::string> Engine::GetHistory() {
    std::vector<std::string> list;
    std::vector<std::pair<int, std::string>> list_all;

    std::ifstream fi(config::path::HISTORY);
    
    if (!fi.is_open()) {
        std::cout << "Cannot open history file\n";
        return list;
    }
    
    std::string query = "";
    std::string rank_s = "";
    std::string time_s = "";
    while (getline(fi, query, ',')) {
        getline(fi, rank_s,',');
        getline(fi, time_s);
        int time = std::stoul(time_s);
        time = -time;
        // std::cout << query << " " << rank << " " << time << "\n";
        list_all.push_back({time, query});
    }
    sort(list_all.begin(), list_all.end());
    fi.close();
    for (auto info : list_all) {
        list.push_back(info.second);
    }
    return list; 
}

bool Engine::RemoveHistory(std::string &query) {
    suggester->RemoveQuery(query);
	std ::ifstream fin(config::path::HISTORY);
	std::ofstream fout("tmp.txt");
	std::string str, tmp_1, tmp;

	if (!fin.is_open()) {
		std::cout << "Cannot open history file\n";
		return false;
	}

	while (!fin.eof())
	{
		getline(fin, str,',');
        getline(fin, tmp, ',');
        getline(fin, tmp_1);
		if (str != query && str != "")
		{
			fout << str << "," << tmp << "," << tmp_1 << std::endl;
		}
	}
	fin.close();
	fout.close();

    std::ifstream fi("tmp.txt");
	std::ofstream fo(config::path::HISTORY);
	while (!fi.eof())
	{
		getline(fi, str,',');
        getline(fi, tmp, ',');
        getline(fi, tmp_1);
        if (str != "") 
			fo << str << "," << tmp << "," << tmp_1 << std::endl;
	}
	fi.close();
	fo.close();

    return true;
}

bool Engine::UpdateFile(int &total_file_update, double &time) {
    // Có sẵn hàm GetListNewsFromIndexFile để lấy ra hết tất cả news
    // có cái doc_map là để lưu lại cặp (id, file_name)
    
    // return true nếu có file update và update xong
    // return false nếu ko có file để update
    util::time::timer::SetTime();
    std::vector<std::string> list = GetListNewsFromIndexFile(config::path::NEWS);

    if (list.size() == doc_map.size()) return false;
    total_file_update = list.size() - doc_map.size();
    int i = doc_map.size();
    while (i < list.size()) {
        doc_map[i] = list[i];
        BuildSearcher(list[i], i);
        i++;
    }
    time = util::time::timer::GetTimeInterval();
    return true;
}

SearchTypeInfo* Engine::Migo_OP(const std::string &query) {
	SearchType type=SearchType::Normal;
	SearchTypeInfo*inFo = new SearchTypeInfo;
	std::string st = query;
	inFo->query = query;
	st = util::string::RemoveUnicode(st);
	st = util::string::Trim(st);
	std::vector<std::string> words = util::string::Split(st);
	for (auto wo : words) {
		if (wo == "AND") {
			inFo->word_1 = words[0];
			inFo->word_2 = words[2];
			type = SearchType::And;
			break;
		}
		else
			if (wo == "OR") {
				inFo->word_1 = words[0];
				inFo->word_2 = words[2];
				type = SearchType::Or;
				break;
			}
			else
				if (wo.find("-") == 0 && wo.length()>1) {
                    int des = st.find(wo);
					st.erase(des, wo.length());
					wo.erase(0, 1);
					inFo->word_exclude = util::string::ToLowerCase(wo);
					inFo->query = st;
					type = SearchType::Exclude;
					break;
				}
				else
					if (wo.find("+") == 0 && wo.length() > 1) {
						wo.erase(0, 1);
                        st.erase(st.find("+"),1);
                        inFo->query=st;
						inFo->word_include = util::string::ToLowerCase(wo);
						type = SearchType::Include;
						break;
					}
					else
						if (util::string::ToLowerCase(wo).find("intitle:") == 0 && wo.length()>8 ) {
							st.erase(st.find("intitle"), 8);
							inFo->query = util::string::ToLowerCase(st);
							type = SearchType::Intitle;
							break;
						}
						else
							if (util::string::ToLowerCase(wo).find("filetype:") == 0 && wo.length()>9) {
								wo.erase(0, 9);
								inFo->query = util::string::ToLowerCase(wo);
								type = SearchType::Filetype;
								break;
							}
							else
								if (wo.find("~") == 0) {
                                    int des = st.find(wo);
									st.erase(des, wo.length());
									wo.erase(0, 1);
                                    wo = util::string::ToLowerCase(wo);
									wo = searcher->GetSynonym_string(wo)+" ";
									st.insert(des, wo);
									inFo->query = st;
									type = SearchType::Synonym;
									break;
								}
								else
									if (wo.find("..") != -1) {
										int des = wo.find("..");
										std::string s1 = wo.substr(0, des);
										std::string s2 = wo.substr(des + 2, wo.length() - des - 2);

										bool ok = true;
										if (s1 == "" || s2 == "")
											ok = false;
										for (int i = 1; i < s1.length()-2; i++)
											if (!isNum(s1[i])) {
												ok = false;
												break;
											}
										for (int i = 1; i < s2.length() - 2; i++)
											if (!isNum(s2[i])) {
												ok = false;
												break;
											}

										if (ok)
										{
											if (isNum(s1[0])&&isNum(s1[s1.length()-1]) && isNum(s2[0]) && isNum(s2[s2.length() - 1])) {
												inFo->range_1 = std::stoi(s1);
												inFo->range_2 = std::stoi(s2);
												type = SearchType::Range_num;

												st.erase(st.find(wo), wo.length());
												st += " " + range_string(inFo->range_1, inFo->range_2, "", 0);
												inFo->query = st;
												break;

											}
											if (s1[0]=='$'&&isNum(s1[s1.length()-1])&& s2[0] == '$'&&isNum(s2[s2.length() - 1])) {
                                                st.erase(st.find(wo), wo.length());
												s1.erase(0, 1);
												s2.erase(0, 1);
												inFo->range_1 = std::stoi(s1);
												inFo->range_2 = std::stoi(s2);
												type = SearchType::Range_usd;
                                                st += " " + range_string(inFo->range_1, inFo->range_2, "$", 1);
												inFo->query = st;
												break;
											}
											if (isNum(s1[0]) && (s1[s1.length() - 1]=='%') && isNum(s2[0]) && (s2[s2.length() - 1]=='%') ) {
												inFo->range_1 = std::stoi(s1);
												inFo->range_2 = std::stoi(s2);
												type = SearchType::Range_pc;
                                                st.erase(st.find(wo), wo.length());
												st += " " + range_string(inFo->range_1, inFo->range_2, "%", 0);
												inFo->query = st;
												break;
											}
											if (isNum(s1[0]) && (s1[s1.length() - 1] == '$') && isNum(s2[0]) && (s2[s2.length() - 1] == '$')) {
												inFo->range_1 = std::stoi(s1);
												inFo->range_2 = std::stoi(s2);
												type = SearchType::Range_usd;
                                                st.erase(st.find(wo), wo.length());
												st += " " + range_string(inFo->range_1, inFo->range_2, "$", 0);
												inFo->query = st;
												break;
											}
											
										}
										
									}
	}


	if (type == SearchType::Normal) {
		if (st.find("#") != -1)
			type = SearchType::Hashtag;
		else
			if (st.find("$") != -1)
				type = SearchType::Price;
			else
				if (st[0] == '\"'&&st[st.length() - 1] == '\"') {
					st.erase(0, 1);
					st.erase(st.length() - 1, 1);
					inFo->query = st;
					if (st.find("*") != -1)
						type = SearchType::Wildcard;
					else
						type = SearchType::Exact;
				}
	}
	inFo->type = type;
	return inFo;
}

bool Engine::TestQuery(double &time_estimate) {

    std::ifstream fi("data/100query.txt");
    std::ofstream fo("analysic.txt");

    double total = 0;
    while (!fi.eof()) {
        std::string query;
        getline(fi, query);
        if (query == "") continue;
        fo << query << "\n";
        SearchResult* result = Search(query);
        total += result->time_estimation;
        fo << result->time_estimation << " " << result->total_result << "\n";
    }
    fo << total*10;
    time_estimate = total * 10;
    fo.close();
    return true;
}