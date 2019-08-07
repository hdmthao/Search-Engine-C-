#include "../../util/util.h"
#include "loadui.h"
#include <vector>
#include <string>


LoadUI::LoadUI() : percent_done(0), title_win(nullptr), progress_bar(nullptr) {}


LoadUI::~LoadUI() {
}


void LoadUI::Start() {    
    // print title
//     std::vector<std::string> title = {
// "_|      _|  _|_|_|    _|_|_|    _|_|  ",
// "_|_|  _|_|    _|    _|        _|    _|",
// "_|  _|  _|    _|    _|  _|_|  _|    _|",
// "_|      _|    _|    _|    _|  _|    _|",
// "_|      _|  _|_|_|    _|_|_|    _|_|  ",
// };
    std::vector<std::string> title = {
"           ___      ___   ___                  ",
"    /|    //| |       / /    //   ) )  //   ) )", 
"   //|   // | |      / /    //        //   / / ",
"  // |  //  | |     / /    //  ____  //   / /  ",  
" //  | //   | |    / /    //    / / //   / /   ",   
"//   |//    | | __/ /___ ((____/ / ((___/ /    ",  
};
    title_win = new Window(title.size(), title[0].size(), 10, (COLS - title[0].size()) / 2);

    wattron(title_win->win, COLOR_PAIR(17));

    wattron(title_win->win, A_BOLD);
    for (int i = 0; i < title.size(); ++i) {
        mvwaddstr(title_win->win, i,0,title[i].c_str());
    }

    wattroff(title_win->win, COLOR_PAIR(17));

    // Loading status
    progress_bar = new Window(4, 102, 20, (COLS - 102) / 2);

    // print layout progress bar
    wattron(progress_bar->win, COLOR_PAIR(57) | A_BOLD);
    mvwaddch(progress_bar->win, 0, 0, ACS_LTEE);
    mvwaddch(progress_bar->win, 0, 101, ACS_RTEE);
    wattron(progress_bar->win, A_BLINK);
    mvwaddstr(progress_bar->win, 3, 46, "Loading...");
    wattroff(progress_bar->win, A_BLINK);


    // Print info
    std::vector<std::string> info = {
"Project :  Mini Search Engine",
"Engine  :  Trie + Posting List",
"UI      :  NCurses Libary",
};
    int info_offset = 8;
    info_win = new Window(9, 38, 26, (COLS - 38) / 2);
    box(info_win->win, 0, 0);
    wattron(info_win->win, A_BOLD);
    for (int i = 0; i < info.size(); ++i) {
        std::string label = info[i].substr(0, info_offset);
        mvwaddstr(info_win->win, i + 1, 1, label.c_str());
    }
    mvwaddstr(info_win->win, 6, 1, "Total news   :  ");
    mvwaddstr(info_win->win, 7, 1, "Time to load :  ");

    wattroff(info_win->win, A_BOLD);
    wattron(info_win->win, A_INVIS);
    for (int i = 0; i < info.size(); ++i) {
        std::string label = info[i].substr(info_offset, info[i].length() - 1);
        mvwaddstr(info_win->win, i + 1, 1 + info_offset, label.c_str());
    }
    mvwhline(info_win->win, 4, 1, '_', info_win->w - 2);
    wattroff(info_win->win, A_INVIS);
    abc_win = new Window(3, 38, 35, (COLS - 38) / 2);
    abc_win->Refresh();    
    refresh();
    progress_bar->Refresh();
    title_win->Refresh();
    info_win->Refresh();
}


void LoadUI::Draw(int total_file, float time) {
    percent_done++;
    mvwaddch(progress_bar->win, 0, percent_done, ACS_CKBOARD);
    if (percent_done == 100) {
        mvwaddstr(progress_bar->win, 3, 38, "I'M Ready For You, MIGO!!");
 
    }
    mvwprintw(info_win->win, 6, 17, "%d", total_file);
    mvwprintw(info_win->win, 7, 17, "%f", time);

    progress_bar->Refresh();
    info_win->Refresh();
}

void LoadUI::ShowTest(double &time_estimate) {
    std::string info = "Test 100 Query Take " + std::to_string(time_estimate) + " ms!!!";
    mvwaddstr(abc_win->win, 0, 1, info.c_str());
    abc_win->Refresh();
}

void LoadUI::Stop() {
    progress_bar->DestroyWin();
    title_win->DestroyWin();
    abc_win->DestroyWin();
    info_win->DestroyWin();
    SAFE_DELETE(title_win);
    SAFE_DELETE(progress_bar);
    SAFE_DELETE(abc_win);
    SAFE_DELETE(info_win);
}