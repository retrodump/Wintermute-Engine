// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADACTOR_H__353D6206_3433_4565_9261_7A1C2F0F8ED9__INCLUDED_)
#define AFX_ADACTOR_H__353D6206_3433_4565_9261_7A1C2F0F8ED9__INCLUDED_

#include "dctypes.h"	// Added by ClassView
#include "AdTypes.h"	// Added by ClassView
#include "BPoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAdActor : public CAdTalkHolder
{
public:	
	TDirection AngleToDirection(int Angle);
	DECLARE_PERSISTENT(CAdActor, CAdTalkHolder);	
	virtual int GetHeight();
	CBSprite* GetTalkStance(char* Stance);
	virtual void GoTo(int X, int Y, TDirection AfterWalkDir=DI_NONE);
	CBPoint* m_TargetPoint;
	virtual HRESULT Update();
	virtual HRESULT Display();
	TDirection m_TargetDir;
	TDirection m_AfterWalkDir;
	virtual void TurnTo(TDirection dir);
	CAdPath* m_Path;
	CAdSpriteSet* m_WalkSprite;
	CAdSpriteSet* m_StandSprite;
	CAdSpriteSet* m_TurnLeftSprite;
	CAdSpriteSet* m_TurnRightSprite;
	CBArray<CAdSpriteSet*, CAdSpriteSet*> m_TalkSprites;
	CBArray<CAdSpriteSet*, CAdSpriteSet*> m_TalkSpritesEx;
	TDirection m_Dir;
	CAdActor(CBGame* inGame/*=NULL*/);
	virtual ~CAdActor();
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);

	// new anim system
	char* m_TalkAnimName;
	char* m_IdleAnimName;
	char* m_WalkAnimName;
	char* m_TurnLeftAnimName;
	char* m_TurnRightAnimName;
	CBArray<CAdSpriteSet*, CAdSpriteSet*> m_Anims;
	virtual HRESULT PlayAnim(char* Filename);
	CAdSpriteSet* GetAnimByName(char* AnimName);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

private:
	HRESULT SetDefaultAnimNames();	
	CBSprite* GetTalkStanceOld(char *Stance);
	HRESULT MergeAnims(char* AnimsFilename);
	CBSprite* m_AnimSprite2;

	void InitLine(CBPoint StartPt, CBPoint EndPt);
	void GetNextStep();
	void FollowPath();				
	double m_PFStepX;
	double m_PFStepY;
	double m_PFX;
	double m_PFY;
	int m_PFCount;
};

#endif // !defined(AFX_ADACTOR_H__353D6206_3433_4565_9261_7A1C2F0F8ED9__INCLUDED_)
