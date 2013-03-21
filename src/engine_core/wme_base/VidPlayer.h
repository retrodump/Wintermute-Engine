// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_VIDPLAYER_H__57877A3B_2CC3_4462_820F_50647FCE59AF__INCLUDED_)
#define AFX_VIDPLAYER_H__57877A3B_2CC3_4462_820F_50647FCE59AF__INCLUDED_

#include "dctypes.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

#define MAX_AUDIO_STREAMS 5
#define MAX_VIDEO_STREAMS 5


class CVidPlayer : public CBBase  
{
public:
	bool m_ShowSubtitle;
	int m_CurrentSubtitle;
	HRESULT LoadSubtitles(char* Filename, char* SubtitleFile);
	bool m_SlowRendering;
	bool IsPlaying();
	char* m_Filename;
	HRESULT Stop();
	HRESULT Play(TVideoPlayback Type=VID_PLAY_CENTER, int X=0, int Y=0, bool FreezeMusic=true);
	DWORD m_TotalVideoTime;
	DWORD m_StartTime;
	CVidRenderer* m_VidRenderer;
	CBSoundAVI* m_Sound;
	bool m_SoundAvailable;
	HRESULT SetDefaults();
	bool m_Playing;
	HRESULT Display();
	HRESULT Update();
	HRESULT Initialize(char* inFilename, char* SubtitleFile=NULL);
	HRESULT Cleanup();
	CVidPlayer(CBGame* inGame);
	virtual ~CVidPlayer();

	PAVIFILE m_AviFile;

	LONG m_LastSample;

	PAVISTREAM m_AudioStream;
	PAVISTREAM m_VideoStream;

	LPWAVEFORMAT m_AudioFormat;

	LPBITMAPINFO m_VideoFormat;
	PGETFRAME m_VideoPGF;
	LONG m_VideoEndTime;

	int m_PlayPosX;
	int m_PlayPosY;
	float m_PlayZoom;

	LPBITMAPV4HEADER m_TargetFormat;

	CBArray<CVidSubtitle*, CVidSubtitle*> m_Subtitles;
};

#endif // !defined(AFX_VIDPLAYER_H__57877A3B_2CC3_4462_820F_50647FCE59AF__INCLUDED_)
