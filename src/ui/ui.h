#ifndef UI_H
#define UI_H

#include "loadui/loadui.h"

#include <ncurses.h>


class UI {
private:
    int MAX_WIDTH, MAX_HEIGHT;
    WINDOW* main_win;
    bool TurnOnColor();
    void DestroyWin(WINDOW* win, int h, int w);

public:
    LoadUI* load_ui;
    
    UI();
    ~UI();

    bool Start();
    void Stop();

    // Loading
    void StartUILoading();
    void StopUILoading();
};

#endif