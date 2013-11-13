// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADPATH3D_H__D56E80D6_E3EB_44D7_9673_3C8B2EDB708A__INCLUDED_)
#define AFX_ADPATH3D_H__D56E80D6_E3EB_44D7_9673_3C8B2EDB708A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdPath3D : public CBBase  
{
public:
	DECLARE_PERSISTENT(CAdPath3D, CBBase);
	D3DXVECTOR3* GetCurrent();
	D3DXVECTOR3* GetNext();
	D3DXVECTOR3* GetFirst();
	bool SetReady(bool ready);
	void AddPoint(float x, float y, float z);
	void AddPoint(D3DXVECTOR3 Point);
	void Reset();
	CAdPath3D(CBGame* inGame);
	virtual ~CAdPath3D();
	bool m_Ready;
	CBArray<D3DXVECTOR3*, D3DXVECTOR3*> m_Points;
	int m_CurrIndex;
};

#endif // !defined(AFX_ADPATH3D_H__D56E80D6_E3EB_44D7_9673_3C8B2EDB708A__INCLUDED_)
