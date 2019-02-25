#include <string.h>
#include <algorithm>
#include "Encoding.h"

unsigned int UTF8StrToUnicode(const char*UTF8String, unsigned int UTF8StringLength, wchar_t *OutUnicodeString, unsigned int UnicodeStringBufferSize)
{
	unsigned int UTF8Index = 0;
	unsigned int UniIndex = 0;

	while (UTF8Index < UTF8StringLength)
	{
		unsigned char UTF8Char = UTF8String[UTF8Index];
		if (UnicodeStringBufferSize != 0 && UniIndex >= UnicodeStringBufferSize)
			break;

		if ((UTF8Char & 0x80) == 0)
		{
			const unsigned int cUTF8CharRequire = 1;
			// UTF8字码不足
			if (UTF8Index + cUTF8CharRequire > UTF8StringLength)
				break;

			if (OutUnicodeString)
			{
				wchar_t &WideChar = OutUnicodeString[UniIndex];
				WideChar = UTF8Char;
			}
			UTF8Index++;
		}
		else if ((UTF8Char & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
		{
			const unsigned int cUTF8CharRequire = 2;
			// UTF8字码不足
			if (UTF8Index + cUTF8CharRequire > UTF8StringLength)
				break;

			if (OutUnicodeString)
			{
				wchar_t &WideChar = OutUnicodeString[UniIndex];
				WideChar = (UTF8String[UTF8Index + 0] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 1] & 0x3F);
			}
			UTF8Index += cUTF8CharRequire;
		}
		else if ((UTF8Char & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
		{
			const unsigned int cUTF8CharRequire = 3;
			// UTF8字码不足
			if (UTF8Index + cUTF8CharRequire > UTF8StringLength)
				break;

			if (OutUnicodeString)
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex];

				WideChar = (UTF8String[UTF8Index + 0] & 0x1F) << 12;
				WideChar |= (UTF8String[UTF8Index + 1] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 2] & 0x3F);
			}
			UTF8Index += cUTF8CharRequire;
		}
		else if ((UTF8Char & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			const unsigned int cUTF8CharRequire = 4;
			// UTF8字码不足
			if (UTF8Index + cUTF8CharRequire > UTF8StringLength)
				break;

			if (OutUnicodeString)
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex];

				WideChar = (UTF8String[UTF8Index + 0] & 0x0F) << 18;
				WideChar = (UTF8String[UTF8Index + 1] & 0x3F) << 12;
				WideChar |= (UTF8String[UTF8Index + 2] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 3] & 0x3F);
			}
			UTF8Index += cUTF8CharRequire;
		}
		else ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
		{
			const unsigned int cUTF8CharRequire = 5;
			// UTF8字码不足
			if (UTF8Index + cUTF8CharRequire > UTF8StringLength)
				break;

			if (OutUnicodeString)
			{
				wchar_t& WideChar = OutUnicodeString[UniIndex];

				WideChar = (UTF8String[UTF8Index + 0] & 0x07) << 24;
				WideChar = (UTF8String[UTF8Index + 1] & 0x3F) << 18;
				WideChar = (UTF8String[UTF8Index + 2] & 0x3F) << 12;
				WideChar |= (UTF8String[UTF8Index + 3] & 0x3F) << 6;
				WideChar |= (UTF8String[UTF8Index + 4] & 0x3F);
			}
			UTF8Index += cUTF8CharRequire;
		}
		UniIndex++;
	}

	return UniIndex;
}

unsigned int UniCharToUTF8(wchar_t UniChar, char *OutUTFString)
{
	unsigned int UTF8CharLength = 0;
	if (UniChar < 0x80)
	{
		if (OutUTFString)
			OutUTFString[UTF8CharLength++] = (char)UniChar;
		else
			UTF8CharLength++;
	}
	else if (UniChar < 0x800)
	{
		if (OutUTFString)
		{
			OutUTFString[UTF8CharLength++] = 0xc0 | (UniChar >> 6);
			OutUTFString[UTF8CharLength++] = 0x80 | (UniChar & 0x3f);
		}
		else
		{
			UTF8CharLength += 2;
		}
	}
	else if (UniChar < 0x10000)
	{
		if (OutUTFString)
		{
			OutUTFString[UTF8CharLength++] = 0xe0 | (UniChar >> 12);
			OutUTFString[UTF8CharLength++] = 0x80 | ((UniChar >> 6) & 0x3f);
			OutUTFString[UTF8CharLength++] = 0x80 | (UniChar & 0x3f);
		}
		else
		{
			UTF8CharLength += 3;
		}
	}
	else if (UniChar < 0x200000)
	{
		if (OutUTFString)
		{
			OutUTFString[UTF8CharLength++] = 0xf0 | ((int)UniChar >> 18);
			OutUTFString[UTF8CharLength++] = 0x80 | ((UniChar >> 12) & 0x3f);
			OutUTFString[UTF8CharLength++] = 0x80 | ((UniChar >> 6) & 0x3f);
			OutUTFString[UTF8CharLength++] = 0x80 | (UniChar & 0x3f);
		}
		else
		{
			UTF8CharLength += 4;
		}
	}

	return UTF8CharLength;
}

std::wstring Encoding::utf8ToUnicode(const std::string &utf8)
{
	size_t n = strlen(utf8.data());
	unsigned int nUnicodeLen = UTF8StrToUnicode(utf8.data(), n, nullptr, 0);
	wchar_t *pUnicode = new wchar_t[nUnicodeLen + 1];
	UTF8StrToUnicode(utf8.data(), n, pUnicode, nUnicodeLen);
	pUnicode[nUnicodeLen] = 0;
	std::wstring wsRet = pUnicode;
	delete []pUnicode;
	return wsRet;
}

std::string Encoding::unicodeToUtf8(const std::wstring &unicode)
{
	std::string sRet;
	for (auto const &wCh : unicode)
	{
		int nUtf8Len = UniCharToUTF8(wCh, nullptr);
		char *pUtf8 = new char[nUtf8Len + 1];
		memset(pUtf8, 0, nUtf8Len + 1);
		UniCharToUTF8(wCh, pUtf8);
		sRet += pUtf8;
		delete[]pUtf8;
	}
	return sRet;
}

std::string Encoding::utf8ToGb2312(const char* utf8)
{
#ifdef  WIN32
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区  
	int nLen = MultiByteToWideChar(CP_UTF8, NULL, utf8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, utf8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;

	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;

	std::string s1 = szBuffer;
	//清理内存  
	delete[]szBuffer;
	delete[]wszBuffer;
	return s1;
#else
	return utf8;
#endif
}

std::string Encoding::gb2312ToUtf8(const char * gb2312)
{
#ifdef WIN32
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
#else
	return gb2312;
#endif
}
