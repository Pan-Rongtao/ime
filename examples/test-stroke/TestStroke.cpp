#include "TestStroke.h"
#include "../test-share/Encoding.h"
#include <chrono>

#ifdef WIN32
#define ETC_DIR				std::string("../../etc/")
#else
#define ETC_DIR				std::string("../etc/")
#endif
#define SEPERATOR_LINE		std::string("---------------------------------")

TestStroke::TestStroke()
{
	m_ipt.setEvent(this);
	init(ETC_DIR + "stroke.db", 9);
}

void TestStroke::run()
{
	while (1)
	{
		m_ipt.get();
	}
}

void TestStroke::OnCharInput(char c)
{
	switch (c)
	{
	case 'a': case 'A':	m_searchStroke += u8"一";	m_searchStrokeNum += std::to_string(1);	break;
	case 's': case 'S':	m_searchStroke += u8"|";	m_searchStrokeNum += std::to_string(2); break;
	case 'd': case 'D':	m_searchStroke += u8"丿";	m_searchStrokeNum += std::to_string(3); break;
	case 'j': case 'J':	m_searchStroke += u8"丶";	m_searchStrokeNum += std::to_string(4); break;
	case 'k': case 'K':	m_searchStroke += u8"乙";	m_searchStrokeNum += std::to_string(5); break;
	default:										break;
	}
	newSearch(m_searchStrokeNum);
}

void TestStroke::OnFunctionKeyInput(int f)
{
	switch (f)
	{
	case 1:		break;
	case 2:		break;
	default:
		break;
	}
}

void TestStroke::OnNumberInput(unsigned int n)
{
	if (!m_searchStroke.empty())
	{
		if (m_curPage >= m_stroke.getCandidatePageCount())
			return;
		std::vector<std::string> cans;
		m_stroke.getCandidateByPage(m_curPage, cans);
		if (n - 1 >= cans.size())
			return;
		std::string sPick = cans[n - 1];
		m_stroke.promote(m_searchStrokeNum, sPick);
		m_searchStroke.clear();
		m_searchStrokeNum.clear();
		newSearch("");
	}
}

void TestStroke::OnPageUpInput()
{
	if (m_curPage < m_stroke.getCandidatePageCount() - 1)
	{
		++m_curPage;
		updateShow();
	}
}

void TestStroke::OnPageDownInput()
{
	if (m_curPage > 0)
	{
		--m_curPage;
		updateShow();
	}
}

void TestStroke::OnDeleteInput()
{
	std::wstring ws = Encoding::utf8ToUnicode(m_searchStroke);
	if (!ws.empty())
		ws.pop_back();
	m_searchStroke = Encoding::unicodeToUtf8(ws);
	if (!m_searchStrokeNum.empty())
		m_searchStrokeNum.pop_back();
	newSearch(m_searchStrokeNum);
}

void TestStroke::OnEscInput()
{
	m_searchStroke.clear();
	m_searchStrokeNum.clear();
	newSearch(m_searchStrokeNum);
}

void TestStroke::OnSpaceInput()
{
	OnNumberInput(1);
}

void TestStroke::init(const std::string &configPath, int pageSize)
{
	auto beg = std::chrono::high_resolution_clock::now();
	bool b = m_stroke.init(configPath);
	m_stroke.setCandidatePageSize(pageSize);
	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beg);
	printf("init stroke engine %s, cost [%d] ms, config [%s], pageSize [%d]\n", b ? "success" : "fail", (int)cost.count(), configPath.data(), pageSize);
}

void TestStroke::newSearch(const std::string &str)
{
	m_stroke.search(str);
	m_curPage = 0;
	updateShow();
}

void TestStroke::updateShow()
{
	Input::cleanScreen();
	showSearchLine();
	showPageCandidate(m_curPage);
}

void TestStroke::showSearchLine()
{
	std::string sAll = SEPERATOR_LINE + "\n" + Encoding::utf8ToGb2312(m_searchStroke.data()) + "\n" + SEPERATOR_LINE + "\n";
	printf("%s", sAll.data());
}

void TestStroke::showPageCandidate(unsigned int index)
{
	if (index >= m_stroke.getCandidatePageCount())
	{
		std::string s(m_stroke.getCandidatePageSize(), '\n');
		printf("%s\n", (s + SEPERATOR_LINE).data());
		return;
	}

	std::string sAll;
	std::vector<std::string> candidates;
	m_stroke.getCandidateByPage(index, candidates);
	for (int i = 0; i != candidates.size(); ++i)
	{
		std::string can = candidates[i];
		sAll = sAll + std::to_string(i + 1) + ".\t" + can + "\n";
	}
#ifdef WIN32
	sAll = Encoding::utf8ToGb2312(sAll.data());
#endif

	sAll.insert(sAll.end(), m_stroke.getCandidatePageSize() - candidates.size(), '\n');
	printf("%s[page:%d/%d, total:%d]\n", (sAll + SEPERATOR_LINE).data(), m_curPage + 1, m_stroke.getCandidatePageCount(), m_stroke.getCandidateCount());
}
