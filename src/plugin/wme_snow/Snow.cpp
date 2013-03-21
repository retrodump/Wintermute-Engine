//////////////////////////////////////////////////////////////////////////
// This class is based on the "WeatherGod" script
// created by Jerrot
//
// http://forum.dead-code.org/index.php?topic=368.0
//////////////////////////////////////////////////////////////////////////


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WmeUtils.h>
#include ".\snow.h"


//////////////////////////////////////////////////////////////////////////
// this constructor is called when the WME script creates a new object
CSnow::CSnow(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
{
	this->Game = Game;
	m_Owner = Owner;

	// default values
	m_NumFlakes = 0;
	m_FlakeImage = NULL;
	m_Flakes = NULL;
	m_LastTime = 0;

	m_LayerEdge = -200;
	m_LayerWidth = Game->GetProperty("ScreenWidth")->GetValInt();
	m_LayerHeight = Game->GetProperty("ScreenHeight")->GetValInt();
	m_FloorY = m_LayerHeight - 180;
	m_Depth = 0;
	m_DirectionWait = 50;

	m_FlakesVelocityMin = 2;
	m_FlakesVelocityMax = 5;
	m_FlakesVelocityNatural = true;
	m_FlakesAlphaDefault = 255;
	m_FlakesAlphaNatural = true;

	m_MovementNatural = true;
	m_MovementStatic = -1;
	m_MovementToLeft = 0;
	m_MovementToRight = 3;

	m_MeltAlpha = 20;
	m_MeltVelocity = 4;


	// read constructor parameters from stack

	// we are expecting 2 parameters
	Stack->ExpectParams(2);
	
	// the first parameter on stack is number of flakes
	m_NumFlakes = Stack->PopValue()->GetValInt();

	// check if the second parameter is a string
	IWmeValue* Val = Stack->PopValue();
	if(Val->IsValString())
	{
		// it is a string, use it
		m_FlakeImage = new char[strlen(Val->GetValString())+1];
		strcpy(m_FlakeImage, Val->GetValString());
	}
	else
	{
		// otherwise use a default flake image
		// which is stored in resources of this DLL
		char DefaultFlakeImage[] = "snowflake.sprite";

		m_FlakeImage = new char[strlen(DefaultFlakeImage)+1];
		strcpy(m_FlakeImage, DefaultFlakeImage);
	}


	// we want to receive an event each time game objects are being updated
	Game->SubscribeEvent(Owner, WME_EVENT_UPDATE);
}


//////////////////////////////////////////////////////////////////////////
// this constructor is called when WME restores objects from a saved game
CSnow::CSnow(IWmeObject* Owner, IWmeGame* Game)
{
	this->Game = Game;
}

//////////////////////////////////////////////////////////////////////////
// destructor
CSnow::~CSnow(void)
{
	if(m_Flakes) delete [] m_Flakes;
	if(m_FlakeImage) delete [] m_FlakeImage;
}

//////////////////////////////////////////////////////////////////////////
// we call this when we receive an update event from WME
bool CSnow::Update()
{
	// only update every 30 milliseconds (this really should be a parameter :)
	
	// ask game about current time
	DWORD Time = Game->GetProperty("CurrentTime")->GetValInt();
	
	// have 30 milliseconds passed since the last update?
	if(Time - m_LastTime > 30)
	{
		m_LastTime = Time;

		// update all flakes
		if(m_Flakes)
		{
			for(int i=0; i<m_NumFlakes; i++) m_Flakes[i].Update(this);
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the script invokes a method of our object
bool CSnow::CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
{
	//////////////////////////////////////////////////////////////////////////
	// Run
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(MethodName, "Run")==0)
	{
		// Run doesn't have parameters so don't expect any
		Stack->ExpectParams(0);

		// some sanity check
		if(m_Flakes) delete [] m_Flakes;
		if(m_NumFlakes==0 || !m_FlakeImage)
		{
			Script->ReportRuntimeError("Snow.Run: Either NumFlakes or FlakeImage is not set.");
			Stack->PushValue(false);
			return true;
		}

		// create the flakes and initialize them
		m_Flakes = new CFlake[m_NumFlakes];
		for(int i=0; i<m_NumFlakes; i++) m_Flakes[i].Initialize(this);

		// set the .Ready property of the encapsulating script object to false
		// this is necessary for the WaitFor() command to work with our object
		m_Owner->SetProperty("Ready", false);

		// never forget to push a return value to the stack
		Stack->PushValue(true);

		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// Stop
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(MethodName, "Stop")==0)
	{
		Stack->ExpectParams(0);

		if(!m_Flakes)
		{
			Stack->PushValue(false);
			return true;
		}

		// delete all the flake entities by calling the Scene.DeleteEntity
		// script method on them
		IWmeObject* Scene = Game->GetProperty("Scene")->GetValNative();
		if(Scene)
		{
			IWmeParamSet* ParamSet = Game->CreateParamSet();
			for(int i=0; i<m_NumFlakes; i++)
			{
				ParamSet->Clean();
				ParamSet->AddParam(m_Flakes[i].m_Entity);
				Scene->CallMethod("DeleteEntity", ParamSet);
			}
			Game->DeleteParamSet(ParamSet);
		}

		// now delete our flake objects as well
		delete [] m_Flakes;
		m_Flakes = NULL;

		// our object is not busy anymore
		m_Owner->SetProperty("Ready", true);

		Stack->PushValue(true);
		return true;

	}

	//////////////////////////////////////////////////////////////////////////
	// StickToRegion
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(MethodName, "StickToRegion")==0)
	{
		Stack->ExpectParams(1);
		IWmeValue* Region = Stack->PopValue();

		if(!m_Flakes)
		{
			Stack->PushValue(false);
			return true;
		}

		// run through all the entities owned by our flakes and call StickToRegion on them
		IWmeParamSet* Params = Game->CreateParamSet();
		Params->AddParam(Region);

		for(int i=0; i<m_NumFlakes; i++)
		{
			if(m_Flakes[i].m_Entity)
			{
				m_Flakes[i].m_Entity->CallMethod("StickToRegion", Params);
			}
		}
		Game->DeleteParamSet(Params);
		
		Stack->PushValue(true);
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script queries a property of our object
bool CSnow::GetProperty(const char* Name, IWmeValue* Value)
{
	//////////////////////////////////////////////////////////////////////////
	// NumFlakes (read only)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "NumFlakes")==0)
	{
		Value->SetVal(m_NumFlakes);
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	// FlakeImage
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FlakeImage")==0)
	{
		if(m_FlakeImage) Value->SetVal(m_FlakeImage);
		else Value->SetVal();
		return true;
	}

	// ...all the member variables of CSnow should be gettable here, but I'm too lazy ;)

	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script sets a property of our object
bool CSnow::SetProperty(const char* Name, IWmeValue* Value)
{
	//////////////////////////////////////////////////////////////////////////
	// FlakeImage
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "FlakeImage")==0)
	{
		const char* Filename = Value->GetValString();

		if(m_FlakeImage) delete [] m_FlakeImage;
		m_FlakeImage = new char[strlen(Filename) + 1];
		strcpy(m_FlakeImage, Filename);

		if(!m_Flakes) return true;

		IWmeParamSet* Params = Game->CreateParamSet();
		Params->AddParam(Filename);

		for(int i=0; i<m_NumFlakes; i++)
		{
			if(m_Flakes[i].m_Entity)
			{
				m_Flakes[i].m_Entity->CallMethod("SetSprite", Params);
			}
		}
		return true;
	}

	// ...all the member variables of CSnow should be settable here, but I'm still too lazy ;)

	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when WME is saving or restoring game
bool CSnow::Serialize(IWmeSerialMgr* SerialMgr)
{
	SerialMgr->TransferValue(&Game);
	SerialMgr->TransferValue(&m_Owner);

	// serialize all the attributes
	CWmeUtils::TransferString(SerialMgr, &m_FlakeImage);
	SerialMgr->TransferValue(&m_NumFlakes);
	SerialMgr->TransferValue(&m_LastTime);

	SerialMgr->TransferValue(&m_LayerEdge);
	SerialMgr->TransferValue(&m_LayerWidth);
	SerialMgr->TransferValue(&m_LayerHeight);
	SerialMgr->TransferValue(&m_FloorY);
	SerialMgr->TransferValue(&m_Depth);
	SerialMgr->TransferValue(&m_DirectionWait);

	SerialMgr->TransferValue(&m_FlakesVelocityMin);
	SerialMgr->TransferValue(&m_FlakesVelocityMax);
	SerialMgr->TransferValue(&m_FlakesVelocityNatural);
	SerialMgr->TransferValue(&m_FlakesAlphaDefault);
	SerialMgr->TransferValue(&m_FlakesAlphaNatural);

	SerialMgr->TransferValue(&m_MovementNatural);
	SerialMgr->TransferValue(&m_MovementStatic);
	SerialMgr->TransferValue(&m_MovementToLeft);
	SerialMgr->TransferValue(&m_MovementToRight);

	SerialMgr->TransferValue(&m_MeltAlpha);
	SerialMgr->TransferValue(&m_MeltVelocity);

	// serialize all the flakes
	if(!SerialMgr->IsSaving())
	{
		// IsSaving() is false, which means we're restoring
		// create the flakes
		m_Flakes = new CFlake[m_NumFlakes];
	}
	for(int i=0; i<m_NumFlakes; i++) m_Flakes[i].Serialize(SerialMgr);


	return true;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when any of the subscribed events occured
// this class is only handling the update event
bool CSnow::ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	switch(Event)
	{
	case WME_EVENT_UPDATE:
		Update();
		return true;
	}	
	return false;
}



//////////////////////////////////////////////////////////////////////////
// CFlake class implementation
//////////////////////////////////////////////////////////////////////////
CFlake::CFlake()
{
	Game = NULL;
	m_Entity = NULL;
	m_X = m_Y = m_Z = m_TempX = 0;
	m_FlightLength = 0;
	m_LastMove = 0;
	m_Velocity = 0;
	m_Wait = 0;
	m_WaitCounter = 0;
}

//////////////////////////////////////////////////////////////////////////
CFlake::~CFlake()
{
}

//////////////////////////////////////////////////////////////////////////
bool CFlake::Serialize(IWmeSerialMgr* SerialMgr)
{
	SerialMgr->TransferValue(&Game);
	SerialMgr->TransferValue(&m_Entity);
	SerialMgr->TransferValue(&m_X);
	SerialMgr->TransferValue(&m_Y);
	SerialMgr->TransferValue(&m_Z);
	SerialMgr->TransferValue(&m_TempX);
	SerialMgr->TransferValue(&m_FlightLength);
	SerialMgr->TransferValue(&m_LastMove);
	SerialMgr->TransferValue(&m_Velocity);
	SerialMgr->TransferValue(&m_Wait);
	SerialMgr->TransferValue(&m_WaitCounter);

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CFlake::Initialize(CSnow* Snow)
{
	Game = Snow->Game;

	// get Scene object
	IWmeObject* Scene = Game->GetProperty("Scene")->GetValNative();
	if(!Scene) return false;

	// create an entity which will represent the flake
	m_Entity = Scene->CallMethod("CreateEntity")->GetValNative();
	if(!m_Entity) return false;

	// we don't want the flakes to be interactive
	m_Entity->SetProperty("Interactive", false);

	// set the sprite
	if(Snow->m_FlakeImage)
	{
		IWmeParamSet* ParamSet = Game->CreateParamSet();
		ParamSet->AddParam(Snow->m_FlakeImage);
		m_Entity->CallMethod("SetSprite", ParamSet);
		Game->DeleteParamSet(ParamSet);
	}

	// set initial position and alpha
	// this code is taked almost verbatim from Jerrot's WeatherGod script
	Reset(Snow);

	m_Y = CWmeUtils::Random(0, Snow->m_LayerHeight);
	m_Z = CWmeUtils::Random(10, 100) + Snow->m_Depth;
	m_FlightLength = CWmeUtils::Random(Snow->m_FloorY, Snow->m_LayerHeight);
	m_Wait = (int)(Snow->m_DirectionWait * ((float)m_Z / 50.0f));
	m_WaitCounter = 0;

	if(Snow->m_FlakesVelocityNatural)
		m_Velocity = CWmeUtils::Random(Snow->m_FlakesVelocityMin, 1+(int)((float)m_Z*Snow->m_FlakesVelocityMax/100.0f)); 
	else
		m_Velocity = Snow->m_FlakesVelocityMax;

	m_Entity->SetProperty("X", m_X);
	m_Entity->SetProperty("Y", m_Y);
	m_Entity->SetProperty("Scale", m_Z);

	m_LastMove = 0;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// this code is taked almost verbatim from Jerrot's WeatherGod script
bool CFlake::Reset(CSnow* Snow)
{
	m_X = CWmeUtils::Random(Snow->m_LayerEdge, Snow->m_LayerWidth);
	m_TempX = m_X;
	m_Y = 0;

	if(Snow->m_FlakesAlphaNatural)
		m_Entity->SetProperty("AlphaColor", (int)CWmeUtils::MakeRGBA(255, 255, 255, Snow->m_FlakesAlphaDefault));
	else
		m_Entity->SetProperty("AlphaColor", (int)CWmeUtils::MakeRGBA(255, 255, 255, (BYTE)(Snow->m_FlakesAlphaDefault - m_Z / 50.0f * 100.0f)));

	return true;
}


//////////////////////////////////////////////////////////////////////////
// this code is taked almost verbatim from Jerrot's WeatherGod script
bool CFlake::Update(CSnow* Snow)
{
	if(!m_Entity) return false;

	// the flake's lifetime is over
	if(m_Y >= m_FlightLength)
	{
		// do the meltdown
		int TempAlpha = CWmeUtils::GetA(m_Entity->GetProperty("AlphaColor")->GetValInt());
		TempAlpha = max(0, TempAlpha-Snow->m_MeltVelocity);
		m_Entity->SetProperty("AlphaColor", (int)CWmeUtils::MakeRGBA(255, 255, 255, (BYTE)TempAlpha));

		// it melted completely, reset it
		if(TempAlpha < Snow->m_MeltAlpha) Reset(Snow);		
	}
	else
	{
		m_Y += m_Velocity;
		if(m_WaitCounter<0)
		{
			if(Snow->m_MovementNatural)
			{
				m_LastMove = (int)(Snow->m_MovementStatic * (m_Velocity/5.0f));
			}
			else
			{
				m_LastMove = (int)((m_LastMove + (CWmeUtils::Random(Snow->m_MovementToLeft, Snow->m_MovementToRight)*(m_Z/100.0f)))/2);
			}
			m_TempX += m_LastMove;
			m_X = m_TempX;
			m_WaitCounter = m_Wait;
		}
		else
		{
			m_TempX += m_LastMove;
			m_X = m_TempX;
			m_WaitCounter -= m_Velocity;
		}
	}
	
	// update the entity position accordingly
	m_Entity->SetProperty("X", m_X);
	m_Entity->SetProperty("Y", m_Y);

	return true;
}
