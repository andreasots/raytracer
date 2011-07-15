#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
	std::string target = "\\APACinsertmetastar{%";
	std::ifstream in;
	std::ofstream out;

	if(argc > 1)
	{
		in.open(argv[1]);
		out.open((std::string(argv[1])+".fixed").c_str());
	}
	else
	{
		std::cerr << "USAGE: " << argv[0] << " [file]" << std::endl;
		return 1;
	}

	unsigned item = 1;
	do
	{
		std::string line;
		std::getline(in, line);
		if(line == target)
			out << item++ << ". " << target << std::endl;
		else
			out << line << std::endl;
	}
	while(!in.eof());
		
	return 0;
}
