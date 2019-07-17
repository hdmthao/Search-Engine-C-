#include "config.h"
#include "util/util.h"
#include "engine/engine.h"
#include "speller/speller.h"
#include <iostream>
#include <vector>
#include <exception>
#include <termios.h>
#include <unistd.h>

#define DONE " \x1b[32;1mDONE\x1b[0m"
#define RUNNING " \x1b[36;1mRUNNING...\x1b[0m"
#define FAIL " \x1b[31;1mFAIL\x1b[0m"


int getch() {
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

void Info(std::string msg, bool newline = true) {
    std::cout << "\x1b[37;1m" << msg << "\x1b[0m";
    if (newline) std::cout << "\n";
}

void Running(std::string msg) {
    std::cout << msg << RUNNING << "\n";
}

void Done(std::string msg) {
    std::cout << msg << DONE << "\n";
}

void Fail(std::string msg) {
    std::cout << msg << FAIL << "\n";
}

std::string getString(std::string msg) {
	std::string x;
    msg+= ": ";
    Info(msg, false);
	std::getline(std::cin, x);
	return x;
}

int getInt(std::string msg) {
    
	int x;
    msg += ": ";
    Info(msg, false);
	std::cin >> x;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return x;
}

void PressToContinue() {
    Info("Press any key to continue...", false);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Test speller here
bool Test_Speller(Engine* engine) {
    try {
        Running("Test Speller");
        engine->StartSpeller();
        // TEst Here
        std::string query = "", fix = "";
        query = getString("Enter Query");
        util::time::timer::SetTime();
        bool spell_status;
        spell_status = engine->CheckSpell(query, fix);
        if (spell_status) {
            Info("Did you mean: ", false);
            std::cout << fix << "\n";
        } else {
            Info("Search for: ", false);
            std::cout << query << "\n";
        }
        Info("Time Eslapse: ", false);
        engine->StopSpeller();
        std::cout << util::time::timer::GetTimeInterval() << "\n";
        Done("Test Speller");
    } catch (std::exception &e) {
        Fail("Test Speller");
        std::cout << e.what() << "\n";
        return false;
    }
    PressToContinue();
    return true;
}


bool Test_Time() {
    try {
        Running("Test Time");
        util::time::timer::SetTime();
        Info("Current Time: ", false);
        std::cout << util::time::timer::GetCurrentTime() << "\n";
        Info("Time Eslapse: ", false);
        std::cout << util::time::timer::GetTimeInterval() << "\n";
        Done("Test Time");
    } catch (std::exception &e) {
        Fail("Test Time");
        std::cout << e.what() << "\n";
    }
    return true;
}

bool Test_Suggester(Engine* engine) {
    try {
        Running("Test Suggester");
        engine->StartSuggester();
        std::string query = "";
        int c = 0;
        Info("Enter Query:");
        while (c = getch()) {
            // std::cout << c << "\n";
            if (c == 10) { // enter
                break;
            }
            if (c == 127) {
                if (query.empty()) continue;
                query.pop_back();
            } else {
                query += (char)(c);
            }
            Info("Enter Query: ", false);
            std::cout << query << "\n";
            std::vector<std::string> suggest_list = engine->GetSuggest(query);
            for (int i = 0; i < suggest_list.size(); ++i) {
                std::string msg = "Suggest (" + std::to_string(i + 1) + ") : ";
                Info(msg, false);
                std::cout << suggest_list[i] << "\n";
            }
            std::cout << "\n";
        }

        Info("Search For: ", false);
        std::cout << query << "\n";
        engine->StopSuggester();
        Done("Test Suggester");
    } catch(std::exception &e) {
        Fail("Test Suggester");
        std::cout << e.what() << "\n";
    }

    return true;
}

int main() {
    // test
    Engine* engine = new Engine();
    
    // Init engine
    try {
        Running("Init Engine");
        engine->Init();
        Done("Init Engine");
    } catch (std::exception e) {
        std::cout << "Init Engine " << FAIL << "\n";
    }

    // Build trie
    try {
        Running("Load Data");
        if (engine->BuildSearcher()) {
            Done("Load Data");
       }
    } catch (std::exception &e) {
        Fail("Load Data");
        std::cout << e.what() << "\n";
    }

    std::vector<std::pair<int, std::string>> menu;
    menu.push_back({3, "Speller"});
    menu.push_back({4, "Time"});
    menu.push_back({5, "Suggester"});
    menu.push_back({0, "Exit"});

    while (true) {
        Info("               MENU                ");
        for (unsigned int i = 0; i < menu.size(); ++i) {
            std::cout << menu[i].first << ". " << menu[i].second << "\n";
        }
        int option;
        option = getInt("Enter Option");
        switch (option) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                Test_Speller(engine);
                break;
            case 4:
                Test_Time();
                break;
            case 5:
                Test_Suggester(engine);
                break;
            default:
                break;
        }
        if (option == 0) break;
    }

    try {
        Running("Stop Engine");
        engine->Exit();
        Done("Stop Engine");
    } catch (std::exception &e) {
        Fail("Stop Engine");
        std::cout << e.what() << "\n";
    }
    SAFE_DELETE(engine);
}
