// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BObject.h"
#include "PartParticle.h"
#include "PartForce.h"

class CPartEmitter : public CBObject
{
public:
	DECLARE_PERSISTENT(CPartEmitter, CBObject);

	CPartEmitter(CBGame* inGame, CBScriptHolder* Owner);
	virtual ~CPartEmitter(void);

	int m_Width;
	int m_Height;

	int m_Angle1;
	int m_Angle2;

	float m_Rotation1;
	float m_Rotation2;

	float m_AngVelocity1;
	float m_AngVelocity2;

	float m_GrowthRate1;
	float m_GrowthRate2;
	bool m_ExponentialGrowth;

	float m_Velocity1;
	float m_Velocity2;
	bool m_VelocityZBased;

	float m_Scale1;
	float m_Scale2;
	bool m_ScaleZBased;

	int m_MaxParticles;

	int m_LifeTime1;
	int m_LifeTime2;
	bool m_LifeTimeZBased;

	int m_GenInterval;
	int m_GenAmount;

	bool m_Running;
	int m_OverheadTime;

	int m_MaxBatches;
	int m_BatchesGenerated;

	RECT m_Border;
	int m_BorderThicknessLeft;
	int m_BorderThicknessRight;
	int m_BorderThicknessTop;
	int m_BorderThicknessBottom;

	int m_FadeInTime;
	int m_FadeOutTime;

	int m_Alpha1;
	int m_Alpha2;
	bool m_AlphaTimeBased;

	bool m_UseRegion;

	char* m_EmitEvent;
	CBScriptHolder* m_Owner;

	HRESULT Start();
	
	HRESULT Update();
	HRESULT Display(CBRegion* Region=NULL);

	HRESULT SortParticlesByZ();
	HRESULT AddSprite(char* Filename);
	HRESULT RemoveSprite(char* Filename);
	HRESULT SetBorder(int X, int Y, int Width, int Height);
	HRESULT SetBorderThickness(int ThicknessLeft, int ThicknessRight, int ThicknessTop, int ThicknessBottom);

	HRESULT AddForce(char* Name, CPartForce::TForceType Type, int PosX, int PosY, float Angle, float Strength);
	HRESULT RemoveForce(char* Name);

	CBArray<CPartForce*, CPartForce*> m_Forces;

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char* Name, CScValue* Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);
	virtual char* ScToString();


private:
	CPartForce* AddForceByName(char* Name);
	int static CompareZ(const void* Obj1, const void* Obj2);
	HRESULT InitParticle(CPartParticle* Particle, DWORD CurrentTime, DWORD TimerDelta);
	HRESULT UpdateInternal(DWORD CurrentTime, DWORD TimerDelta);
	DWORD m_LastGenTime;
	CBArray<CPartParticle*, CPartParticle*> m_Particles;	
	CBArray<char*, char*> m_Sprites;
};
