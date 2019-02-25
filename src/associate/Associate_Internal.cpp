#include "Associate_Internal.h"
#include <exception>
#include <cstdlib>
#include <string.h>

using namespace ime::associate;

Associate_Internal::Associate_Internal()
	: m_init(false)
	, m_candidatePageSize(5)
{
}

Associate_Internal::~Associate_Internal()
{
}

bool Associate_Internal::init(const std::string &associateDBPath)
{
	deinit();
	m_init = Database::load(associateDBPath);
	return m_init;
}

bool Associate_Internal::hasInit() const
{
	return m_init;
}

bool Associate_Internal::deinit()
{
	m_init = false;
	m_query.release();
	Database::unload();
	return true;
}

void Associate_Internal::setCandidatePageSize(unsigned int size)
{
	if (size == 0)	throw std::out_of_range("setCandidatePageSize size by 0.");
	m_candidatePageSize = size;
}

unsigned int Associate_Internal::getCandidatePageSize() const
{
	return m_candidatePageSize;
}

bool Associate_Internal::search(const std::string &prefix)
{
	checkInit();
	return m_query.search(prefix, Query::Condition::like);
}

unsigned int Associate_Internal::getCandidateCount() const
{
	checkInit();
	return m_query.recordCount();
}

unsigned int Associate_Internal::getCandidatePageCount() const
{
	checkInit();
	auto count = getCandidateCount();
	return (count % getCandidatePageSize() == 0) ? count / getCandidatePageSize() : count / getCandidatePageSize() + 1;
}

void Associate_Internal::getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const
{
	checkInit();
	if (count == 0)
		return;

	if (index >= getCandidateCount())
		throw std::out_of_range(std::string(__FUNCTION__) + "->index(" + std::to_string(index) + ") is out of range [0, " + std::to_string(getCandidateCount()) + ")");
	
	int nGet = 0;
	for (auto i = index; nGet != count && i < getCandidateCount(); ++i)
	{
		auto record = m_query.getRecord(i);
		candidates.push_back(std::get<0>(record));
		++nGet;
	}
}

void Associate_Internal::getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const
{
	getCandidate(page * getCandidatePageSize(), getCandidatePageSize(), candidates);
}

bool Associate_Internal::learn(const std::string & word)
{
	checkInit();
	if (word.empty())
		return false;

	Query q;
	q.search(word, Query::Condition::eq);
	if (q.recordCount() == 0)
	{
		int priority = 0;
		Query firstLetterQ;
		std::wstring ws = utf8ToUnicode(word);
		firstLetterQ.search(unicodeToUtf8(ws.substr(0, 1)), Query::Condition::like);
		if (firstLetterQ.recordCount() != 0)
			priority = std::stoi(std::get<1>(firstLetterQ.getRecord(0)));
		return Database::learn(word, priority + 1);
	}
	else
	{
		return false;
	}
}

bool Associate_Internal::forget(const std::string & word)
{
	checkInit();
	if (word.empty())
		return false;

	return Database::forget(word);
}

bool Associate_Internal::promote(const std::string &prefix, const std::string & word)
{
	checkInit();
	if (prefix.empty() || word.empty())
		return false;

	Query q;
	q.search(prefix, Query::Condition::like);
	int highestPriority = 0;
	if (q.recordCount() != 0)
		highestPriority = std::stoi(std::get<1>(q.getRecord(0)));

	return Database::update(word, highestPriority + 1);
}

void Associate_Internal::checkInit() const
{
	if (!m_init)
	{
		printf("any api must called after init success, exit.\r\n");
		std::exit(-1);
	}
}

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

std::wstring Associate_Internal::utf8ToUnicode(const std::string &utf8)
{
	size_t n = strlen(utf8.data());
	unsigned int nUnicodeLen = UTF8StrToUnicode(utf8.data(), n, nullptr, 0);
	wchar_t *pUnicode = new wchar_t[nUnicodeLen + 1];
	UTF8StrToUnicode(utf8.data(), n, pUnicode, nUnicodeLen);
	pUnicode[nUnicodeLen] = 0;
	std::wstring wsRet = pUnicode;
	delete[]pUnicode;
	return wsRet;
}

std::string Associate_Internal::unicodeToUtf8(const std::wstring &unicode)
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
