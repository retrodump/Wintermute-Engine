#pragma once

#include <wme_plugin.h>


class CSamplePixel
{
public:
	IWmeGame* Game;
	IWmeObject* m_Owner;

	IWmeSubFrame* m_Subframe;
	int m_Sample;

	CSamplePixel(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
	CSamplePixel(IWmeObject* Owner, IWmeGame* Game);
	virtual ~CSamplePixel(void);

	bool Update();
	bool CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
	bool GetProperty(const char* Name, IWmeValue* Value);
	bool SetProperty(const char* Name, IWmeValue* Value);
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2);


	//////////////////////////////////////////////////////////////////////////
	// these static methods work as an interface between the plugin
	// and the internal CSamplePixel class
	static DWORD Construct(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
	{
		return (DWORD)new CSamplePixel(Owner, Game, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static DWORD ConstructEmpty(IWmeObject* Owner, IWmeGame* Game)
	{
		return (DWORD)new CSamplePixel(Owner, Game);
	}

	//////////////////////////////////////////////////////////////////////////
	static void Destruct(DWORD ID)
	{
		delete (CSamplePixel*)ID;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CallMethod(DWORD ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
	{
		return ((CSamplePixel*)ID)->CallMethod(MethodName, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSamplePixel*)ID)->GetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool SetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSamplePixel*)ID)->SetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool Serialize(DWORD ID, IWmeSerialMgr* SerialMgr)
	{
		return ((CSamplePixel*)ID)->Serialize(SerialMgr);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool ReceiveEvent(DWORD ID, EWmeEvent Event, void* EventData1, void* EventData2)
	{
		return ((CSamplePixel*)ID)->ReceiveEvent(Event, EventData1, EventData2);
	}

};
