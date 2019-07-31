#include "config.h"


// path
const std::string config::path::DATA = "data/demo.txt";
const std::string config::path::HISTORY = "history.txt";
const std::string config::path::NEWS = "data/___index.txt";

// privacy
bool config::privacy::private_mode = false; // startup mode is default mode

void config::privacy::SetMode() {
    if (private_mode == false) private_mode = true; else private_mode = false;
}

// search
unsigned int config::search::max_suggest = 5; // default max suggest is 3

void SetMaxSuggest(unsigned int val) {
    config::search::max_suggest = val;
}