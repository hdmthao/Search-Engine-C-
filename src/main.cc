#include "migo.h"
#include "util/util.h"


int main() {
    Migo* app = nullptr;
    app = new Migo();

    app->Run();

    SAFE_DELETE(app);
}