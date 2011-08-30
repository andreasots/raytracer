#include <map>
#include <string>
#include <fstream>
#include <iostream>

std::map<std::string, std::string> values;

void readINI(std::string file)
{
	std::ifstream ini(file.c_str());
	std::string section = "/", line;
	while(!ini.eof())
	{
		std::getline(ini, line);
		if(line[0] == '[')
			section = '/'+line.substr(1, line.find_last_of(']')-1);
		else if(line[0] == ';' || line == "")
			continue;
		else
		{
			if(line.find_first_of('=') == line.npos)
				continue;
			std::string key, value;
			key = line.substr(0, line.find_first_of('='));
			value = line.substr(line.find_first_of('=')+1);
			values.insert(std::make_pair(section+'/'+key, value));
		}
	}
}

int main(int argc, char *argv[])
{
	readINI(argv[1]);
	for(auto i = values.begin(); i != values.end(); i++)
		std::cout << "'" << i->first << "' = '" << i->second << "'" << std::endl;
	return 0;
}
