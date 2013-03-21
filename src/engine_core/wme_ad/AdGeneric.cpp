// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdGeneric.h"


//////////////////////////////////////////////////////////////////////////
CAdGeneric::CAdGeneric(CBGame* inGame):CBScriptable(inGame, false, false)
{
	m_Mesh = NULL;
	m_Active = true;
	m_ReceiveShadows = false;
}


//////////////////////////////////////////////////////////////////////////
CAdGeneric::~CAdGeneric()
{
	SAFE_DELETE(m_Mesh);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdGeneric::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Active));
	return S_OK;
}
