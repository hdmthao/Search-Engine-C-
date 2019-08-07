#ifndef SEARCH_UI
#define SEARCH_UI

#include "../window/window.h"
#include "../../command/command.h"

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
    Window* msg_win;

    bool DestroyWin(WINDOW* win, int height, int width);
    bool CreateBoxSearch(int size);
    bool DrawCommand();
    std::vector<std::string> Split(const std::string &, int );
public:
    Window* history_win;

    SearchUI();
    ~SearchUI();


    bool DrawLogo();
    bool Start();
    bool Draw(std::vector<std::string> &menu, int choose, SearchCommand command);
    bool ShowUpdate(bool up_to_date, int total_file_update, double time); 
    bool ShowHistory(std::vector<std::string> &list, int &page, int &total_page);
    bool Stop();

};

#endif