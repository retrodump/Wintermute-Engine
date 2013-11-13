#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WmeUtils.h>
#include <stdio.h>
#include <time.h>
#include ".\AnotherSample.h"


//////////////////////////////////////////////////////////////////////////
// this constructor is called when the WME script creates a new object
CAnotherSample::CAnotherSample(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
{
	this->Game = Game;
	m_Owner = Owner;

	m_LastTime = 0;

	Stack->ExpectParams(0);

	Game->SubscribeEvent(m_Owner, WME_EVENT_UPDATE);
	Game->SubscribeEvent(m_Owner, WME_EVENT_SCENE_INIT);
}


//////////////////////////////////////////////////////////////////////////
// this constructor is called when WME restores objects from a saved game
CAnotherSample::CAnotherSample(IWmeObject* Owner, IWmeGame* Game)
{
	this->Game = Game;
}

//////////////////////////////////////////////////////////////////////////
// destructor
CAnotherSample::~CAnotherSample(void)
{
}

//////////////////////////////////////////////////////////////////////////
// we call this when we receive an update event from WME
bool CAnotherSample::Update()
{
	DWORD Time = Game->GetProperty("CurrentTime")->GetValInt();

	// display current time every 10 seconds
	if(Time - m_LastTime >= 10000)
	{
		m_LastTime = Time;

		// get current time
		time_t Time;
		time(&Time);
		tm* CurrTime = localtime(&Time);
		
		// format the message
		char StrTime[100];
		sprintf(StrTime, "Current time: %02d:%02d", CurrTime->tm_hour, CurrTime->tm_min);

		// and finally call the Game.Msg method and pass the message as a parameter
		IWmeParamSet* Params = Game->CreateParamSet();
		Params->AddParam(StrTime);
		Game->CallMethod("Msg", Params);
		Game->DeleteParamSet(Params);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// we call this when we receive a scene-init event from WME
bool CAnotherSample::SceneInit(IWmeObject* Scene)
{
	IWmeParamSet* Params = Game->CreateParamSet();

	// display a welcome message everytime we enter a new scene
	char Message[1000];
	sprintf(Message, "Welcome to scene %s", Scene->GetProperty("Name")->GetValString());
	
	Params->AddParam(Message);
	Game->CallMethod("Msg", Params);
	Game->DeleteParamSet(Params);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the script invokes a method of our object
bool CAnotherSample::CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
{
	return false;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script queries a property of our object
bool CAnotherSample::GetProperty(const char* Name, IWmeValue* Value)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script sets a property of our object
bool CAnotherSample::SetProperty(const char* Name, IWmeValue* Value)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when WME is saving or restoring game
bool CAnotherSample::Serialize(IWmeSerialMgr* SerialMgr)
{
	SerialMgr->TransferValue(&Game);
	SerialMgr->TransferValue(&m_Owner);

	// serialize all the attributes
	SerialMgr->TransferValue(&m_LastTime);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when any of the subscribed events occured
bool CAnotherSample::ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	switch(Event)
	{
	case WME_EVENT_UPDATE:
		Update();
		return true;

	case WME_EVENT_SCENE_INIT:
		SceneInit((IWmeObject*)EventData1);
		return true;
	}	
	return false;
}
