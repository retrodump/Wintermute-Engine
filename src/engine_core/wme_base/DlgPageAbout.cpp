// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgPageAbout.h"
#include "resource.h"
#include <windowsx.h>


//////////////////////////////////////////////////////////////////////////
CDlgPageAbout::CDlgPageAbout(CBGame* inGame) : CDlgPage(inGame)
{
	m_LinkFont = NULL;
}

//////////////////////////////////////////////////////////////////////////
CDlgPageAbout::~CDlgPageAbout(void)
{
	if(m_LinkFont) ::DeleteObject(m_LinkFont);
}


//////////////////////////////////////////////////////////////////////////
char* CDlgPageAbout::GetTabName()
{
	return "/SYSENG0050/About";
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPageAbout::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	// create link font
	HFONT OrigFont = (HFONT)::SendMessage(GetDlgItem(m_Hwnd, IDC_WME_LINK), WM_GETFONT, 0, 0);
	LOGFONT lf;
	::GetObject(OrigFont, sizeof(LOGFONT), &lf);
	lf.lfUnderline = TRUE;
	m_LinkFont = ::CreateFontIndirect(&lf);


	// events
	//RegisterEvent(IDC_WME_LOGO, STN_CLICKED, (EVENT_HANDLER_PROC)OnWmeLink);

	// controls
	//m_BtnAccelerated = RegisterButton(IDC_ACCELERATED);

	// initialize

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CDlgPageAbout::CursorAtLink()
{
	HWND Logo = GetDlgItem(m_Hwnd, IDC_WME_LOGO);
	HWND Link = GetDlgItem(m_Hwnd, IDC_WME_LINK);

	POINT CursorPos;
	GetCursorPos(&CursorPos);

	RECT rc;
	::GetWindowRect(Logo, &rc);
	if(PtInRect(&rc, CursorPos)) return true;

	::GetWindowRect(Link, &rc);
	if(PtInRect(&rc, CursorPos)) return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK CDlgPageAbout::DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg==WM_SETCURSOR)
	{
		if(CursorAtLink())
		{
			SetCursor(LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_LINK)));
			return TRUE;
		}

	}

	else if(uMsg==WM_LBUTTONDOWN)
	{
		if(CursorAtLink())
		{
			CBPlatform::ShellExecute(m_Hwnd, "open", "http://www.wintermute-engine.org", NULL, NULL, SW_SHOW);
			return TRUE;
		}

	}
	else if(uMsg==WM_CTLCOLORSTATIC)
	{
		HWND Link = GetDlgItem(m_Hwnd, IDC_WME_LINK);
		if((HWND)lParam==Link)
		{
			HDC dc = (HDC)wParam;
			::SetTextColor(dc, RGB(0, 0, 255));
			::SetBkMode(dc, TRANSPARENT);

			if(m_LinkFont) ::SelectObject(dc, m_LinkFont);

			return (INT_PTR)::GetStockObject(HOLLOW_BRUSH);
		}
	}

	return CDlgPage::DlgMsgHandler(hwnd, uMsg, wParam, lParam);
}
