#include <fstream>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

using VectorOfString = std::vector<std::string>;

VectorOfString explode(const std::string& s, 
					   const char& c)
{
	std::string buffer{""};
	VectorOfString result;
	result.reserve(s.size());
	for (auto n : s)
	{
		if (n != c)
		{
			buffer += n;
		}
		else if (n == c && 
				 buffer != "") 
		{ 
			result.push_back(buffer); 
			buffer = ""; 
		}
	}
	if (buffer != "")
	{
		result.push_back(buffer);
	}
	return result;
}

void remove(std::string& str, char c)
{
	size_t pos;
	while ((pos = str.find(c)) != std::string::npos)
	{
		str.erase(pos, 1);
	}
}

char *optarg = '\0';
int optind = 0;

char getopt(int argc, char* argv[], const std::string& separators)
{
	VectorOfString s = explode(separators, ':');
	for (int i = optind; i < argc; ++i)
	{
		std::string str = argv[i];
		if (str.size() == 2)
		{
			remove(str, '-');
			if (std::find(s.begin(), s.end(), str) != s.end())
			{
				if (strlen(argv[i + 1]) > 0)
				{
					optarg = argv[i + 1];
				}
				optind = i + 2;
				return str[0];
			};
		}
	}
	return '\0';
}

void get_options(int argc,
	char * argv[],
	std::string& directory,
	std::string& outputfile)
{
	if (argc != 5)
	{
		printf("USAGE: app.exe -d <directory> -o <output file result>\n");
		exit(1);
	}
	int opt = 0;
	char *direct = nullptr;
	char *output = nullptr;
	/* Use getopt to fetch the host and port */
	while ((opt = getopt(argc, argv, "d:o:")) != 0)
	{
		switch (opt)
		{
		case 'd':
			direct = _strdup(optarg);
			directory = std::string(direct);
			break;
		case 'o':
			output = _strdup(optarg);
			outputfile = std::string(output);
			break;
		default:
			printf("Unknown option\n"); exit(1);
		}
	}
	if (output == nullptr ||
		direct == nullptr)
	{
		printf("USAGE: app.exe -d <directory> -o <output file result>\n");
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	std::string directory{};
	std::string outputfile{};
	get_options(argc, argv, directory, outputfile);
	std::ifstream read;
	std::ofstream write(outputfile, std::ofstream::out);
	for (auto& p : fs::directory_iterator(directory))
	{
		std::string name = fs::path(p).filename().generic_string();
		read.open(fs::path(p));
		if (read.is_open()) 
		{
			int lnt = 0;
			for (std::string line; std::getline(read, line);)
			{
				if (lnt == 1) 
				{
					std::vector<std::string> line_t{explode(name,'.')};
					std::vector<std::string> line_v{explode(line,';')};
					write << line_t[0] << ";" << line_v[0] << ";" << line_v[1] << ";" << line_v[2] << ";" << line_v[3] << ";" << std::endl;
				}
				lnt++;
			}
		}
		read.close();
	}
}
