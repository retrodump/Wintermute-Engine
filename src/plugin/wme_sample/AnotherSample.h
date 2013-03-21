#pragma once

#include <wme_plugin.h>


class CAnotherSample
{
public:
	IWmeGame* Game;
	IWmeObject* m_Owner;

	int m_LastTime;

	CAnotherSample(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
	CAnotherSample(IWmeObject* Owner, IWmeGame* Game);
	virtual ~CAnotherSample(void);

	bool Update();
	bool SceneInit(IWmeObject* Scene);

	bool CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
	bool GetProperty(const char* Name, IWmeValue* Value);
	bool SetProperty(const char* Name, IWmeValue* Value);
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2);


	//////////////////////////////////////////////////////////////////////////
	// these static methods work as an interface between the plugin
	// and the internal CAnotherSample class
	static DWORD Construct(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
	{
		return (DWORD)new CAnotherSample(Owner, Game, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static DWORD ConstructEmpty(IWmeObject* Owner, IWmeGame* Game)
	{
		return (DWORD)new CAnotherSample(Owner, Game);
	}

	//////////////////////////////////////////////////////////////////////////
	static void Destruct(DWORD ID)
	{
		delete (CAnotherSample*)ID;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CallMethod(DWORD ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
	{
		return ((CAnotherSample*)ID)->CallMethod(MethodName, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CAnotherSample*)ID)->GetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool SetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CAnotherSample*)ID)->SetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool Serialize(DWORD ID, IWmeSerialMgr* SerialMgr)
	{
		return ((CAnotherSample*)ID)->Serialize(SerialMgr);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool ReceiveEvent(DWORD ID, EWmeEvent Event, void* EventData1, void* EventData2)
	{
		return ((CAnotherSample*)ID)->ReceiveEvent(Event, EventData1, EventData2);
	}

};
