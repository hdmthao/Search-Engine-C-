#ifndef COMMAND_H
#define COMMAND_H

enum class StateMigo {
    Loading = 101,
    Search = 200,
    Result = 300,
    ChangeMode,
    Stop = 404,
};

enum class SearchCommand {
    HighlightQuery = 900,
    SearchCurrentQuery,
    SlectAllCurrentQuery,
    UpdateData,
    RemoveHistory,
    ShowHistory,
    ChangeMode,
    Quit,
};

enum class ResultCommand {
    NewSearch = 788,
    SelectAll = 777,
    ShowPage = 666,
    SelectSearchBox = 799,
    ShowSuggest = 805,
    SelectStatistic = 800,
    SelectResult = 801,
    BackToSearch = 803,
    Quit = 804,
};

enum class SearchType {
    Normal = 1000,
    And,      // type 1
    Or,       // type 2
    Exclude, // type 3
    Intitle, // type 4
    Include, // Type 5
    Filetype, // type 6
    Price,    // type 7
    Hashtag,  // type 8
    Exact,    // type 9
    Wildcard, // type 10
    Range_num, Range_usd,Range_pc,    // type 11
    Synonym,  // type 12 
};

#endif