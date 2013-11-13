// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "XAnimation.h"
#include "XFrameNode.h"

class CXModel;

class CXAnimationSet :	public CBNamedObject
{
public:
	//////////////////////////////////////////////////////////////////////////
	class CXAnimationEvent
	{
	public:
		char* m_EventName;
		int m_Frame;		

		CXAnimationEvent()
		{
			m_EventName = NULL;
			m_Frame = -1;
		}

		CXAnimationEvent(char* Name, int Frame)
		{
			m_EventName = NULL;
			CBUtils::SetString(&m_EventName, Name);
			m_Frame = Frame;
		}

		virtual ~CXAnimationEvent()
		{
			if(m_EventName) delete [] m_EventName;
		}

		HRESULT Persist(CBPersistMgr *PersistMgr)
		{
			PersistMgr->Transfer(TMEMBER(m_EventName));
			PersistMgr->Transfer(TMEMBER(m_Frame));
			return S_OK;
		}
	};


	CXAnimationSet(CBGame* inGame, CXModel* Model);
	virtual ~CXAnimationSet();

	HRESULT FindBones(CXFrameNode* RootFrame);
	HRESULT AddAnimation(CXAnimation* Anim);
	HRESULT AddEvent(CXAnimationEvent* Event);
	HRESULT Update(int Slot, DWORD LocalTime, float LerpValue);
	HRESULT OnFrameChanged(int CurrentFrame, int PrevFrame);

	int GetFrameTime();
	DWORD GetTotalTime();

	bool m_Looping;	

	HRESULT Persist(CBPersistMgr* PersistMgr);
	
protected:
	CBArray<CXAnimation*, CXAnimation*> m_Animations;
	CBArray<CXAnimationEvent*, CXAnimationEvent*> m_Events;

	int m_FrameTime;
	DWORD m_TotalTime;
	CXModel* m_Model;
};
