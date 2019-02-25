#include "associate/Associate.h"
#include "Associate_Internal.h"
#include <cstdlib>

using namespace ime::associate;

static bool g_instanced = false;

Associate::Associate()
	: m_internal(nullptr)
{
	if (!g_instanced)
	{
		m_internal = new Associate_Internal();
		g_instanced = true;
	}
	else
	{
		printf("not allow to create two associate engines in one process, exit with code 1.\n");
		std::exit(1);
	}
}

Associate::~Associate()
{
	delete m_internal;
}

bool Associate::init(const std::string & associateDBPath)
{
	return m_internal->init(associateDBPath);
}

bool Associate::hasInit() const
{
	return m_internal->hasInit();
}

bool Associate::deinit()
{
	return m_internal->deinit();
}

void Associate::setCandidatePageSize(unsigned int size)
{
	m_internal->setCandidatePageSize(size);
}

unsigned int Associate::getCandidatePageSize() const
{
	return m_internal->getCandidatePageSize();
}

bool Associate::search(const std::string & prefix)
{
	return m_internal->search(prefix);
}

unsigned int Associate::getCandidateCount() const
{
	return m_internal->getCandidateCount();
}

unsigned int Associate::getCandidatePageCount() const
{
	return m_internal->getCandidatePageCount();
}

void Associate::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	m_internal->getCandidate(index, count, candidates);
}

void Associate::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	m_internal->getCandidateByPage(page, candidates);
}

bool Associate::learn(const std::string & word)
{
	return m_internal->learn(word);
}

bool Associate::forget(const std::string & word)
{
	return m_internal->forget(word);
}

bool Associate::promote(const std::string &prefix, const std::string & word)
{
	return m_internal->promote(prefix, word);
}
