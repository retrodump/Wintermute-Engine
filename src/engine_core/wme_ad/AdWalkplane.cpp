// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdWalkplane.h"


//////////////////////////////////////////////////////////////////////////
CAdWalkplane::CAdWalkplane(CBGame* inGame):CBScriptable(inGame, false, false)
{
	m_Mesh = NULL;
	m_Active = true;
	m_ReceiveShadows = false;
}


//////////////////////////////////////////////////////////////////////////
CAdWalkplane::~CAdWalkplane()
{
	SAFE_DELETE(m_Mesh);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWalkplane::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Active));
	return S_OK;
}
