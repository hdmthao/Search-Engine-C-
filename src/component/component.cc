#include "component.h"


ResultInfo::ResultInfo() : title(""), file_name(""), total_keywords(0), total_words(0), paragraph("") {}

ResultInfo::~ResultInfo() {}

SearchResult::SearchResult() : query(""), hint(""), total_result(0), time_estimation(0) {}

SearchResult::~SearchResult() {}