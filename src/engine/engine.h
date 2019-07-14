#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"
#include "../speller/speller.h"

class Engine {
private:
    
    Searcher* searcher;

public:
    Speller* speller;

    Engine();
    ~Engine();

    bool Init();   
    bool Exit(); 
    
    // Searcher 
    bool BuildSearcher();


    // Speller
    bool StartSpeller();
    bool StopSpeller();
};

#endif
