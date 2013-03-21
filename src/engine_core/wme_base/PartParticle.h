// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CPartEmitter;

class CPartParticle : public CBBase
{
public:
	typedef enum TParticleState
	{
		PARTICLE_NORMAL, PARTICLE_FADEIN, PARTICLE_FADEOUT
	};

	CPartParticle(CBGame* inGame);
	virtual ~CPartParticle(void);

	float m_GrowthRate;
	bool m_ExponentialGrowth;

	float m_Rotation;
	float m_AngVelocity;

	int m_Alpha1;
	int m_Alpha2;

	RECT m_Border;
	D3DXVECTOR2 m_Pos;
	float m_PosZ;
	D3DXVECTOR2 m_Velocity;
	float m_Scale;
	CBSprite* m_Sprite;
	DWORD m_CreationTime;
	int m_LifeTime;
	bool m_IsDead;
	TParticleState m_State;

	HRESULT Update(CPartEmitter* Emitter, DWORD CurrentTime, DWORD TimerDelta);
	HRESULT Display(CPartEmitter* Emitter);

	HRESULT SetSprite(char* Filename);

	HRESULT FadeIn(DWORD CurrentTime, int FadeTime);
	HRESULT FadeOut(DWORD CurrentTime, int FadeTime);

	HRESULT Persist(CBPersistMgr* PersistMgr);
private:
	DWORD m_FadeStart;
	int m_FadeTime;
	int m_CurrentAlpha;
	int m_FadeStartAlpha;
};
