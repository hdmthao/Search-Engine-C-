#include "searchui.h"
#include "../../util/util.h"
#include "../../config.h"

#include <iostream>

QueryComponent::QueryComponent() : is_history(false), is_highlight(false), query("") {}

QueryComponent::~QueryComponent() {}


SearchUI::SearchUI() : search_win(nullptr), title_win(nullptr) {}

SearchUI::~SearchUI() {
}

bool SearchUI::Start() {
    search_win = new Window(26, 84, 14, (COLS - 80) / 2);
    title_win = new Window(5, 36, 6, (COLS - 22) / 2);
    command_win = new Window(2, COLS - 4, 41, 1);
    msg_win = new Window(6, 50, 34, (COLS - 50) / 2);
    history_win = new Window(34, 140, 6, 12);

    refresh();
    DrawLogo();
    DrawCommand();
    CreateBoxSearch(1);
    return true;
}


bool SearchUI::Draw(std::vector<std::string> &menu, int choose, SearchCommand command) {
    search_win->Reset(false);
    if (menu.size() == 0) return true;
    int offset = 1;
    int padding = 2;
    int cur_y = 0, cur_x = 0;
    // current query
    if (command == SearchCommand::SlectAllCurrentQuery) {
        wattron(search_win->win, COLOR_PAIR(61));
    }
    wattron(search_win->win, A_BOLD);
    std::vector<std::string> list_sub_query = util::string::DivideToLine(menu[choose], search_win->w - 3);
    for (int i = 0; i < list_sub_query.size(); ++i) {
        mvwaddstr(search_win->win, offset++, padding, list_sub_query[i].c_str());
        cur_x = list_sub_query[i].length();
    }
    if (list_sub_query.size() == 0) offset++;
    cur_y = offset - 1;
    wattroff(search_win->win,  COLOR_PAIR(61));
    mvwhline(search_win->win, offset++, padding-1, '.', 1000); //horizontal for fist block
    wattroff(search_win->win, A_BOLD);
    
    // suggest list
    wattron(search_win->win, A_INVIS);
    for (int i = 1; i < menu.size(); ++i) {
        if (choose == i) {
            wattron(search_win->win, A_STANDOUT);
        }
        int len = menu[i].size();
        int left = 0;
        std::vector<std::string> list_sub_query = util::string::DivideToLine(menu[i], search_win->w - 3);
        for (int j = 0; j < list_sub_query.size(); ++j) {
            mvwaddstr(search_win->win, offset++, padding, list_sub_query[j].c_str() );
        }
        wattroff(search_win->win, A_STANDOUT);
        if (i + 1 < menu.size()) {
            mvwhline(search_win->win, offset++, padding-1, ACS_HLINE, 1000); //horizontal for sparate
        }
    }
    wattroff(search_win->win, A_INVIS);
    
    CreateBoxSearch(offset - 1);
    wmove(search_win->win, cur_y, cur_x + 2);

    search_win->Refresh();
    return true;
}

bool SearchUI::CreateBoxSearch(int size) {
    mvwaddch(search_win->win, 0, 0, ACS_ULCORNER); // up left corner
    mvwhline(search_win->win, 0, 1, ACS_HLINE, 1000); // up horizontal
    mvwaddch(search_win->win, 0, search_win->w - 1, ACS_URCORNER); // right corner
    mvwaddch(search_win->win, size + 1, 0, ACS_LLCORNER); // down left corner
    mvwhline(search_win->win, size + 1, 1, ACS_HLINE, 1000); // down horiontal
    mvwaddch(search_win->win, size + 1, search_win->w - 1, ACS_LRCORNER); // down right corner
    mvwvline(search_win->win, 1, 0, ACS_VLINE, size); // left vertical
    mvwvline(search_win->win, 1, search_win->w - 1, ACS_VLINE, size); // right vertical
    search_win->Refresh();
    return true;
}

bool SearchUI::DrawLogo() {
    std::vector<std::string> title = {
" _      _  _____ ____",
"/ \\__/|/ \\/  __//  _ \\",
"| |\\/||| || |  _| / \\|",
"| |  ||| || |_//| \\_/|",
"\\_/  \\|\\_/\\____\\\\____/",
};
    wattron(title_win->win, A_BOLD);
    for (int i = 0; i < title.size(); ++i) {
        mvwaddstr(title_win->win, i,0,title[i].c_str());
    }
    if (config::privacy::private_mode) {
        mvwaddstr(title_win->win, title.size() - 1, title.back().size() + 2, "Private Mode");
    }
    title_win->Refresh();

    return true;
}

bool SearchUI::DrawCommand() {
    std::vector<std::string> list_description = {
        "Search Query", "Update Data", "Remove Selected History", "Show History",
    };
    std::vector<std::string> list_command = {
        "^M/Enter", "^u", "^x", "^g",
    };

    if (config::privacy::private_mode) {
        list_description.push_back("Normal Mode"); list_command.push_back("^n");
    } else {
        list_description.push_back("Private Mode"); list_command.push_back("^n");
    }
    list_description.push_back("Exit"); list_command.push_back("^c");

    int y_offset = 1, x_offset = (command_win->w - 1) / 8;
    int x_pos = x_offset;

    for (int i = 0; i < list_command.size(); ++i) {
        if (i == 8) y_offset++, x_offset = x_offset;
        wattron(command_win->win, A_STANDOUT | A_BOLD);
        mvwaddstr(command_win->win, y_offset, x_pos, list_command[i].c_str());
        wattroff(command_win->win, A_STANDOUT);
        mvwaddstr(command_win->win, y_offset, x_pos + list_command[i].size() + 1, list_description[i].c_str());
        x_pos += list_command[i].size() + list_description[i].size() + 6;
        wattroff(command_win->win, A_BOLD);
    }
    command_win->Refresh();
    return true;
}

bool SearchUI::ShowUpdate(bool update, int total_file_update, double time_estimate) {
    curs_set(0);
    box(msg_win->win, 0, 0);
    std::string title = "Update News";
    std::string info = "";
    std::string time = "";
    if (!update) {
        info = "Everything is up to date!!!";
    } else {
        info = "Updated " + std::to_string(total_file_update) + " News!!!";
        time = std::to_string(time_estimate);
    }
    mvwaddstr(msg_win->win, 0, (msg_win->w - title.size()) / 2, title.c_str());
    mvwaddstr(msg_win->win, 2, (msg_win->w - info.size()) / 2, info.c_str());
    mvwaddstr(msg_win->win, 3, (msg_win->w - info.size()) / 2, time.c_str());
    msg_win->Refresh();
    util::input::Update(-1);
    msg_win->Reset(true);
    curs_set(1);
    return true;
}

bool SearchUI::ShowHistory(std::vector<std::string> &list, int &page, int &total_page) {
    history_win->Reset(false);
    // Box
    wattron(history_win->win, COLOR_PAIR(25));
    box(history_win->win, 0, 0);
    wattroff(history_win->win, COLOR_PAIR(25));
    wattron(history_win->win, A_BOLD);

    // Print title
    std::string title = "History";
    mvwaddstr(history_win->win, 0, (history_win->w - title.length()) / 2, title.c_str());
    // print page number
    std::string page_number = "(Page " + std::to_string(page) + " of " + std::to_string(total_page) + ")";
    mvwaddstr(history_win->win, 0, (history_win->w - page_number.size() - 2), page_number.c_str());
    // print content
    int max_line = 28, offset = 2;
    int start = (page - 1) * max_line;
    int stop = std::min(start + max_line, (int)list.size());
    for (int i = start; i < stop; ++i) {
        mvwaddstr(history_win->win, offset++, 2, list[i].c_str());
    }
    wattroff(history_win->win, A_BOLD);
    history_win->Refresh();
    return true;
}

bool SearchUI::Stop() {
    curs_set(0);
    search_win->DestroyWin();
    title_win->DestroyWin();
    command_win->DestroyWin();
    SAFE_DELETE(title_win);
    SAFE_DELETE(command_win);
    SAFE_DELETE(search_win);
    return true;
}
