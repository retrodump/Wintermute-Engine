// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "XAnimationSet.h"

class CXActiveAnimation : public CBBase
{
public:
	CXActiveAnimation(CBGame* inGame, CXModel* Model);
	virtual ~CXActiveAnimation(void);

	HRESULT Start(CXAnimationSet* Animation, bool Looping = false);
	HRESULT Update(int Slot=0, bool PrevFrameOnly=false, float LerpValue=0.0f, bool ForceStartFrame=false);
	HRESULT ResetStartTime();
	HRESULT Persist(CBPersistMgr* PersistMgr);
	HRESULT SetLooping(bool Looping);

	char* GetName();
	CXAnimationSet* GetAnimSet()
	{
		return m_Animation;
	};

	bool IsLooping()
	{
		return m_Looping;
	};

	bool IsFinished()
	{
		return m_Finished;
	};

private:
	CXModel* m_Model;
	int m_CurrentFrame;
	DWORD m_StartTime;
	bool m_Looping;
	bool m_Finished;
	DWORD m_LastLocalTime;

	CXAnimationSet* m_Animation;
};
