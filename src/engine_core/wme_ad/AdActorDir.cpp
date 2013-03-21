// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdActorDir.h"

IMPLEMENT_PERSISTENT(CAdActorDir, false);


//////////////////////////////////////////////////////////////////////////
CAdActorDir::CAdActorDir(CBGame* inGame): CBBase(inGame)
{
}


//////////////////////////////////////////////////////////////////////////
CAdActorDir::~CAdActorDir(void)
{
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActorDir::Persist(CBPersistMgr* PersistMgr)
{
	//PersistMgr->Transfer(TMEMBER(x));

	return S_OK;
}