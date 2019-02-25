/*************************************************
*	PRTStrokePrivate，继承IPRTStroke
*
*	
**************************************************/
#pragma once
#include <string>
#include <vector>
#include "Database.h"

namespace ime{ namespace stroke{

class Stroke_Internal
{
public:
	Stroke_Internal();
	~Stroke_Internal();
	Stroke_Internal(const Stroke_Internal &other) = delete;
	void operator =(const Stroke_Internal &other) = delete;

	bool init(const std::string &associateDBPath);
	bool hasInit() const;
	bool deinit();
	void setCandidatePageSize(unsigned int size);
	unsigned int getCandidatePageSize() const;
	bool search(const std::string &stroke);
	unsigned int getCandidateCount() const;
	unsigned int getCandidatePageCount() const;
	void getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const;
	void getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const;
	bool promote(const std::string &stroke, const std::string &word);

private:
	void checkInit() const;

	bool					m_init;
	int						m_candidatePageSize;
	Query					m_query;
};

}}