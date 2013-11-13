// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include <windowsx.h>
#include "ThemeLibEx.h"

class CWinWindow
{
public:
	typedef void (CWinWindow::*EVENT_HANDLER_PROC)();

	CWinWindow();
	virtual ~CWinWindow(void);

	HRESULT BindWindow(HWND Win);
	inline HWND GetHwnd(){ return m_Hwnd; }

	// methods
	bool SetWindowText(char* Text, TTextEncoding TextEncoding);
	int GetWindowText(char* StrBuf, int MaxCount);
	bool ShowWindow(int CmdShow=SW_SHOW);
	bool EnableWindow(bool Enable);
	bool SetForegroundWindow();
	bool IsWindowEnabled();
	HWND SetFocus();
	int MessageBox(char* Text, char* Caption=NULL, int Type=MB_OK);	
	bool DestroyWindow();
	bool SetWindowPos(HWND InsertAfter, int X, int Y, int cx, int cy, UINT Flags);
	bool GetWindowRect(RECT* Rect);

protected:
	HWND m_Hwnd;

	void RemoveBinding();

	const CThemeLibEx& GetThemeLib() const;

	// returns a pointer the window (stored as the WindowLong)
	inline static CWinWindow* GetObjectFromWindow(HWND hWnd)
	{
		return (CWinWindow*)(INT_PTR)::GetWindowLong(hWnd, GWL_USERDATA);
	}
};
