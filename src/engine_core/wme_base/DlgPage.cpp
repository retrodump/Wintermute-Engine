// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\dlgpage.h"

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE                0x0128
#endif

//////////////////////////////////////////////////////////////////////////
CDlgPage::CDlgPage(CBGame* inGame) : CWinDialog()
{
	Game = inGame;
}

//////////////////////////////////////////////////////////////////////////
CDlgPage::~CDlgPage(void)
{
	Game = NULL; // ref only
}

//////////////////////////////////////////////////////////////////////////
bool CDlgPage::IsValid()
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPage::OnConfirm()
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK CDlgPage::DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*
	if(uMsg==WM_ERASEBKGND)
	{
		if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive())
		{
			HDC dc = (HDC)wParam;

			HTHEME	hTheme = GetThemeLib().OpenThemeData(m_Hwnd, L"Tab");
			if (hTheme)
			{
				RECT rect;
				::GetClientRect(m_Hwnd, &rect);
				GetThemeLib().DrawThemeBackground(hTheme, dc, TABP_BODY, 0, &rect, NULL);

				GetThemeLib().CloseThemeData(hTheme);
			}
			return TRUE;
		}
	}

	else if(uMsg==WM_CTLCOLORSTATIC)
	{		
		if (GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive())
		{
			HDC dc = (HDC)wParam;
			HWND hwnd = (HWND)lParam;

			if (::SendMessage(hwnd, WM_GETDLGCODE, 0, 0L) & (DLGC_STATIC|DLGC_BUTTON))
			{
				// themed buttons need to repaint the background
				// for whatever reason
				if(::SendMessage(hwnd, WM_GETDLGCODE, 0, 0L) & (DLGC_BUTTON))
				{
					RECT rc;
					GetClientRect(hwnd, &rc);
					GetThemeLib().DrawThemeParentBackground(hwnd, dc, &rc);
				}
				::SetBkMode(dc, TRANSPARENT);
				return (INT_PTR)::GetStockObject(HOLLOW_BRUSH);
			}

		}
	}

	else if(uMsg==WM_UPDATEUISTATE)
	{
		// just repaint everything...
		InvalidateRect(m_Hwnd, NULL, TRUE);
	}
	*/
	return CWinDialog::DlgMsgHandler(hwnd, uMsg, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgPage::OnInitDialog()
{
	if(GetThemeLib().IsAvailable() && GetThemeLib().IsThemeActive())
		GetThemeLib().EnableThemeDialogTexture(m_Hwnd, ETDT_ENABLETAB);

	return S_OK;
}