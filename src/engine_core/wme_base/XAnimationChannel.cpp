// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "XAnimationChannel.h"


//////////////////////////////////////////////////////////////////////////
CXAnimationChannel::CXAnimationChannel(CBGame* inGame, CXModel* Model) : CBBase(inGame)
{
	m_Anim[0] = m_Anim[1] = NULL;

	m_Transitioning = false;
	m_TransitionStart = m_TranstitionTime = 0;
	m_StopTransitionTime = 0;

	m_Model = Model;
}

//////////////////////////////////////////////////////////////////////////
CXAnimationChannel::~CXAnimationChannel(void)
{
	SAFE_DELETE(m_Anim[0]);
	SAFE_DELETE(m_Anim[1]);

	m_Model = NULL; // ref only
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationChannel::PlayAnim(CXAnimationSet* AnimSet, DWORD TransitionTime, DWORD StopTransitionTime)
{
	m_StopTransitionTime = StopTransitionTime;

	CXActiveAnimation* Anim = NULL;
	if(AnimSet!=NULL)
	{
		Anim = new CXActiveAnimation(Game, m_Model);
		Anim->Start(AnimSet, AnimSet->m_Looping);
	}

	if(TransitionTime==0)
	{
		SAFE_DELETE(m_Anim[0]);
		SAFE_DELETE(m_Anim[1]);

		m_Anim[0] = Anim;
		m_Transitioning = false;
	}
	else
	{
		SAFE_DELETE(m_Anim[1]);
		if(m_Anim[0])
		{
			m_Anim[1] = Anim;
			m_Transitioning = (Anim != NULL);
			m_TranstitionTime = TransitionTime;
			m_TransitionStart = Game->m_Timer;
		}
		else
		{
			m_Anim[0] = Anim;
			SAFE_DELETE(m_Anim[1]);
			m_Transitioning = false;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationChannel::StopAnim(DWORD TransitionTime)
{
	if(TransitionTime==0 || !m_Anim[0])
	{
		m_Transitioning = false;
		SAFE_DELETE(m_Anim[0]);
		SAFE_DELETE(m_Anim[1]);
	}
	else
	{
		SAFE_DELETE(m_Anim[1]);
		
		if(m_Anim[0]) m_Anim[0]->SetLooping(false);
		m_Transitioning = true;
		m_TranstitionTime = TransitionTime;
		m_TransitionStart = Game->m_Timer;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationChannel::Update(bool Debug)
{
	if(m_Transitioning)
	{
		DWORD Delta = Game->m_Timer - m_TransitionStart;

		if(Delta >= m_TranstitionTime)
		{			
			m_Transitioning = false;

			// shift second animation to first slot and update it
			SAFE_DELETE(m_Anim[0]);
			m_Anim[0] = m_Anim[1];
			m_Anim[1] = NULL;

			if(m_Anim[0])
			{
				// we have to reset the start time because we wasted some on transitioning
				m_Anim[0]->ResetStartTime();
				return m_Anim[0]->Update();
			}
		}
		else
		{
			float LerpValue = float(Game->m_Timer - m_TransitionStart) / float(m_TranstitionTime);

			if(m_Anim[0])
			{
				m_Anim[0]->Update(0, true, LerpValue);
			}
			
			if(m_Anim[1])
			{
				m_Anim[1]->Update(1, true);
			}
			else
			{
				// disabled in WME 1.8.7, was causing glitches
				//m_Anim[0]->Update(1, true, 0, true);
			}


			return S_OK;
		}
	}
	else
	{
		if(m_Anim[0])
		{
			//return m_Anim[0]->Update();
			m_Anim[0]->Update();
			if(m_Anim[0]->IsFinished())
			{
				StopAnim(m_StopTransitionTime);
			}
			return S_OK;
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CXAnimationChannel::IsPlaying()
{
	if(m_Anim[1] && ! m_Anim[1]->IsFinished()) return true;
	else if(m_Anim[0] && ! m_Anim[0]->IsFinished()) return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
char* CXAnimationChannel::GetName()
{
	if(m_Anim[1]) return m_Anim[1]->GetName();
	else if(m_Anim[0]) return m_Anim[0]->GetName();
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationChannel::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Transitioning));
	PersistMgr->Transfer(TMEMBER(m_TransitionStart));
	PersistMgr->Transfer(TMEMBER(m_TranstitionTime));
	
	if(PersistMgr->CheckVersion(1, 7, 94))
	{
		PersistMgr->Transfer(TMEMBER(m_StopTransitionTime));
	}
	else m_StopTransitionTime = 0;

	for(int i=0; i<2; i++)
	{
		bool AnimExists = false;
		if(PersistMgr->m_Saving) AnimExists = (m_Anim[i] != NULL);
		PersistMgr->Transfer(TMEMBER(AnimExists));
		
		if(!PersistMgr->m_Saving)
		{
			if(AnimExists) m_Anim[i] = new CXActiveAnimation(Game, m_Model);
			else m_Anim[i] = NULL;
		}
		if(m_Anim[i]) m_Anim[i]->Persist(PersistMgr);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationChannel::UnloadAnim(CXAnimationSet* AnimSet)
{
	if(m_Anim[0] && m_Anim[0]->GetAnimSet()==AnimSet)
	{
		SAFE_DELETE(m_Anim[0]);
	}

	if(m_Anim[1] && m_Anim[1]->GetAnimSet()==AnimSet)
	{
		SAFE_DELETE(m_Anim[1]);
	}

	return S_OK;
}