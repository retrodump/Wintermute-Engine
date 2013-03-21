// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\PartForce.h"


//////////////////////////////////////////////////////////////////////////
CPartForce::CPartForce(CBGame* inGame) : CBNamedObject(inGame)
{
	m_Pos = D3DXVECTOR2(0.0f, 0.0f);
	m_Direction = D3DXVECTOR2(0.0f, 0.0f);
	m_Type = FORCE_POINT;
}


//////////////////////////////////////////////////////////////////////////
CPartForce::~CPartForce(void)
{
}


//////////////////////////////////////////////////////////////////////////
HRESULT CPartForce::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Name));
	PersistMgr->Transfer(TMEMBER(m_Pos));
	PersistMgr->Transfer(TMEMBER(m_Direction));
	PersistMgr->Transfer(TMEMBER_INT(m_Type));

	return S_OK;
}