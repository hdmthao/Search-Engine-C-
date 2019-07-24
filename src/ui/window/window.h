#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

class Window {
private:

public:
    int h, w;
    int start_y, start_x;
    WINDOW* win;
    
    Window(int h, int w, int start_y, int start_x);
    ~Window();
    
    bool Refresh();
    bool Reset(bool is_refresh);
    bool DestroyWin();

};

#endif