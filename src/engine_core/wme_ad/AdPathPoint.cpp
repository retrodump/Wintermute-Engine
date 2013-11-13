// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdPathPoint.h"


IMPLEMENT_PERSISTENT(CAdPathPoint, false);

//////////////////////////////////////////////////////////////////////////
CAdPathPoint::CAdPathPoint()
{
	x = y = 0;
	m_Distance = 0;

	m_Marked = false;
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdPathPoint::CAdPathPoint(int initX, int initY, int initDistance)
{
	x = initX;
	y = initY;
	m_Distance = initDistance;

	m_Marked = false;
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdPathPoint::~CAdPathPoint()
{
	m_Origin = NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdPathPoint::Persist(CBPersistMgr* PersistMgr){
	
	CBPoint::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Distance));
	PersistMgr->Transfer(TMEMBER(m_Marked));
	PersistMgr->Transfer(TMEMBER(m_Origin));

	return S_OK;
}