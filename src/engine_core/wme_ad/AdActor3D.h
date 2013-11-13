// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADACTOR3D_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_)
#define AFX_ADACTOR3D_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdObject3D.h"

class CAdActor3D : public CAdObject3D
{
public:
	virtual void Talk(char* Text, char* Sound=NULL, DWORD Duration=0, char* Stances=NULL, TTextAlign Align=TAL_CENTER);
	virtual int GetHeight();
	HRESULT PlayAnim(char* Name);
	HRESULT PlayAnim(int Channel, char *Name);

	float m_AfterWalkAngle;
	char* m_TalkAnimName;
	char* m_IdleAnimName;
	char* m_WalkAnimName;

	TDirectWalkMode m_DirectWalkMode;
	TDirectTurnMode m_DirectTurnMode;
	char* m_DirectWalkAnim;
	char* m_DirectTurnAnim;
	float m_DirectWalkVelocity;
	float m_DirectTurnVelocity;

	DECLARE_PERSISTENT(CAdActor3D, CAdObject3D);
	bool m_TurningLeft;
	void InitLine(D3DXVECTOR3 StartPt, D3DXVECTOR3 EndPt, bool FirstStep);
	void GetNextStep();
	void FollowPath();
	void GoTo(D3DXVECTOR3 TargetPos, float TargetAngle = -1.0f);
	HRESULT TurnTo(float Angle);
	D3DXVECTOR3 m_TargetPoint;
	float m_TargetAngle;
	//HRESULT LoadModel(char* Filename);
	virtual HRESULT Display();
	virtual HRESULT Update();
	//CBModel* m_Model;	
	CAdActor3D(CBGame* inGame);
	virtual ~CAdActor3D();	
	CAdPath3D* m_Path;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	float DirToAngle(TDirection Dir);
	TDirection AngleToDir(float Angle);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

	static CAdObject3D* CreateFromFile(CBGame* Game, char* Filename);

private:
	static bool IsActorX(CBGame* Game, char* Filename);
	bool TurnToStep(float Velocity);
	bool PrepareTurn(float TargetAngle);
};

#endif // !defined(AFX_ADACTOR3D_H__71F35861_1DD2_41C6_8C2B_5280AF057EBA__INCLUDED_)
