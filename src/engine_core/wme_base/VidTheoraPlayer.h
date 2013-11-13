// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include <theora/theora.h>

class CVidTheoraPlayer : public CBBase
{
private:
	enum { THEORA_STATE_NONE=0, THEORA_STATE_PLAYING=1, THEORA_STATE_PAUSED=2, THEORA_STATE_FINISHED=3 };
public:
	DECLARE_PERSISTENT(CVidTheoraPlayer, CBBase);

	CVidTheoraPlayer(CBGame* inGame);
	virtual ~CVidTheoraPlayer(void);

	// Vorbis/Theora structs
	ogg_sync_state   m_OggSyncState;
	ogg_page         m_OggPage;
	ogg_stream_state m_VorbisStreamState;
	ogg_stream_state m_TheoraStreamState;
	
	theora_info      m_TheoraInfo;
	theora_comment   m_TheoraComment;
	theora_state     m_TheoraState;
	
	vorbis_info      m_VorbisInfo;
	vorbis_dsp_state m_VorbisDSPState;
	vorbis_block     m_VorbisBlock;
	vorbis_comment   m_VorbisComment;

	int m_TheoraStreams;
	int m_VorbisStreams;

	ogg_int64_t m_AudiobufGranulepos; //time position of last sample


	// external objects
	CBFile* m_File;
	char* m_Filename;

	CBSoundTheora* m_Sound;
	ogg_int16_t* m_AudioBuf;
	int m_AudioBufSize;
	int m_AudioBufFill;

	CBSurface* m_Texture;
	CVidSubtitler* m_Subtitler;

	// control methods
	HRESULT Initialize(char* Filename, char* SubtitleFile=NULL);
	HRESULT InitializeSimple();
	HRESULT Update();
	HRESULT Play(TVideoPlayback Type=VID_PLAY_CENTER, int X=0, int Y=0, bool FreezeGame=false, bool FreezeMusic=true, bool Looping=false, DWORD StartTime=0, float ForceZoom=-1.0f, int Volume=-1);
	HRESULT Stop();
	HRESULT Display(DWORD Alpha=0xFFFFFFFF);
	HRESULT RenderFrame(CBSurface* Texture, yuv_buffer* yuv);

	HRESULT Pause();
	HRESULT Resume();

	bool IsPlaying()  { return m_State==THEORA_STATE_PLAYING;  };
	bool IsFinished() { return m_State==THEORA_STATE_FINISHED; };
	bool IsPaused()   { return m_State==THEORA_STATE_PAUSED;   };

	float GetMovieTime();
	int GetMovieFrame();

	CBSurface* GetTexture();

	int m_State;
	DWORD m_StartTime;

	int m_SavedState;
	DWORD m_SavedPos;


	// alpha related
	CBImage* m_AlphaImage;
	char* m_AlphaFilename;
	HRESULT SetAlphaImage(char* Filename);
	__inline BYTE GetAlphaAt(int X, int Y);

	HRESULT SeekToTime(DWORD Time);
	
	
	void Cleanup();
	HRESULT ResetStream();

	// video properties
	TVideoPlayback m_PlaybackType;
	int m_PosX;
	int m_PosY;
	float m_PlayZoom;
	int m_Volume;

	bool m_Looping;
	bool m_DontDropFrames;
	bool m_FreezeGame;
	DWORD m_CurrentTime;


private:
	// data streaming
	int BufferData(ogg_sync_state* OggSyncState);
	int StreamInData();


	// lookup tables
	unsigned int m_YTable[256];
	unsigned int m_BUTable[256];
	unsigned int m_GUTable[256];
	unsigned int m_GVTable[256];
	unsigned int m_RVTable[256];

	void GenLookupTables();


	// seeking support	
	bool m_SeekingKeyframe;
	float m_TimeOffset;

	bool m_FrameRendered;


	// decoding
	void DecodeVorbis();
	void DecodeTheora();

	bool m_AudioFrameReady;
	bool m_VideoFrameReady;
	float m_VideobufTime;

	HRESULT WriteAudio();
	HRESULT WriteVideo();

	bool m_PlaybackStarted;	

	// helpers
	void SetDefaults();

};
