// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinDialog.h"
#include "DlgPage.h"

class CDlgSettings : public CWinDialog
{
public:
	CDlgSettings(CBGame* inGame, CHWManager* HwMgr);
	virtual ~CDlgSettings(void);

	virtual HRESULT RegisterControls();
	virtual HRESULT OnKeyDown(UINT KeyCode, UINT Flags);
	virtual HRESULT OnEnterIdle();

	void CheckValidity();

private:
	CBGame* Game;
	CHWManager* m_HwMgr;

	int m_CurrPage;
	void SelNextPage();
	void SelPrevPage();

	HRESULT AddPage(CDlgPage* Page);

	// even handlers
	void OnOK();
	void OnCancel();
	void OnTabChanged();	
	void OnDontShow();

	// controls
	CWinButton* m_BtnOK;
	CWinButton* m_DontShowAgain;
	CWinTabCtrl* m_TabPages;
	CWinStatic* m_StVersion;

	CBArray<CDlgPage*, CDlgPage*> m_Pages;
};
