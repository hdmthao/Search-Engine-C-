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


bool Engine::StopSpeller() {
    SAFE_DELETE(speller);

    return true;
}
