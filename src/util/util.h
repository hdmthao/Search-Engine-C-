#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <ctype.h>
#include <ctime>


namespace util {
    // function for string
    namespace string {
        const std::string WHITESPACE = " \n\r\t\f\v";

        std::string LTrim(const std::string &s);
        std::string RTrim(const std::string &s);
        std::string RemoveMark(const std::string &s); // remove . , : ; ! ?
        std::string Trim(const std::string &s); // remove trailing space
        std::vector<std::string> Split(const std::string &s); // split a string to vector. Ex : "My name is Google" => ["My", "name", "is", "Google"]
        std::string ToLowerCase(const std::string &s);
        std::string Normalize(const std::string &s);
        std::vector<std::string> DivideToLine(const std::string &, int);
    }

    // function for time
    namespace time {
        class timer {
        public: 
            static clock_t t;
            static bool SetTime(); // Set clock
            static float GetTimeInterval(); // get time interval from start time
            static long long GetCurrentTime(); // get current time to second
        };
    }

    namespace input {
        bool NoKeyPressed();

        bool IsPressed(int key);


        int GetInput(int delay_ms=-1);
        // Nếu delay_ms = -1 thì sẽ đợi cho đến khi user bấm phím
        // Nếu delay_ms = 0 thì sẽ return dù user có bấm hay không
        // Nếu delay_ms > 0 thì sẽ đợi user bấm phím trong delay_ms milisecond.
        void Update(int delay_ms=0);
        char GetAlphabet();
        bool GetIcon(char &c);
        int  ctrl(char x); 

        // phím nào đã được nhấn
        extern int pressed_key;
    }
}

#define SAFE_DELETE(pointer) {\
    if (pointer != nullptr) {\
        delete pointer;\
        pointer = nullptr;\
    }\
\
} \


#endif