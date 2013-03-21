// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdPathPoint3D.h"


IMPLEMENT_PERSISTENT(CAdPathPoint3D, false);

//////////////////////////////////////////////////////////////////////////
CAdPathPoint3D::CAdPathPoint3D():CBBase()
{
	m_Pos = D3DXVECTOR3(0, 0, 0);
	m_Distance = 0;

	m_Marked = false;
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdPathPoint3D::CAdPathPoint3D(D3DXVECTOR3 Pos, float Dist)
{
	m_Pos = Pos;
	m_Distance = Dist;

	m_Marked = false;
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdPathPoint3D::~CAdPathPoint3D()
{
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdPathPoint3D::Persist(CBPersistMgr* PersistMgr){
	
	PersistMgr->Transfer(TMEMBER(m_Distance));
	PersistMgr->Transfer(TMEMBER(m_Marked));
	PersistMgr->Transfer(TMEMBER(m_Origin));
	PersistMgr->Transfer(TMEMBER(m_Pos));

	return S_OK;
}