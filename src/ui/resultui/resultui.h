#ifndef RESULT_UI
#define RESULT_UI

#include "../window/window.h"
#include "../../component/component.h"
#include "../../command/command.h"

class ResultUI {
private:
    Window* logo_win;
    Window* result_win;   
    Window* search_win;
    Window* statistic_win;
    Window* page_win;

    void DrawLogo();
    bool CreateBox(WINDOW* win, int size);
    bool DrawSearchBox(const std::string &query, ResultCommand &command, int &cur_y, int &cur_x);
    bool DrawStatistic(SearchResult *result, bool choose);
    bool DrawResult(SearchResult *result, bool choose, int pos);
    bool ShowPage(std::vector<std::string> &news, int &page, int &total_page);

public:
    ResultUI();
    ~ResultUI();

    void Start();
    void Draw(const std::string &query, SearchResult* result, int &choose, ResultCommand &command);
    void ClearPage();
    void Stop();
};

#endif