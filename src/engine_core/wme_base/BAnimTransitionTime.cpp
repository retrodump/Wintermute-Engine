// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BAnimTransitionTime.h"

//////////////////////////////////////////////////////////////////////////
CBAnimTransitionTime::CBAnimTransitionTime(char* From, char* To, DWORD Time)
{
	m_AnimFrom = NULL;
	m_AnimTo = NULL;

	CBUtils::SetString(&m_AnimFrom, From);
	CBUtils::SetString(&m_AnimTo, To);
	m_Time = Time;
}

//////////////////////////////////////////////////////////////////////////
CBAnimTransitionTime::CBAnimTransitionTime()
{
	m_AnimFrom = NULL;
	m_AnimTo = NULL;
	m_Time = 0;
}

//////////////////////////////////////////////////////////////////////////
CBAnimTransitionTime::~CBAnimTransitionTime(void)
{
	SAFE_DELETE_ARRAY(m_AnimFrom);
	SAFE_DELETE_ARRAY(m_AnimTo);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAnimTransitionTime::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_AnimFrom));
	PersistMgr->Transfer(TMEMBER(m_AnimTo));
	PersistMgr->Transfer(TMEMBER(m_Time));

	return S_OK;
}
