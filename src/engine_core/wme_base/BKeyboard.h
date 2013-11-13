// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BKEYBOARD_H__98357BF2_2D88_41B0_B090_F62A6C43C526__INCLUDED_)
#define AFX_BKEYBOARD_H__98357BF2_2D88_41B0_B090_F62A6C43C526__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "dcgf.h"

class CBKeyboard : public CBBase  
{
public:
	bool Key(BYTE Key);
	LPDIRECTINPUT m_DirectInput;
	void Update();
	void UnAcquire(void);
	void ReAcquire(void);
	HRESULT Create(HINSTANCE hInst, HWND hWnd);
	CBKeyboard(CBGame* inGame=NULL);
	virtual ~CBKeyboard();
	LPDIRECTINPUTDEVICE m_DIKeyboard;
	BYTE Keys[256];
};

#endif // !defined(AFX_BKEYBOARD_H__98357BF2_2D88_41B0_B090_F62A6C43C526__INCLUDED_)
