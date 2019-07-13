#include "config.h"
#include "util/util.h"
#include "engine/engine.h"
#include <iostream>
#include "speller/speller.h"
#include <fstream>
// Test speller here
#include <time.h>

bool Test_Speller(Engine* &engine) {
    engine->StartSpeller();
	std::string fix;
	std::string origin = "Te ycerattacks otok wn Irwnia";
	float rtime = clock();
	std::cout << "ORIGIN WORD : " << origin << "\n";
	if (engine->speller->Check(origin, fix)) {
		std::cout << "Correct Sentence Is :" << fix << "\n";
	}
	else {
		std::cout << "This is a Incorrect Sentence !" << "\n";
	}
	rtime = (clock() - rtime) / CLOCKS_PER_SEC;
	std::cout << "RUN TIME : " << float(rtime) << "s\n";
	engine->StopSpeller();
    return true;
}


int main() {
    Engine* engine = new Engine();

    engine->Init();
    engine->BuildSearcher();
    Test_Speller(engine);
    engine->Exit();
    SAFE_DELETE(engine);
	system("pause");
}
