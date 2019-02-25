#include "wubi/Wubi.h"
#include "Wubi_Internal.h"
#include <cstdlib>


using namespace ime::wubi;
static bool g_instanced = false;

Wubi::Wubi()
{
	if (!g_instanced)
	{
		m_internal = new Wubi_Internal();
		g_instanced = true;
	}
	else
	{
		printf("not allow to create two stroke engines in one process, exit with code 1.\n");
		std::exit(1);
	}
}

Wubi::~Wubi()
{
	delete m_internal;
}

bool Wubi::init(const std::string & WubiDBPath)
{
	return m_internal->init(WubiDBPath);
}

bool Wubi::hasInit() const
{
	return m_internal->hasInit();
}

bool Wubi::deinit()
{
	return m_internal->deinit();
}

void Wubi::setCandidatePageSize(unsigned int size)
{
	m_internal->setCandidatePageSize(size);
}

unsigned int Wubi::getCandidatePageSize() const
{
	return m_internal->getCandidatePageSize();
}

bool Wubi::search(const std::string & zigen)
{
	return m_internal->search(zigen);
}

unsigned int Wubi::getCandidateCount() const
{
	return m_internal->getCandidateCount();
}

unsigned int Wubi::getCandidatePageCount() const
{
	return m_internal->getCandidatePageCount();
}

void Wubi::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	m_internal->getCandidate(index, count, candidates);
}

void Wubi::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	m_internal->getCandidateByPage(page, candidates);
}

bool Wubi::promote(const std::string & zigen, const std::string & word)
{
	return m_internal->promote(zigen, word);
}
