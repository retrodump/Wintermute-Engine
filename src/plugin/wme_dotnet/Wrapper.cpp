#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WmeUtils.h>
#include ".\Wrapper.h"


//////////////////////////////////////////////////////////////////////////
// this constructor is called when the WME script creates a new object
CWrapper::CWrapper(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
{
	m_TestClass = gcnew TestClass();

	this->Game = Game;
	m_Owner = Owner;
	
	Stack->ExpectParams(1);
	m_Sample = Stack->PopValue()->GetValInt();

	Game->SubscribeEvent(m_Owner, WME_EVENT_UPDATE);
}


//////////////////////////////////////////////////////////////////////////
// this constructor is called when WME restores objects from a saved game
CWrapper::CWrapper(IWmeObject* Owner, IWmeGame* Game)
{
	m_TestClass = gcnew TestClass();

	this->Game = Game;
}

//////////////////////////////////////////////////////////////////////////
// destructor
CWrapper::~CWrapper(void)
{
	m_TestClass->~TestClass();
}

//////////////////////////////////////////////////////////////////////////
// we call this when we receive an update event from WME
bool CWrapper::Update()
{
	return true;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the script invokes a method of our object
bool CWrapper::CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
{
	//////////////////////////////////////////////////////////////////////////
	// SampleMethod
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(MethodName, "SampleMethod")==0)
	{
		Stack->ExpectParams(1);
		m_Sample = Stack->PopValue()->GetValInt();

		m_TestClass->TestMethod();

		Stack->PushValue();
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script queries a property of our object
bool CWrapper::GetProperty(const char* Name, IWmeValue* Value)
{
	//////////////////////////////////////////////////////////////////////////
	// SampleProperty
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SampleProperty")==0)
	{
		Value->SetVal(m_Sample);
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script sets a property of our object
bool CWrapper::SetProperty(const char* Name, IWmeValue* Value)
{
	//////////////////////////////////////////////////////////////////////////
	// SampleProperty
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SampleProperty")==0)
	{
		m_Sample = Value->GetValInt();
		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when WME is saving or restoring game
bool CWrapper::Serialize(IWmeSerialMgr* SerialMgr)
{
	SerialMgr->TransferValue(&Game);
	SerialMgr->TransferValue(&m_Owner);

	// serialize all the attributes
	SerialMgr->TransferValue(&m_Sample);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when any of the subscribed events occured
bool CWrapper::ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	switch(Event)
	{
	case WME_EVENT_UPDATE:
		Update();
		return true;
	}	
	return false;
}
