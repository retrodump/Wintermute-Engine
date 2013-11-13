// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSENTENCE_H__A6781CBC_C532_4512_8542_B8880438649F__INCLUDED_)
#define AFX_ADSENTENCE_H__A6781CBC_C532_4512_8542_B8880438649F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "dctypes.h"	// Added by ClassView

class CAdSentence : public CBBase  
{
public:	
	bool m_Freezable;
	bool m_FixedPos;
	CBSprite* m_CurrentSprite;
	char* m_CurrentSkelAnim;
	HRESULT Update(TDirection Dir=DI_DOWN);
	HRESULT SetupTalkFile(char* SoundFilename);
	DECLARE_PERSISTENT(CAdSentence, CBBase);
	HRESULT Finish();
	void SetSound(CBSound* Sound);
	bool m_SoundStarted;
	CBSound* m_Sound;
	TTextAlign m_Align;
	HRESULT Display();
	int m_Width;
	POINT m_Pos;
	CBFont* m_Font;
	char* GetNextStance();
	char* GetCurrentStance();
	void SetStances(char* Stances);
	void SetText(char* Text);
	int m_CurrentStance;
	DWORD m_StartTime;
	char* m_Stances;
	char* m_Text;	
	DWORD m_Duration;
	CAdSentence(CBGame* inGame);
	virtual ~CAdSentence();
	CAdTalkDef* m_TalkDef;
	bool CanSkip();

private:
	char* m_TempStance;
	char* GetStance(int Stance);
	
};

#endif // !defined(AFX_ADSENTENCE_H__A6781CBC_C532_4512_8542_B8880438649F__INCLUDED_)
