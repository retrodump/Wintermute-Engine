// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSoundWAV.h"


//////////////////////////////////////////////////////////////////////////
CBSoundWAV::CBSoundWAV(CBGame* inGame):CBSoundBuffer(inGame)
{
	m_WaveDataOffset = 0;
}


//////////////////////////////////////////////////////////////////////////
CBSoundWAV::~CBSoundWAV()
{

}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::InitializeBuffer(CBFile *File)
{
	if(!File) return E_FAIL;
	
	char chunk[4];
	DWORD size;

	File->Seek(0);

	File->Read(chunk, 4);
	if(strncmp(chunk, "RIFF", 4)!=0) return DSERR_BADFORMAT;
	File->Read(&size, sizeof(DWORD));

	File->Read(chunk, 4);
	if(strncmp(chunk, "WAVE", 4)!=0) return DSERR_BADFORMAT;

	File->Read(chunk, 4);
	if(strncmp(chunk, "fmt ", 4)!=0) return DSERR_BADFORMAT;
	File->Read(&size, sizeof(DWORD));

	LPWAVEFORMATEX header = (LPWAVEFORMATEX)new BYTE[size];
	File->Read(header, size);

	// skip 'fact' chunks
	size = 0;
	do{
		File->Seek(size, SEEK_TO_CURRENT);
		File->Read(chunk, 4);
		File->Read(&size, sizeof(DWORD));
	} while (strncmp(chunk, "data", 4)!=0);

	// now we should be at 'data' chunk
	m_WaveDataOffset = m_File->GetPos();
	m_TotalDataLength = size;


	HRESULT ret;

	if(header->wFormatTag == WAVE_FORMAT_PCM){
		memcpy(&m_Format, header, sizeof(PCMWAVEFORMAT));
		// create buffer
		ret = CreateSoundBuffer(m_TotalDataLength, (PCMWAVEFORMAT*)header);
	}
	else ret = DSERR_UNSUPPORTED;
	
	delete [] header;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::FillBuffer(CBFile *File)
{
	if(!m_SoundBuffer || !File || m_Streamed) return S_OK;

	LPVOID Data1, Data2;
	DWORD  Data1Size, Data2Size;
	HRESULT ret;
	
	ret = m_SoundBuffer->Lock(0, 0, &Data1, &Data1Size, &Data2, &Data2Size, DSBLOCK_ENTIREBUFFER);
	if(FAILED(ret)) return ret;

	
	File->Seek(m_WaveDataOffset);
	if(Data1Size>0)	File->Read(Data1, Data1Size);
	if(Data2Size>0) File->Read(Data2, Data2Size);
	
	ret = m_SoundBuffer->Unlock(Data1, Data1Size, Data2, Data2Size);
	if(FAILED(ret)) return ret;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::FillRawBuffer(CBFile *File)
{
	if(!File) return E_FAIL;

	SAFE_DELETE_ARRAY(m_RawBuffer);
	m_RawBuffer = new BYTE[m_TotalDataLength];
	if(!m_RawBuffer) return E_FAIL;

	File->Seek(m_WaveDataOffset);
	File->Read(m_RawBuffer, m_TotalDataLength);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::ResetStream()
{
	if(!m_File) return S_OK;

	m_File->Seek(m_WaveDataOffset);
	m_TotalDataRead = 0;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::LoadSegment(int Segment)
{
	LPVOID Data;
	DWORD  DataSize;
	
	HRESULT res = m_SoundBuffer->Lock(Segment*m_StreamBlockSize, m_StreamBlockSize, &Data, &DataSize, 0, 0, 0);
	if(FAILED(res)) return res;

	DWORD DataLeft = m_TotalDataLength - m_TotalDataRead;
	m_File->Read(Data, min(DataLeft, DataSize));
	m_TotalDataRead += min(DataLeft, DataSize);

	if(DataLeft<DataSize){
		if(m_Looping){
			if(m_LoopStart>0)
			{
				m_TotalDataRead = m_LoopStart * m_Format.wf.nBlockAlign;
				m_File->Seek(m_WaveDataOffset + m_TotalDataRead);
			}
			else ResetStream();			
			m_File->Read((BYTE*)Data+DataLeft, DataSize-DataLeft);
			m_TotalDataRead += DataSize-DataLeft;
		}
		else {
			memset((BYTE*)Data+DataLeft, 0, DataSize-DataLeft);
			if(m_LastStreamBlock==-1){
				m_LastStreamBlock = Segment;
				m_StopNow = false;
			}
		}
	}
	m_SoundBuffer->Unlock(Data, DataSize, 0, 0);
	
	return DS_OK;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundWAV::GetPosition()
{
	if(!m_SoundBuffer) return 0;

	DWORD PlayPos;
	m_SoundBuffer->GetCurrentPosition(&PlayPos, NULL);

	if(m_Streamed) return (m_TotalDataRead /*- (m_NumStreamBlocks * m_StreamBlockSize)*/) / m_Format.wf.nBlockAlign;
	else return PlayPos / m_Format.wf.nBlockAlign;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundWAV::SetPosition(DWORD Pos)
{
	if(!m_SoundBuffer) return E_FAIL;

	if(m_Streamed){
		if(m_File){
			m_TotalDataRead = Pos * m_Format.wf.nBlockAlign;
			m_File->Seek(m_WaveDataOffset + m_TotalDataRead);
			m_CurrentStreamBlock = 0;
			m_LastStreamBlock = -1;
			Update(true, true);			
		}
	}
	else{
		m_SoundBuffer->SetCurrentPosition(Pos * m_Format.wf.nBlockAlign);
	}

	return S_OK;
}
