#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <WmeUtils.h>
#include ".\SamplePixel.h"


//////////////////////////////////////////////////////////////////////////
// this constructor is called when the WME script creates a new object
CSamplePixel::CSamplePixel(IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack)
{
	// remember the reference to Game
	// and to the WME object wrapping our CSamplePixel instance
	this->Game = Game;
	m_Owner = Owner;
	
	// we expect one parameter: the subframe object
	Stack->ExpectParams(1);
	IWmeObject* Obj = Stack->PopValue()->GetValNative();
	
	// use the GetInterface method to correctly cast
	// the pointer from IWmeObject to IWmeSubFrame
	m_Subframe = (IWmeSubFrame*)Obj->GetInterface("IWmeSubFrame");

	// and subscribe to some game events
	Game->SubscribeEvent(m_Owner, WME_EVENT_UPDATE);
	Game->SubscribeEvent(m_Owner, WME_EVENT_SCENE_SHUTDOWN);
}


//////////////////////////////////////////////////////////////////////////
// this constructor is called when WME restores objects from a saved game
CSamplePixel::CSamplePixel(IWmeObject* Owner, IWmeGame* Game)
{
	this->Game = Game;
}

//////////////////////////////////////////////////////////////////////////
// destructor
CSamplePixel::~CSamplePixel(void)
{
}

//////////////////////////////////////////////////////////////////////////
// we call this when we receive an update event from WME
bool CSamplePixel::Update()
{
	// do some painting on each update
	if(m_Subframe)
	{
		// get the dimensions of the subframe
		int Width = m_Subframe->GetWidth();
		int Height = m_Subframe->GetHeight();
		
		// prepare the subframe for painting
		m_Subframe->StartPixelOperations();
		
		// change all the pixels to some random color
		for(int Row=0; Row<Height; Row++)
		{
			for(int Col=0; Col<Width; Col++)
			{
				m_Subframe->PutPixel(Col, Row, CWmeUtils::MakeRGBA(Col, Row, 255, 128));
			}
		}

		// and commit the changes
		m_Subframe->EndPixelOperations();
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the script invokes a method of our object
bool CSamplePixel::CallMethod(const char* MethodName, IWmeScript* Script, IWmeStack* Stack)
{
	//////////////////////////////////////////////////////////////////////////
	// SampleMethod
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(MethodName, "SampleMethod")==0)
	{
		Stack->ExpectParams(1);
		m_Sample = Stack->PopValue()->GetValInt();

		Stack->PushValue(); // no return value
		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// this method is called whenever the WME script queries a property of our object
bool CSamplePixel::GetProperty(const char* Name, IWmeValue* Value)
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
bool CSamplePixel::SetProperty(const char* Name, IWmeValue* Value)
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
bool CSamplePixel::Serialize(IWmeSerialMgr* SerialMgr)
{
	SerialMgr->TransferValue(&Game);
	SerialMgr->TransferValue(&m_Owner);

	// serialize all the attributes
	SerialMgr->TransferValue(&m_Sample);
	SerialMgr->TransferValue(&m_Subframe);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// this method is called when any of the subscribed events occured
bool CSamplePixel::ReceiveEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	switch(Event)
	{
	case WME_EVENT_UPDATE:
		Update();
		return true;
	case WME_EVENT_SCENE_SHUTDOWN:
		// we invalidate the subframe on scene transitions
		// if your subrames can live across multiple scenes you may need to do some
		// more sophisticated validation, such as remembering the object which owns
		// the subframe and checking its validity using Game.ValidObject()
		m_Subframe = NULL;
		return true;
	}	
	return false;
}
