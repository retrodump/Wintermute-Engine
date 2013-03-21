// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "XAnimationSet.h"
#include "XModel.h"

//////////////////////////////////////////////////////////////////////////
CXAnimationSet::CXAnimationSet(CBGame* inGame, CXModel* Model) : CBNamedObject(inGame)
{
	m_FrameTime = -1;
	m_TotalTime = 0;
	m_Looping = false;
	m_Model = Model;
}


//////////////////////////////////////////////////////////////////////////
CXAnimationSet::~CXAnimationSet()
{
	// remove child animations
	for(int i=0; i<m_Animations.GetSize(); i++)
	{
		delete m_Animations[i];
	}
	m_Animations.RemoveAll();


	// remove events
	for(int i=0; i<m_Events.GetSize(); i++)
	{
		delete m_Events[i];
	}
	m_Events.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::FindBones(CXFrameNode* RootFrame)
{
	for(int i=0; i<m_Animations.GetSize(); i++)
	{
		m_Animations[i]->FindBone(RootFrame);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::AddAnimation(CXAnimation* Anim)
{
	if(!Anim) return E_FAIL;
	else
	{
		m_Animations.Add(Anim);
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::AddEvent(CXAnimationEvent* Event)
{
	if(!Event) return E_FAIL;
	else
	{
		int FrameTime = GetFrameTime();
		if(FrameTime<0)
		{
			Game->LOG(0, "Error adding animation event %s, no keyframes found", Event->m_EventName);
			SAFE_DELETE(Event);
			return E_FAIL;
		}

		int TotalFrames = 0;
		if(FrameTime>0) TotalFrames = GetTotalTime() / FrameTime + 1;

		if(Event->m_Frame < 1) Event->m_Frame = 1;
		if(Event->m_Frame > TotalFrames) Event->m_Frame = TotalFrames;

		m_Events.Add(Event);
		return S_OK;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::Update(int Slot, DWORD LocalTime, float LerpValue)
{
	HRESULT res;
	for(int i=0; i<m_Animations.GetSize(); i++)
	{
		res = m_Animations[i]->Update(Slot, LocalTime * ((float)m_Model->m_TicksPerSecond / 1000.0f), LerpValue);
		if(FAILED(res)) return res;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
int CXAnimationSet::GetFrameTime()
{
	if(m_FrameTime >= 0) return m_FrameTime;

	m_FrameTime = 0;
	for(int i=0; i<m_Animations.GetSize(); i++)
	{
		int FrameTime = m_Animations[i]->GetFrameTime();
		if(m_FrameTime == 0) m_FrameTime = FrameTime / ((float)m_Model->m_TicksPerSecond / 1000.0f);
		else if(FrameTime > 0) m_FrameTime = min(m_FrameTime, FrameTime / ((float)m_Model->m_TicksPerSecond / 1000.0f));
	}
	return m_FrameTime;
}


//////////////////////////////////////////////////////////////////////////
DWORD CXAnimationSet::GetTotalTime()
{
	if(m_TotalTime > 0) return m_TotalTime;

	m_TotalTime = 0;
	for(int i=0; i<m_Animations.GetSize(); i++)
	{
		m_TotalTime = max(m_TotalTime, m_Animations[i]->GetTotalTime() / ((float)m_Model->m_TicksPerSecond / 1000.0f));
	}
	return m_TotalTime;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::OnFrameChanged(int CurrentFrame, int PrevFrame)
{
	if(!m_Model || !m_Model->m_Owner) return S_OK;

	if(PrevFrame > CurrentFrame)
	{
		for(int i=0; i<m_Events.GetSize(); i++)
		{
			if(m_Events[i]->m_Frame > PrevFrame)
			{
				m_Model->m_Owner->ApplyEvent(m_Events[i]->m_EventName);
			}
		}
		PrevFrame = -1;
	}

	for(int i=0; i<m_Events.GetSize(); i++)
	{
		if(m_Events[i]->m_Frame > PrevFrame && m_Events[i]->m_Frame <= CurrentFrame)
		{
			m_Model->m_Owner->ApplyEvent(m_Events[i]->m_EventName);
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimationSet::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_Looping));

	// persist events
	int NumEvents;
	if(PersistMgr->m_Saving) NumEvents = m_Events.GetSize();
	PersistMgr->Transfer(TMEMBER(NumEvents));

	for(int i=0; i<NumEvents; i++)
	{
		if(PersistMgr->m_Saving) m_Events[i]->Persist(PersistMgr);
		else
		{
			CXAnimationEvent* Event = new CXAnimationEvent();
			Event->Persist(PersistMgr);
			m_Events.Add(Event);
		}
	}

	return S_OK;
}