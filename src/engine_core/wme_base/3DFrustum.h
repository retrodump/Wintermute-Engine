// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DFRUSTUM_H__7296E777_6AC8_44B4_A9D5_BA1C41306FD7__INCLUDED_)
#define AFX_3DFRUSTUM_H__7296E777_6AC8_44B4_A9D5_BA1C41306FD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DFrustum : public CBBase  
{
public:
	bool CubeInFrustum(float x, float y, float z, float size);
	bool SphereInFrustum(float x, float y, float z, float radius);
	bool PointInFrustum(float x, float y, float z);
	void CalcualteFrustum();
	enum EFrustumSide{
		RIGHT	= 0,		// The RIGHT side of the frustum
		LEFT	= 1,		// The LEFT	 side of the frustum
		BOTTOM	= 2,		// The BOTTOM side of the frustum
		TOP		= 3,		// The TOP side of the frustum
		BACK	= 4,		// The BACK	side of the frustum
		FRONT	= 5			// The FRONT side of the frustum
	}; 

	D3DXPLANE m_Planes[6];
	C3DFrustum(CBGame* inGame);
	virtual ~C3DFrustum();

};

#endif // !defined(AFX_3DFRUSTUM_H__7296E777_6AC8_44B4_A9D5_BA1C41306FD7__INCLUDED_)
