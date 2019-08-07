#ifndef LoadUI_H
#define LoadUI_H

#include <ncurses.h>
#include "../window/window.h"

class LoadUI {
private:
    Window* progress_bar;
    Window* title_win;
    Window* info_win;
    Window* abc_win;
    int percent_done;
    
public:
    LoadUI();
    ~LoadUI();

    void Start();
    void Stop();
    void Draw(int total_file, float time);
    void ShowTest(double &);
};

#endif