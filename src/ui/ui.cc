#include "ui.h"
#include "../util/util.h"

#include <iostream>

UI::UI() : MAX_WIDTH(0), MAX_HEIGHT(0), main_win(nullptr), load_ui(nullptr)
{}

UI::~UI() {
}

bool UI::Start() {
    initscr();
    raw();

    if (!TurnOnColor()) {
        return false;
    }

    getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
    main_win = newwin(MAX_HEIGHT, MAX_WIDTH, 0, 0);
    box(main_win, 0, 0);
    refresh();
    wrefresh(main_win);

    return true;
}

bool UI::TurnOnColor() {
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        return false;
    }
    
    start_color();
    int k = 1;
	for (int i = COLOR_BLACK; i <= COLOR_WHITE; i++)
	{
		for (int j = COLOR_BLACK; j <= COLOR_WHITE; j++)
		{
			init_pair(k, i, j);
			k++;
		}
	}
    return true;
}


void UI::Stop() {
    DestroyWin(main_win, MAX_HEIGHT, MAX_WIDTH);
    endwin();
}


void UI::DestroyWin(WINDOW* win, int h, int w) {
    for(int j = 0; j < h; ++j)
	for(int i = 0; i < w; ++i)
	    mvwaddch(win, j, i, ' ');
				
	wrefresh(win);
	delwin(win);
}

// Loading Screen
void UI::StartUILoading() {
    load_ui = new LoadUI();
    load_ui->Start();
}

void UI::StopUILoading() {
    load_ui->Stop();
    SAFE_DELETE(load_ui);
}