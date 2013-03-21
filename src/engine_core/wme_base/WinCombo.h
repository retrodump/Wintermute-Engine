// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "WinWindow.h"

class CWinCombo : public CWinWindow
{
public:
	CWinCombo(void);
	virtual ~CWinCombo(void);

	// methods
	int GetCount();
	bool ResetContent();
	int AddString(char* Str, TTextEncoding TextEncoding);
	int InsertString(int Index, char* Str, TTextEncoding TextEncoding);
	int DeleteString(int Index);
	int SetItemData(int Index, void* Data);
	void* GetItemData(int Index);
	int FindString(char* Str, TTextEncoding TextEncoding, int StartIndex=0);
	int FindItemData(void* Data, int StartIndex=0);
	int GetCurSel();
	int SetCurSel(int Index);

private:
	int ComboSendMessage(int Msg, char* Str, TTextEncoding TextEncoding, WPARAM wParam = 0L);
};
