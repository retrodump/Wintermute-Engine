// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIEDIT_H__28A0A080_D6E4_4295_B5B5_E39DE69DB480__INCLUDED_)
#define AFX_UIEDIT_H__28A0A080_D6E4_4295_B5B5_E39DE69DB480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"

class CUIEdit : public CUIObject  
{
public:
	DECLARE_PERSISTENT(CUIEdit, CUIObject);
	int m_MaxLength;
	int InsertChars(int Pos, BYTE* Chars, int Num);
	int DeleteChars(int Start, int End);
	bool m_CursorVisible;
	DWORD m_LastBlinkTime;
	virtual HRESULT Display(int OffsetX, int OffsetY);
	virtual bool HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData);
	int m_ScrollOffset;
	int m_FrameWidth;
	DWORD m_CursorBlinkRate;
	void SetCursorChar(char* Char);
	char* m_CursorChar;
	int m_SelEnd;
	int m_SelStart;
	CBFont* m_FontSelected;
	CUIEdit(CBGame* inGame);
	virtual ~CUIEdit();

	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UIEDIT_H__28A0A080_D6E4_4295_B5B5_E39DE69DB480__INCLUDED_)
