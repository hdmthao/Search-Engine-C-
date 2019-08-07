#include "migo.h"
#include "util/util.h"

#include <exception>
#include <iostream>


Migo::Migo() : engine(nullptr), ui(nullptr) { }

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

    Loading();

    Running();

    StopUI();
    StopEngine();
    
    return  true;
}


bool Migo::Loading() {
    ui->StartUILoading();
    engine->Loading(ui->load_ui);
    util::input::Update(-1);
    if (util::input::IsPressed(util::input::ctrl('p'))) {
        double time = 0;
        engine->TestQuery(time);
        ui->load_ui->ShowTest(time);
        util::input::Update(-1);
    }
    ui->StopUILoading();
    return true;
}


bool Migo::Running() {
    ui->Init(engine);
    state = StateMigo::Search;
    std::string query = "";
    while (1) {
        if (state == StateMigo::Search) {
            ui->StartSearching();
            query = "";
            ui->GetQuery(state, query);
            ui->StopSearching();
        } else if (state == StateMigo::Result) {
            ui->StartUIResult();
            ui->ShowResult(state, query);
            ui->StopUIResult();
        } else if (state == StateMigo::ChangeMode) {
            config::privacy::SetMode();
            state = StateMigo::Search;
        } else {
            break;
        }
    }
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