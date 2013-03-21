// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SXDATE_H__E82B68EC_8907_4447_BDD4_7D6BDA63DFB5__INCLUDED_)
#define AFX_SXDATE_H__E82B68EC_8907_4447_BDD4_7D6BDA63DFB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BScriptable.h"

class CSXDate : public CBScriptable  
{
public:
	int ScCompare(CBScriptable* Value);
	DECLARE_PERSISTENT(CSXDate, CBScriptable);
	CSXDate(CBGame* inGame, CScStack* Stack);
	virtual ~CSXDate();
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	char* ScToString();
	char* m_String;
	struct tm m_tm;
};

#endif // !defined(AFX_SXDATE_H__E82B68EC_8907_4447_BDD4_7D6BDA63DFB5__INCLUDED_)
