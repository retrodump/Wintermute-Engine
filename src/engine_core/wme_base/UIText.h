// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UITEXT_H__96F1A70A_72F2_4B56_BF9A_B1AD2BC8F9BC__INCLUDED_)
#define AFX_UITEXT_H__96F1A70A_72F2_4B56_BF9A_B1AD2BC8F9BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"

class CUIText : public CUIObject  
{
public:
	HRESULT SizeToFit();
	virtual HRESULT Display(int OffsetX, int OffsetY);
	DECLARE_PERSISTENT(CUIText, CUIObject);
	CUIText(CBGame* inGame=NULL);
	virtual ~CUIText();
	TTextAlign m_TextAlign;
	TVerticalAlign m_VerticalAlign;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UITEXT_H__96F1A70A_72F2_4B56_BF9A_B1AD2BC8F9BC__INCLUDED_)
