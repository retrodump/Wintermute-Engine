// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BPoint.h"


IMPLEMENT_PERSISTENT(CBPoint, false);

//////////////////////////////////////////////////////////////////////////
CBPoint::CBPoint()
{
	x = y = 0;
}


//////////////////////////////////////////////////////////////////////////
CBPoint::~CBPoint()
{

}


//////////////////////////////////////////////////////////////////////////
CBPoint::CBPoint(int initX, int initY)
{
	x = initX;
	y = initY;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPoint::Persist(CBPersistMgr* PersistMgr){

	PersistMgr->Transfer(TMEMBER(x));
	PersistMgr->Transfer(TMEMBER(y));

	return S_OK;
}