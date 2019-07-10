#include "config.h"
#include "util/util.h"
#include "engine/engine.h"
#include <iostream>

int main() {
    // test
    Engine* engine = new Engine();

    engine->Init();
    engine->Exit();
    SAFE_DELETE(engine);
}
