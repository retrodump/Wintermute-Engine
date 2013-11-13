// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "coll_templ.h"

class CXFrameNode;
class CXAnimationSet;

class CXAnimation :	public CBBase
{
public:
	CXAnimation(CBGame* inGame);
	virtual ~CXAnimation();

#if WME_D3D9
	HRESULT Load(LPDIRECTXFILEDATA XObj, CXAnimationSet* ParentAnimSet);
#else
	HRESULT Load(LPDIRECTXFILEOBJECT XObj, CXAnimationSet* ParentAnimSet);
#endif

	HRESULT FindBone(CXFrameNode* RootFrame);
	HRESULT Update(int Slot, DWORD LocalTime, float AnimLerpValue);

	int GetFrameTime();
	DWORD GetTotalTime();


	// data types
protected:
	struct BONEPOSITIONKEY
	{
		DWORD Time;
		D3DXVECTOR3 Pos;
	};

	struct BONESCALEKEY
	{
		DWORD Time;
		D3DXVECTOR3 Scale;
	};

	struct BONEROTATIONKEY
	{
		DWORD Time;
		D3DXQUATERNION Rotation;
	};


protected:
	char* m_TargetName;
	CXFrameNode* m_TargetFrame;

	CBArray<BONEPOSITIONKEY*, BONEPOSITIONKEY*> m_PosKeys;
	CBArray<BONEROTATIONKEY*, BONEROTATIONKEY*> m_RotKeys;
	CBArray<BONESCALEKEY*, BONESCALEKEY*> m_ScaleKeys;

private:
	HRESULT LoadAnimationKeyData(BYTE* Buffer);
	HRESULT CXAnimation::LoadAnimationOptionData(BYTE* Buffer, CXAnimationSet* ParentAnimSet);
};
