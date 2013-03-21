// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIBUTTON_H__65B29CD7_489F_4BAD_BE4E_E9D5B35D36D1__INCLUDED_)
#define AFX_UIBUTTON_H__65B29CD7_489F_4BAD_BE4E_E9D5B35D36D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"
#include "dctypes.h"	// Added by ClassView

class CUIButton : public CUIObject  
{
public:
	bool m_PixelPerfect;
	bool m_StayPressed;
	bool m_CenterImage;
	bool m_OneTimePress;
	DWORD m_OneTimePressTime;
	DECLARE_PERSISTENT(CUIButton, CUIObject);
	void Press();
	virtual HRESULT Display(int OffsetX=0, int OffsetY=0);
	bool m_Press;
	bool m_Hover;
	void CorrectSize();
	TTextAlign m_Align;
	CBSprite* m_ImageHover;
	CBSprite* m_ImagePress;
	CBSprite* m_ImageDisable;
	CBSprite* m_ImageFocus;
	CBFont* m_FontDisable;
	CBFont* m_FontPress;
	CBFont* m_FontHover;
	CBFont* m_FontFocus;
	CUITiledImage* m_BackPress;
	CUITiledImage* m_BackHover;
	CUITiledImage* m_BackDisable;
	CUITiledImage* m_BackFocus;
	CUIButton(CBGame* inGame=NULL);
	virtual ~CUIButton();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UIBUTTON_H__65B29CD7_489F_4BAD_BE4E_E9D5B35D36D1__INCLUDED_)
