// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdSceneState.h"


IMPLEMENT_PERSISTENT(CAdSceneState, false);

//////////////////////////////////////////////////////////////////////////
CAdSceneState::CAdSceneState(CBGame* inGame):CBBase(inGame)
{
	m_Filename = NULL;
}


//////////////////////////////////////////////////////////////////////////
CAdSceneState::~CAdSceneState()
{
	SAFE_DELETE_ARRAY(m_Filename);

	for(int i=0; i<m_NodeStates.GetSize(); i++)	delete m_NodeStates[i];
	m_NodeStates.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneState::Persist(CBPersistMgr *PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Filename));
	m_NodeStates.Persist(PersistMgr);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CAdSceneState::SetFilename(char *Filename)
{
	SAFE_DELETE_ARRAY(m_Filename);
	m_Filename = new char [strlen(Filename)+1];
	if(m_Filename) strcpy(m_Filename, Filename);
}


//////////////////////////////////////////////////////////////////////////
CAdNodeState* CAdSceneState::GetNodeState(char *Name, bool Saving)
{
	for(int i=0; i<m_NodeStates.GetSize(); i++){
		if(stricmp(m_NodeStates[i]->m_Name, Name)==0) return m_NodeStates[i];
	}

	if(Saving){
		CAdNodeState* ret = new CAdNodeState(Game);
		ret->SetName(Name);
		m_NodeStates.Add(ret);

		return ret;
	}
	else return NULL;
}
