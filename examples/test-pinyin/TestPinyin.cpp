#include "TestPinyin.h"
#include <iostream>
#include <map>
#include <thread>
#include <stdarg.h>
#include <chrono>
#include <algorithm>
#include "../test-share/Encoding.h"

#ifdef WIN32
#define ETC_DIR				std::string("../../etc/")
#else
#define ETC_DIR				std::string("../etc/")
#endif
#define SEPERATOR_LINE		std::string("---------------------------------")

TestPinyin::TestPinyin()
{
	m_ipt.setEvent(this);
	showHelp();
	init(ETC_DIR, true, true, 9);
}

void TestPinyin::run()
{
	while (1)
	{
		m_ipt.get();
	}
}

void TestPinyin::OnCharInput(char c)
{
	if (c >= 'A' && c <= 'Z')
		m_inputPinyin += c + 32;
	else if (c >= 'a' && c <= 'z')
		m_inputPinyin += c;
	else
		return;

	if (m_pickedCandidates.empty())
	{
		newSearch(m_inputPinyin);
	}
	else
	{
		newSearch(m_pinyin.getSearchingDividedPinyin() + c);
	}
}

void TestPinyin::OnFunctionKeyInput(int f)
{
	switch (f)
	{
	case 1:		showHelp();		break;
	case 2:		showConfig();	break;
	case 12: 
	{
		auto k = std::chrono::high_resolution_clock::now();
			m_pinyin.search("tapotipotipoti");
			showPageCandidate(1);
		m_pinyin.search("wohxhkhkhkhkhkhkhkhkhkhkhkhkhkkhkhkhkhkhkhkhkhkhkhhkhkhkhkhkhkhkhhkh");
		//	m_pinyin.learn("j'sha", u8"给杀");
		//	m_pinyin.forget("hao'de", u8"好的");
		//	m_pinyin.promote("hao'de", u8"好得");
		auto kk = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - k);
		printf("test search cost %d ms.\n", (int)kk.count());
	}
	default:
		break;
	}
}

void TestPinyin::OnDivideInput()
{
	if (isOnSearching())
	{
		if (!m_inputPinyin.empty() && m_inputPinyin.back() != '\'')
			m_inputPinyin.append("'");
		updateShow();
	}
	else
	{
		m_result += " ";
		updateShow();
	}
}

void TestPinyin::OnSpaceInput()
{
	if (isOnSearching())
	{
		pick(m_curPage, 0);
	}
	else
	{
		m_result += " ";
		updateShow();
	}
}

void TestPinyin::OnPageUpInput()
{
	if (isOnSearching())
	{
		if (m_curPage < m_pinyin.getCandidatePageCount() - 1)
		{
			++m_curPage;
			updateShow();
		}
	}
}

void TestPinyin::OnPageDownInput()
{
	if (isOnSearching())
	{
		if (m_curPage > 0)
		{
			--m_curPage;
			updateShow();
		}
	}
}

void TestPinyin::OnDeleteInput()
{
	if (isOnSearching())
	{
		if (m_pickedCandidates.empty())
		{
			m_inputPinyin.pop_back();
			newSearch(m_inputPinyin);
		}
		else
		{
			std::string sDevidedPinyin = m_pinyin.getSearchingDividedPinyin();
			sDevidedPinyin = m_pickedCandidates.back().devidedPinyin + sDevidedPinyin;
			m_pickedCandidates.pop_back();
			newSearch(sDevidedPinyin);
		}
	}
	else
	{
		std::wstring ws = Encoding::utf8ToUnicode(m_result);
		if (!ws.empty())
			ws.pop_back();
		m_result = Encoding::unicodeToUtf8(ws);
		updateShow();
	}
}

void TestPinyin::OnNumberInput(unsigned int n)
{
	if (isOnSearching())
	{
		pick(m_curPage, n - 1);
	}
	else
	{
		m_result += std::to_string(n);
		updateShow();
	}
}

void TestPinyin::OnEscInput()
{
	finished();
}

void TestPinyin::OnEnterInput()
{
	if(isOnSearching())
	{
		auto picked = getPicked();
		std::string s = m_pinyin.getSearchingDividedPinyin();
		s.assign(s.begin(), std::remove(s.begin(), s.end(), '\''));
		std::string sPickCombine = std::get<1>(picked) + s;
		m_result += sPickCombine;
		updateShow();
		finished();
	}
	else
	{
		m_result += "\n";
		updateShow();
	}
}

void TestPinyin::OnShiftNumberInput(unsigned int n)
{
	n = n - 1;
	std::vector<std::string> cans;
	if(m_curPage < m_pinyin.getCandidatePageCount())
		m_pinyin.getCandidateByPage(m_curPage, cans);
	if (n < cans.size())
	{
		ime::pinyin::CandidateInfo info;
		m_pinyin.getCandidateInfo(m_curPage, n, info);
		bool b = m_pinyin.forget(info.pinyin, info.cizu);
		newSearch(m_inputPinyin);
	}
}

void TestPinyin::init(const std::string & configPath, bool enableCombieCandidate, bool enableAssociateCandidate, int pageSize)
{
	printf("【init】\n\n");
	auto beg = std::chrono::high_resolution_clock::now();
	bool b = m_pinyin.init(configPath + "system-py.db", configPath + "learn-py.db");
	m_pinyin.enableAICombineCandidate(enableCombieCandidate);
	m_pinyin.enableAssociateCandidate(enableAssociateCandidate);
	m_pinyin.setCandidatePageSize(pageSize);
	auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - beg);
	printf("init pinyin engine %s, cost [%d] ms. configPath[%s], enableCombieCandidate[%s], enableAssociateCandidate[%s], pageSize[%d]\n", 
		b ? "success" : "fail", (int)cost.count(), configPath.data(), b ? "success" : "fail", b ? "success" : "fail", pageSize);
}

bool TestPinyin::isOnSearching() const
{
	return !m_inputPinyin.empty();
}

void TestPinyin::newSearch(const std::string &str)
{
	m_pinyin.search(str);
	m_curPage = 0;
	updateShow();
}

void TestPinyin::updateShow()
{
	Input::cleanScreen();
	showSearchLine();
	showPageCandidate(m_curPage);
	showResult();
}

void TestPinyin::showResult()
{
	printf("\n%s", Encoding::utf8ToGb2312(m_result.data()).data());
#ifndef WIN32
	printf("\r");
#endif
}

void TestPinyin::showSearchLine()
{
	auto picked = getPicked();
	std::string sContent = Encoding::utf8ToGb2312(std::get<1>(picked).data()) + m_pinyin.getSearchingDividedPinyin();
	if (!m_inputPinyin.empty() && m_inputPinyin.back() == '\'')
		sContent += "'";

	std::string sAll = SEPERATOR_LINE + "\n" + sContent + "\n" + SEPERATOR_LINE + "\n";
	printf("%s", sAll.data());
}

void TestPinyin::showPageCandidate(unsigned int index)
{
	if (index >= m_pinyin.getCandidatePageCount())
	{
		std::string s(m_pinyin.getCandidatePageSize(), '\n');
		printf("%s\n", (s + SEPERATOR_LINE).data());
		return;
	}

	std::string sAll;
	std::vector<std::string> candidates;
	m_pinyin.getCandidateByPage(index, candidates);
	for (int i = 0; i != candidates.size(); ++i)
	{
		ime::pinyin::CandidateInfo info;
		m_pinyin.getCandidateInfo(m_curPage, i, info);
		std::string can = candidates[i] + (info.canForget ? "\t\t(x)" : "");
		sAll = sAll + std::to_string(i + 1) + ".\t" + can + "\n";
	}
#ifdef WIN32
	sAll = Encoding::utf8ToGb2312(sAll.data());
#endif

	sAll.insert(sAll.end(), m_pinyin.getCandidatePageSize() - candidates.size(), '\n');
	printf("%s[page:%d/%d, total:%d]\n", (sAll + SEPERATOR_LINE).data(), m_curPage + 1, m_pinyin.getCandidatePageCount(), m_pinyin.getCandidateCount());
}

void TestPinyin::showHelp()
{
	Input::cleanScreen();
	printf("【help】\n\n");
	printf("F1:help | F2:config | [a~z]:type | [1~9]:pick | [shift 1~9]:remote | [-=]:switch page.\n\n\n");
}

std::string TestPinyin::getLine()
{
	std::string ret;
	char c;
	do {
		c = getchar();
		if (c == '\n')
			break;
		ret += c;
	} while (1);
	return ret;
}

void TestPinyin::showConfig()
{
	Input::cleanScreen();
	printf("[config]\n\n");
	printf("config ciku path(../../etc/):");
	std::string s = getLine();
	std::string sConfigPath = s.empty() ? "../../etc/" : s;
	printf("%s\n", sConfigPath.data());

	printf("config enable combined cizu:(y/n)");
	std::string ss = getLine();
	ss = ss.empty() || ss == "y" ? "y" : "n";
	bool bEnableCombinedCizu = ss == "y" ? true : false;
	printf("%s\n", ss == "y" ? "y" : "n");
	
	printf("config enable associate cizu:(y/n)");
	std::string sss = getLine();
	sss = sss.empty() || sss == "y" ? "y" : "n";
	bool bEnableAssociateCizu = sss == "y" ? true : false;
	printf("%s\n", sss == "y" ? "y" : "n");

	printf("config page size:(9)");
	std::string ssss = getLine();
	int pageSize = 9;
	try {
		pageSize = std::stoi(ssss);
	}
	catch (...) {}
	printf("%d\n", pageSize);

	init(sConfigPath, bEnableCombinedCizu, bEnableAssociateCizu, pageSize);
}

void TestPinyin::pick(unsigned int page, unsigned int index)
{
	std::vector<std::string> candidates;
	m_pinyin.getCandidateByPage(page, candidates);
	if (page >= m_pinyin.getCandidatePageCount() || index >= candidates.size())
		return;

	ime::pinyin::CandidateInfo info;
	m_pinyin.getCandidateInfo(page, index, info);
	m_pickedCandidates.push_back(info);

	//截取候选词后计算余下pinyin并新搜索
	std::string sRemainPinyin = m_pinyin.getSearchingDividedPinyin().substr(info.devidedPinyin.size());
	if (!sRemainPinyin.empty())
	{
		if (sRemainPinyin.front() == '\'')
			sRemainPinyin.erase(sRemainPinyin.begin());
		newSearch(sRemainPinyin);
	}
	else
	{
		//学习新词，如果存在则提高该次的词频
		auto picked = getPicked();
		std::string pickedPinyins = std::get<0>(picked);
		std::string pickedCizus = std::get<1>(picked);
		std::string pickedCompletedPinyins = std::get<2>(picked);
		bool bExists = false;
		bool b = m_pinyin.learn(pickedCompletedPinyins, pickedCizus, bExists);
		if (bExists)
		{
			m_pinyin.promote(pickedCompletedPinyins, pickedCizus);
		}
		m_result += pickedCizus;
		finished();
	}
}

std::tuple<std::string, std::string, std::string> TestPinyin::getPicked() const
{
	std::string sPickPinyins;
	std::string sPickedCizus;
	std::string sPickedCompletedPinyins;
	for (int i = 0; i != m_pickedCandidates.size(); ++i)
	{
		sPickPinyins += m_pickedCandidates[i].devidedPinyin;
		sPickedCompletedPinyins += m_pickedCandidates[i].pinyin;
		if (i != m_pickedCandidates.size() - 1)
		{
			sPickPinyins += "'";
			sPickedCompletedPinyins += "'";
		}
		sPickedCizus += m_pickedCandidates[i].cizu;
	}
	return std::make_tuple(sPickPinyins, sPickedCizus, sPickedCompletedPinyins);
}

void TestPinyin::finished()
{
	m_pinyin.search("");
	m_inputPinyin.clear();
	m_curPage = 0;
	m_pickedCandidates.clear();
	updateShow();
}
