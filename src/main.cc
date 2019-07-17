#include "config.h"
#include "util/util.h"
#include "engine/engine.h"
#include "speller/speller.h"
#include <iostream>
#include <vector>
#include <exception>

#define DONE " \x1b[32;1mDONE\x1b[0m"
#define RUNNING " \x1b[36;1mRUNNING...\x1b[0m"
#define FAIL " \x1b[31;1mFAIL\x1b[0m"


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
        bool spell_status;
        spell_status = engine->speller->Check(query, fix);
        if (spell_status) {
            Info("Did you mean: ", false);
            std::cout << fix << "\n";
        } else {
            Info("Search for: ", false);
            std::cout << query << "\n";
        }
        engine->StopSpeller();
        Done("Test Speller");
    } catch (std::exception &e) {
        Fail("Test Speller");
        std::cout << e.what() << "\n";
        return false;
    }
    PressToContinue();
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
