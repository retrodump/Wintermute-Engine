#pragma once

#include <wme_plugin.h>


class CSample
{
public:
	IWmeGame* Game;
	IWmeObject* m_Owner;

	int m_Sample;

	CSample(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
	CSample(IWmeObject* Owner, IWmeGame* Game);
	virtual ~CSample(void);

	bool Update();
	bool CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
	bool GetProperty(const char* Name, IWmeValue* Value);
	bool SetProperty(const char* Name, IWmeValue* Value);
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2);


	//////////////////////////////////////////////////////////////////////////
	// these static methods work as an interface between the plugin
	// and the internal CSample class
	static DWORD Construct(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
	{
		return (DWORD)new CSample(Owner, Game, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static DWORD ConstructEmpty(IWmeObject* Owner, IWmeGame* Game)
	{
		return (DWORD)new CSample(Owner, Game);
	}

	//////////////////////////////////////////////////////////////////////////
	static void Destruct(DWORD ID)
	{
		delete (CSample*)ID;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CallMethod(DWORD ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
	{
		return ((CSample*)ID)->CallMethod(MethodName, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSample*)ID)->GetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool SetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSample*)ID)->SetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool Serialize(DWORD ID, IWmeSerialMgr* SerialMgr)
	{
		return ((CSample*)ID)->Serialize(SerialMgr);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool ReceiveEvent(DWORD ID, EWmeEvent Event, void* EventData1, void* EventData2)
	{
		return ((CSample*)ID)->ReceiveEvent(Event, EventData1, EventData2);
	}

};
