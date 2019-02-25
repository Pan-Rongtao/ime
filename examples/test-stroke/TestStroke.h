#include "stroke/Stroke.h"
#include "../test-share/Input.h"

class TestStroke : public InputEvent
{
public:
	TestStroke();
	void run();

private:
	virtual void OnCharInput(char c) override;
	virtual void OnFunctionKeyInput(int f) override;
	virtual void OnNumberInput(unsigned int n) override;
	virtual void OnPageUpInput() override;
	virtual void OnPageDownInput() override;
	virtual void OnDeleteInput() override;
	virtual void OnEscInput() override;
	virtual void OnSpaceInput() override;

	void init(const std::string &configPath, int pageSize);
	void newSearch(const std::string &str);
	void updateShow();
	void showSearchLine();
	void showPageCandidate(unsigned int index);

	Input						m_ipt;
	ime::stroke::Stroke			m_stroke;
	std::string					m_searchStroke;
	std::string					m_searchStrokeNum;
	unsigned int				m_curPage{ 0 };
};