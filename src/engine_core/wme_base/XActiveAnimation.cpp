// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "XActiveAnimation.h"
#include "XModel.h"


//////////////////////////////////////////////////////////////////////////
CXActiveAnimation::CXActiveAnimation(CBGame* inGame, CXModel* Model):CBBase(inGame)
{
	m_Model = Model;

	m_Animation = NULL;

	m_Looping = false;
	m_Finished = true;
	m_StartTime = 0;
	m_LastLocalTime = 0;

	m_CurrentFrame = -1;
}


//////////////////////////////////////////////////////////////////////////
CXActiveAnimation::~CXActiveAnimation(void)
{
	m_Animation = NULL; // ref only
	m_Model = NULL; // ref only
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXActiveAnimation::Start(CXAnimationSet* Animation, bool Looping)
{
	m_Animation = Animation;
	m_StartTime = Game->m_Timer;
	m_Looping = Looping;
	m_Finished = false;
	m_CurrentFrame = -1;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXActiveAnimation::ResetStartTime()
{
	m_StartTime = Game->m_Timer;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXActiveAnimation::Update(int Slot, bool PrevFrameOnly, float LerpValue, bool ForceStartFrame)
{
	// PrevFrameOnly means: don't update, just use the last pose, since
	// we're transitioning from it to another animation

	if(!m_Animation) return E_FAIL;

	DWORD LocalTime;
//Game->LOG(0, "%s %d %d %f %d", m_Animation->m_Name, Slot, PrevFrameOnly, LerpValue, ForceStartFrame);
	if(PrevFrameOnly)
	{
		LocalTime = m_LastLocalTime;
	}
	else
	{
		if(!m_Finished)
		{
			LocalTime = Game->m_Timer - m_StartTime;
			if(LocalTime > m_Animation->GetTotalTime())
			{
				if(m_Looping)
				{
					if(m_Animation->GetTotalTime()==0) LocalTime = 0;
					else LocalTime %= m_Animation->GetTotalTime();
				}
				else
				{
					m_Finished = true;
				}
			}
		}
	}
	if(m_Finished)
	{
		LocalTime = m_Animation->GetTotalTime() - 1;
	}

	m_LastLocalTime = LocalTime;

	if(ForceStartFrame)
	{
		LocalTime = 0;
	}

	// handle events
	int Frame;
	if(m_Animation->GetFrameTime() > 0)
		Frame = LocalTime / m_Animation->GetFrameTime() + 1;
	else
		Frame = 0;

	if(Frame != m_CurrentFrame)
	{	
		// don't trigger events when transitioning
		if(!PrevFrameOnly) m_Animation->OnFrameChanged(Frame, m_CurrentFrame);
		m_CurrentFrame = Frame;
	}
//Game->LOG(0, "%s %d %f", m_Animation->m_Name, LocalTime, LerpValue);
	return m_Animation->Update(Slot, LocalTime, LerpValue);
	/*
	if(!m_Finished) return m_Animation->Update(Slot, LocalTime, LerpValue);
	else return S_OK;
	*/
}

//////////////////////////////////////////////////////////////////////////
char* CXActiveAnimation::GetName()
{
	if(m_Animation) return m_Animation->m_Name;
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXActiveAnimation::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_CurrentFrame));
	PersistMgr->Transfer(TMEMBER(m_StartTime));
	PersistMgr->Transfer(TMEMBER(m_Looping));
	PersistMgr->Transfer(TMEMBER(m_Finished));
	PersistMgr->Transfer(TMEMBER(m_LastLocalTime));

	if(PersistMgr->m_Saving) PersistMgr->Transfer(TMEMBER(m_Animation->m_Name));
	else
	{
		char* AnimName;
		PersistMgr->Transfer(TMEMBER(AnimName));
		if(AnimName) m_Animation = m_Model->GetAnimationSetByName(AnimName);
		else m_Animation = NULL;

		SAFE_DELETE_ARRAY(AnimName);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXActiveAnimation::SetLooping(bool Looping)
{
	m_Looping = Looping;
	return S_OK;
}