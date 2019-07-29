#include "resultui.h"
#include "../../util/util.h"


ResultUI::ResultUI() : search_win(nullptr), logo_win(nullptr), statistic_win(nullptr), result_win(nullptr) {}

ResultUI::~ResultUI() {}

void ResultUI::Start() {
    logo_win = new Window(4, 12, 1, 2);
    search_win = new Window(4, 103, 1, 12);
    statistic_win = new Window(3, 103, 5, 12);
    result_win = new Window(32, 130, 8, 12);
    page_win = new Window(34, 140, 6, 12);
    refresh();

    DrawLogo();
}


void ResultUI::DrawLogo() {
    std::vector<std::string> logo = {
" _  '_  ",   
"//)/(/()",
"   _/   "
};
    for (int i = 0; i < logo.size(); ++i) {
        mvwaddstr(logo_win->win, i, 0, logo[i].c_str());
    }
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

    if (command == ResultCommand::ShowFile) {

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
        ResultInfo news = result->result_list[i];
        // std::string info_ = "File name: "  + news.file_name + " ("  + std::to_string(news.total_keywords) +  " keywords / " + std::to_string(news.total_words) + " words)";
        std::string info = "File name: " + news.file_name + " (Score: " + std::to_string(news.score) + " )";
        mvwaddstr(result_win->win, offset++, 1, info.c_str());
        std::vector<std::string> line = util::string::DivideToLine(news.title, result_win->w - 1);
        for (int k = 0; k < line.size(); ++k) {
            // printf("%s***", line[k].c_str());
            int padding = 1, j = 0, len = line[k].length();
            while (j <  len) {
                if (j + 1 < len && line[k][j] == '<'  && line[k][j + 1] == '>') {
                    wattron(result_win->win, COLOR_PAIR(61));
                    j += 2;
                } else if (j + 2 < len && line[k][j] == '<' && line[k][j + 1] == '/' && line[k][j + 2] == '>' ) {
                    wattroff(result_win->win, COLOR_PAIR(61));
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
                    wattron(result_win->win, COLOR_PAIR(61));
                    j += 2;
                } else if (j + 2 < len && line[k][j] == '<' && line[k][j + 1] == '/' && line[k][j + 2] == '>' ) {
                    wattroff(result_win->win, COLOR_PAIR(61));
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
    cur_y = offset - 1;
    wattroff(search_win->win, A_BOLD | COLOR_PAIR(61));

    CreateBox(search_win->win, offset - 1);
    return true;
}

bool ResultUI::DrawStatistic(SearchResult *result, bool choose) {
    statistic_win->Reset(false);
    if (choose) {
        wattron(statistic_win->win, A_BOLD);
    }
    int offset = 1;
    if (result->hint != "") {
        std::string hint = "Did you mean: " + result->hint;
        std::vector<std::string> list_sub_hint = util::string::DivideToLine(hint, statistic_win->w - 3);
        for (int i = 0; i < list_sub_hint.size(); ++i) {
            mvwaddstr(statistic_win->win, offset++, 2, list_sub_hint[i].c_str());
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

bool ResultUI::ShowPage(std::vector<std::string> &news, int &page, int &total_page) {
    page_win->Reset(false);
    box(page_win->win, 0, 0);
    page_win->Refresh();
}

void ResultUI::ClearPage() {
    page_win->Reset();
}
void ResultUI::Stop() {
    logo_win->DestroyWin();
    result_win->DestroyWin();
    statistic_win->DestroyWin();
    page_win->DestroyWin();
    search_win->DestroyWin();
    
    SAFE_DELETE(logo_win);
    SAFE_DELETE(result_win);
    SAFE_DELETE(statistic_win);
    SAFE_DELETE(page_win);
    SAFE_DELETE(search_win);
}