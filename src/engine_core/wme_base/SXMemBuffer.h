// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BScriptable.h"

class CSXMemBuffer : public CBScriptable  
{
public:	
	virtual int ScCompare(CBScriptable* Val);
	DECLARE_PERSISTENT(CSXMemBuffer, CBScriptable);
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	char* ScToString();
	CSXMemBuffer(CBGame* inGame, CScStack* Stack);
	CSXMemBuffer(CBGame* inGame, void* Buffer);
	virtual ~CSXMemBuffer();
	virtual void* ScToMemBuffer();
	int m_Size;
private:
	HRESULT Resize(int NewSize);
	void* m_Buffer;
	void Cleanup();
	bool CheckBounds(CScScript* Script, int Start, int Length);
};
