#ifndef MIGO_H
#define MIGO_H

#include "engine/engine.h"
#include "ui/ui.h"
#include "command/command.h"

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
    bool Running();
    
public:
    Migo();
    ~Migo();

    bool Run();
};

#endif