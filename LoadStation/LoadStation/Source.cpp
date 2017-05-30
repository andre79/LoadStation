#include <fstream>
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

const std::vector<std::string> explode(const std::string& s, const char& c)
{
	std::string buff{ "" };
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if (buff != "") v.push_back(buff);

	return v;
}

int main(int argc, char *argv[])
{
	FILE * pFile;
	char buffer[100];

	std::string pathIn = "C:\\Users\\funceme\\Documents\\Visual Studio 2015\\Projects\\LinkedListTemplate\\LoadStation\\Debug\\here";
	std::string pathOut = "C:\\Users\\funceme\\Documents\\Visual Studio 2015\\Projects\\LinkedListTemplate\\LoadStation\\Debug";

	std::ifstream read;
	std::ofstream write(pathOut + "\\file.txt", std::ofstream::out);
		
	fs::create_directories(pathIn);
	
	for (auto& p : fs::directory_iterator(pathIn))
	{
		std::string name = fs::path(p).filename().generic_string();
		read.open(fs::path(p));
		if (read.is_open()) 
		{
			int lnt = 0;
			for (std::string line; std::getline(read, line);)
			{
				if (lnt == 1) {
					std::vector<std::string> line_t{ explode(name,'.') };
					std::vector<std::string> line_v{explode(line,';')};
					write << line_t[0] << ";" << line_v[0] << ";" << line_v[1] << ";" << line_v[2] << ";" << line_v[3] << ";" << std::endl;
				}
				lnt++;
			}
		}
		read.close();
	}
}
