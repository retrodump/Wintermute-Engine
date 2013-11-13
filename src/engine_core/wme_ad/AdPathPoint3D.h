// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADPATHPOINT3D_H__B689B188_64E6_4C68_8BFF_C7C0C9DCF3D4__INCLUDED_)
#define AFX_ADPATHPOINT3D_H__B689B188_64E6_4C68_8BFF_C7C0C9DCF3D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdPathPoint3D: public CBBase
{
public:
	DECLARE_PERSISTENT(CAdPathPoint3D, CBBase);
	CAdPathPoint3D(D3DXVECTOR3 Pos, float Dist);
	CAdPathPoint3D();
	virtual ~CAdPathPoint3D();
	CAdPathPoint3D* m_Origin;
	bool m_Marked;
	float m_Distance;
	D3DXVECTOR3 m_Pos;
};

#endif // !defined(AFX_ADPATHPOINT3D_H__B689B188_64E6_4C68_8BFF_C7C0C9DCF3D4__INCLUDED_)
