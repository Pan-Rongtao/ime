﻿#include "hwr/Hwr.h"
#include "../test-share/Input.h"

class TestHwr : public InputEvent
{
public:
	TestHwr();
	void run();

private:
	virtual void OnCharInput(char c) override;
	virtual void OnFunctionKeyInput(int f) override;
	virtual void OnNumberInput(unsigned int n) override;
	virtual void OnSpaceInput() override;
	virtual void OnPageUpInput() override;
	virtual void OnPageDownInput() override;
	virtual void OnDeleteInput() override;
	virtual void OnEscInput() override;

	void init(const std::string &configPath, int pageSize);
	void newSearch(const std::string &str);
	void updateShow();
	void showSearchLine();
	void showPageCandidate(unsigned int index);

	Input						m_ipt;
	ime::hwr::Hwr				m_hwr;
	std::string					m_searchWordUtf8;
	unsigned int				m_curPage{ 0 };
};