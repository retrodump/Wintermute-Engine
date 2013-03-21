// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SxObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(CSXObject, false);

//////////////////////////////////////////////////////////////////////////
CSXObject::CSXObject(CBGame* inGame, CScStack* Stack):CBObject(inGame)
{
	int NumParams = Stack->Pop()->GetInt(0);
	for(int i=0; i<NumParams; i++){
		AddScript(Stack->Pop()->GetString());
	}
}


//////////////////////////////////////////////////////////////////////////
CSXObject::~CSXObject()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXObject::Persist(CBPersistMgr* PersistMgr)
{
	CBObject::Persist(PersistMgr);

	return S_OK;
}

