// implement for string
#include "util.h"
#include <fstream>

std::string util::string::LTrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string util::string::RTrim(const std::string& s)
{
	size_t end = s.find_last_not_of(util::string::WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string util::string::ToLowerCase(const std::string &s)
{
	std::string st = s;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (int(s[i]) >= 65 && int(s[i] <= 90))
			st[i] = tolower(s[i]);
	}
	return st;
}

std::string util::string::Trim(const std::string &s)
{
	std::string st = RTrim(LTrim(s));
	unsigned int i = 0;
	while (i < st.length())
	{
		if (st[i] == ' '&& st[i + 1] == ' ')
		{
			for (auto j = i; j < st.length() - 1; j++)
				st[j] = st[j + 1];
			st.pop_back();
			--i;
		}
		++i;
	}
	return st;
}


std::string util::string::Normalize(const std::string &s) {
    std::string word = util::string::Trim(s);
	word = RemoveUnicode(word);
	word = RemoveMark(word);
    return util::string::ToLowerCase(word);
}

std::vector<std::string> util::string::Split(const std::string &str)
{
    // Trim s for sure
    std::string s = util::string::Trim(str);

	std::vector<std::string> words;
    std::string word = "";
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] != ' ')
		{
            word += s[i];
		}
		else
		{
			words.push_back(word);
            word = "";
		}
	}
    if (word.empty() == false) words.push_back(word);
	return words;
}

std::vector<std::string> util::string::DivideToLine(const std::string &s, int size) {
	std::string q = s;
	std::vector<std::string> list;

	// int i = 0;
	// while (i < q.size()) {
	// 	if (i > 1 && q[i] == '>' && q[i - 1] == '<') {
	// 	size += 2;
	// 	} else if (i > 2 && q[i] == '>' && q[i - 1] == '/' && q[i - 2] == '<') {
	// 			size += 3;
	// 	}
	// 	i += 1;
	// }
	// while (q.empty() == false) {
	// 	if (size >= q.size()) {
	// 		list.push_back(q);
	// 		q = "";
	// 	} else {
	// 		if (q[size - 1] == ' ') {
	// 			list.push_back(q.substr(0, size));
	// 			q.erase(0, size);
	// 		} else {
	// 			list.push_back(q.substr(0, size - 1));
	// 			q.erase(0, size - 1);
	// 			list[list.size() - 1] += "-";
	// 		}
	// 	}
	// }
	int i = 0;
	int  pre = 0, tmp_size = size;
	while (i < q.size()) {
		if (i > 1 && q[i] >= '>' && q[i - 1] == '<') {
			size += 2;
		} else if (i > 2 && q[i] == '>' && q[i - 1] == '/' && q[i - 2] == '<') {
			size += 3;
		}
		if (i > size && q[i] != ' ') {
			if (pre == 0) pre = i;
			std::string line = q.substr(0, pre);
			if (line[0] == ' ') line.erase(0, 1);
			list.push_back(line);
			q.erase(0, pre);
			pre = 0;
			i  = 0;
			size = tmp_size;
			continue;
		} 
		if (q[i] == ' ' && i > size) {
			std::string line = q.substr(0, pre);
			if (line[0] == ' ') line.erase(0, 1);
			list.push_back(line);
			q.erase(0, pre);

			pre = 0;
			size = tmp_size;
			i = 0;
			continue;
		} else {
			if (q[i] == ' ') pre = i;
		}
		i++;
	}
	if (q.size() > 0) {
		if (q[0] == ' ') q.erase(0, 1);
		if (q.size() > 0) list.push_back(q);
	}
	return list;
}


std::string util::string::RemoveMark(const std::string &s)
{
	std::string st = s;
	int i = 0;
	std::string mark = "+^`~><-,.?!:;_'`\"()[]{}/\\";

	while (i < st.length()) {
		bool is_mark = false;
		for (auto c : mark) {
			if (st[i] == c) {
				is_mark = true;
				break;
			}
		}
		if (is_mark) {
			st.erase(i, 1);
		} else {
			++i;
		}
	}

	return st;
}


std::string util::string::RemoveUnicode(const std::string &s)
{
	std::string st = s;
	int i = 0;
	while (i < st.length())
	{
		if (int(st[i]) < 0 || int (st[i]) > 255)
		{
			st.erase(i, 1);
		} else {
			++i;
		}
	}
	return st;
}

std::string util::string::RemoveStopWord(const std::string &query)
{
	std:: ifstream fin("data/stopwords.txt");
	if (!fin.is_open())
		return query;
	std::vector <std::string> stopwords;
	std::vector <std::string> words = util::string::Split(util::string::Trim(query));
	std::string st = "";
	while (!fin.eof())
	{
		std::string str;
		getline(fin, str);
		stopwords.push_back(str);
	}
	fin.close();
	for (auto wo : words) {
		bool ok = false;
		for (auto stowo : stopwords)
			if (ToLowerCase(wo) == ToLowerCase(stowo)) {
				
				ok = true;
				break;
			}
		if (!ok)
			st += wo + " ";
	}
	if (!st.empty()) st.pop_back();
	// printf("%s", st.c_str());
	return st;
}

bool util::string::isNum(char ch)
{
	if (ch <= '9'&& ch >= '0')
		return true;
	else
		return false;
}

bool util::string::isNum_string(std::string st) {
	for (int i = 0; i < st.length(); i++)
	{
		if( !isNum(st[i]))
			return false;
	}
	return true;
}

std::string util::string::range_string(int n1, int n2, std::string ch, int pos) {
	std::string str = "";
	if (n2 < n1 || (n2 - n1)>1000000)
		return "invalid numbers";
	if (pos == 1)
		for (int i = n1; i <= n2; i++)
			str += ch + std::to_string(i) + " ";
	else
		for (int i = n1; i <= n2; i++)
			str += std::to_string(i) + ch + " ";
	return str;
}