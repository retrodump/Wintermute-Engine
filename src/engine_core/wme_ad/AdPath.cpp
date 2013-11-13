// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdPath.h"


IMPLEMENT_PERSISTENT(CAdPath, false);

//////////////////////////////////////////////////////////////////////////
CAdPath::CAdPath(CBGame* inGame):CBBase(inGame)
{
	m_CurrIndex = -1;
	m_Ready = false;
}


//////////////////////////////////////////////////////////////////////////
CAdPath::~CAdPath()
{
	Reset();
}


//////////////////////////////////////////////////////////////////////////
void CAdPath::Reset()
{
	for(int i=0; i<m_Points.GetSize(); i++)
		delete m_Points[i];

	m_Points.RemoveAll();
	m_CurrIndex = -1;
	m_Ready = false;
}


//////////////////////////////////////////////////////////////////////////
CBPoint* CAdPath::GetFirst()
{
	if(m_Points.GetSize() > 0){
		m_CurrIndex = 0;
		return m_Points[m_CurrIndex];
	}
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
CBPoint* CAdPath::GetNext()
{
	m_CurrIndex++;
	if(m_CurrIndex < m_Points.GetSize()) return m_Points[m_CurrIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
CBPoint* CAdPath::GetCurrent()
{
	if(m_CurrIndex >= 0 && m_CurrIndex < m_Points.GetSize()) return m_Points[m_CurrIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
void CAdPath::AddPoint(CBPoint *point)
{
	m_Points.Add(point);
}


//////////////////////////////////////////////////////////////////////////
bool CAdPath::SetReady(bool ready)
{
	bool orig = m_Ready;
	m_Ready = ready;

	return orig;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdPath::Persist(CBPersistMgr* PersistMgr){
	
	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_CurrIndex));
	m_Points.Persist(PersistMgr);
	PersistMgr->Transfer(TMEMBER(m_Ready));

	return S_OK;
}
