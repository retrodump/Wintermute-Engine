// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CAdGeneric : public CBScriptable
{
public:
	bool m_ReceiveShadows;
	HRESULT Persist(CBPersistMgr* PersistMgr);
	bool m_Active;
	CAdGeneric(CBGame* inGame);
	virtual ~CAdGeneric();
	C3DMesh* m_Mesh;
};
