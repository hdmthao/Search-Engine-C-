#include "config.h"
#include "util/util.h"
#include "engine/engine.h"
#include <iostream>
#include "speller/speller.h"

// Test speller here

bool Test_Speller(Engine* &engine) {
    engine->StartSpeller();
    // TEst Here
    engine->StopSpeller();
    return true;
}


int main() {
    // test
    Engine* engine = new Engine();

    engine->Init();
    engine->BuildSearcher();
    Test_Speller(engine);
    engine->Exit();
    SAFE_DELETE(engine);
}
