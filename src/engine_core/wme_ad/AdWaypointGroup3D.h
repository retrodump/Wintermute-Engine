// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADWAYPOINTGROUP3D_H__BB721C8A_E318_4084_A6B2_17C08EC3B1C0__INCLUDED_)
#define AFX_ADWAYPOINTGROUP3D_H__BB721C8A_E318_4084_A6B2_17C08EC3B1C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdSceneGeometry;

class CAdWaypointGroup3D : public CBBase  
{
public:
	//DECLARE_PERSISTENT(CAdWaypointGroup3D, CBBase);
	bool m_Active;
	HRESULT AddFromMesh(C3DMesh* Mesh);
	CAdWaypointGroup3D(CBGame* inGame);
	virtual ~CAdWaypointGroup3D();
	CBArray<D3DXVECTOR3*, D3DXVECTOR3*> m_Points;
};

#endif // !defined(AFX_ADWAYPOINTGROUP3D_H__BB721C8A_E318_4084_A6B2_17C08EC3B1C0__INCLUDED_)
