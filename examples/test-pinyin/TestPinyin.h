#pragma once
#include "pinyin/Pinyin.h"
#include "../test-share/Input.h"

class TestPinyin : public InputEvent
{
public:
	TestPinyin();
	void run();

private:
	virtual void OnFunctionKeyInput(int f) override;
	virtual void OnSpaceInput() override;
	virtual void OnDivideInput() override;
	virtual void OnPageUpInput() override;
	virtual void OnPageDownInput() override;
	virtual void OnDeleteInput() override;
	virtual void OnNumberInput(unsigned int n) override;
	virtual void OnShiftNumberInput(unsigned int n) override;
	virtual void OnEscInput() override;
	virtual void OnEnterInput() override;
	virtual void OnCharInput(char c) override;

private:
	void init(const std::string &configPath, bool enableCombieCandidate, bool enableAssociateCandidate, int pageSize);
	bool isOnSearching() const;
	void newSearch(const std::string &str);
	void updateShow();
	void showResult();
	void showSearchLine();
	void showPageCandidate(unsigned int index);
	void showHelp();
	std::string getLine();
	void showConfig();
	void pick(unsigned int page, unsigned int index);
	std::tuple<std::string, std::string, std::string> getPicked() const;
	void finished();

	Input									m_ipt;
	ime::pinyin::Pinyin						m_pinyin;
	std::string								m_inputPinyin;
	std::string								m_result;
	unsigned int							m_curPage{ 0 };
	std::vector<ime::pinyin::CandidateInfo>	m_pickedCandidates;
};