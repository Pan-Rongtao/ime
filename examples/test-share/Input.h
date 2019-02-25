#include <iostream>
#include <functional>

class InputEvent;
class Input
{
public:
	Input();
	~Input();
	void setEvent(InputEvent *p);
	void get();

	static int getChar();
	static void cleanScreen();

private:
	InputEvent	*m_pEvent{ nullptr };
};

class InputEvent
{
public:
	virtual void OnFunctionKeyInput(int f) {}
	virtual void OnSpaceInput() {}
	virtual void OnDivideInput() {}
	virtual void OnPageUpInput() {}
	virtual void OnPageDownInput() {}
	virtual void OnDeleteInput() {}
	virtual void OnNumberInput(unsigned int n) {}
	virtual void OnShiftNumberInput(unsigned int n) {}
	virtual void OnEscInput() {}
	virtual void OnEnterInput() {}
	virtual void OnCharInput(char c) {}
};
