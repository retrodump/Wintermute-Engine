// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdResponseContext.h"


IMPLEMENT_PERSISTENT(CAdResponseContext, false);

//////////////////////////////////////////////////////////////////////////
CAdResponseContext::CAdResponseContext(CBGame* inGame):CBBase(inGame)
{
	m_ID = 0;
	m_Context = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdResponseContext::~CAdResponseContext()
{
	SAFE_DELETE_ARRAY(m_Context);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdResponseContext::Persist(CBPersistMgr *PersistMgr)
{
	if(PersistMgr->CheckVersion(1, 8, 7))
	{
		PersistMgr->Transfer(TMEMBER(Game));
	}

	PersistMgr->Transfer(TMEMBER(m_Context));
	PersistMgr->Transfer(TMEMBER(m_ID));

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CAdResponseContext::SetContext(char* Context)
{
	SAFE_DELETE_ARRAY(m_Context);
	if(Context){
		m_Context = new char [strlen(Context)+1];
		if(m_Context) strcpy(m_Context, Context);
	}
}