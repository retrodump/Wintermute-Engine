// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\DlgSettings.h"
#include "DlgPageDisplay.h"
#include "DlgPageSound.h"
#include "DlgPageAccess.h"
#include "DlgPageAbout.h"
#include "resource.h"


//////////////////////////////////////////////////////////////////////////
CDlgSettings::CDlgSettings(CBGame* inGame, CHWManager* HwMgr) : CWinDialog()
{
	Game = inGame;
	m_HwMgr = HwMgr;

	m_DontShowAgain = NULL;
	m_TabPages = NULL;
	m_BtnOK = NULL;
	m_StVersion = NULL;

	m_CurrPage = -1;
}


//////////////////////////////////////////////////////////////////////////
CDlgSettings::~CDlgSettings(void)
{
	for(int i=0; i<m_Pages.GetSize(); i++)
		delete m_Pages[i];
	m_Pages.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CDlgSettings::RegisterControls()
{
	Game->m_StringTable->LocalizeWindow(m_Hwnd);

	RegisterEvent(IDOK, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgSettings::OnOK);
	RegisterEvent(IDCANCEL, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgSettings::OnCancel);
	RegisterEvent(IDC_DONT_SHOW, BN_CLICKED, (EVENT_HANDLER_PROC)&CDlgSettings::OnDontShow);

	RegisterEvent(IDC_TAB_PAGES, TCN_SELCHANGE, (EVENT_HANDLER_PROC)&CDlgSettings::OnTabChanged);

	m_BtnOK = RegisterButton(IDOK);
	m_TabPages = RegisterTabCtrl(IDC_TAB_PAGES);
	m_DontShowAgain = RegisterButton(IDC_DONT_SHOW);
	m_StVersion = RegisterStatic(IDC_WME_VER);

	// setup tab pages
	CDlgPage* Page;

	// display
	Page = new CDlgPageDisplay(Game, m_HwMgr);
	Page->CreateIndirect(IDD_PAGE_DISPLAY, GetHwnd());
	AddPage(Page);

	// sound
	Page = new CDlgPageSound(Game, m_HwMgr);
	Page->CreateIndirect(IDD_PAGE_SOUND, GetHwnd());
	AddPage(Page);

	// accessibility
	if(Game->m_SettingsAllowAccessTab)
	{
		Page = new CDlgPageAccess(Game);
		Page->CreateIndirect(IDD_PAGE_ACCESS, GetHwnd());
		AddPage(Page);
	}

	// about
	if(Game->m_SettingsAllowAboutTab)
	{
		Page = new CDlgPageAbout(Game);
		Page->CreateIndirect(IDD_PAGE_ABOUT, GetHwnd());
		AddPage(Page);
	}


	SelNextPage();

	// setup
	m_DontShowAgain->SetCheck(!m_HwMgr->m_ShowSettings);

	if(Game->m_DEBUG_DebugMode)
	{
		char ver[50];
		sprintf(ver, "ver: %d.%d.%d%s", DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD, DCGF_VER_SUFFIX);
		m_StVersion->SetWindowText(ver, TEXT_ANSI);
	}
	else m_StVersion->SetWindowText("", TEXT_ANSI);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgSettings::AddPage(CDlgPage* Page)
{
	if(!Page || !Page->GetHwnd()) return E_FAIL;

	// add tab page
	m_Pages.Add(Page);
	m_TabPages->InsertItem(	m_TabPages->GetItemCount(),
							(char*)Game->m_StringTable->ExpandStatic(Page->GetTabName()),
							Game->m_TextEncoding);

	RECT DispRect;
	m_TabPages->GetWindowRect(&DispRect);
	m_TabPages->AdjustRect(false, &DispRect);

	::MapWindowPoints(NULL, GetHwnd(), (LPPOINT)&DispRect, 2);

	/*
	POINT DispPos;
	DispPos.x = DispRect.left;
	DispPos.y = DispRect.top;
	::ScreenToClient(GetHwnd(), &DispPos);
	
	Page->SetWindowPos(
		HWND_TOP, 
		DispPos.x, DispPos.y,
		DispRect.right-DispRect.left, DispRect.bottom-DispRect.top,
		SWP_HIDEWINDOW);
	*/

	Page->SetWindowPos(
		HWND_TOP, 
		DispRect.left, DispRect.top,
		DispRect.right-DispRect.left, DispRect.bottom-DispRect.top,
		SWP_HIDEWINDOW);


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::OnOK()
{
	for(int i=0; i<m_Pages.GetSize(); i++)
	{
		m_Pages[i]->OnConfirm();
	}

	EndDialog(IDOK);
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::OnCancel()
{
	EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::OnTabChanged()
{
	m_CurrPage = m_TabPages->GetCurSel();
	for(int i=0; i<m_Pages.GetSize(); i++)
	{
		if(i==m_CurrPage) m_Pages[i]->ShowWindow(SW_SHOW);
		else m_Pages[i]->ShowWindow(SW_HIDE);
	}

	HWND NextControl = GetNextDlgTabItem(GetHwnd(), m_TabPages->GetHwnd(), FALSE);
	if(NextControl) ::SetFocus(NextControl);
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::SelNextPage()
{
	m_CurrPage++;
	if(m_CurrPage >= m_Pages.GetSize()) m_CurrPage = 0;

	m_TabPages->SetCurSel(m_CurrPage);
	OnTabChanged();
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::SelPrevPage()
{
	m_CurrPage--;
	if(m_CurrPage < 0) m_CurrPage = m_Pages.GetSize() - 1;

	m_TabPages->SetCurSel(m_CurrPage);
	OnTabChanged();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CDlgSettings::OnKeyDown(UINT KeyCode, UINT Flags)
{
	if(KeyCode==VK_TAB && (CBUtils::IsKeyDown(VK_CONTROL) || CBUtils::IsKeyDown(VK_RCONTROL)))
	{
		if(CBUtils::IsKeyDown(VK_SHIFT) || CBUtils::IsKeyDown(VK_RSHIFT))
			SelPrevPage();
		else
			SelNextPage();

		return S_OK;
	}
	else if(KeyCode==VK_PRIOR && (CBUtils::IsKeyDown(VK_CONTROL) || CBUtils::IsKeyDown(VK_RCONTROL)))
	{
		SelPrevPage();
		return S_OK;
	}
	else if(KeyCode==VK_NEXT && (CBUtils::IsKeyDown(VK_CONTROL) || CBUtils::IsKeyDown(VK_RCONTROL)))
	{
		SelNextPage();
		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CDlgSettings::OnEnterIdle()
{
	CheckValidity();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::CheckValidity()
{
	bool IsValid = true;
	for(int i=0; i<m_Pages.GetSize(); i++)
	{
		if(!m_Pages[i]->IsValid())
		{
			IsValid = false;
			break;
		}
	}
	m_BtnOK->EnableWindow(IsValid);
}

//////////////////////////////////////////////////////////////////////////
void CDlgSettings::OnDontShow()
{
	m_HwMgr->m_ShowSettings = !m_DontShowAgain->GetCheck();
}
