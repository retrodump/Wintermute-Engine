// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADENTITY_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_)
#define AFX_ADENTITY_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdEntity : public CAdTalkHolder
{
public:
	CVidTheoraPlayer* m_Theora;
	HRESULT SetSprite(char* Filename);
	int m_WalkToX;
	int m_WalkToY;
	TDirection m_WalkToDir;
	void SetItem(char* ItemName);
	char* m_Item;
	DECLARE_PERSISTENT(CAdEntity, CAdTalkHolder);
	void UpdatePosition();
	virtual int GetHeight();
	CBRegion* m_Region;
	virtual HRESULT SaveAsText(CBDynBuffer *Buffer, int Indent);
	virtual HRESULT Update();
	virtual HRESULT Display();
	CAdEntity(CBGame* inGame);
	virtual ~CAdEntity();
	HRESULT LoadFile(char * Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	TEntityType m_Subtype;

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

};

#endif // !defined(AFX_ADENTITY_H__14E2C0E8_C417_4761_8884_91F5AD05B8CB__INCLUDED_)
