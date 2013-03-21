// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADITEM_H__ED5D8300_31F9_45B5_AD24_911FA491145E__INCLUDED_)
#define AFX_ADITEM_H__ED5D8300_31F9_45B5_AD24_911FA491145E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdTalkHolder.h"

class CAdItem : public CAdTalkHolder
{
public:
	bool m_DisplayAmount;
	int m_Amount;
	int m_AmountOffsetX;
	int m_AmountOffsetY;
	TTextAlign m_AmountAlign;
	char* m_AmountString;
	

	HRESULT Update();
	DECLARE_PERSISTENT(CAdItem, CAdTalkHolder);
	HRESULT Display(int X, int Y);
	bool GetExtendedFlag(char *FlagName);
	bool m_InInventory;
	bool m_CursorCombined;
	CBSprite* m_SpriteHover;
	CBSprite* m_CursorNormal;
	CBSprite* m_CursorHover;
	CAdItem(CBGame* inGame);
	virtual ~CAdItem();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_ADITEM_H__ED5D8300_31F9_45B5_AD24_911FA491145E__INCLUDED_)
