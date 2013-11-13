// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "DlgPage.h"

class CDlgPageAbout : public CDlgPage
{
public:
	CDlgPageAbout(CBGame* inGame);
	virtual ~CDlgPageAbout(void);

	// overrides
	virtual char* GetTabName();
	virtual HRESULT RegisterControls();


protected:
	virtual INT_PTR CALLBACK DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HFONT m_LinkFont;
	bool CursorAtLink();
};
