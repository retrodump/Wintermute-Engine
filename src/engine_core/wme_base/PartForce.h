// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CPartForce : public CBNamedObject
{
public:
	typedef enum TForceType
	{
		FORCE_POINT, FORCE_GLOBAL
	};
	
	CPartForce(CBGame* inGame);
	virtual ~CPartForce(void);

	D3DXVECTOR2 m_Pos;
	D3DXVECTOR2 m_Direction;
	TForceType m_Type;

	HRESULT Persist(CBPersistMgr* PersistMgr);
};
