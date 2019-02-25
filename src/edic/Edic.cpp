#include "edic/Edic.h"
#include "Edic_Internal.h"
#include <cstdlib>

using namespace ime::edic;

static bool g_instanced = false;

Edic::Edic()
{
	if (!g_instanced)
	{
		m_internal = new Edic_Internal();
		g_instanced = true;
	}
	else
	{
		printf("not allow to create two edic engines in one process, exit with code 1.\n");
		std::exit(1);
	}
}

Edic::~Edic()
{
	delete m_internal;
}

bool Edic::init(const std::string & EdicDBPath)
{
	return m_internal->init(EdicDBPath);
}

bool Edic::hasInit() const
{
	return m_internal->hasInit();
}

bool Edic::deinit()
{
	return m_internal->deinit();
}

void Edic::setCandidatePageSize(unsigned int size)
{
	m_internal->setCandidatePageSize(size);
}

unsigned int Edic::getCandidatePageSize() const
{
	return m_internal->getCandidatePageSize();
}

bool Edic::search(const std::string & prefix)
{
	return m_internal->search(prefix);
}

unsigned int Edic::getCandidateCount() const
{
	return m_internal->getCandidateCount();
}

unsigned int Edic::getCandidatePageCount() const
{
	return m_internal->getCandidatePageCount();
}

void Edic::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	m_internal->getCandidate(index, count, candidates);
}

void Edic::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	m_internal->getCandidateByPage(page, candidates);
}

bool Edic::learn(const std::string & word)
{
	return m_internal->learn(word);
}

bool Edic::forget(const std::string & word)
{
	return m_internal->forget(word);
}

bool Edic::promote(const std::string & prefix, const std::string & word)
{
	return m_internal->promote(prefix, word);
}
