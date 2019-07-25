#ifndef RESULTCOMPONENT_H
#define RESULTCOMPONENT_H

#include "../util/util.h"

struct ResultComponent {
	std::string title;
	int total_keywords;
	std::string paragraph;
	ResultComponent() {
		title = "";
		total_keywords = 0;
		paragraph = "";
	}

};

ResultComponent *getFile(const std::string &s, std::string filename);

#endif