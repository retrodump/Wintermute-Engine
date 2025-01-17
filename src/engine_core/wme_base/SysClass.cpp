// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "dcgf.h"
#include "SysClass.h"



//////////////////////////////////////////////////////////////////////////
CSysClass::CSysClass(const AnsiString& name, PERSISTBUILD build, PERSISTLOAD load, bool persistent_class)
{
	m_Name = name;

	m_Build = build;
	m_Load = load;
	m_Next = NULL;
	m_SavedID = -1;
	m_Persistent = persistent_class;
	m_NumInst = 0;

	CSysClassRegistry::GetInstance()->RegisterClass(this);
}


//////////////////////////////////////////////////////////////////////////
CSysClass::~CSysClass()
{
	CSysClassRegistry::GetInstance()->UnregisterClass(this);
	RemoveAllInstances();
}

//////////////////////////////////////////////////////////////////////////
bool CSysClass::RemoveAllInstances()
{
	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		delete (*it);
	}
	m_Instances.clear();
	m_InstanceMap.clear();

	return true;
}

//////////////////////////////////////////////////////////////////////////
CSysInstance* CSysClass::AddInstance(void* instance, int id, int savedId)
{
	CSysInstance* inst = new CSysInstance(instance, id, this);	
	inst->SetSavedID(savedId);
	m_Instances.insert(inst);

	m_InstanceMap[instance] = inst;

	CSysClassRegistry::GetInstance()->AddInstanceToTable(inst, instance);

	return inst;
}


//////////////////////////////////////////////////////////////////////////
bool CSysClass::RemoveInstance(void* instance)
{
	InstanceMap::iterator mapIt = m_InstanceMap.find(instance);
	if (mapIt == m_InstanceMap.end()) return false;

	Instances::iterator it = m_Instances.find((*mapIt).second);
	if (it != m_Instances.end())
	{
		delete (*it);
		m_Instances.erase(it);
	}

	m_InstanceMap.erase(mapIt);

	return false;
}

//////////////////////////////////////////////////////////////////////////
int CSysClass::GetInstanceID(void* pointer)
{	
	InstanceMap::iterator mapIt = m_InstanceMap.find(pointer);
	if (mapIt == m_InstanceMap.end()) return -1;
	else return (*mapIt).second->GetID();
}

//////////////////////////////////////////////////////////////////////////
void* CSysClass::IDToPointer(int savedID)
{
	//slow
	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		if ((*it)->GetSavedID() == savedID) return (*it)->GetInstance();
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
int CSysClass::GetNumInstances()
{
	return m_Instances.size();
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::Dump(FILE* stream)
{
	fprintf(stream, "%03d %c %-20s instances: %d\n", m_ID, m_Persistent?'p':' ', m_Name.c_str(), GetNumInstances());
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::SaveTable(CBGame* Game, CBPersistMgr* PersistMgr)
{
	PersistMgr->PutString(m_Name.c_str());
	PersistMgr->PutDWORD(m_ID);
	PersistMgr->PutDWORD(m_Instances.size());

	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		PersistMgr->PutDWORD((*it)->GetID());
	}
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::LoadTable(CBGame* Game, CBPersistMgr* PersistMgr)
{
	m_SavedID = PersistMgr->GetDWORD();
	int numInstances = PersistMgr->GetDWORD();

	for (int i = 0; i < numInstances; i++)
	{
		if (m_Persistent)
		{
			int instId = PersistMgr->GetDWORD();

			if (i > 0)
			{
				Game->LOG(0, "Warning: attempting to load multiple instances of persistent class %s (%d)", m_Name.c_str(), numInstances);
				continue;
			}

			Instances::iterator it = m_Instances.begin();
			if (it != m_Instances.end())
			{
				(*it)->SetSavedID(instId);
				CSysClassRegistry::GetInstance()->AddInstanceToTable((*it), (*it)->GetInstance());
			}
			else Game->LOG(0, "Warning: instance %d of persistent class %s not found", i, m_Name.c_str());
		}
		// normal instances, create empty objects
		else
		{
			void* emptyObject = m_Build();
			AddInstance(emptyObject, CSysClassRegistry::GetInstance()->GetNextID(), PersistMgr->GetDWORD());
		}

	}
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::SaveInstances(CBGame* Game, CBPersistMgr* PersistMgr)
{
	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		// write instace header
		PersistMgr->PutDWORD(m_ID);
		PersistMgr->PutDWORD((*it)->GetID());

		m_Load((*it)->GetInstance(), PersistMgr);
	}
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::LoadInstance(void* instance, CBPersistMgr* PersistMgr)
{
	m_Load(instance, PersistMgr);
}


//////////////////////////////////////////////////////////////////////////
void CSysClass::ResetSavedIDs()
{
	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		(*it)->SetSavedID(-1);
	}
}

//////////////////////////////////////////////////////////////////////////
void CSysClass::InstanceCallback(SYS_INSTANCE_CALLBACK lpCallback, void* lpData)
{
	Instances::iterator it;
	for (it = m_Instances.begin(); it != m_Instances.end(); ++it)
	{
		lpCallback((*it)->GetInstance(), lpData);
	}
}
