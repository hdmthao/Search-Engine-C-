#include "component.h"


ResultInfo::ResultInfo() : title(""), file_name(""), score(0), total_keywords(0), paragraph("") {}

ResultInfo::~ResultInfo() {}

SearchResult::SearchResult() : query(""), highlight_query(""), hint(""), total_result(0), time_estimation(0), is_search_insted(false) {}

SearchResult::~SearchResult() {}

SearchTypeInfo::SearchTypeInfo() : word_1(""), word_2(""), query(""), range_1(0), range_2(0),word_exclude(""),word_include("") {}

SearchTypeInfo::~SearchTypeInfo() {}