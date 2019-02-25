#pragma once
#include <string>
#include <vector>
#include "Database.h"

namespace ime{ namespace wubi {

class Wubi_Internal
{
public:
	Wubi_Internal();
	~Wubi_Internal();
	Wubi_Internal(const Wubi_Internal &other) = delete;
	void operator =(const Wubi_Internal &other) = delete;

	bool init(const std::string &wubiDBPath);
	bool hasInit() const;
	bool deinit();
	void setCandidatePageSize(unsigned int size);
	unsigned int getCandidatePageSize() const;
	bool search(const std::string &zigen);
	unsigned int getCandidateCount() const;
	unsigned int getCandidatePageCount() const;
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;
	bool promote(const std::string &zigen, const std::string &word);

private:
	void checkInit() const;

	bool					m_init;
	int						m_candidatePageSize;
	Query					m_query;
};

}}