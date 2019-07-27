#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>

struct ResultInfo {
	std::string title;
	int total_keywords;
	std::string paragraph;
	
	ResultInfo();
	~ResultInfo();
};

struct SearchResult {
	int total_result;
	double time_estimation;
	std::string query;
	std::string hint;
	std::vector<ResultInfo> result_list;

	SearchResult();
	~SearchResult();
};

#endif