#ifndef CONFIG_H
#define CONFIG_H

#define ALPHABET_SIZE 255

#include <string>

namespace config {
    namespace path {
        extern const std::string DATA;
        extern const std::string NEWS;
        extern const std::string HISTORY;
    }

    namespace privacy {
        extern bool private_mode;
        void SetMode(bool is_private_mode); // false is default mode, true is private mode
    }

    namespace search {
        extern unsigned int max_suggest;
        void SetMaxSuggest(unsigned int max_suggest);
    }
}

#endif
