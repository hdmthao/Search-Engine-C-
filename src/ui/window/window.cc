#include "window.h"

Window::Window(int h, int w, int start_y, int start_x) : h(h), w(w), start_x(start_x), start_y(start_y), win(nullptr) {
    win = newwin(h, w, start_y, start_x);
}

Window::~Window() {}


bool Window::Reset(bool is_refresh = true) {
    for(int j = 0; j < h; ++j)
	for(int i = 0; i < w; ++i)
	    mvwaddch(win, j, i, ' ');
    if (is_refresh) wrefresh(win);
    return true;
}

bool Window::Refresh() {
    wrefresh(win);
    return true;
}
bool Window::DestroyWin() {
    Reset();
    delwin(win);
    return true;
}