// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "sapi.h"

class CBObject;

class CBAccessMgr : public CBBase
{
public:
	CBAccessMgr(CBGame* inGame);
	virtual ~CBAccessMgr(void);
	
	bool m_TTSAvailable;
	HRESULT Initialize();
	HRESULT Speak(char* Str, TTTSType Type);	
	HRESULT Stop();	
	CBObject* GetNextObject();
	CBObject* GetPrevObject();
	
	HRESULT InitLoop();
	HRESULT DisplayBeforeGUI();
	HRESULT DisplayAfterGUI();	
	HRESULT SetHintRect(RECT* HintRect=NULL, bool AfterGUI=false);
	HRESULT SetActiveObject(CBObject* ActiveObj=NULL);
	CBObject* GetActiveObject();

private:
	TTTSType m_PlayingType;
	HRESULT DisplayInternal();
	ISpVoice* m_Voice;
	CBObject* m_ActiveObject;
	CBObject* m_PrevActiveObject;
	RECT m_HintRect;
	bool m_HintAfterGUI;
};
