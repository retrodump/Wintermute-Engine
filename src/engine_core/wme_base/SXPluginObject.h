// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BObject.h"
#include "wme_plugin.h"

class CSXPluginObject :	public CBObject
{
public:
	DECLARE_PERSISTENT(CSXPluginObject, CBObject);
	CSXPluginObject(CBGame* inGame);
	virtual ~CSXPluginObject(void);

	SWmeClassDef m_Func;
	char* m_Class;

	HRESULT Initialize(char* ClassName, HMODULE DllHandle);
	HRESULT Construct(CScScript* Script, CScStack* Stack);
	//HRESULT Tick();

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

	bool m_SubscribedEvents[WME_EVENT_MAX];
	DWORD GetObjectID(){ return m_ObjectID; };
	bool ApplyEvent(EWmeEvent Event, void* EventData1, void* EventData2);

private:
	DWORD m_ObjectID;
	bool m_Initialized;
};
