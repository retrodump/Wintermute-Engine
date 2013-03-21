// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\SXPluginObject.h"

IMPLEMENT_PERSISTENT(CSXPluginObject, false);

//////////////////////////////////////////////////////////////////////////
CSXPluginObject::CSXPluginObject(CBGame* inGame):CBObject(inGame)
{
	memset(&m_Func, 0, sizeof(SWmeClassDef));
	m_Class = NULL;
	m_Initialized = false;
	m_ObjectID = 0;

	memset(m_SubscribedEvents, 0, sizeof(bool) * WME_EVENT_MAX);
}


//////////////////////////////////////////////////////////////////////////
CSXPluginObject::~CSXPluginObject(void)
{
	Game->m_PluginMgr->RemoveObject(this);

	if(m_Initialized && m_Func.Destruct) m_Func.Destruct(m_ObjectID);
	SAFE_DELETE_ARRAY(m_Class);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::Initialize(char* ClassName, HMODULE DllHandle)
{
	if(!ClassName || !DllHandle) return E_FAIL;
	
	// TODO: handle stack corruption
	PLUGIN_GETCLASSDEFINITION GetClassDefinition = (PLUGIN_GETCLASSDEFINITION)::GetProcAddress(DllHandle, "GetClassDefinition");
	if(!GetClassDefinition) return E_FAIL;

	if(m_Class!=ClassName) CBUtils::SetString(&m_Class, ClassName);
	GetClassDefinition(ClassName, &m_Func);

	m_Initialized = true;

	Game->m_PluginMgr->AddObject(this);
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::Construct(CScScript* Script, CScStack* Stack)
{
	if(m_Func.Construct) m_ObjectID = m_Func.Construct(this, Game, Script, Stack);
	else Stack->CorrectParams(0);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name)
{
	bool Handled = false;

	if(m_Initialized && m_Func.CallMethod)
	{
		Handled = m_Func.CallMethod(m_ObjectID, Name, Script, Stack);
	}
	
	if(!Handled) return CBObject::ScCallMethod(Script, Stack, ThisStack, Name);
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXPluginObject::ScGetProperty(char* Name)
{
	m_ScValue->SetNULL();

	bool Handled = false;
	if(m_Initialized && m_Func.GetProperty)
	{
		Handled = m_Func.GetProperty(m_ObjectID, Name, m_ScValue);
	}
	if(!Handled) return CBObject::ScGetProperty(Name);
	else return m_ScValue;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::ScSetProperty(char* Name, CScValue* Value)
{
	bool Handled = false;

	if(m_Initialized && m_Func.SetProperty)
	{
		Handled = m_Func.SetProperty(m_ObjectID, Name, Value);
	}
	if(!Handled)
	{
		//////////////////////////////////////////////////////////////////////////
		// Ready
		//////////////////////////////////////////////////////////////////////////
		if(stricmp(Name, "Ready")==0)
		{
			m_Ready = Value->GetBool();
			return S_OK;
		}
		else return CBObject::ScSetProperty(Name, Value);
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
char* CSXPluginObject::ScToString()
{
	return "[plugin object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::Persist(CBPersistMgr *PersistMgr)
{
	CBObject::Persist(PersistMgr);
	PersistMgr->Transfer(TMEMBER(m_Class));

	// initialize object
	if(!PersistMgr->m_Saving)
	{
		memset(&m_Func, 0, sizeof(SWmeClassDef));
		m_Initialized = false;
		m_ObjectID = 0;

		HMODULE DllHandle = Game->m_PluginMgr->GetDllHandle(m_Class);
		if(!DllHandle)
		{
			Game->LOG(0, "Fatal: Error restoring plugin class '%s'", m_Class);
			return E_FAIL;
		}
		Initialize(m_Class, DllHandle);
		if(m_Initialized && m_Func.ConstructEmpty) m_ObjectID = m_Func.ConstructEmpty(this, Game);
	}

	// save/load subscribed events
	for(int i=0; i<WME_EVENT_MAX; i++)
	{
		bool B;
		if(PersistMgr->m_Saving)
		{
			B = m_SubscribedEvents[i];
			PersistMgr->Transfer(TMEMBER(B));
		}
		else
		{
			PersistMgr->Transfer(TMEMBER(B));
			m_SubscribedEvents[i] = B;
		}
	}

	if(m_Initialized && m_Func.Serialize)
	{
		m_Func.Serialize(m_ObjectID, PersistMgr);
	}

	return S_OK;
}

/*
//////////////////////////////////////////////////////////////////////////
HRESULT CSXPluginObject::Tick()
{
	if(m_Initialized && m_Func.Tick)
	{
		if(m_Func.Tick(m_ObjectID)) return S_OK;
		else return E_FAIL;
	}
	else return S_OK;
}
*/

//////////////////////////////////////////////////////////////////////////
bool CSXPluginObject::ApplyEvent(EWmeEvent Event, void* EventData1, void* EventData2)
{
	if(!m_Func.ReceiveEvent || !m_SubscribedEvents[Event]) return false;
	else return m_Func.ReceiveEvent(m_ObjectID, Event, EventData1, EventData2);
}
