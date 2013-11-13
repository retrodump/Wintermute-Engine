// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SXSTRING_H__122F43C8_2CFB_4DC9_9047_A8400564D3F6__INCLUDED_)
#define AFX_SXSTRING_H__122F43C8_2CFB_4DC9_9047_A8400564D3F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BScriptable.h"

class CSXString : public CBScriptable  
{
public:
	virtual int ScCompare(CBScriptable* Val);
	DECLARE_PERSISTENT(CSXString, CBScriptable);
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	void ScSetString(char* Val);
	char* ScToString();
	void SetStringVal(char* Val);
	char* m_String;
	CSXString(CBGame* inGame, CScStack* Stack);
	virtual ~CSXString();
	int m_Capacity;
};

#endif // !defined(AFX_SXSTRING_H__122F43C8_2CFB_4DC9_9047_A8400564D3F6__INCLUDED_)
