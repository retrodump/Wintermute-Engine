// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_AdActor3DX_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_)
#define AFX_AdActor3DX_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdObject3D.h"
#include "XModel.h"
#include "BAnimTransitionTime.h"

class CAdActor3DX : public CAdObject3D
{
public:
	virtual CPartEmitter* CreateParticleEmitter(bool FollowParent=false, int OffsetX=0, int OffsetY=0);
	virtual CPartEmitter* CreateParticleEmitter(char* BoneName, D3DXVECTOR3 Offset);
	virtual HRESULT UpdatePartEmitter();
	char* m_PartBone;
	D3DXVECTOR3 m_PartOffset;


	HRESULT DisplayShadowVolume();
	virtual HRESULT RestoreDeviceObjects();
	virtual HRESULT InvalidateDeviceObjects();
	int m_StateAnimChannel;
	virtual void Talk(char* Text, char* Sound=NULL, DWORD Duration=0, char* Stances=NULL, TTextAlign Align=TAL_CENTER);
	virtual int GetHeight();
	HRESULT PlayAnim(char* Name, bool SetState);
	HRESULT PlayAnim(int Channel, char *Name, bool SetState);
	
	virtual DWORD GetAnimTransitionTime(char* From, char* To);
	CBArray<CBAnimTransitionTime*, CBAnimTransitionTime*> m_TransitionTimes;

	virtual HRESULT RenderModel();

	DWORD m_DefaultTransTime;
	DWORD m_DefaultStopTransTime;

	float m_AfterWalkAngle;
	char* m_TalkAnimName;
	char* m_IdleAnimName;
	char* m_WalkAnimName;
	char* m_TurnLeftAnimName;
	char* m_TurnRightAnimName;

	int m_TalkAnimChannel;

	TDirectWalkMode m_DirectWalkMode;
	TDirectTurnMode m_DirectTurnMode;
	char* m_DirectWalkAnim;
	char* m_DirectTurnAnim;
	float m_DirectWalkVelocity;
	float m_DirectTurnVelocity;

	int m_GoToTolerance;

	DECLARE_PERSISTENT(CAdActor3DX, CAdObject3D);
	bool m_TurningLeft;

	void InitLine3D(D3DXVECTOR3 StartPt, D3DXVECTOR3 EndPt, bool FirstStep);
	void GetNextStep3D();
	void FollowPath3D();

	void GetNextStep2D();
	void FollowPath2D();

	void GoTo3D(D3DXVECTOR3 TargetPos, float TargetAngle = -1.0f);
	void GoTo2D(int X, int Y, float TargetAngle = -1.0f);
	HRESULT TurnTo(float Angle);
	D3DXVECTOR3 m_TargetPoint3D;
	CBPoint* m_TargetPoint2D;
	float m_TargetAngle;
	//HRESULT LoadModel(char* Filename);
	virtual HRESULT Display();
	virtual HRESULT Update();
	//CBModel* m_Model;	
	CAdActor3DX(CBGame* inGame);
	virtual ~CAdActor3DX();	
	CAdPath3D* m_Path3D;
	CAdPath* m_Path2D;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	float DirToAngle(TDirection Dir);
	TDirection AngleToDir(float Angle);

	HRESULT UpdateAttachments();
	HRESULT DisplayAttachments(bool RegisterObjects);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

private:
	HRESULT ParseEffect(BYTE* Buffer);
	CBArray<CAdAttach3DX*, CAdAttach3DX*> m_Attachments;
	bool TurnToStep(float Velocity);
	bool PrepareTurn(float TargetAngle);
	HRESULT MergeAnimations(char* Filename);
	HRESULT MergeAnimations2(char* Filename);
	HRESULT UnloadAnimation(char* AnimName);
	bool IsGoToNeeded(int X, int Y);
};

#endif // !defined(AFX_AdActor3DX_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_)
