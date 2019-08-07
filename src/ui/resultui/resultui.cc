#include "resultui.h"
#include "../../util/util.h"


ResultUI::ResultUI() : search_win(nullptr), logo_win(nullptr), statistic_win(nullptr), result_win(nullptr) {}

ResultUI::~ResultUI() {}

void ResultUI::Start() {
    logo_win = new Window(4, 12, 1, 2);
    search_win = new Window(4, 103, 1, 12);
    statistic_win = new Window(3, 130, 5, 12);
    result_win = new Window(32, 130, 8, 12);
    page_win = new Window(34, 140, 6, 12);
    command_win = new Window(2, COLS - 4, 41, 1);

    refresh();
    DrawCommand(0); // result screen;
    DrawLogo();
}


void ResultUI::DrawLogo() {
    std::vector<std::string> logo = {
" _  '_  ",   
"//)/(/()",
"   _/   "
};
    wattron(logo_win->win, A_BOLD | COLOR_PAIR(17));
    for (int i = 0; i < logo.size(); ++i) {
        mvwaddstr(logo_win->win, i, 0, logo[i].c_str());
    }
    wattroff(logo_win->win, A_BOLD | COLOR_PAIR(17));

    logo_win->Refresh();
}


void ResultUI::Draw(const std::string &query, SearchResult* result, int &choose, ResultCommand &command) {
    //
    int cur_y = 0, cur_x = 0;
    DrawSearchBox(query, command, cur_y, cur_x);

    if (command == ResultCommand::SelectStatistic)
        DrawStatistic(result, true);
    else
        DrawStatistic(result, false);

    if (command == ResultCommand::SelectResult)
        DrawResult(result, true, choose);
    else
        DrawResult(result, false, choose);

    if (command == ResultCommand::SelectSearchBox) {
        curs_set(1);
        wmove(search_win->win, cur_y, cur_x + 2);
        search_win->Refresh();
    } else {
        curs_set(0);
    }

    return;
}


bool ResultUI::DrawResult(SearchResult* result, bool choose, int pos) {
    result_win->Reset(false);
    int offset = 1;
    int len = result->result_list.size();
    int stop = std::max(0, (pos - 6)) + 5;
    for (int i = 0 + std::max(0, (pos - 6)); i < std::min(len, stop); ++i) {
        if (pos - 2 == i) wattron(result_win->win, A_BOLD);
        // wattron(result_win->win, A_BOLD);
        ResultInfo news = result->result_list[i];
        // std::string info_ = "File name: "  + news.file_name + " ("  + std::to_string(news.total_keywords) +  " keywords / " + std::to_string(news.total_words) + " words)";
        std::string info = "File name: " + news.file_name + " (Score: " + std::to_string(news.score) + " )";
        mvwaddstr(result_win->win, offset++, 1, info.c_str());
        std::string title = ">> " + news.title;
        std::vector<std::string> line = util::string::DivideToLine(title, result_win->w - 1);
        for (int k = 0; k < line.size(); ++k) {
            // printf("%s***", line[k].c_str());
            int padding = 1, j = 0, len = line[k].length();
            while (j <  len) {
                if (j + 1 < len && line[k][j] == '<'  && line[k][j + 1] == '>') {
                    wattron(result_win->win, A_BOLD | COLOR_PAIR(9));
                    // wattron(result_win->win, A_REVERSE);
                    j += 2;
                } else if (j + 2 < len && line[k][j] == '<' && line[k][j + 1] == '/' && line[k][j + 2] == '>' ) {
                    if (pos - 2 != i) wattroff(result_win->win, A_BOLD | COLOR_PAIR(9)); else wattroff(result_win->win, COLOR_PAIR(9));
                                        // wattroff(result_win->win, A_REVERSE);

                    j += 3;
                } else {
                    mvwaddch(result_win->win, offset, padding++, line[k][j]);
                    j++;
                }
            }
            offset++;
        }  
        // mvwaddstr(result_win->win, offset++, 1, news.title.c_str());
        line = util::string::DivideToLine(news.paragraph, result_win->w - 1);
        for (int k = 0; k < line.size(); ++k) {
            int padding = 1, j = 0, len = line[k].length();
            while (j <  len) {
                if (j + 1 < len && line[k][j] == '<'  && line[k][j + 1] == '>') {
                    wattron(result_win->win, COLOR_PAIR(9));
                    j += 2;
                } else if (j + 2 < len && line[k][j] == '<' && line[k][j + 1] == '/' && line[k][j + 2] == '>' ) {
                    wattroff(result_win->win, COLOR_PAIR(9));
                    j += 3;
                } else {
                    mvwaddch(result_win->win, offset, padding++, line[k][j]);
                    j++;
                }
            }
            offset++;
        }
        // mvwaddstr(result_win->win, offset++, 1, news.paragraph.c_str());
        offset+=2;
        wattroff(result_win->win, A_BOLD);
    }
    result_win->Refresh();
    return true;
}

bool ResultUI::DrawSearchBox(const std::string &query, ResultCommand &command, int &cur_y, int &cur_x) {
    search_win->Reset(false);
    if (command == ResultCommand::SelectSearchBox || command == ResultCommand::SelectAll) {
        wattron(search_win->win, A_BOLD);
        if (command == ResultCommand::SelectAll) wattron(search_win->win, COLOR_PAIR(61));
    }
    std::vector<std::string> list_sub_query = util::string::DivideToLine(query, search_win->w - 3);
    int offset = 1;
    for (int i = 0; i < list_sub_query.size(); ++i) {
        mvwaddstr(search_win->win, offset++, 2, list_sub_query[i].c_str());
        cur_x = list_sub_query[i].length();
    }
    wattroff(search_win->win, A_BOLD | COLOR_PAIR(61));
    if (offset == 1) offset++;
    cur_y = offset - 1;
    CreateBox(search_win->win, offset - 1);
    return true;
}

bool ResultUI::DrawStatistic(SearchResult *result, bool choose) {
    statistic_win->Reset(false);
    if (choose) {
        wattron(statistic_win->win, A_BOLD);
    }
    int offset = 0;
    if (result->hint != "") {
        std::string hint = "Did you mean: " + result->hint + "?";
        std::vector<std::string> list_sub_hint = util::string::DivideToLine(hint, statistic_win->w - 2);
        for (int i = 0; i < list_sub_hint.size(); ++i) {
            mvwaddstr(statistic_win->win, offset++, 2, list_sub_hint[i].c_str());
        }
        if (result->is_search_insted) {
            std::string msg = "Showing results for: " + result->highlight_query;
            list_sub_hint = util::string::DivideToLine(msg, statistic_win->w - 2);
            for (int i = 0; i < list_sub_hint.size(); ++i) {
                mvwaddstr(statistic_win->win, offset++, 2, list_sub_hint[i].c_str());
            }   
        }
    }
    std::string statistic = "About " + std::to_string(result->total_result) + " (" + std::to_string(result->time_estimation) + " Seconds)";
    mvwaddstr(statistic_win->win, offset++, 2, statistic.c_str());
    wattroff(statistic_win->win, A_BOLD);
    statistic_win->Refresh();
    return true;
}


bool ResultUI::CreateBox(WINDOW* win, int size) {
    mvwaddch(win, 0, 0, ACS_ULCORNER); // up left corner
    mvwhline(win, 0, 1, ACS_HLINE, 1000); // up horizontal
    mvwaddch(win, 0, search_win->w - 1, ACS_URCORNER); // right corner
    mvwaddch(win, size + 1, 0, ACS_LLCORNER); // down left corner
    mvwhline(win, size + 1, 1, ACS_HLINE, 1000); // down horiontal
    mvwaddch(win, size + 1, search_win->w - 1, ACS_LRCORNER); // down right corner
    mvwvline(win, 1, 0, ACS_VLINE, size); // left vertical
    mvwvline(win, 1, search_win->w - 1, ACS_VLINE, size); // right vertical
    wrefresh(win);
    return true;
}

bool ResultUI::ShowPage(std::vector<std::string> &news, std::string &title, int &page, int &total_page) {
    page_win->Reset(false);
    // Box
    wattron(page_win->win, COLOR_PAIR(25));
    box(page_win->win, 0, 0);
    wattroff(page_win->win, COLOR_PAIR(25));
    wattron(page_win->win, A_BOLD);

    // Print title
    mvwaddstr(page_win->win, 0, (page_win->w - title.length()) / 2, title.c_str());
    // print page number
    std::string page_number = "(Page " + std::to_string(page) + " of " + std::to_string(total_page) + ")";
    mvwaddstr(page_win->win, 0, (page_win->w - page_number.size() - 2), page_number.c_str());
    // print content
    int max_line = 28, offset = 2;
    int start = (page - 1) * max_line;
    int stop = std::min(start + max_line, (int)news.size());
    for (int i = start; i < stop; ++i) {
        mvwaddstr(page_win->win, offset++, 2, news[i].c_str());
    }
    wattroff(page_win->win, A_BOLD);
    page_win->Refresh();
    return true;
}

void ResultUI::ClearPage() {
    page_win->Reset(true);
}

bool ResultUI::DrawCommand(int kind) {
    command_win->Reset(false);
    std::vector<std::string> list_description;
    std::vector<std::string> list_command;

    if (kind == 0) { // command result screen
        list_description.push_back("Select Next"); list_command.push_back("Key Down");
        list_description.push_back("Select Previoust"); list_command.push_back("Key Up");
        list_description.push_back("Select all Query"); list_command.push_back("^a");
        list_description.push_back("Back to Search"); list_command.push_back("Esc");
        list_description.push_back("Exit"); list_command.push_back("^c");

    } else if (kind == 1) {
        list_description.push_back("Page Next"); list_command.push_back("Key Down / Key Right");
        list_description.push_back("Page Previous"); list_command.push_back("Key Up / Key Left");
        list_description.push_back("Back to Result"); list_command.push_back("Esc");
        list_description.push_back("Exit"); list_command.push_back("^c");
    }
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

void ResultUI::Stop() {
    logo_win->DestroyWin();
    result_win->DestroyWin();
    statistic_win->DestroyWin();
    page_win->DestroyWin();
    command_win->DestroyWin();
    search_win->DestroyWin();
    
    SAFE_DELETE(logo_win);
    SAFE_DELETE(result_win);
    SAFE_DELETE(statistic_win);
    SAFE_DELETE(page_win);
    SAFE_DELETE(command_win);
    SAFE_DELETE(search_win);
}