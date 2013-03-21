// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DCAMERA_H__6F680B78_ED11_4633_92FF_8EDE782B7264__INCLUDED_)
#define AFX_3DCAMERA_H__6F680B78_ED11_4633_92FF_8EDE782B7264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DCamera : public CBNamedObject
{
public:
	void Move(float Speed);
	void RotateView(float X, float Y, float Z);
	void SetupPos(D3DXVECTOR3 Pos, D3DXVECTOR3 Target, float Bank=0);
	HRESULT GetViewMatrix(D3DXMATRIX* ViewMatrix);
	C3DCamera(CBGame* inGame);
	virtual ~C3DCamera();

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Target;
	float m_Bank;
	float m_FOV;
	float m_OrigFOV;
	float m_NearClipPlane;
	float m_FarClipPlane;

};

#endif // !defined(AFX_3DCAMERA_H__6F680B78_ED11_4633_92FF_8EDE782B7264__INCLUDED_)
