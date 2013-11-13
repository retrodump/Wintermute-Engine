// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSoundAVI.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CBSoundAVI::CBSoundAVI(CBGame* inGame):CBSoundBuffer(inGame)
{
	m_AudioStream = NULL;
}


//////////////////////////////////////////////////////////////////////////
CBSoundAVI::~CBSoundAVI()
{
	m_AudioStream = NULL; // ref only
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundAVI::InitializeBuffer(PAVISTREAM Stream)
{
	if(!Stream) return E_FAIL;

	SetStreaming(true);
	m_Type = SOUND_SFX;
	m_Looping = false;

	m_AudioStream = Stream;

	LONG FormatSize;
	if(AVIStreamReadFormat(m_AudioStream, 0, NULL, &FormatSize)!=0) return E_FAIL;

	LPWAVEFORMAT Format = (LPWAVEFORMAT)new BYTE[FormatSize];
	if(AVIStreamReadFormat(m_AudioStream, 0, Format, &FormatSize)!=0){
		delete [] (BYTE*)Format;
		return E_FAIL;
	}
	
	m_TotalDataLength = AVIStreamLength(m_AudioStream) * Format->nBlockAlign;


	HRESULT ret;

	memcpy(&m_Format, Format, sizeof(PCMWAVEFORMAT));
	m_Format.wf.wFormatTag = WAVE_FORMAT_PCM;
	
	// create buffer
	ret = CreateSoundBuffer(m_TotalDataLength, (PCMWAVEFORMAT*)Format);

	
	delete [] (BYTE*)Format;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundAVI::ResetStream()
{
	m_TotalDataRead = 0;
	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundAVI::FillBuffer(CBFile *File){
	
	return E_NOTIMPL;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundAVI::LoadSegment(int Segment)
{
	LPVOID Data;
	DWORD  DataSize;
	
	HRESULT res = m_SoundBuffer->Lock(Segment*m_StreamBlockSize, m_StreamBlockSize, &Data, &DataSize, 0, 0, 0);
	if(FAILED(res)) return res;

	DWORD DataLeft = m_TotalDataLength - m_TotalDataRead;
	AVIStreamRead(m_AudioStream, m_TotalDataRead / m_Format.wf.nBlockAlign, min(DataLeft, DataSize)/m_Format.wf.nBlockAlign, Data, min(DataLeft, DataSize), NULL, NULL);
	m_TotalDataRead += min(DataLeft, DataSize);

	if(DataLeft<DataSize){
		memset((BYTE*)Data+DataLeft, 0, DataSize-DataLeft);
		if(m_LastStreamBlock==-1){
			m_LastStreamBlock = Segment;
			m_StopNow = false;
		}
	}
	m_SoundBuffer->Unlock(Data, DataSize, 0, 0);
	
	return DS_OK;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundAVI::GetPosition()
{
	if(!m_SoundBuffer) return 0;

	DWORD PlayPos;
	m_SoundBuffer->GetCurrentPosition(&PlayPos, NULL);

	if(m_Streamed) return (m_TotalDataRead - (m_NumStreamBlocks * m_StreamBlockSize)) / m_Format.wf.nBlockAlign;
	else return PlayPos / m_Format.wf.nBlockAlign;
}
