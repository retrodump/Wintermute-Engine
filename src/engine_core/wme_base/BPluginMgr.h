// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "SXPluginObject.h"

#define PLUGIN_MASK "wme_*.dll"

class CBPluginMgr :	public CBBase
{
public:
	CBPluginMgr(CBGame* inGame);
	virtual ~CBPluginMgr(void);

	CBArray<CBPlugin*, CBPlugin*> m_Plugins;

	HRESULT RegisterPlugins();
	
	HRESULT RegisterClasses();
	HRESULT HandleExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	HMODULE GetDllHandle(const char* ClassName);

	CBArray<CSXPluginObject*, CSXPluginObject*> m_Objects;
	HRESULT AddObject(CSXPluginObject* Obj);
	HRESULT RemoveObject(CSXPluginObject* Obj);

	bool SubscribeEvent(IWmeObject* Object, EWmeEvent Event);
	bool UnsubscribeEvent(IWmeObject* Object, EWmeEvent Event);
	bool ApplyEvent(EWmeEvent Event, void* EventData1, void* EventData2);

	bool ValidObject(CBObject* Object);

private:
	bool m_IsDisposing;
	HRESULT RegisterPlugins(char* Path);
	HRESULT RegisterPlugin(char* Path, char* Filename);
};
