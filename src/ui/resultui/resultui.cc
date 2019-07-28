#include "resultui.h"
#include "../../util/util.h"


ResultUI::ResultUI() : search_win(nullptr), logo_win(nullptr), statistic_win(nullptr), result_win(nullptr) {}

ResultUI::~ResultUI() {}

void ResultUI::Start() {
    logo_win = new Window(4, 12, 1, 2);
    search_win = new Window(4, 103, 1, 12);
    statistic_win = new Window(3, 103, 5, 12);
    result_win = new Window(34, 130, 8, 12);
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


void ResultUI::Draw(SearchResult* result, int &choose, ResultCommand &command) {
    //
    if (command == ResultCommand::SelectSearchBox)
        DrawSearchBox(result, true);
    else
        DrawSearchBox(result,false);

    if (command == ResultCommand::SelectStatistic)
        DrawStatistic(result, true);
    else
        DrawStatistic(result, false);

    if (command == ResultCommand::SelectResult)
        DrawResult(result, true, choose);
    else
        DrawResult(result, false, choose);

    return;
}


bool ResultUI::DrawResult(SearchResult* result, bool choose, int pos) {
    int offset = 1;
    for (int i = 0; i < result->result_list.size(); ++i) {
        if (pos - 2 == i) wattron(result_win->win, A_BOLD);
        ResultInfo news = result->result_list[i];
        std::string info_ = "File name: "  + news.file_name + " ("  + std::to_string(news.total_keywords) +  " keywords / " + std::to_string(news.total_words) + " words)";
        mvwaddstr(result_win->win, offset++, 1, info_.c_str());
        mvwaddstr(result_win->win, offset++, 1, news.title.c_str());
        mvwaddstr(result_win->win, offset++, 1, news.paragraph.c_str());
        offset+=2;
        wattroff(result_win->win, A_BOLD);
    }
    result_win->Refresh();
    return true;
}

bool ResultUI::DrawSearchBox(SearchResult *result, bool choose) {
    if (choose) {
        wattron(search_win->win, A_BOLD);
    }
    std::vector<std::string> list_sub_query = util::string::DivideToLine(result->query, search_win->w - 3);
    int offset = 1;
    for (int i = 0; i < list_sub_query.size(); ++i) {
        mvwaddstr(search_win->win, offset++, 2, list_sub_query[i].c_str());
    }
    CreateBox(search_win->win, offset - 1);
    wattroff(search_win->win, A_BOLD);
    return true;
}

bool ResultUI::DrawStatistic(SearchResult *result, bool choose) {
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

void ResultUI::Stop() {
    logo_win->DestroyWin();
    result_win->DestroyWin();
    statistic_win->DestroyWin();
    search_win->DestroyWin();
    SAFE_DELETE(logo_win);
    SAFE_DELETE(result_win);
    SAFE_DELETE(statistic_win);
    SAFE_DELETE(search_win);
}