// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinDialog.h"

class CDlgPage : public CWinDialog
{
public:
	CDlgPage(CBGame* inGame);
	virtual ~CDlgPage(void);

	virtual char* GetTabName() = 0;
	virtual bool IsValid();
	virtual HRESULT OnConfirm();	
	virtual HRESULT OnInitDialog();

	CBGame* Game;

protected:
	virtual INT_PTR CALLBACK DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
