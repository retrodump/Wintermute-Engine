// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\winwindow.h"


//////////////////////////////////////////////////////////////////////////
CWinWindow::CWinWindow()
{
	m_Hwnd = NULL;
}

//////////////////////////////////////////////////////////////////////////
CWinWindow::~CWinWindow(void)
{
	RemoveBinding();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CWinWindow::BindWindow(HWND Win)
{
	RemoveBinding();

	if(Win) ::SetWindowLong(Win, GWL_USERDATA, (long)(INT_PTR)(this));
	m_Hwnd = Win;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CWinWindow::RemoveBinding()
{
	if(m_Hwnd)
	{
		::SetWindowLong(m_Hwnd, GWL_USERDATA, (long)0);
		m_Hwnd = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////////////////////////
bool CWinWindow::SetWindowText(char* Text, TTextEncoding TextEncoding)
{
	if(m_Hwnd)
	{
		CBTextUtils::SetWindowText(TextEncoding, m_Hwnd, (BYTE*)Text);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
int CWinWindow::GetWindowText(char* StrBuf, int MaxCount)
{
	// FIXME: UTF-8 support
	if(m_Hwnd)
	{
		return ::GetWindowText(m_Hwnd, StrBuf, MaxCount);
	}
	else return 0;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::ShowWindow(int CmdShow)
{
	if(m_Hwnd)
	{
		return ::ShowWindow(m_Hwnd, CmdShow) != FALSE;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::EnableWindow(bool Enable)
{
	if(m_Hwnd)
	{
		return ::EnableWindow(m_Hwnd, Enable) != FALSE;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::SetForegroundWindow()
{
	if(m_Hwnd)
	{
		return ::SetForegroundWindow(m_Hwnd) != FALSE;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::IsWindowEnabled()
{
	if(m_Hwnd)
	{
		return ::IsWindowEnabled(m_Hwnd) != FALSE;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
HWND CWinWindow::SetFocus()
{
	if(m_Hwnd)
	{
		return ::SetFocus(m_Hwnd);
	}
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
int CWinWindow::MessageBox(char* Text, char* Caption, int Type)
{
	return ::MessageBox(m_Hwnd, Text, Caption?Caption:"", Type);
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::DestroyWindow()
{
	if(m_Hwnd)
	{
		bool Ret = ::DestroyWindow(m_Hwnd) != FALSE;
		m_Hwnd = NULL;
		return Ret;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::SetWindowPos(HWND InsertAfter, int X, int Y, int cx, int cy, UINT Flags)
{
	if(m_Hwnd)
	{
		return ::SetWindowPos(m_Hwnd, InsertAfter, X, Y, cx, cy, Flags) != FALSE;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CWinWindow::GetWindowRect(RECT* Rect)
{
	if(m_Hwnd) return ::GetWindowRect(m_Hwnd, Rect) != FALSE;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
const CThemeLibEx& CWinWindow::GetThemeLib() const
{
	static CThemeLibEx themeLib;
	return themeLib;
}
