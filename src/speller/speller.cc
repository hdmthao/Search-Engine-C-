#include "speller.h"
#include "../util/util.h"
#include <iostream>
#include <queue>

Speller::Speller(Searcher* searcher) : searcher(searcher), dict(nullptr) {
    dict = new Trie();
}

Speller::~Speller() {
    SAFE_DELETE(dict);
}

bool Speller::AutoCorrect(const std::string & origin, std::string & fix){
	fix = origin;
	if (Search(fix))
		return true;
	std::queue < std::pair<std::string, int> > inCorrectList;
	inCorrectList.push(make_pair(fix, 0));
	while (!inCorrectList.empty()) {
		std::string curWord = inCorrectList.front().first;
		//std::cout << curWord << "\n";
		int rep = inCorrectList.front().second;
		inCorrectList.pop();
		// swap 2 character
		for (std::string::size_type i = 0; i < curWord.length() - 1; i++) {
			std::string newWord = curWord.substr(0, i) + curWord[i+1] + curWord[i] + curWord.substr(i + 2);
			if (Search(newWord)) {
				fix = newWord;
				return 1;
			}
			if (rep < 0) {
				inCorrectList.push(make_pair(newWord, rep + 1));
			}
		}
		for (char j = 'a'; j <= 'z'; j++) {
			//Insert a character
			for (std::string::size_type i = 0; i < curWord.length() + 1; i++) {
				std::string newWord = curWord.substr(0, i) + j + curWord.substr(i);
				if (Search(newWord)) {
					fix = newWord;
					return 1;
				}
				if (rep < 1) {
					inCorrectList.push(make_pair(newWord, rep + 1));
				}
			}
			// Alterate a character
			for (std::string::size_type i = 0; i < curWord.length() - 1; i++) {
				std::string newWord = curWord.substr(0, i) + j + curWord.substr(i + 1);
				if (Search(newWord)) {
					fix = newWord;
					return 1;
				}
				if (rep < 0) {
					inCorrectList.push(make_pair(newWord, rep + 1));
				}
			}
		}
		// delete a character
		for (std::string::size_type i = 0; i < curWord.length(); i++) {
			std::string newWord = curWord.substr(0, i) + curWord.substr(i + 1);
			if (Search(newWord)) {
				fix = newWord;
				return 1;
			}
			if (rep < 1) {
				inCorrectList.push(make_pair(newWord, rep + 1));
			}
		}

	}
	return false;
}

bool Speller::Check(const std::string & origin, std::string & fix) {
	fix = "";

	std::string query = util::string::Normalize(origin);

	std::vector <std::string> originWord = util::string::Split(query);
	int count = 0;
	for (int i = 0; i < originWord.size(); i++) {
		count += AutoCorrect(originWord[i], originWord[i]);
		fix += originWord[i] + " ";
	}
	fix.erase(fix.length() - 1, 1);
	if (count == originWord.size()) return false; else return true;
}


bool Speller::Insert(std::string &s) {
    return dict->Insert(s);
}

bool Speller::Search(std::string &s) {
    return searcher->IsExistWord(s);
}
