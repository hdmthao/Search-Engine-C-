#ifndef SEARCH_UI
#define SEARCH_UI

#include "../window/window.h"
#include "../command.h"

#include <ncurses.h>
#include <vector>
#include <string>


class QueryComponent {
public:
    bool is_history;
    bool is_highlight;
    std::string query;

    QueryComponent();
    ~QueryComponent();
};

class SearchUI {
private:
    Window* search_win;
    Window* title_win;
    Window* command_win;
    bool DestroyWin(WINDOW* win, int height, int width);
    bool CreateBoxSearch(int size);
    bool DrawLogo();
    bool DrawCommand();
    std::vector<std::string> Split(const std::string &, int );
public:
    SearchUI();
    ~SearchUI();

    bool Start();
    bool Draw(std::vector<std::string> &menu, int choose, SearchCommand command);
    bool Stop();

};

#endif