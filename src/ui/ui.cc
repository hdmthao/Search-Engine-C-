#include "ui.h"
#include "../util/util.h"

#include <iostream>

using namespace util::input;

UI::UI() : MAX_WIDTH(0), MAX_HEIGHT(0), main_win(nullptr), engine(nullptr), load_ui(nullptr)
{}

UI::~UI() {
}

bool UI::Start() {
    initscr();
    raw();
    curs_set(0);
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
void UI::StartSearching(Engine* engine) {
    this->engine = engine;
    search_ui = new SearchUI();

    search_ui->Start();
}

void UI::RunSearching() {
    std::string cur_query = "";
    int choose = 0;
    SearchCommand command = SearchCommand::HighlightQuery;
    std::vector<std::string> suggest_list;

    // just wait for 3 secoond, if not key is press, so show best query history
    Update(3000);
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
            suggest_list = engine->GetSuggest(cur_query);
            suggest_list.insert(suggest_list.begin(), cur_query);
        } 
        search_ui->Draw(suggest_list, choose, command);
        if (command == SearchCommand::SearchCurrentQuery) {
            std::printf( "Search For: %s", suggest_list[choose].c_str());
            break;
        }
    }
    Update(-1);
}

void UI::StopSearching() {
    search_ui->Stop();
    SAFE_DELETE(search_ui);
}

bool UI::GetEventInSearchScreen(std::string &query, std::vector<std::string> &suggest_list, int &choose, SearchCommand &command) {
    if (IsPressed(KEY_DOWN)) { // len
        choose++;
        choose = std::min(choose, (int)suggest_list.size() - 1);
        command = SearchCommand::HighlightQuery;
    } else if (IsPressed(KEY_UP)) { // xuong 
        choose--;
        choose = std::max(choose, 0);
        command = SearchCommand::HighlightQuery;
    } else if (IsPressed(127)) { // xoa
        if (command == SearchCommand::SlectAllCurrentQuery) {
            query = "";
            command = SearchCommand::HighlightQuery;
        } else if (!query.empty()) query.pop_back();
        choose = 0;
    } else if (IsPressed('\n') || IsPressed(10)) { // enter
        command = SearchCommand::SearchCurrentQuery;
    } else if (IsPressed(ctrl('a'))) { // ctrl + a 
        command = SearchCommand::SlectAllCurrentQuery;
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