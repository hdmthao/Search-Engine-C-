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
    NewSearch = 788,
    SelectAll = 777,
    Scroll = 666,
    SelectSearchBox = 799,
    SelectStatistic = 800,
    SelectResult = 801,
    BackToSearch = 803,
    Quit = 804,
};

enum SearchType {
    And = 202,      // type 1
    Or,       // type 2
    Exclude, // type 3
    Intitle, // type 4
    Stopword, // Type 5
    Filetype, // type 6
    Price,    // type 7
    Hashtag,  // type 8
    Exact,    // type 9
    Wildcard, // type 10
    Range,    // type 11
    Synonym  // type 12 
};

#endif