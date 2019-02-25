#include "TestWubi.h"
#include "../test-share/Encoding.h"
#include <chrono>

#ifdef WIN32
#define ETC_DIR				std::string("../../etc/")
#else
#define ETC_DIR				std::string("../etc/")
#endif
#define SEPERATOR_LINE		std::string("---------------------------------")

TestWubi::TestWubi()
{
	m_ipt.setEvent(this);
	init(ETC_DIR + "wubi.db", 9);
}

void TestWubi::run()
{
	while (1)
	{
		m_ipt.get();
	}
}

void TestWubi::OnCharInput(char c)
{
	m_searchZigen += c;
	newSearch(m_searchZigen);
}

void TestWubi::OnFunctionKeyInput(int f)
{
	switch (f)
	{
	case 1:		break;
	case 2:		break;
	default:	break;
	}
}

void TestWubi::OnNumberInput(unsigned int n)
{
	if (!m_searchZigen.empty())
	{
		if (m_curPage >= m_wubi.getCandidatePageCount())
			return;
		std::vector<std::string> cans;
		m_wubi.getCandidateByPage(m_curPage, cans);
		if (n - 1 >= cans.size())
			return;
		std::string sPick = cans[n - 1];
		m_wubi.promote(m_searchZigen, sPick);
		m_searchZigen.clear();
		newSearch("");
	}
}

void TestWubi::OnSpaceInput()
{
	OnNumberInput(1);
}

void TestWubi::OnPageUpInput()
{
	if (m_curPage < m_wubi.getCandidatePageCount() - 1)
	{
		++m_curPage;
		updateShow();
	}
}

void TestWubi::OnPageDownInput()
{
	if (m_curPage > 0)
	{
		--m_curPage;
		updateShow();
	}
}

void TestWubi::OnDeleteInput()
{
	if (!m_searchZigen.empty())
		m_searchZigen.pop_back();
	newSearch(m_searchZigen);
}

void TestWubi::OnEscInput()
{
	m_searchZigen.clear();
	newSearch(m_searchZigen);
}

void TestWubi::init(const std::string &configPath, int pageSize)
{
	auto beg = std::chrono::high_resolution_clock::now();
	bool b = m_wubi.init(configPath);
	m_wubi.setCandidatePageSize(pageSize);
	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beg);
	printf("init wubi engine %s, cost [%d] ms, config [%s], pageSize [%d]\n", b ? "success" : "fail", (int)cost.count(), configPath.data(), pageSize);
}

void TestWubi::newSearch(const std::string &str)
{
	m_wubi.search(str);
	m_curPage = 0;
	updateShow();
}

void TestWubi::updateShow()
{
	Input::cleanScreen();
	showSearchLine();
	showPageCandidate(m_curPage);
}

void TestWubi::showSearchLine()
{
	std::string sAll = SEPERATOR_LINE + "\n" + Encoding::utf8ToGb2312(m_searchZigen.data()) + "\n" + SEPERATOR_LINE + "\n";
	printf("%s", sAll.data());
}

void TestWubi::showPageCandidate(unsigned int index)
{
	if (index >= m_wubi.getCandidatePageCount())
	{
		std::string s(m_wubi.getCandidatePageSize(), '\n');
		printf("%s\n", (s + SEPERATOR_LINE).data());
		return;
	}

	std::string sAll;
	std::vector<std::string> candidates;
	m_wubi.getCandidateByPage(index, candidates);
	for (int i = 0; i != candidates.size(); ++i)
	{
		std::string can = candidates[i];
		sAll = sAll + std::to_string(i + 1) + ".\t" + can + "\n";
	}
#ifdef WIN32
	sAll = Encoding::utf8ToGb2312(sAll.data());
#endif

	sAll.insert(sAll.end(), m_wubi.getCandidatePageSize() - candidates.size(), '\n');
	printf("%s[page:%d/%d, total:%d]\n", (sAll + SEPERATOR_LINE).data(), m_curPage + 1, m_wubi.getCandidatePageCount(), m_wubi.getCandidateCount());
}
