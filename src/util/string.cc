// implement for string
#include "util.h"

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string util::string::ToLowerCase(const std::string &s)
{
	std::string st = s;
	for (int i = 0; i < s.length(); i++)
	{
		if (int(s[i]) >= 65 && int(s[i] <= 90))
			st[i] = tolower(s[i]);
	}
	return st;
}

std::string util::string::Trim(const std::string &s)
{
	std::string st = rtrim(ltrim(s));
	int i = 0;
	while (i < st.length())
	{
		if (st[i] == ' '&& st[i + 1] == ' ')
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

std::vector<std::string> util::string:: Split(const std::string &s)
{
	std::vector<std::string> str, st{ "" };
	str.push_back("[");
	std::string tmp;
	tmp.push_back(char(34));
	str.push_back(tmp);
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] != ' ')
		{
			std::string tmp1;
			tmp1.push_back(s[i]);
			str.push_back(tmp1);
		}
		else
		{
			std::string tmp2;
			tmp2.push_back(char(34));
			tmp2 += ", ";
			tmp2.push_back(char(34));
			str.push_back(tmp2);
		}
	}
	str.push_back(tmp);
	str.push_back("]");
	return str;
}