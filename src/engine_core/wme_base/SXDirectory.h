// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BScriptable.h"

class CSXDirectory : public CBScriptable  
{
public:
	DECLARE_PERSISTENT(CSXDirectory, CBScriptable);
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	CSXDirectory(CBGame* inGame);
	virtual ~CSXDirectory();
};
