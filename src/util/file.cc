#include "util.h"
#include <fstream>
#include <iostream>

std::vector<std::string> util::file::GetListNewsFromFile(const std::string &path) {
    std::vector<std::string> file_list;
    std::ifstream fi(path);
    if (!fi.is_open()) {
        std::cout << "Not exist data file\n";
        return file_list; // just empty
    }
    std::string file_name;
    while (getline(fi, file_name)) {
        file_list.push_back(file_name);
    }
    fi.close();
    return file_list;
}