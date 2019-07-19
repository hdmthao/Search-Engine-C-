// implement for string
#include "util.h"

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

std::string util::string::RemoveMark(const std::string &s)
{
	std:: string st = s;
	int i = 0;
	while (i < st.length())
	{
		if ((st[i] == ' ' || st[i] == ',' || st[i] == '.' || st[i] == '?' || st[i] == '!' || st[i] == ':' || st[i] == ';') &&
			(st[i + 1] == ' ' || st[i + 1] == ',' || st[i + 1] == '.' || st[i + 1] == '?' || st[i + 1] == '!' || st[i + 1] == ':' || st[i + 1] == ';'))
		{
			st[i] = st[i + 1] = ' ';
			for (int j = i; j < st.length() - 1; j++)
				st[j] = st[j + 1];
			st.pop_back();
			--i;
		}
		++i;
	}
	--i;
	if (st[i] == ',' || st[i] == '.' || st[i] == '?' || st[i] == '!' || st[i] == ':' || st[i] == ';')
		st.pop_back();
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
			for (int j = i; j < st.length() - 1; j++)
				st[j] = st[j + 1];
			st.pop_back();
			--i;
		}
		++i;
	}
	return st;
}
