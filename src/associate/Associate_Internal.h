#pragma once
#include <string>
#include <vector>
#include "Database.h"

namespace ime{ namespace associate{

class Associate_Internal
{
public:
	Associate_Internal();
	~Associate_Internal();
	Associate_Internal(const Associate_Internal &other) = delete;
	void operator =(const Associate_Internal &other) = delete;

	bool init(const std::string &associateDBPath);
	bool hasInit() const;
	bool deinit();
	void setCandidatePageSize(unsigned int size);
	unsigned int getCandidatePageSize() const;
	bool search(const std::string &prefix);
	unsigned int getCandidateCount() const;
	unsigned int getCandidatePageCount() const;
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;
	bool learn(const std::string &word);
	bool forget(const std::string &word);
	bool promote(const std::string &prefix, const std::string &word);

private:
	void checkInit() const;
	std::wstring utf8ToUnicode(const std::string &utf8);
	std::string unicodeToUtf8(const std::wstring &unicode);

	int							m_candidatePageSize;
	bool						m_init;
	Query						m_query;
};

}}