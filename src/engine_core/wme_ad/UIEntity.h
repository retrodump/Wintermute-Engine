// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UIENTITY_H__F503B0A0_75A9_4340_A740_1BE21274CDB5__INCLUDED_)
#define AFX_UIENTITY_H__F503B0A0_75A9_4340_A740_1BE21274CDB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUIEntity : public CUIObject  
{
public:
	DECLARE_PERSISTENT(CUIEntity, CUIObject);
	CUIEntity(CBGame* inGame);
	virtual ~CUIEntity();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	virtual HRESULT Display(int OffsetX=0, int OffsetY=0);
	CAdEntity* m_Entity;
	HRESULT SetEntity(char* Filename);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();
};

#endif // !defined(AFX_UIENTITY_H__F503B0A0_75A9_4340_A740_1BE21274CDB5__INCLUDED_)
