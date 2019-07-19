#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"
#include "../speller/speller.h"
#include "../suggester/suggester.h"

#include <vector>
#include <string>

class Engine {
private:
    Searcher* searcher;
    Speller* speller;
    Suggester* suggester;

    bool BuildSearcher(const std::string &path_to_file);

public:

    Engine();
    ~Engine();

    // Engine
    bool Init();   
    bool Exit(); 
    

    // Searcher 
    // bool Loading(LoadUI* loadui);
    bool Loading();

    // Speller
    bool StartSpeller();
    bool CheckSpell(const std::string &orign, std::string &fix);
    bool StopSpeller();

    // Suggester
    bool StartSuggester();
    std::vector<std::string> GetSuggest(const std::string &orgin_query);
    bool StopSuggester();
};

#endif
