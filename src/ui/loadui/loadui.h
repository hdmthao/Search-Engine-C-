#ifndef LoadUI_H
#define LoadUI_H

#include <ncurses.h>

class LoadUI {
private:
    WINDOW* progress_bar;
    WINDOW* title_win;
    int percent_done;
    void DestroyWin(WINDOW* win, int h, int w);
    
public:
    LoadUI();
    ~LoadUI();

    void Start();
    void Stop();
    void Draw();
};

#endif