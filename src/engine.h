#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"
#include "../speller/speller.h"
#include "../suggester/suggester.h"
#include "../ui/loadui/loadui.h"
#include "../component/component.h"

#include <map>
#include <vector>
#include <string>

class Engine {
private:
    Speller* speller;
    Suggester* suggester;
    
    std::vector<std::string> GetListNewsFromFile(const std::string &path);
    std::unordered_map<int, std::string> doc_map;
    
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
    bool BuildSearcher(const std::string &path_to_file, int &id);
    SearchResult* Search(const std::string &query);
    bool StopSearcher();

    // Speller
    bool StartSpeller();
    bool CheckSpell(const std::string &orign, std::string &fix);
    bool StopSpeller();

    // Suggester
    bool StartSuggester();
    std::vector<std::string> GetSuggest(const std::string &orgin_query);
    bool StopSuggester();

    std::vector<std::string> GetNews(std::string &file_name, int &size);
};

#endif
