// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\WinButton.h"

//////////////////////////////////////////////////////////////////////////
CWinButton::CWinButton(void) : CWinWindow()
{
}


//////////////////////////////////////////////////////////////////////////
CWinButton::~CWinButton(void)
{
}


//////////////////////////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////////////////////////
bool CWinButton::SetCheck(bool Check)
{
	if(m_Hwnd)
	{
		Button_SetCheck(m_Hwnd, Check);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinButton::GetCheck()
{
	if(m_Hwnd)
	{
		return Button_GetCheck(m_Hwnd) != FALSE;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
int CWinButton::GetState()
{
	if(m_Hwnd)
	{
		return Button_GetState(m_Hwnd);
	}
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CWinButton::SetState(int State)
{
	if(m_Hwnd)
	{
		Button_SetState(m_Hwnd, State);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinButton::SetStyle(UINT Style, bool Redraw)
{
	if(m_Hwnd)
	{
		Button_SetStyle(m_Hwnd, Style, Redraw);
		return true;
	}
	else return false;
}
