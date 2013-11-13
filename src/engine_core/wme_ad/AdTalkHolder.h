// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADTALKHOLDER_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_)
#define AFX_ADTALKHOLDER_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdTalkHolder : public CAdObject  
{
public:
	DECLARE_PERSISTENT(CAdTalkHolder, CAdObject);
	virtual CBSprite* GetTalkStance(char *Stance);
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	CBSprite* m_Sprite;
	CBArray<CBSprite*, CBSprite*> m_TalkSprites;
	CBArray<CBSprite*, CBSprite*> m_TalkSpritesEx;
	CAdTalkHolder(CBGame* inGame);
	virtual ~CAdTalkHolder();

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

};

#endif // !defined(AFX_ADTALKHOLDER_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_)
