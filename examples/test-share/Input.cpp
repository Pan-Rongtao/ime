#include "Input.h"
#ifdef WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

#ifndef WIN32
static struct termios oldt;
void restore_terminal_settings(void)
{
    //Apply saved settings
    tcsetattr(0, TCSANOW, &oldt); 
}

void disable_terminal_return(void)
{
    struct termios newt;
    
    //save terminal settings
    tcgetattr(0, &oldt); 
    //init new settings
    newt = oldt;  
    //change settings
    newt.c_lflag &= ~(ICANON | ECHO);
    //apply settings
    tcsetattr(0, TCSANOW, &newt);
    
    //make sure settings will be restored when program ends
    atexit(restore_terminal_settings);
}
#endif

Input::Input()
{
#ifndef WIN32
	disable_terminal_return();
#endif 
}

Input::~Input()
{

}

void Input::setEvent(InputEvent *p)
{
	m_pEvent = p;
}

int Input::getChar()
{
#ifdef WIN32
	return _getch();
#else
	return getchar();	//QNX控制台下经常（特别是在有中文输出的情况下）获取到错误的键位（此问题不知如何解决），比如第一次输入s可能会获取到54,99两个char，此问题导致有中文的几个测试程序有测试问题的表现，但实际上引擎无问题
#endif
}

void Input::cleanScreen()
{
#ifdef WIN32
	system("cls");
#elif __QNX__	//QNX无清屏命令

#else
	system("clear");
#endif
}


void Input::get()
{
	char c = getChar();
	if (c == 0)			//功能键F1-F10有两个char（每个系统的功能键都不一样，目前只有windows有效）
	{
		c = getChar();
		if (c >= 59 && c <= 69)
			m_pEvent->OnFunctionKeyInput(c - 58);
	}
	else if (c == -32)	//F11, F12
	{
		c = getChar();
		switch (c)
		{
#ifdef WIN32
		case -123:	m_pEvent->OnFunctionKeyInput(11);	break;
		case -122:	m_pEvent->OnFunctionKeyInput(12);	break;
#endif
		default:										break;
		}
	}
	else
	{
		switch (c)
		{
		case ' ':	m_pEvent->OnSpaceInput();			break;
		case '\'':	m_pEvent->OnDivideInput();			break;
		case '=':	m_pEvent->OnPageUpInput();			break;
		case '-':	m_pEvent->OnPageDownInput();		break;
		case '1': case '2': case '3': case '4': 
		case '5': case '6': case '7': case '8': 
		case '9':	m_pEvent->OnNumberInput(c - 48);	break;
		case '!':	m_pEvent->OnShiftNumberInput(1);	break;
		case '@':	m_pEvent->OnShiftNumberInput(2);	break;
		case '#':	m_pEvent->OnShiftNumberInput(3);	break;
		case '$':	m_pEvent->OnShiftNumberInput(4);	break;
		case '%':	m_pEvent->OnShiftNumberInput(5);	break;
		case '^':	m_pEvent->OnShiftNumberInput(6);	break;
		case '&':	m_pEvent->OnShiftNumberInput(7);	break;
		case '*':	m_pEvent->OnShiftNumberInput(8);	break;
		case '(':	m_pEvent->OnShiftNumberInput(9);	break;
		case 27:	m_pEvent->OnEscInput();				break;
		case '\b':
		case 127:	m_pEvent->OnDeleteInput();			break;
		case '\r':
		case '\n':	m_pEvent->OnEnterInput();			break;
		default:	m_pEvent->OnCharInput(c);			break;
		}
	}
}
