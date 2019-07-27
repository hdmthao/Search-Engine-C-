#ifndef COMMAND_H
#define COMMAND_H

enum StateMigo {
    Loading = 101,
    Search = 200,
    Result = 300,
    Stop = 404,
};

enum SearchCommand{
    HighlightQuery = 900,
    SearchCurrentQuery = 901,
    SlectAllCurrentQuery = 902
};

enum ResultCommand {
    SelectSearchBox = 799,
    SelectStatistic = 800,
    SelectResult = 801,
    BackToSearch = 803,
    Quit = 804,
};

#endif