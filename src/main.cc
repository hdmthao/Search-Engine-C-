#include "migo.h"
#include "util/util.h"
#include <iostream>

int main() {
    Migo* app = nullptr;
    std::cerr << "Log";
    app = new Migo();

    app->Run();

    SAFE_DELETE(app);
}