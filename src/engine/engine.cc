#include "engine.h"
#include "../util/util.h"

#include <iostream>
#include <fstream>

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
    file_list = GetListNewsFromFile(config::path::NEWS);
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
    util::time::timer::SetTime();
    SearchResult* result = new SearchResult();
    
    int total_result = 0;
    result->query = query;
    result->hint = speller->GetHint(query);
    std::vector<std::pair<int, double>> result_id_list = searcher->GetResultWithNormalSearch(query, total_result);
    result->total_result = total_result;
    std::ofstream fo("log_engine.txt");

    for (auto news : result_id_list) {

        std::string file_name = doc_map[news.first];
                fo << file_name << "\n";
        ResultInfo info = searcher->HighlightResult(query, file_name, news.first);
        // ResultInfo info;
        info.score = news.second;
        result->result_list.push_back(info);
    }
    result->time_estimation = util::time::timer::GetTimeInterval();
    suggester->SaveQuery(query );
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

std::vector<std::string> Engine::GetListNewsFromFile(const std::string &path) {
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
