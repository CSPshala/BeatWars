#include <Windows.h>
#include "StringHelper.h"
#include <string>
#include <fstream>

// Written by Sean Eshbaugh @ http://www.geekpedia.com/tutorial38_Searching-for-a-string-in-a-File.html

bool StrHlp::FileSearch(const char* pFile, const char* lpszSearchString) 
{
	std::string search_string = lpszSearchString;
	std::string inbuf;
	std::fstream stream(pFile, std::ios::in);
	while(std::getline(stream, inbuf))
		if(inbuf.find(search_string) != std::string::npos)
			return true;

	return false;
}