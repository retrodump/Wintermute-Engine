// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SCSTACK_H__49142383_4BFC_4FDA_A66F_7529238CD837__INCLUDED_)
#define AFX_SCSTACK_H__49142383_4BFC_4FDA_A66F_7529238CD837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "wme_plugin.h"

class CScStack : public CBBase, public IWmeStack
{
public:
	CScValue* GetAt(int Index);
	CScValue* GetPushValue();
	DECLARE_PERSISTENT(CScStack, CBBase);
	void PushNative(CBScriptable* Val, bool Persistent);
	void PushString(char* Val);
	void PushBool(bool Val);
	void PushInt(int Val);
	void PushFloat(double Val);
	void PushNULL();
	void CorrectParams(DWORD expected_params);
	CScValue* GetTop();
	void Push(CScValue* Val);
	CScValue* Pop();
	CScStack(CBGame* inGame);
	virtual ~CScStack();
	CBArray<CScValue*, CScValue*> m_Values;
	int m_SP;

// IWmeStack
public:
	IWmeValue* PopValue();
	bool PushValue(int Value);
	bool PushValue(const char* Value);
	bool PushValue(double Value);
	bool PushValue(bool Value);
	bool PushValue(IWmeObject* Value);
	bool PushValue(IWmeValue* Value);
	bool PushValue();
	bool ExpectParams(int NumParams);

};

#endif // !defined(AFX_SCSTACK_H__49142383_4BFC_4FDA_A66F_7529238CD837__INCLUDED_)
