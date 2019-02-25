#include "associate/Associate.h"
#include "../test-share/Input.h"

class TestAssociate : public InputEvent
{
public:
	TestAssociate();
	void run();

private:
	virtual void OnCharInput(char c) override;
	virtual void OnFunctionKeyInput(int f) override;
	virtual void OnNumberInput(unsigned int n) override;
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
	ime::associate::Associate	m_associate;
	std::string					m_searchWord2312;
	std::string					m_searchWordUtf8;
	unsigned int				m_curPage{ 0 };
};