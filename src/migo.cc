#include "migo.h"
#include "util/util.h"

#include <exception>
#include <iostream>


Migo::Migo() : engine(nullptr), ui(nullptr), state(StateMigo::Loading) { }

Migo::~Migo() {
    SAFE_DELETE(engine);
    SAFE_DELETE(ui);
}

// The main loop
bool Migo::Run() {
    
    if (StartUI() == false) { // Something wrong with ui
        std::cout << "Something wrong with UI\n";
        return false;
    }
    if (StartEngine() == false) { // Something wrong with engine
        std:: cout << "Some thing wrong with Engine\n";
        return false;
    }

    if (state == StateMigo::Loading) {
        Loading();
        state = StateMigo::Running;
    }


    while (state != StateMigo::Stopping) {
        if (state == StateMigo::Running) {
            Running();
        }
        state = StateMigo::Stopping;
    }

    if (state == StateMigo::Stopping) {
        StopUI();
        StopEngine();
    }
    
    return  true;
}


bool Migo::Loading() {
    ui->StartUILoading();
    engine->Loading(ui->load_ui);
    util::input::Update(-1);
    ui->StopUILoading();
    return true;
}


bool Migo::Running() {
    ui->StartSearching(engine);
    ui->RunSearching();
    ui->StopSearching();
    return true;
}


bool Migo::StartUI() {
    ui = new UI();
    if (ui->Start() == false) {
        return false;
    } else {
        return true;
    }
}


bool Migo::StopUI() {
    ui->Stop();
    SAFE_DELETE(ui);
    return true;
}

bool Migo::StartEngine() {
    engine = new Engine();
    engine->Init();
    return true;
}

bool Migo::StopEngine() {
    engine->Exit();
    SAFE_DELETE(engine);
    return true;
}