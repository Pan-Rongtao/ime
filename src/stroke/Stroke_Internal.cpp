#include "Stroke_Internal.h"
#include <exception>
#include <cstdlib>

using namespace ime::stroke;

Stroke_Internal::Stroke_Internal()
	: m_init(false)
	, m_candidatePageSize(5)
{
}

Stroke_Internal::~Stroke_Internal()
{
}

bool Stroke_Internal::init(const std::string &associateDBPath)
{
	deinit();
	m_init = Database::load(associateDBPath);
	return m_init;
}

bool Stroke_Internal::hasInit() const
{
	return m_init;
}

bool Stroke_Internal::deinit()
{
	m_init = false;
	m_query.release();
	Database::unload();
	return true;
}

void Stroke_Internal::setCandidatePageSize(unsigned int size)
{
	if (size == 0)	throw std::out_of_range("setCandidatePageSize size by 0.");
	m_candidatePageSize = size;
}

unsigned int Stroke_Internal::getCandidatePageSize() const
{
	return m_candidatePageSize;
}

bool Stroke_Internal::search(const std::string &stroke)
{
	return m_query.search(stroke, Query::Condition::like);
}

unsigned int Stroke_Internal::getCandidateCount() const
{
	return m_query.recordCount();
}

unsigned int Stroke_Internal::getCandidatePageCount() const
{
	checkInit();
	auto count = getCandidateCount();
	return (count % getCandidatePageSize() == 0) ? count / getCandidatePageSize() : count / getCandidatePageSize() + 1;
}

void Stroke_Internal::getCandidate(unsigned int index, unsigned int count, std::vector<std::string> &candidates) const
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

void Stroke_Internal::getCandidateByPage(unsigned int page, std::vector<std::string> &candidates) const
{
	getCandidate(page * getCandidatePageSize(), getCandidatePageSize(), candidates);
}

bool Stroke_Internal::promote(const std::string & stroke, const std::string & word)
{
	if (stroke.empty() || word.empty())
		return false;

	Query q;
	int highestPriority = q.searchHighestPriority(stroke.substr(0, 1));
	return Database::update(word, highestPriority + 1);
}

void Stroke_Internal::checkInit() const
{
	if (!m_init)
	{
		printf("any api must called after init success, exit.\r\n");
		std::exit(-1);
	}
}
