// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdWaypointGroup3D.h"


//IMPLEMENT_PERSISTENT(CAdWaypointGroup3D, false);

//////////////////////////////////////////////////////////////////////////
CAdWaypointGroup3D::CAdWaypointGroup3D(CBGame* inGame):CBBase(inGame)
{
	m_Active = true;
}


//////////////////////////////////////////////////////////////////////////
CAdWaypointGroup3D::~CAdWaypointGroup3D()
{
	for(int i=0; i<m_Points.GetSize(); i++) delete m_Points[i];
	m_Points.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup3D::AddFromMesh(C3DMesh *Mesh)
{
	D3DXVECTOR3 Min = D3DXVECTOR3(0,0,0);
	D3DXVECTOR3 Max = D3DXVECTOR3(0,0,0);

	if(Mesh->m_NumVertices > 0)	Min = Max = Mesh->m_Vertices[0].m_Pos;

	for(int i=0; i<Mesh->m_NumVertices; i++){
		Min.x = min(Min.x, Mesh->m_Vertices[i].m_Pos.x);
		Min.y = min(Min.y, Mesh->m_Vertices[i].m_Pos.y);
		Min.z = min(Min.z, Mesh->m_Vertices[i].m_Pos.z);

		Max.x = max(Max.x, Mesh->m_Vertices[i].m_Pos.x);
		Max.y = max(Max.y, Mesh->m_Vertices[i].m_Pos.y);
		Max.z = max(Max.z, Mesh->m_Vertices[i].m_Pos.z);	
	}

	D3DXVECTOR3* vect = new D3DXVECTOR3;
	vect->x = Min.x + (Max.x - Min.x) / 2;
	vect->y = Min.y + (Max.y - Min.y) / 2;
	vect->z = Min.z + (Max.z - Min.z) / 2;

	m_Points.Add(vect);
		
	return S_OK;
}

/*
//////////////////////////////////////////////////////////////////////////
HRESULT CAdWaypointGroup3D::Persist(CBPersistMgr* PersistMgr){

	PersistMgr->Transfer(TMEMBER(Game));
	PersistMgr->Transfer(TMEMBER(m_Active));
	//m_Points.Persist(PersistMgr);

	return S_OK;
}
*/