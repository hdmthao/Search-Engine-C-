#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"
#include "../speller/speller.h"
#include "../suggester/suggester.h"
#include "../ui/loadui/loadui.h"

#include <vector>
#include <string>

class Engine {
private:
    Speller* speller;
    Suggester* suggester;

    bool BuildSearcher(const std::string &path_to_file);
    std::vector<std::string> GetListNewsFromFile(const std::string &path);
public:

    Searcher* searcher;
    Engine();
    ~Engine();

    // Engine
    bool Init();   
    bool Exit(); 
    bool Loading(LoadUI* loadui);
    

    // Searcher 
    bool StartSearcher();
    bool StopSearcher();

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
