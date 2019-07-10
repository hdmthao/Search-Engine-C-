#ifndef ENGINE_H
#define ENGINE_H

#include "../searcher/searcher.h"


class Engine {
private:
    Searcher* searcher;

public:
    Engine();
    ~Engine();

    bool Init();   
    bool Exit(); 
};

#endif
