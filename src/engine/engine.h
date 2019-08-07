#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"
#include "../speller/speller.h"
#include "../suggester/suggester.h"
#include "../ui/loadui/loadui.h"
#include "../component/component.h"
#include "../command/command.h"

#include <map>
#include <vector>
#include <string>

class Engine {
private:
    Speller* speller;
    Suggester* suggester;
    
    std::vector<std::string> GetListNewsFromIndexFile(const std::string &path);
    std::unordered_map<int, std::string> doc_map;
    SearchTypeInfo* Migo_OP(const std::string &origin_query);

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
    bool UpdateFile(int &total_file_update, double &time);
    std::vector<std::string> GetNews(std::string &file_name, int &size);
    std::vector<std::string> GetHistory();
    bool RemoveHistory(std::string &query);
    bool TestQuery(double &time_estimate);
};

#endif
