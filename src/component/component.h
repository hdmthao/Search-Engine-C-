#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>
#include "../command/command.h"

struct ResultInfo {
	int total_keywords;
	double score;
	std::string title;
	std::string file_name;
	std::string paragraph;
	std::vector<std::string> keywords_notFound;
	
	ResultInfo();
	~ResultInfo();
};

struct SearchResult {
	int total_result;
	double time_estimation;
	bool is_search_insted;
	std::string query;
	std::string highlight_query;
	std::string hint;
	std::vector<ResultInfo> result_list;

	SearchResult();
	~SearchResult();
};

struct SearchTypeInfo {
	SearchType type;

	std::string word_1; // for "and, or"
	std::string word_2;
	std::string word_exclude;
	std::string word_include;
	std::string query;
	int range_1;
	int range_2;
	SearchTypeInfo();
	~SearchTypeInfo();
};

#endif