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
    SelectSearchBox = 799,
    SelectStatistic = 800,
    SelectResult = 801,
    BackToSearch = 803,
    Quit = 804,
};

// enum SearchType {
//     And = 202,      // type 1
//     Or = 203,      // type 2
//     Exclude = 204, // type 3
//     Intitle = 205, // type 4
//     Stopword = 206 // Type 5
//     Filetype = 207, // type 6
//     Price = 208,    // type 7
//     Hashtag = 209,  // type 8
//     Exact = 210,    // type 9
//     Wildcard = 211, // type 10
//     Range = 212,    // type 11
//     Synonym = 213  // type 12 
// };

#endif