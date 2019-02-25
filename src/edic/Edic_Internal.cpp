#include "Edic_Internal.h"
#include <exception>
#include <cstdlib>

using namespace ime::edic;

Edic_Internal::Edic_Internal()
	: m_init(false)
	, m_candidatePageSize(5)
{
}

Edic_Internal::~Edic_Internal()
{
}

bool Edic_Internal::init(const std::string &associateDBPath)
{
	deinit();
	m_init = Database::load(associateDBPath);
	return m_init;
}

bool ime::edic::Edic_Internal::hasInit() const
{
	return m_init;
}

bool Edic_Internal::deinit()
{
	m_init = false;
	m_query.release();
	Database::unload();
	return true;
}

void Edic_Internal::setCandidatePageSize(unsigned int size)
{
	if (size == 0)	throw std::out_of_range("setCandidatePageSize size by 0.");
	m_candidatePageSize = size;
}

unsigned int Edic_Internal::getCandidatePageSize() const
{
	return m_candidatePageSize;
}

bool Edic_Internal::search(const std::string &prefix)
{
	checkInit();
	return m_query.search(prefix, Query::Condition::like);
}

unsigned int Edic_Internal::getCandidateCount() const
{
	checkInit();
	return m_query.recordCount();
}

unsigned int Edic_Internal::getCandidatePageCount() const
{
	checkInit();
	auto count = getCandidateCount();
	return (count % getCandidatePageSize() == 0) ? count / getCandidatePageSize() : count / getCandidatePageSize() + 1;
}

void Edic_Internal::getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const
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

void Edic_Internal::getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const
{
	getCandidate(page * getCandidatePageSize(), getCandidatePageSize(), candidates);
}

bool Edic_Internal::learn(const std::string & word)
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
		firstLetterQ.search(word.substr(0, 1), Query::Condition::like);
		if(firstLetterQ.recordCount() != 0)
			priority = std::stoi(std::get<1>(firstLetterQ.getRecord(0)));
		return Database::learn(word, priority + 1);
	}
	else
	{
		return false;
	}
}

bool Edic_Internal::forget(const std::string & word)
{
	checkInit();
	if (word.empty())
		return false;

	return Database::forget(word);
}

bool Edic_Internal::promote(const std::string & prefix, const std::string & word)
{
	checkInit();
	if (prefix.empty() || word.empty())
		return false;

	Query q;
	q.search(prefix.substr(0, 1), Query::Condition::like);
	int highestPriority = 0;
	if (q.recordCount() != 0)
		highestPriority = std::stoi(std::get<1>(q.getRecord(0)));

	return Database::update(word, highestPriority + 1);
}

void Edic_Internal::checkInit() const
{
	if (!m_init)
	{
		printf("any api must called after init success, exit.\r\n");
		std::exit(-1);
	}
}
