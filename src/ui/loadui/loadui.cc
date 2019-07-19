#include "loadui.h"
#include <iostream>
#include <vector>
#include <string>

LoadUI::LoadUI() : percent_done(0), title_win(nullptr), progress_bar(nullptr) {}

LoadUI::~LoadUI() {
}

void LoadUI::Start() {
    progress_bar = newwin(4, 102, 30, 17);
    title_win = newwin(8, 49, 15, 43);
    refresh();

// print title
    std::vector<std::string> title = {
"XXXX        XXX   XXXX    XXXXXXX      XXXXXXXX",
"XX X       X XX    XX    XX     XX    XX      XX",
"XX  X     X  XX    XX   XX       XX   XX      XX",
"XX   X   X   XX    XX   XX	       XX      XX",
"XX    XXX    XX    XX   XX            XX      XX",
"XX           XX    XX   XX     XXXX   XX      XX",
"XX           XX    XX    XX   XX XX   XX      XX",
"XX           XX   XXXX    XXXX   XX    XXXXXXXX",
};
    wattron(title_win, A_BOLD);
    for (int i = 0; i < title.size(); ++i) {
        mvwaddstr(title_win, i,0,title[i].c_str());
    }
    wrefresh(title_win);


    // print layout progress bar
    wattron(progress_bar, COLOR_PAIR(57) | A_BOLD);
    mvwaddch(progress_bar, 0, 0, ACS_LTEE);
    mvwaddch(progress_bar, 0, 101, ACS_RTEE);

    mvwaddstr(progress_bar, 3, 38, "Loading...");
    wrefresh(progress_bar);
    // wattroff(progress_bar, A_BLINK);
}


void LoadUI::Draw() {
    percent_done++;
    mvwaddch(progress_bar, 0, percent_done, ACS_CKBOARD);
    wrefresh(progress_bar);
}

void LoadUI::Stop() {
    DestroyWin(progress_bar, 4, 102);
    DestroyWin(title_win, 8, 49);
}

void LoadUI::DestroyWin(WINDOW* win, int h, int w) {
    for(int j = 0; j < h; ++j)
	for(int i = 0; i < w; ++i)
	    mvwaddch(win, j, i, ' ');
				
	wrefresh(win);
	delwin(win);
}