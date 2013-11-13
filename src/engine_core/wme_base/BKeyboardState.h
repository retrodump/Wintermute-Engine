// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BKEYBOARDSTATE_H__23D60008_F10D_437A_88E2_1089086B79BC__INCLUDED_)
#define AFX_BKEYBOARDSTATE_H__23D60008_F10D_437A_88E2_1089086B79BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBKeyboardState : public CBScriptable
{
public:
	DWORD m_CurrentKeyData;
	DWORD m_CurrentCharCode;
	bool m_CurrentPrintable;

	bool m_CurrentShift;
	bool m_CurrentAlt;
	bool m_CurrentControl;

	DECLARE_PERSISTENT(CBKeyboardState, CBScriptable);
	CBKeyboardState(CBGame* inGame);
	virtual ~CBKeyboardState();
	HRESULT ReadKey(bool Printable, DWORD CharCode, DWORD KeyData);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_BKEYBOARDSTATE_H__23D60008_F10D_437A_88E2_1089086B79BC__INCLUDED_)
