#pragma once
#include <string>
#include <list>
#include <set>
#include <vector>
#ifdef WIN32
#include <Windows.h>
#endif

class Encoding
{
public:
	static std::wstring utf8ToUnicode(const std::string &utf8);
	static std::string unicodeToUtf8(const std::wstring &unicode);
	static std::string utf8ToGb2312(const char* utf8);
	static std::string gb2312ToUtf8(const char *gb2312);
};
