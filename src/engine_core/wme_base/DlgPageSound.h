// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "DlgPage.h"

class CDlgPageSound : public CDlgPage
{
public:
	CDlgPageSound(CBGame* inGame, CHWManager* HwMgr);
	virtual ~CDlgPageSound(void);

	virtual char* GetTabName();

	virtual HRESULT RegisterControls();
	virtual bool IsValid();

	// controls
	CWinStatic* m_StWarnText;
	CWinStatic* m_StWarnIcon;

	// event handlers
	void OnDeviceSelected();

private:
	CHWManager* m_HwMgr;
	void UpdateDevices();
	CWinCombo* m_ComboDevice;
};
