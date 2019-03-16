#include "TestEdic.h"
#include "../test-share/Encoding.h"
#include <chrono>

#define ETC_DIR				std::string("../etc/")
#define SEPERATOR_LINE		std::string("---------------------------------")

TestEdic::TestEdic()
{
	m_ipt.setEvent(this);
	init(ETC_DIR + "edic.db", 9);
}

void TestEdic::run()
{
	while (1)
	{
		m_ipt.get();
	}
}

void TestEdic::OnCharInput(char c)
{
	m_searchWordUtf8 += c;
	newSearch(m_searchWordUtf8);
}

void TestEdic::OnFunctionKeyInput(int f)
{
	switch (f)
	{
	case 1:		m_edic.learn(u8"PanRongtao");		break;
	case 2:		m_edic.forget(u8"PanRongtao");		break;
	default:
		break;
	}
}

void TestEdic::OnNumberInput(unsigned int n)
{
	if (!m_searchWordUtf8.empty())
	{
		if (m_curPage >= m_edic.getCandidatePageCount())
			return;
		std::vector<std::string> cans;
		m_edic.getCandidateByPage(m_curPage, cans);
		if (n - 1 >= cans.size())
			return;
		std::string sPick = cans[n - 1];
		m_edic.promote(m_searchWordUtf8, sPick);
		m_searchWordUtf8.clear();
		newSearch("");
	}
}

void TestEdic::OnSpaceInput()
{
//	OnNumberInput(1);
	OnCharInput(' ');
}

void TestEdic::OnPageUpInput()
{
	if (m_curPage < m_edic.getCandidatePageCount() - 1)
	{
		++m_curPage;
		updateShow();
	}
}

void TestEdic::OnPageDownInput()
{
	if (m_curPage > 0)
	{
		--m_curPage;
		updateShow();
	}
}

void TestEdic::OnDeleteInput()
{
	if (!m_searchWordUtf8.empty())
		m_searchWordUtf8.pop_back();
	newSearch(m_searchWordUtf8);
}

void TestEdic::OnEscInput()
{
	m_searchWordUtf8.clear();
	newSearch(m_searchWordUtf8);
}

void TestEdic::init(const std::string &configPath, int pageSize)
{
	auto beg = std::chrono::high_resolution_clock::now();
	bool b = m_edic.init(configPath);
	m_edic.setCandidatePageSize(pageSize);
	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beg);
	printf("init edic engine %s, cost [%d] ms, config [%s], pageSize [%d]\n", b ? "success" : "fail", (int)cost.count(), configPath.data(), pageSize);
}

void TestEdic::newSearch(const std::string &str)
{
	m_edic.search(str);
	m_curPage = 0;
	updateShow();
}

void TestEdic::updateShow()
{
	Input::cleanScreen();
	showSearchLine();
	showPageCandidate(m_curPage);
}

void TestEdic::showSearchLine()
{
	std::string sAll = SEPERATOR_LINE + "\n" + m_searchWordUtf8 + "\n" + SEPERATOR_LINE + "\n";
	printf("%s", sAll.data());
}

void TestEdic::showPageCandidate(unsigned int index)
{
	if (index >= m_edic.getCandidatePageCount())
	{
		std::string s(m_edic.getCandidatePageSize(), '\n');
		printf("%s\n", (s + SEPERATOR_LINE).data());
		return;
	}

	std::string sAll;
	std::vector<std::string> candidates;
	m_edic.getCandidateByPage(index, candidates);
	for (int i = 0; i != candidates.size(); ++i)
	{
		std::string can = candidates[i];
		sAll = sAll + std::to_string(i + 1) + ".\t" + can + "\n";
	}
#ifdef WIN32
	sAll = Encoding::utf8ToGb2312(sAll.data());
#endif

	sAll.insert(sAll.end(), m_edic.getCandidatePageSize() - candidates.size(), '\n');
	printf("%s[page:%d/%d, total:%d]\n", (sAll + SEPERATOR_LINE).data(), m_curPage + 1, m_edic.getCandidatePageCount(), m_edic.getCandidateCount());
}
