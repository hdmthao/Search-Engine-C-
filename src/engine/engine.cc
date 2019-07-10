#include "engine.h"
#include "../util/util.h"


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
