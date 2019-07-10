#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <ctype.h>
// 
namespace util {
    // function for string
    namespace string {
        
        std::string Trim(const std::string &s); // remove trailing space
        std::vector<std::string> Split(const std::string &s); // split a string to vector. Ex : "My name is Google" => ["My", "name", "is", "Google"]
        std::string ToLowerCase(const std::string &s);
    }

    // function for time
    namespace time { // use time.h libary.
        // init time function
        // get time interval of a process
        // get current time to second
        // get different time of 2 time.
    }
}

#endif
