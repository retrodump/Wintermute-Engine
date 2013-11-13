// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "DlgPage.h"

class CDlgPageAccess : public CDlgPage
{
public:
	CDlgPageAccess(CBGame* inGame);
	virtual ~CDlgPageAccess(void);

	// overrides
	virtual char* GetTabName();
	virtual HRESULT RegisterControls();
	virtual HRESULT OnConfirm();

private:
	CWinButton* m_BtnTTSEnabled;
	CWinButton* m_BtnTTSTalk;
	CWinButton* m_BtnTTSCaptions;
	CWinButton* m_BtnTTSKeypress;
	CWinButton* m_BtnKeyboardEnabled;
	CWinButton* m_BtnKeyboardCursorSkip;
	CWinButton* m_BtnKeyboardPause;
	
	void SetState();
};
