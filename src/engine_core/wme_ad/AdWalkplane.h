// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADWALKPLANE_H__804371CA_FCB3_4457_A318_EFBDB1E269E0__INCLUDED_)
#define AFX_ADWALKPLANE_H__804371CA_FCB3_4457_A318_EFBDB1E269E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdWalkplane : public CBScriptable
{
public:
	bool m_ReceiveShadows;
	HRESULT Persist(CBPersistMgr* PersistMgr);
	bool m_Active;
	C3DMesh* m_Mesh;
	CAdWalkplane(CBGame* inGame);
	virtual ~CAdWalkplane();	
};

#endif // !defined(AFX_ADWALKPLANE_H__804371CA_FCB3_4457_A318_EFBDB1E269E0__INCLUDED_)
