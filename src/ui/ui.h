#ifndef UI_H
#define UI_H

#include "loadui/loadui.h"
#include "searchui/searchui.h"
#include "resultui/resultui.h"
#include "../engine/engine.h"
#include "../command/command.h"

#include <ncurses.h>


class UI {
private:
    int MAX_WIDTH, MAX_HEIGHT;
    WINDOW* main_win;
    
    Engine* engine;
    bool TurnOnColor();
    void DestroyWin(WINDOW* win, int h, int w);
    bool GetEventInSearchScreen(std::string &query, std::vector<std::string> &suggest_list, int &choose, SearchCommand &command);
    bool GetEventInResultScreen(std::string &query, std::vector<std::string> &suggest_list, SearchResult* result, int &choose, ResultCommand &command);
    bool GetEventWhenOpenPage(ResultCommand &command, int &page, int &total_page);

public:
    LoadUI* load_ui;
    SearchUI* search_ui;
    ResultUI* result_ui;
    UI();
    ~UI();

    bool Start();
    void Stop();
    bool Init(Engine* engine);

    // Loading
    void StartUILoading();
    void StopUILoading();

    // Running
    void StartSearching();
    void GetQuery(StateMigo &state, std::string &query);
    void StopSearching();

    // Result
    void StartUIResult();
    void ShowResult(StateMigo &state, std::string &query);
    void StopUIResult();

};

#endif