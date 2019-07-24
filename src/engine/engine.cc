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

    int rate = file_list.size() / 100;

    for (int i = 1; i <= file_list.size(); ++i) {
        BuildSearcher(file_list[i - 1]);
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

bool Engine::BuildSearcher(const std::string &path_to_file) {
    std::string path = "data/" + path_to_file;
    std::ifstream fi(path);
    if (!fi.is_open()) {
        std::cout << "Can not read " << path_to_file << "\n";
        return false;
    }
    // std::ofstream fo("log.txt", std::ios::app);
    std::string s;
    while (fi >> s) {
        // fo << path_to_file <<  s << "\n";
        searcher->AddData(s);
    }

    // fo.close();
    fi.close();

    return true;
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

bool Engine::CheckSpell(const std::string &origin, std::string &fix) {
    // return True if the origin string was fix atleast once word
    bool spell_status = speller->Check(origin, fix);

    return spell_status;
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