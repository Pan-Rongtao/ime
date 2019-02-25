#include "stroke/Stroke.h"
#include "Stroke_Internal.h"
#include <cstdlib>

using namespace ime::stroke;
static bool g_instanced = false;

Stroke::Stroke()
{
	if (!g_instanced)
	{
		m_internal = new Stroke_Internal();
		g_instanced = true;
	}
	else
	{
		printf("not allow to create two stroke engines in one process, exit with code 1.\n");
		std::exit(1);
	}
}

Stroke::~Stroke()
{
	delete m_internal;
}

bool Stroke::init(const std::string & StrokeDBPath)
{
	return m_internal->init(StrokeDBPath);
}

bool Stroke::hasInit() const
{
	return m_internal->hasInit();
}

bool Stroke::deinit()
{
	return m_internal->deinit();
}

void Stroke::setCandidatePageSize(unsigned int size)
{
	m_internal->setCandidatePageSize(size);
}

unsigned int Stroke::getCandidatePageSize() const
{
	return m_internal->getCandidatePageSize();
}

bool Stroke::search(const std::string & stroke)
{
	return m_internal->search(stroke);
}

unsigned int Stroke::getCandidateCount() const
{
	return m_internal->getCandidateCount();
}

unsigned int Stroke::getCandidatePageCount() const
{
	return m_internal->getCandidatePageCount();
}

void Stroke::getCandidate(unsigned int index, unsigned int count, std::vector<std::string>& candidates) const
{
	m_internal->getCandidate(index, count, candidates);
}

void Stroke::getCandidateByPage(unsigned int page, std::vector<std::string>& candidates) const
{
	m_internal->getCandidateByPage(page, candidates);
}

bool Stroke::promote(const std::string & stroke, const std::string & word)
{
	return m_internal->promote(stroke, word);
}

