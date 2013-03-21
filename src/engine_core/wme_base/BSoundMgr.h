// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUNDMGR_H__19F2AEDD_A516_405F_8938_5A752305F37A__INCLUDED_)
#define AFX_BSOUNDMGR_H__19F2AEDD_A516_405F_8938_5A752305F37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBSoundMgr : public CBBase  
{
public:	
	LONG PosToPan(int X, int Y);
	HRESULT ResumeAll();
	HRESULT PauseAll(bool IncludingMusic=true);
	HRESULT Cleanup();
	//DECLARE_PERSISTENT(CBSoundMgr, CBBase);
	BYTE GetMasterVolumePercent();
	HRESULT SetMasterVolumePercent(BYTE Percent);
	BYTE GetVolumePercent(TSoundType Type);
	HRESULT SetVolumePercent(TSoundType Type, BYTE Percent);
	HRESULT SetVolume(TSoundType Type, int Volume);
	HRESULT SetVolumeWave(DWORD Volume);
	DWORD m_VolumeOriginal;
	DWORD m_VolumeWave;
	int m_VolumeMusic;
	int m_VolumeSpeech;
	int m_VolumeSFX;
	HRESULT RemoveSound(CBSoundBuffer* Sound);
	CBSoundBuffer* AddSound(char* Filename, TSoundType Type=SOUND_SFX, bool Streamed=false);
	HRESULT AddSound(CBSoundBuffer* Sound, TSoundType Type=SOUND_SFX);
	HRESULT InitLoop();
	HRESULT Initialize(HWND Window, CHWManager* hwManager);
	bool m_SoundAvailable;
	CBSoundMgr(CBGame* inGame);
	virtual ~CBSoundMgr();
	LPDIRECTSOUND8 m_DirectSound;
	CBArray<CBSoundBuffer*, CBSoundBuffer*> m_Sounds;
};

#endif // !defined(AFX_BSOUNDMGR_H__19F2AEDD_A516_405F_8938_5A752305F37A__INCLUDED_)
