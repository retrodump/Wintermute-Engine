// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUND_H__F0DC68A8_2AE1_4D27_A594_049AE9FC20E4__INCLUDED_)
#define AFX_BSOUND_H__F0DC68A8_2AE1_4D27_A594_049AE9FC20E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "dctypes.h"	// Added by ClassView

class CBSound : public CBBase  
{
public:
	HRESULT GetRawData(DWORD Time, int* LeftChannel=NULL, int* RightChannel=NULL);
	HRESULT LoadRawData();
	HRESULT SetPan(LONG Pan);
	int m_SoundPrivateVolume;
	int GetVolume();
	HRESULT SetVolume(int Volume);
	HRESULT SetPrivateVolume(int Volume);
	HRESULT SetLoopStart(DWORD Pos);
	DWORD GetPositionTime();
	HRESULT SetPositionTime(DWORD Time);
	bool m_SoundPaused;
	bool m_SoundFreezePaused;
	bool IsPlaying();
	bool IsPaused();
	bool m_SoundPlaying;
	bool m_SoundLooping;
	DWORD m_SoundLoopStart;
	DWORD m_SoundPosition;
	DECLARE_PERSISTENT(CBSound, CBBase);
	HRESULT Resume();
	HRESULT Pause(bool FreezePaused = false);
	HRESULT Stop();
	HRESULT Play(bool Looping=false);
	DWORD GetLength();
	bool m_SoundStreamed;
	TSoundType m_SoundType;
	char* m_SoundFilename;
	HRESULT SetSoundSimple();
	HRESULT SetSound(char* Filename, TSoundType Type=SOUND_SFX, bool Streamed=false, DWORD initialPrivateVolume=100);	
	CBSound(CBGame* inGame);
	virtual ~CBSound();

	HRESULT ApplyFX(TSFXType Type=SFX_NONE, float Param1=0, float Param2=0, float Param3=0, float Param4=0);

	TSFXType m_SFXType;
	float m_SFXParam1;
	float m_SFXParam2;
	float m_SFXParam3;
	float m_SFXParam4;

private:
	CBSoundBuffer* m_Sound;

};

#endif // !defined(AFX_BSOUND_H__F0DC68A8_2AE1_4D27_A594_049AE9FC20E4__INCLUDED_)
