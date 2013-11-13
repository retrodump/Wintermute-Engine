// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BSOUNDBUFFER_H__BD3339E6_82D2_480D_8633_34638856ADDE__INCLUDED_)
#define AFX_BSOUNDBUFFER_H__BD3339E6_82D2_480D_8633_34638856ADDE__INCLUDED_

#include "dctypes.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CBSoundBuffer : public CBBase  
{
public:
	bool m_NoUpdate;
	bool m_FreezePaused;
	DWORD m_LoopStart;
	HRESULT GetRawData(DWORD Time, DWORD* LeftChannel=NULL, DWORD* RightChannel=NULL);
	BYTE* m_RawBuffer;
	HRESULT SetPan(LONG Pan);
	HRESULT SetPrivateVolume(int Volume);
	virtual DWORD GetPosition();
	virtual HRESULT SetPosition(DWORD Pos);
	bool IsPlaying();
	HRESULT SetVolume(int Volume);
	void SetType(TSoundType Type);
	TSoundType m_Type;
	DWORD GetLength();
	virtual HRESULT ResetStream() { return E_FAIL; };
	DWORD m_TotalDataLength;
	DWORD m_TotalDataRead;
	bool m_Looping;
	bool m_StopNow;
	DWORD m_CurrentStreamBlock;
	PCMWAVEFORMAT m_Format;
	int m_LastStreamBlock;
	HRESULT Restore(bool* Restored);
	HRESULT Pause();
	HRESULT Play(bool Looping=false, DWORD StartSample=0);
	HRESULT Resume();
	DWORD m_StreamBlockSize;
	DWORD m_NumStreamBlocks;
	virtual HRESULT FillBuffer(CBFile* File) { return E_FAIL; };
	virtual HRESULT FillRawBuffer(CBFile* File) { return E_FAIL; };
	virtual HRESULT InitializeBuffer(CBFile* File) { return E_FAIL; };
	CBFile* m_File;
	char* m_Filename;
	bool m_Streamed;
	int m_PrivateVolume;
	HRESULT Stop();
	virtual HRESULT LoadFromFile(char* Filename, bool ForceReload=false);
	void SetStreaming(bool Streamed, DWORD NumBlocks=0, DWORD BlockSize=0);
	HRESULT Update(bool FillAll=false, bool NoReset=false);
	CBSoundBuffer(CBGame* inGame);
	virtual ~CBSoundBuffer();
	LPDIRECTSOUNDBUFFER m_SoundBuffer;
	HRESULT CreateSoundBuffer(DWORD BufSize, PCMWAVEFORMAT *Format, bool AdoptFormat=false);
	virtual HRESULT LoadSegment(int Segment) { return E_FAIL; };
	HRESULT ApplyFX(TSFXType Type, float Param1, float Param2, float Param3, float Param4);
	DWORD m_BufferSize;
};

#endif // !defined(AFX_BSOUNDBUFFER_H__BD3339E6_82D2_480D_8633_34638856ADDE__INCLUDED_)
