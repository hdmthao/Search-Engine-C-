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
    bool DrawSearchBox(SearchResult *result, bool choose);
    bool DrawStatistic(SearchResult *result, bool choose);

public:
    ResultUI();
    ~ResultUI();

    void Start();
    void Draw(SearchResult* result, int &choose, ResultCommand &command);
    void Stop();
};

#endif