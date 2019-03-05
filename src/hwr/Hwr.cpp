#include "hwr/hwr.h"
#include "Hwr_Internal.h"
#include <cstdlib>

using namespace ime::hwr;
static bool g_instanced = false;

Hwr::Hwr()
{
	if (!g_instanced)
	{
		m_internal = new Hwr_Internal();
		g_instanced = true;
	}
	else
	{
		printf("not allow to create two hwr engines in one process, exit with code 1.\n");
		std::exit(1);
	}
}

Hwr::~Hwr()
{
	delete m_internal;
}

bool Hwr::init(const std::string &authDir, const std::string &appKey, const std::string &developerKey, const std::string &cloudUrl, const std::string &dataDir, const std::string &logPath)
{
	return m_internal->init(authDir, appKey, developerKey, cloudUrl, dataDir, logPath);
}

bool Hwr::hasInit() const
{
	return m_internal->hasInit();
}

bool Hwr::deinit()
{
	return m_internal->deinit();
}

void Hwr::setCandidatePageSize(unsigned int size)
{
	m_internal->setCandidatePageSize(size);
}

unsigned int Hwr::getCandidatePageSize() const
{
	return m_internal->getCandidatePageSize();
}

bool Hwr::recognize(const std::vector<short>& points)
{
	return m_internal->recognize(points);
}

unsigned int Hwr::getCandidateCount() const
{
	return m_internal->getCandidateCount();
}

unsigned int Hwr::getCandidatePageCount() const
{
	return m_internal->getCandidatePageCount();
}

void Hwr::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	m_internal->getCandidate(index, count, candidates);
}

void Hwr::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	m_internal->getCandidateByPage(page, candidates);
}
