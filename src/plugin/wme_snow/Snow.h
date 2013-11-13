#pragma once

#include <wme_plugin.h>


class CFlake;

class CSnow
{
public:
	IWmeGame* Game;
	IWmeObject* m_Owner;

	int m_NumFlakes;
	CFlake* m_Flakes;
	int m_LastTime;
	
	char* m_FlakeImage;	
	int m_LayerEdge;
	int m_LayerWidth;
	int m_LayerHeight;
	int m_FloorY;
	int m_Depth;
	int m_DirectionWait;
	
	int m_FlakesVelocityMin;
	int m_FlakesVelocityMax;
	bool m_FlakesVelocityNatural;
	int m_FlakesAlphaDefault;
	bool m_FlakesAlphaNatural;

	bool m_MovementNatural;
	int m_MovementStatic;
	int m_MovementToLeft;
	int m_MovementToRight;

	int m_MeltAlpha;
	int m_MeltVelocity;

	CSnow(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
	CSnow(IWmeObject* Owner, IWmeGame* Game);
	virtual ~CSnow(void);
	
	bool Update();
	bool CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
	bool GetProperty(const char* Name, IWmeValue* Value);
	bool SetProperty(const char* Name, IWmeValue* Value);
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2);


	//////////////////////////////////////////////////////////////////////////
	// these static methods work as an interface between the plugin
	// and the internal CSnow class
	static DWORD Construct(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
	{
		return (DWORD)new CSnow(Owner, Game, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static DWORD ConstructEmpty(IWmeObject* Owner, IWmeGame* Game)
	{
		return (DWORD)new CSnow(Owner, Game);
	}

	//////////////////////////////////////////////////////////////////////////
	static void Destruct(DWORD ID)
	{
		delete (CSnow*)ID;
	}

	//////////////////////////////////////////////////////////////////////////
	static bool CallMethod(DWORD ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
	{
		return ((CSnow*)ID)->CallMethod(MethodName, Script, Stack);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool GetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSnow*)ID)->GetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool SetProperty(DWORD ID, const char* Name, IWmeValue* Value)
	{
		return ((CSnow*)ID)->SetProperty(Name, Value);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool Serialize(DWORD ID, IWmeSerialMgr* SerialMgr)
	{
		return ((CSnow*)ID)->Serialize(SerialMgr);
	}

	//////////////////////////////////////////////////////////////////////////
	static bool ReceiveEvent(DWORD ID, EWmeEvent Event, void* EventData1, void* EventData2)
	{
		return ((CSnow*)ID)->ReceiveEvent(Event, EventData1, EventData2);
	}

};


//////////////////////////////////////////////////////////////////////////
class CFlake
{
public:
	CFlake();
	virtual ~CFlake();
	bool Serialize(IWmeSerialMgr* SerialMgr);
	bool Initialize(CSnow* Snow);
	bool Reset(CSnow* Snow);
	bool Update(CSnow* Snow);
	IWmeGame* Game;

	int m_X;
	int m_Y;
	int m_Z;
	int m_TempX;
	int m_FlightLength;
	IWmeObject* m_Entity;
	int m_Velocity;
	int m_LastMove;
	int m_Wait;
	int m_WaitCounter;
};
