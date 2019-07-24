#ifndef UI_H
#define UI_H

#include "loadui/loadui.h"
#include "../engine/engine.h"
#include "searchui/searchui.h"
#include "command.h"

#include <ncurses.h>


class UI {
private:
    int MAX_WIDTH, MAX_HEIGHT;
    WINDOW* main_win;
    
    Engine* engine;
    bool TurnOnColor();
    void DestroyWin(WINDOW* win, int h, int w);
    bool GetEventInSearchScreen(std::string &query, std::vector<std::string> &suggest_list, int &choose, SearchCommand &command);

public:
    LoadUI* load_ui;
    SearchUI* search_ui;

    UI();
    ~UI();

    bool Start();
    void Stop();

    // Loading
    void StartUILoading();
    void StopUILoading();

    // Running
    void StartSearching(Engine* engine);
    void RunSearching();
    void StopSearching();

};

#endif