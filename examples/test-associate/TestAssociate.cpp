#include "TestAssociate.h"
#include "../test-share/Encoding.h"
#include <chrono>

#ifdef WIN32
#define ETC_DIR				std::string("../../etc/")
#else
#define ETC_DIR				std::string("../etc/")
#endif
#define SEPERATOR_LINE		std::string("---------------------------------")

TestAssociate::TestAssociate()
{
	m_ipt.setEvent(this);
	init(ETC_DIR + "associate.db", 9);
}

void TestAssociate::run()
{
	while (1)
	{
		m_ipt.get();
	}
}

void TestAssociate::OnCharInput(char c)
{
	m_searchWord2312 += c;
#ifdef WIN32
	m_searchWordUtf8 = Encoding::gb2312ToUtf8(m_searchWord2312.data());
#else
	m_searchWordUtf8 = m_searchWord2312;
#endif
	newSearch(m_searchWordUtf8);
}

void TestAssociate::OnFunctionKeyInput(int f)
{
	switch (f)
	{
	case 1:		m_associate.learn(u8"我爱我");		break;
	case 2:		m_associate.forget(u8"我爱我");		break;
	default:
		break;
	}
}

void TestAssociate::OnNumberInput(unsigned int n)
{
	if (!m_searchWordUtf8.empty())
	{
		if (m_curPage >= m_associate.getCandidatePageCount())
			return;
		std::vector<std::string> cans;
		m_associate.getCandidateByPage(m_curPage, cans);
		if (n - 1 >= cans.size())
			return;
		std::string sPick = cans[n - 1];
		m_associate.promote(m_searchWordUtf8, sPick);
		m_searchWord2312.clear();
		m_searchWordUtf8.clear();
		newSearch("");
	}
}

void TestAssociate::OnPageUpInput()
{
	if (m_curPage < m_associate.getCandidatePageCount() - 1)
	{
		++m_curPage;
		updateShow();
	}
}

void TestAssociate::OnPageDownInput()
{
	if (m_curPage > 0)
	{
		--m_curPage;
		updateShow();
	}
}

void TestAssociate::OnDeleteInput()
{
	std::wstring ws = Encoding::utf8ToUnicode(m_searchWordUtf8);
	if (!ws.empty())
		ws.pop_back();
	m_searchWordUtf8 = Encoding::unicodeToUtf8(ws);
	m_searchWord2312 = Encoding::utf8ToGb2312(m_searchWordUtf8.data());
	newSearch(m_searchWordUtf8);
}

void TestAssociate::OnEscInput()
{
	m_searchWord2312.clear();
	m_searchWordUtf8.clear();
	newSearch(m_searchWordUtf8);
}

void TestAssociate::init(const std::string &configPath, int pageSize)
{
	auto beg = std::chrono::high_resolution_clock::now();
	bool b = m_associate.init(configPath);
	m_associate.setCandidatePageSize(pageSize);
	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beg);
	printf("init associate engine %s, cost [%d] ms, config [%s], pageSize [%d]\n", b ? "success" : "fail", (int)cost.count(), configPath.data(), pageSize);
}

void TestAssociate::newSearch(const std::string &str)
{
	m_associate.search(str);
	m_curPage = 0;
	updateShow();
}

void TestAssociate::updateShow()
{
	Input::cleanScreen();
	showSearchLine();
	showPageCandidate(m_curPage);
}

void TestAssociate::showSearchLine()
{
	std::string sAll = SEPERATOR_LINE + "\n" + Encoding::utf8ToGb2312(m_searchWordUtf8.data()) + "\n" + SEPERATOR_LINE + "\n";
	printf("%s", sAll.data());
}

void TestAssociate::showPageCandidate(unsigned int index)
{
	if (index >= m_associate.getCandidatePageCount())
	{
		std::string s(m_associate.getCandidatePageSize(), '\n');
		printf("%s\n", (s + SEPERATOR_LINE).data());
		return;
	}

	std::string sAll;
	std::vector<std::string> candidates;
	m_associate.getCandidateByPage(index, candidates);
	for (int i = 0; i != candidates.size(); ++i)
	{
		std::string can = candidates[i];
		sAll = sAll + std::to_string(i + 1) + ".\t" + can + "\n";
	}
#ifdef WIN32
	sAll = Encoding::utf8ToGb2312(sAll.data());
#endif

	sAll.insert(sAll.end(), m_associate.getCandidatePageSize() - candidates.size(), '\n');
	printf("%s[page:%d/%d, total:%d]\n", (sAll + SEPERATOR_LINE).data(), m_curPage + 1, m_associate.getCandidatePageCount(), m_associate.getCandidateCount());
}
