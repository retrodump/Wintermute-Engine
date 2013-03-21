// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SXARRAY_H__DD2DA1E8_484D_40D4_8962_FF57D96990E9__INCLUDED_)
#define AFX_SXARRAY_H__DD2DA1E8_484D_40D4_8962_FF57D96990E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BScriptable.h"

class CSXArray : public CBScriptable  
{
public:
	HRESULT Push(CScValue* Val);
	bool ValidNumber(const char* OrigStr, char* OutStr);
	DECLARE_PERSISTENT(CSXArray, CBScriptable);
	CSXArray(CBGame* inGame, CScStack* Stack);
	CSXArray(CBGame* inGame);
	virtual ~CSXArray();
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	char* ScToString();
	int m_Length;
	CScValue* m_Values;
};

#endif // !defined(AFX_SXARRAY_H__DD2DA1E8_484D_40D4_8962_FF57D96990E9__INCLUDED_)
