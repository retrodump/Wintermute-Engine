// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DVERTEX_H__E3FA26E0_7CD7_4963_8626_B5FE53655E02__INCLUDED_)
#define AFX_3DVERTEX_H__E3FA26E0_7CD7_4963_8626_B5FE53655E02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class C3DVertex
{
public:
	int m_BoneID;
	D3DXVECTOR3 m_Pos;

	C3DVertex();
	virtual ~C3DVertex();

};

#endif // !defined(AFX_3DVERTEX_H__E3FA26E0_7CD7_4963_8626_B5FE53655E02__INCLUDED_)
