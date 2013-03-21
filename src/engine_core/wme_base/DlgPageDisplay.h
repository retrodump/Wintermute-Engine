// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "DlgPage.h"

class CDlgPageDisplay :	public CDlgPage
{
public:
	CDlgPageDisplay(CBGame* inGame, CHWManager* HwMgr);
	virtual ~CDlgPageDisplay(void);
	
	// controls
	CWinButton* m_BtnAccelerated;
	CWinButton* m_BtnWindowed;
	CWinButton* m_BtnTL;
	CWinButton* m_BtnAspect;
	CWinButton* m_BtnDesktop;
	CWinButton* m_BtnMore;
	CWinCombo* m_ComboDevice;
	CWinCombo* m_ComboColors;
	CWinCombo* m_ComboFSAA;
	CWinStatic* m_StFSAALabel;

	CWinStatic* m_StWarnText;
	CWinStatic* m_StWarnIcon;

	// overrides
	virtual char* GetTabName();
	virtual bool IsValid();
	virtual HRESULT RegisterControls();

	// event handlers
	void OnDeviceSelected();
	void OnColorsSelected();
	void OnFSAASelected();
	void OnAccelerated();
	void OnWindowed();
	void OnTL();
	void OnAspect();
	void OnDesktop();
	void OnMore();

private:
	void UpdateDevices();
	void UpdateSettings();
	void UpdateState();
	void UpdateMultisample();
	CHWManager* m_HwMgr;
};
