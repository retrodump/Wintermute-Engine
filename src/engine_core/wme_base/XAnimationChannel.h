// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "XAnimationSet.h"
#include "XActiveAnimation.h"

class CXAnimationChannel : public CBBase
{
public:

	CXAnimationChannel(CBGame* inGame, CXModel* Model);
	virtual ~CXAnimationChannel(void);

	HRESULT PlayAnim(CXAnimationSet* AnimSet, DWORD TransitionTime = 0, DWORD StopTransitionTime = 0);
	HRESULT StopAnim(DWORD TransitionTime);

	HRESULT Update(bool Debug);

	bool IsPlaying();
	char* GetName();

	HRESULT Persist(CBPersistMgr* PersistMgr);
	HRESULT UnloadAnim(CXAnimationSet* AnimSet);

private:
	CXModel* m_Model;
	CXActiveAnimation* m_Anim[2];
	bool m_Transitioning;
	DWORD m_TransitionStart;
	DWORD m_TranstitionTime;
	DWORD m_StopTransitionTime;
};
