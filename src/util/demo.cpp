#include <assert.h>
#include <iostream>
#include <string>
#include <vector>

#include "util.h"

int main() {

    std::string s = "  Hello   World";
    assert(util::string::Trim(s) == "Hello World");
    s = "  This is     OK    ";
    assert(util::string::Trim(s) == "This is OK");
    s = "Nothing to do with this";
    assert(util::string::Trim(s) == "Nothing to do with this");
    std::cout << "Trim string OK\n";

    s = "";
    std::vector<std::string> words;
    words = util::string::Split(s);
    assert(words.size() == 0);
    words.clear();

    s = "This is a really really long words";
    words = util::string::Split(s);
    assert(words.size() == 7);
    assert(words[0] == "This" && words[1] == "is" && words[2] == "a" &&
            words[3] == "really" && words[4] == "really" && words[5] == "long" && words[6] == "words");
    words.clear();

    s = "Just1";
    words = util::string::Split(s);
    assert(words.size() == 1 && words[0] == "Just1");
    words.clear();

    std::cout << "Split string OK\n";
}
