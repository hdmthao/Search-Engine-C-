#include "resultcomponent.h"

ResultComponent *getFile(const std::string &s, std::string filename)
{
		std::ifstream fin(filename);

	if (!fin.is_open())
		return NULL;

	ResultComponent *fil = new ResultComponent;
	std::string st = s;
	st = util::string::RemoveUnicode(st);
	st = util::string::Trim(s);
	st = util::string::RemoveMark(st);

	std::vector <std::string> words = util::string::Split(st);
	
	int max_word = 0;
	std::string line_max="";
	std::string line_se="";
	while (!fin.eof())
	{
		std::string str;

		int count = 0;
		getline(fin, str);
		str = util::string::RemoveUnicode(str);
		str = util::string::Trim(str);
		std::vector <std::string> word_str = util::string::Split(util::string::Trim(str));
			
		if (fil->title == "") {
			if (str.length() < 100)
				fil->title = str;			
			else {
				if (str.find(".") != -1 && str.find(".") < 120 )
					fil->title = str.substr(0, str.find("."));
				else {
					fil->title = str.substr(0, 100);
					std::string ti = fil->title;
					while (ti[ti.length()-1] != ' ')
						ti.pop_back();
					ti.pop_back();
					fil->title = ti + "...";
				}
			}
		}
			
		for (auto it = words.begin(); it != words.end(); it++) {
			if (util::string::ToLowerCase( str).find(util::string::ToLowerCase( *it)) != -1) {
				for (auto wo : word_str)
					if (util::string::ToLowerCase( wo).find(util::string::ToLowerCase(*it))!=-1) {
						fil->total_keywords++;
						++count;
					}
			}
		}
		if (count > max_word) {
			line_se = line_max;
			line_max = str;
			max_word = count;
		}
	}
	fin.close();
	if (line_max.length() < 80)
		fil->paragraph = line_max + "..." + line_se;
	else
		if (line_max.length() < 150)
			fil->paragraph = line_max;
		else {
			if (line_max.find(".") != -1 && line_max.find(".") < 150)
				fil->paragraph = line_max.substr(0, line_max.find("."));
			else {
				fil->paragraph = line_max.substr(0, 150);
				std::string pa = fil->paragraph;
				while (pa[pa.length() - 1] != ' ')
					pa.pop_back();
				pa.pop_back();
				fil->paragraph = pa + "...";
			}
		}

	std::vector <std::string> word_para = util::string::Split(util::string::Trim(fil->paragraph));
	std::string para = "";
	
	for (auto wo : word_para) {
		bool ok = false;
		for (auto it : words)
			if (util::string::ToLowerCase(wo).find(util::string::ToLowerCase( it)) != -1)
				ok = true;
		if (ok)
			para += "<hl>" + wo + "<hl> ";
		else
			para += wo + " ";
	}
	para.pop_back();
	fil->paragraph = para;
	return fil;
}

