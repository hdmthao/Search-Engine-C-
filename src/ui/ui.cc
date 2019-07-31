#include "ui.h"
#include "../util/util.h"

#include <iostream>
#include <fstream>
#include <set>

using namespace util::input;

UI::UI() : MAX_WIDTH(0), MAX_HEIGHT(0), main_win(nullptr), engine(nullptr), load_ui(nullptr), search_ui(nullptr), result_ui(nullptr)
{}

UI::~UI() {
}

bool UI::Start() {
    initscr();
    raw();
    noecho();
    curs_set(0);
    ESCDELAY = 50;
    nodelay(stdscr, true);
    keypad(stdscr, true);
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

bool UI::Init(Engine* engine) {
    this->engine = engine;
    return true;
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

// Running Screen
void UI::StartSearching() {
    search_ui = new SearchUI();

    search_ui->Start();
}

void UI::GetQuery(StateMigo &state, std::string &query) {
    std::string cur_query = "";
    int choose = 0;
    SearchCommand command = SearchCommand::HighlightQuery;
    std::vector<std::string> suggest_list;
    std::vector<std::string> cache_suggest = engine->GetSuggest(cur_query);
    // just wait for 3 secoond, if not key is press, so show best query history
    Update(2000);
    if (IsPressed(ctrl('c'))) {
        state = StateMigo::Stop;
        return;
    }
    curs_set(1);

    char c;
    if (GetIcon(c)) {
        cur_query += c;
    }
    suggest_list = engine->GetSuggest(cur_query);
    suggest_list.insert(suggest_list.begin(), cur_query);

    search_ui->Draw(suggest_list, choose, command);
    while(true) {
        Update(-1);
        bool is_get_event = GetEventInSearchScreen(cur_query, suggest_list, choose, command);
        if (is_get_event) {
            if (cur_query != suggest_list.front()) { 
                if (cur_query != "") {
                    suggest_list = engine->GetSuggest(cur_query);
                } else suggest_list = cache_suggest;
                suggest_list.insert(suggest_list.begin(), cur_query);
            }
        } 
        search_ui->Draw(suggest_list, choose, command);
        if (command == SearchCommand::SearchCurrentQuery) {
            query = suggest_list[choose];
            state = StateMigo::Result;
            return; 
        } else if (command == SearchCommand::Quit) {
            state = StateMigo::Stop;
            return;
        }
    }
}

void UI::StopSearching() {
    search_ui->Stop();
    SAFE_DELETE(search_ui);
}

void UI::StartUIResult() {
    result_ui = new ResultUI();
    result_ui->Start();
}

void UI::ShowResult(StateMigo &state, std::string &query) {
    ResultCommand command = ResultCommand::SelectSearchBox;
    std::set<int> cache;
    std::vector<std::string> suggest_list;
    int choose = 0;
    SearchResult* result;
    curs_set(1);
    result = engine->Search(query);
    cache.insert(0);
    cache.insert(1);
    for (int i = 2; i < std::min(7, 2 + result->total_result); ++i) {
        cache.insert(i);
    }
    result_ui->Draw(query, result, choose, command);
    while (true) {
        Update(-1);
        bool is_get_event = GetEventInResultScreen(query, suggest_list, result, choose, command);
        if (is_get_event) {
            if (cache.find(choose) == cache.end()) {
                std::string file_name = result->result_list[choose - 2].file_name;
                int tmp_total = result->result_list[choose - 2].total_keywords;
                double tmp_score = result->result_list[choose - 2].score;
                ResultInfo info = engine->searcher->HighlightResult(result->highlight_query, file_name);
                info.file_name = file_name;
                info.total_keywords = tmp_total;
                info.score = tmp_score;
                result->result_list[choose - 2] = info;
                cache.insert(choose);
            }
            if (command == ResultCommand::ShowPage) {
                int max_line = 28, max_size = 135;
                std::string file_name = result->result_list[choose - 2].file_name;
                std::vector<std::string> news = engine->GetNews(file_name, max_size);

                int page = 1, total_page = news.size() / max_line + 1;
                result_ui->DrawCommand(1); // command for view file screen
                result_ui->ShowPage(news, file_name, page, total_page);
                while (1) {
                    Update(-1);
                    bool is_get_event = GetEventWhenOpenPage(command, page, total_page);
                    if (is_get_event) {
                        if (command == ResultCommand::SelectResult) {
                            break;
                        }
                        if (command == ResultCommand::Quit) {
                            state = StateMigo::Stop;
                            return;
                        }
                        result_ui->ShowPage(news, file_name, page, total_page);
                    }
                }
                result_ui->ClearPage();
                result_ui->DrawCommand(0); // command for result screen
            } else if (command == ResultCommand::NewSearch) {
                result = engine->Search(query);
                command = ResultCommand::SelectSearchBox;
                cache.clear();
                cache.insert(0);
                cache.insert(1);
            }
        }
        result_ui->Draw(query, result, choose, command);
        if (command == ResultCommand::BackToSearch) {
            state = StateMigo::Search;
            return;
        }
        if (command == ResultCommand::Quit) {
            state = StateMigo::Stop;
            return;
        }
    }
}


void UI::StopUIResult() {
    result_ui->Stop();
    curs_set(0);
    SAFE_DELETE(result_ui);
}

bool UI::GetEventInResultScreen(std::string &query, std::vector<std::string> &suggest_list, SearchResult* result, int &choose, ResultCommand &command)  {

    if (IsPressed(KEY_DOWN)) {
        choose++;
        if (choose - 1 > (int)result->result_list.size()) {
            choose = 0;
        }
    } else if (IsPressed(KEY_UP)) {
        choose--;
        // if (choose < 0) choose = result->result_list.size() + 1;
        if (choose <0) choose = 0;
    } else if (IsPressed('\n') || IsPressed(10)) {// enter
        if (command == ResultCommand::SelectSearchBox) {
            command = ResultCommand::NewSearch;
            choose = 0;
            return true;
        }
        if (command == ResultCommand::SelectResult) {
            command = ResultCommand::ShowPage;
            return true;
        }
        if (command == ResultCommand::SelectStatistic) {
            query = result->hint;
            command = ResultCommand::NewSearch;
            choose = 0;
            return true;
        }
    } else if (IsPressed(127)) { // xoa
        if (command == ResultCommand::SelectAll) {
            query = "";
            command = ResultCommand::SelectSearchBox;
        } else {
            if (!query.empty()) query.pop_back();
        }
        choose = 0;
    } else if (IsPressed(ctrl('c'))) { // Ctrl + c -> quit
        command = ResultCommand::Quit;
        return true;
    } else if (IsPressed(ctrl('a'))) { // Ctrl + a
        command = ResultCommand::SelectAll;
        return true;
    } else if (IsPressed(27)) { // Esc -> return search win
        if (command != ResultCommand::ShowSuggest) {
            command = ResultCommand::BackToSearch;
        } else {
            command = ResultCommand::SelectResult;
        }
        return true;
    } else {
        char c;
        if (!GetIcon(c)) return false;
        if (command == ResultCommand::SelectAll) {
            query = "";
            command = ResultCommand::SelectSearchBox;
        }
        query += c;
        choose = 0;
    }
    if (choose == 0) {
        command = ResultCommand::SelectSearchBox;
    } else if (choose == 1) {
        command = ResultCommand::SelectStatistic;
    } else {
        command = ResultCommand::SelectResult;
    }
    return true;
}

bool UI::GetEventInSearchScreen(std::string &query, std::vector<std::string> &suggest_list, int &choose, SearchCommand &command) {
    if (IsPressed(KEY_DOWN)) { // len va phai
        choose++;
        if (choose > suggest_list.size() - 1) choose = 0;
        command = SearchCommand::HighlightQuery;
    } else if (IsPressed(KEY_UP)) { // xuong  va trai
        choose--;
        if (choose < 0) choose = suggest_list.size() - 1;
        command = SearchCommand::HighlightQuery;
    } else if (IsPressed(127)) { // xoa
        if (command == SearchCommand::SlectAllCurrentQuery) {
            query = "";
            command = SearchCommand::HighlightQuery;
        } else {
            query = suggest_list[choose];
            if (!query.empty()) query.pop_back();
        }
        choose = 0;
    } else if (IsPressed('\n') || IsPressed(10)) { // enter
        command = SearchCommand::SearchCurrentQuery;
    } else if (IsPressed(ctrl('a'))) { // ctrl + a 
        command = SearchCommand::SlectAllCurrentQuery;
    } else if (IsPressed(ctrl('c'))) {
        command = SearchCommand::Quit;
    } else {
        char c;
        if (!GetIcon(c)) return false;
        if (command == SearchCommand::SlectAllCurrentQuery) {
            query = "";
            command = SearchCommand::HighlightQuery;
        } else {
            if (choose != 0) {
                query = suggest_list[choose];
            }
        }
        query += c;
        choose = 0;
    }
    return true;
}

bool UI::GetEventWhenOpenPage(ResultCommand &command, int &page, int &total_page) {
    if (IsPressed(KEY_UP) || IsPressed(KEY_LEFT)) {
        page--;
        if (page < 1) page = 1;
    } else if (IsPressed(KEY_DOWN) || IsPressed(KEY_RIGHT)) {
        page++;
        if (page > total_page) page = total_page;
    } else if (IsPressed(ctrl('c'))) {
        command = ResultCommand::Quit;
    } else if (IsPressed(27)) {
        command = ResultCommand::SelectResult;
    } else {
        return false;
    }
    return true;
}