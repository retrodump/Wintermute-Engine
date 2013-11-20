// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BTransitionMgr.h"


//////////////////////////////////////////////////////////////////////////
CBTransitionMgr::CBTransitionMgr(CBGame* inGame):CBBase(inGame)
{
	m_State = TRANS_MGR_READY;
	m_Type = TRANSITION_NONE;
	m_OrigInteractive = false;
	m_PreserveInteractive = false;
	m_LastTime = 0;
	m_Started = false;
}



//////////////////////////////////////////////////////////////////////////
CBTransitionMgr::~CBTransitionMgr()
{

}


//////////////////////////////////////////////////////////////////////////
bool CBTransitionMgr::IsReady()
{
	return (m_State==TRANS_MGR_READY);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBTransitionMgr::Start(TTransitionType Type, bool NonInteractive)
{
	if(m_State != TRANS_MGR_READY) return S_OK;

	if(Type==TRANSITION_NONE || Type>=NUM_TRANSITION_TYPES){
		m_State = TRANS_MGR_READY;
		return S_OK;
	}

	if(NonInteractive){
		m_PreserveInteractive = true;
		m_OrigInteractive = Game->m_Interactive;
		Game->m_Interactive = false;
	}
	else m_PreserveInteractive;

	m_Type = Type;	
	m_State = TRANS_MGR_RUNNING;
	m_Started = false;

	return S_OK;
}

#define FADE_DURATION 200

//////////////////////////////////////////////////////////////////////////
HRESULT CBTransitionMgr::Update()
{
	if(IsReady()) return S_OK;

	if(!m_Started){
		m_Started = true;
		m_LastTime = timeGetTime();
	}

	switch(m_Type){
		case TRANSITION_NONE:
			m_State = TRANS_MGR_READY;
		break;

		case TRANSITION_FADE_OUT:
		{		
			DWORD time = timeGetTime() - m_LastTime;
			int Alpha = 255 - (float)time / (float)FADE_DURATION * 255;
			Alpha = min(255, max(Alpha, 0));
			Game->m_Renderer->Fade((WORD)Alpha);
			
			if(time > FADE_DURATION) m_State = TRANS_MGR_READY;
		}	
		break;

		case TRANSITION_FADE_IN:
		{		
			DWORD time = timeGetTime() - m_LastTime;
			int Alpha = (float)time / (float)FADE_DURATION * 255;
			Alpha = min(255, max(Alpha, 0));
			Game->m_Renderer->Fade((WORD)Alpha);
			
			if(time > FADE_DURATION) m_State = TRANS_MGR_READY;
		}	
		break;

	}

	if(IsReady()){
		if(m_PreserveInteractive) Game->m_Interactive = m_OrigInteractive;
	}
	return S_OK;
}
