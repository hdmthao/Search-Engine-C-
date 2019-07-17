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
bool Engine::BuildSearcher() {
    std::ifstream fi(config::path::DATA);

    if (!fi.is_open()) {
        // Log Here
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