// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#include "dcgf.h"
#include "SysInstance.h"
#include "SysClass.h"


//////////////////////////////////////////////////////////////////////////
CSysInstance::CSysInstance(void* Instance, int ID, CSysClass* sysClass)
{
	m_Instance = Instance;
	m_ID = ID;
	m_SavedID = -1;
	m_Class = sysClass;
	
	m_Used = false;
}


//////////////////////////////////////////////////////////////////////////
CSysInstance::~CSysInstance()
{
}
