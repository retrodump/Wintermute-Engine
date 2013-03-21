// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BSoundTheora.h"


//////////////////////////////////////////////////////////////////////////
CBSoundTheora::CBSoundTheora(CBGame* inGame):CBSoundBuffer(inGame)
{
	m_Player = NULL;
	m_NoUpdate = true;
	m_NumBuffers = 0;
	m_LastPlayBlock = 0;
	m_CurrentTime = 0;
}


//////////////////////////////////////////////////////////////////////////
CBSoundTheora::~CBSoundTheora(void)
{
	m_Player = NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundTheora::InitializeBuffer(CVidTheoraPlayer* Player)
{
	if(!Player) return E_FAIL;

	SetStreaming(true);
	m_Type = SOUND_SFX;
	m_Looping = false;


	m_Format.wf.wFormatTag = WAVE_FORMAT_PCM;
	m_Format.wBitsPerSample = 16;
	m_Format.wf.nChannels = Player->m_VorbisInfo.channels;
	m_Format.wf.nSamplesPerSec = Player->m_VorbisInfo.rate;

	m_Format.wf.nBlockAlign = m_Format.wBitsPerSample / 8 * m_Format.wf.nChannels;
	m_Format.wf.nAvgBytesPerSec = m_Format.wf.nSamplesPerSec * m_Format.wf.nBlockAlign;

	//m_TotalDataLength=ov_pcm_total(&m_VorbisFile, -1) * m_Format.wf.nBlockAlign;

	m_TotalDataLength = 1000000; // TODO

	m_Player = Player;

	return CreateSoundBuffer(m_TotalDataLength, &m_Format);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundTheora::ResetStream()
{
	m_TotalDataRead = 0;
	m_NumBuffers = 0;
	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundTheora::FillBuffer(CBFile *File){

	return E_NOTIMPL;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundTheora::LoadSegment(int Segment)
{
	return E_NOTIMPL;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundTheora::GetPosition()
{
	if(!m_SoundBuffer) return 0;

	DWORD pos;
	HRESULT res = m_SoundBuffer->GetCurrentPosition(&pos, NULL);
	if(FAILED(res)) return 0;

	int PosBlock = pos / m_StreamBlockSize;

	if(m_LastPlayBlock > PosBlock) m_NumBuffers++;
	m_LastPlayBlock = PosBlock;


	m_CurrentTime = m_NumBuffers;
	m_CurrentTime = m_CurrentTime * m_NumStreamBlocks * m_StreamBlockSize;
	m_CurrentTime += pos;
	m_CurrentTime /= m_Format.wf.nBlockAlign;
	m_CurrentTime /= (m_Format.wf.nSamplesPerSec / 1000.0f);

	return m_CurrentTime;
}

//////////////////////////////////////////////////////////////////////////
bool CBSoundTheora::WriteBlock(BYTE* Buffer, DWORD Size)
{
	DWORD pos;
	HRESULT res = m_SoundBuffer->GetCurrentPosition(&pos, NULL);
	if(FAILED(res)) return false;

	int PosBlock = pos / m_StreamBlockSize;

	if(m_LastPlayBlock > PosBlock) m_NumBuffers++;
	m_LastPlayBlock = PosBlock;

	int CurrBlock = m_LastStreamBlock;
	CurrBlock++;
	if(CurrBlock >= m_NumStreamBlocks) CurrBlock = 0;
	
	if(m_LastStreamBlock==-1 || PosBlock!=CurrBlock)
	{
		LPVOID Data;
		DWORD  DataSize;
		HRESULT res = m_SoundBuffer->Lock(CurrBlock*m_StreamBlockSize, m_StreamBlockSize, &Data, &DataSize, 0, 0, 0);
		if(SUCCEEDED(res))
		{
			memcpy((BYTE*)Data, (BYTE*)Buffer, Size);
			m_SoundBuffer->Unlock(Data, Size, NULL, NULL);
		}
		
		m_LastStreamBlock = CurrBlock;
		return true;
	}
	else return false;
}
