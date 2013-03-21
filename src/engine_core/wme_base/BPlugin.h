// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "wme_plugin.h"

class CBPlugin : public CBBase
{
public:
	CBPlugin(CBGame* inGame);
	virtual ~CBPlugin(void);

	char* m_DllPath;
	CBArray<char*, char*> m_Classes;

	HMODULE m_DllHandle;

	HRESULT Create(char* Path, HMODULE Handle);
	HRESULT HandleExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	SWmePluginInfo m_PluginInfo;
};
