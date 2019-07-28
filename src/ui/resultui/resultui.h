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

    void DrawLogo();
    bool CreateBox(WINDOW* win, int size);
    bool DrawSearchBox(std::string &query, ResultCommand &command, int &cur_y, int &cur_x);
    bool DrawStatistic(SearchResult *result, bool choose);
    bool DrawResult(SearchResult *result, bool choose, int pos);


public:
    ResultUI();
    ~ResultUI();

    void Start();
    void Draw(std::string &query, SearchResult* result, int &choose, ResultCommand &command);
    void Stop();
};

#endif