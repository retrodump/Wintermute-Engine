// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdPath3D.h"


IMPLEMENT_PERSISTENT(CAdPath3D, false);

//////////////////////////////////////////////////////////////////////////
CAdPath3D::CAdPath3D(CBGame* inGame):CBBase(inGame)
{
	m_CurrIndex = -1;
	m_Ready = false;
}


//////////////////////////////////////////////////////////////////////////
CAdPath3D::~CAdPath3D()
{
	Reset();
}


//////////////////////////////////////////////////////////////////////////
void CAdPath3D::Reset()
{
	for(int i=0; i<m_Points.GetSize(); i++)
		delete m_Points[i];

	m_Points.RemoveAll();
	m_CurrIndex = -1;
	m_Ready = false;
}


//////////////////////////////////////////////////////////////////////////
void CAdPath3D::AddPoint(D3DXVECTOR3 Point)
{
	m_Points.Add(new D3DXVECTOR3(Point));
}


//////////////////////////////////////////////////////////////////////////
void CAdPath3D::AddPoint(float x, float y, float z)
{
	m_Points.Add(new D3DXVECTOR3(x, y, z));
}


//////////////////////////////////////////////////////////////////////////
bool CAdPath3D::SetReady(bool ready)
{
	bool orig = m_Ready;
	m_Ready = ready;

	return orig;
}


//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3* CAdPath3D::GetFirst()
{
	if(m_Points.GetSize() > 0){
		m_CurrIndex = 0;
		return m_Points[m_CurrIndex];
	}
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3* CAdPath3D::GetNext()
{
	m_CurrIndex++;
	if(m_CurrIndex < m_Points.GetSize()) return m_Points[m_CurrIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3* CAdPath3D::GetCurrent()
{
	if(m_CurrIndex >= 0 && m_CurrIndex < m_Points.GetSize()) return m_Points[m_CurrIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdPath3D::Persist(CBPersistMgr* PersistMgr){

	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_CurrIndex));	
	PersistMgr->Transfer(TMEMBER(m_Ready));

	if(PersistMgr->m_Saving)
	{
		int j = m_Points.GetSize();
		PersistMgr->Transfer("ArraySize", &j);
		for(int i=0; i<j; i++)
		{
			PersistMgr->Transfer("x", &m_Points[i]->x);
			PersistMgr->Transfer("y", &m_Points[i]->y);
			PersistMgr->Transfer("z", &m_Points[i]->z);
		}
	}
	else
	{
		int j = 0;
		PersistMgr->Transfer("ArraySize", &j);
		for(int i=0; i<j; i++)
		{
			float x, y, z;
			PersistMgr->Transfer("x", &x);
			PersistMgr->Transfer("y", &y);
			PersistMgr->Transfer("z", &z);
			AddPoint(x, y, z);
		}

	}

	return S_OK;
}