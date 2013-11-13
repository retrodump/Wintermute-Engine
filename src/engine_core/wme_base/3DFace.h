// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DFACE_H__D90C3C58_0FF0_4DF0_A223_9C21CFD5ED7A__INCLUDED_)
#define AFX_3DFACE_H__D90C3C58_0FF0_4DF0_A223_9C21CFD5ED7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class C3DFace
{
public:
	WORD m_Vertices[3];
	D3DXVECTOR3 m_Normals[3];
	C3DFace();
	virtual ~C3DFace();
	float m_u[3];
	float m_v[3];

};

#endif // !defined(AFX_3DFACE_H__D90C3C58_0FF0_4DF0_A223_9C21CFD5ED7A__INCLUDED_)
