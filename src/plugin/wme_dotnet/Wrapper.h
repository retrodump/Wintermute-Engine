#pragma once

#include <vcclr.h>
#include <wme_plugin.h>

using namespace TestComponent;

class CWrapper
{
public:
	IWmeGame* Game;
	IWmeObject* m_Owner;

	int m_Sample;

	gcroot<TestClass^> m_TestClass;

	CWrapper(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
	CWrapper(IWmeObject* Owner, IWmeGame* Game);
	virtual ~CWrapper(void);

	bool Update();
	bool CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
	bool GetProperty(const char* Name, IWmeValue* Value);
	bool SetProperty(const char* Name, IWmeValue* Value);
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2);


	//////////////////////////////////////////////////////////////////////////
	// these static methods work as an interface between the plugin
	// and the internal CWrapper class
	static DWORD Construct(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
	{
		return (DWORD)new CWrapper(Owner, Game, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static DWORD ConstructEmpty(IWmeObject* Owner, IWmeGame* Game)
	{
		return (DWORD)new CWrapper(Owner, Game);
	}

	//////////////////////////////////////////////////////////////////////////
	static void Destruct(DWORD ID)
	{
		delete (CWrapper*)ID;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CallMethod(DWORD ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
	{
		return ((CWrapper*)ID)->CallMethod(MethodName, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CWrapper*)ID)->GetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool SetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CWrapper*)ID)->SetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool Serialize(DWORD ID, IWmeSerialMgr* SerialMgr)
	{
		return ((CWrapper*)ID)->Serialize(SerialMgr);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool ReceiveEvent(DWORD ID, EWmeEvent Event, void* EventData1, void* EventData2)
	{
		return ((CWrapper*)ID)->ReceiveEvent(Event, EventData1, EventData2);
	}

};
