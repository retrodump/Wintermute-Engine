// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\bnamedobject.h"


//////////////////////////////////////////////////////////////////////////
CBNamedObject::CBNamedObject(CBGame* inGame) : CBBase(inGame)
{
	m_Name = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBNamedObject::CBNamedObject() : CBBase()
{
	m_Name = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBNamedObject::CBNamedObject(TDynamicConstructor, TDynamicConstructor)
{
	m_Name = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBNamedObject::~CBNamedObject(void)
{
	SAFE_DELETE_ARRAY(m_Name);
}


//////////////////////////////////////////////////////////////////////
void CBNamedObject::SetName(char * Name)
{
	SAFE_DELETE_ARRAY(m_Name);

	m_Name = new char [strlen(Name)+1];
	if(m_Name!=NULL) strcpy(m_Name, Name);
}
