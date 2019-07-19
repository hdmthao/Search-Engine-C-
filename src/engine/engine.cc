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
    searcher = new Searcher();
    return true;
}

// free memory
bool Engine::Exit() {
    SAFE_DELETE(searcher);

    return true;
}

//Searcher
// bool Engine::Loading(LoadUI* loadui) {
//     std::vector<std::string> file_list;

//     for (int i = 0; i < 100; ++i)
//     file_list.push_back(config::path::DATA);

//     int rate = file_list.size() / 100;

//     for (int i = 1; i <= file_list.size(); ++i) {
//         BuildSearcher(file_list[i - 1]);
//         if (i >= rate && i % rate == 0) {
//             loadui->Draw();
//         }
//     }
    
//     return true;
// }

bool Engine::Loading() {
    std::vector<std::string> file_list;
    util::time::timer::SetTime();
    file_list = util::file::GetListNewsFromFile(config::path::NEWS);

    int rate = file_list.size() / 100;

    for (int i = 1; i <= file_list.size(); ++i) {
        BuildSearcher(file_list[i - 1]);
    }

    std::cout << "Build trie take: " << util::time::timer::GetTimeInterval() << "\n";
    return true;
}

bool Engine::BuildSearcher(const std::string &path_to_file) {
    std::string path = "data/" + path_to_file;
    std::ifstream fi(path);
    if (!fi.is_open()) {
        std::cout << "Can not read " << path_to_file << "\n";
        return false;
    }

    std::string s;
    while (fi >> s) {
        searcher->AddData(s);
    }
    fi.close();

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
    // Init Suggester
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
    // Free memory
    SAFE_DELETE(suggester);

    return true;
}