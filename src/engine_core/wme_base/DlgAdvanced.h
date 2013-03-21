// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinDialog.h"

class CDlgAdvanced : public CWinDialog
{
public:
	CDlgAdvanced(CBGame* inGame, CHWManager* HwMgr);
	virtual ~CDlgAdvanced(void);

	virtual HRESULT RegisterControls();

private:
	CBGame* Game;
	CHWManager* m_HwMgr;

	CWinButton* m_Btn16Bit;
	CWinButton* m_BtnSubdivide;

	void OnOK();
	void OnCancel();
};
