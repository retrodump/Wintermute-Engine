// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinWindow.h"

class CWinButton : public CWinWindow
{
public:
	CWinButton(void);
	virtual ~CWinButton(void);

	// methods
	bool SetCheck(bool Check=true);
	bool GetCheck();
	int GetState();
	bool SetState(int State);
	bool SetStyle(UINT Style, bool Redraw=true);
};
