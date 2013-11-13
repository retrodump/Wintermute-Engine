// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdBlock.h"


//////////////////////////////////////////////////////////////////////////
CAdBlock::CAdBlock(CBGame* inGame):CBScriptable(inGame, false, false)
{
	m_Mesh = NULL;
	m_Active = true;
	m_ReceiveShadows = false;
}


//////////////////////////////////////////////////////////////////////////
CAdBlock::~CAdBlock()
{
	SAFE_DELETE(m_Mesh);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdBlock::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Active));
	return S_OK;
}
