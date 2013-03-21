// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinWindow.h"
#include <CommCtrl.h>

class CWinTabCtrl :	public CWinWindow
{
public:
	CWinTabCtrl(void);
	virtual ~CWinTabCtrl(void);

	// methods
	int GetItemCount();
	bool GetItem(int Index, TCITEM* Item);

	bool SetItemA(int Index, TCITEMA* Item);
	bool SetItemW(int Index, TCITEMW* Item);
	
	int InsertItemA(int Index, TCITEMA* Item);
	int InsertItemW(int Index, TCITEMW* Item);
	
	int InsertItem(UINT Mask, int Index, char* Str, int Image, LPARAM lParam, TTextEncoding TextEncoding);
	int InsertItem(int Index, char* Str, TTextEncoding TextEncoding);
	int InsertItem(int Index, char* Str, int Image, TTextEncoding TextEncoding);
	
	bool DeleteItem(int Index);
	bool DeleteAllItems();
	int GetCurSel();
	int SetCurSel(int Index);
	bool AdjustRect(bool Larger, LPRECT Rect);
};
