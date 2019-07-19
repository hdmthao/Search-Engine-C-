#ifndef MIGO_H
#define MIGO_H

#include "engine/engine.h"
#include "ui/ui.h"


enum StateMigo {
    Loading = 101,
    Running = 200,
    Stopping = 404,
};


class Migo {
private:
    Engine* engine;
    UI* ui;
    StateMigo state;

    bool StartUI();
    bool StopUI();
    bool StartEngine();
    bool StopEngine();

    bool Loading();

public:
    Migo();
    ~Migo();

    bool Run();
};

#endif