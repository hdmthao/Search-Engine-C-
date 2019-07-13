#include "speller.h"
#include "../util/util.h"
#include <iostream>

Speller::Speller(Searcher* searcher) : searcher(searcher), dict(nullptr) {
    dict = new Trie();
}

Speller::~Speller() {
    SAFE_DELETE(searcher);
    SAFE_DELETE(dict);
}

bool Speller::Check(const std::string & origin, std::string & fix){
	fix = origin;
	if (Search(fix))
		return true;
	Insert(fix);
	std::queue < std::pair<std::string, int> > inCorrectList;
	inCorrectList.push(make_pair(fix, 0));
	while (!inCorrectList.empty()) {
		std::string curWord = inCorrectList.front().first;
		int rep = inCorrectList.front().second;
		inCorrectList.pop();
		// delete a character
		for (int i = 0; i <= curWord.length(); i++) {
			std::string newWord = curWord;
			newWord.erase(i, 1);
			//std::cout << newWord << "\n";
			if (Search(newWord)) {
				fix = newWord;
				return true;
			}
			if (rep < 1 && !dict->IsExistWord(newWord)) {
				Insert(newWord);
				inCorrectList.push(make_pair(newWord, rep + 1));
			}
		}
		// change a character
		for (int i = 0; i < curWord.length(); i++) {
			for (int j = 'a'; j <= 'z'; j++) {
				std::string newWord = curWord;
				newWord[i] = j;
				//std::cout << newWord << "\n";
				if (Search(newWord)) {
					fix = newWord;
					return true;
				}
				if (rep < 0 && !dict->IsExistWord(newWord)) {
					Insert(newWord);
					inCorrectList.push(make_pair(newWord, rep + 1));
				}
			}
		}

		// swap 2 characters

		for (int i = 0; i < curWord.length(); i++) {
			for (int j = i + 1; j < curWord.length(); j++) {
				std::string newWord = curWord;
				std::swap(newWord[i], newWord[j]);
				//std::cout << newWord << "\n";
				if (Search(newWord)) {
					fix = newWord;
					return true;
				}
				if (rep < 0 && !dict->IsExistWord(newWord)) {
					Insert(newWord);
					inCorrectList.push(make_pair(newWord, rep + 1));
				}
			}
		}

		// add a character
		std::string newWord = "_" + curWord;
		for (int j = 'a'; j <= 'z'; j++) {
			newWord[0] = j;
			if (Search(newWord)) {
				fix = newWord;
				return true;
			}
		}
		for (int i = 1; i <= curWord.length(); i++) {
			std::swap(newWord[i], newWord[i - 1]);
			for (int j = 'a'; j <= 'z'; j++) {
				newWord[i] = j;
				//std::cout << newWord << "\n";
				if (Search(newWord)) {
					fix = newWord;
					return true;
				}
				if (rep < 1 && !dict->IsExistWord(newWord)) {
					Insert(newWord);
					inCorrectList.push(make_pair(newWord, rep + 1));
				}
			}
		}


	}
	return false;
}


bool Speller::Insert(std::string &s) {
    if (dict->IsExistWord(s)) {
        return false; // this world already exist
    }

    return dict->Insert(s);
}

bool Speller::Search(std::string &s) {
    return searcher->IsExistWord(s);
}