// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIMARKUP_H__FAE8DFBB_1FBE_49D2_B600_378D8DC59EB2__INCLUDED_)
#define AFX_UIMARKUP_H__FAE8DFBB_1FBE_49D2_B600_378D8DC59EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"

class CUIMarkup : public CUIObject  
{
public:
	DECLARE_PERSISTENT(CUIMarkup, CUIObject);
	CUIMarkup(CBGame* InGame);
	virtual ~CUIMarkup();
	virtual HRESULT Display(int OffsetX=0, int OffsetY=0);

	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

};

#endif // !defined(AFX_UIMARKUP_H__FAE8DFBB_1FBE_49D2_B600_378D8DC59EB2__INCLUDED_)
