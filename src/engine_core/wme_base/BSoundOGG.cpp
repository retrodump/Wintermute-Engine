// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSoundOGG.h"

#ifdef _DEBUG
	#pragma comment(lib, "libogg_d.lib")
	#pragma comment(lib, "libvorbis_d.lib")
	#pragma comment(lib, "libvorbisfile_d.lib")
#else
	#pragma comment(lib, "libogg.lib")
	#pragma comment(lib, "libvorbis.lib")
	#pragma comment(lib, "libvorbisfile.lib")
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CBSoundOGG::CBSoundOGG(CBGame* inGame):CBSoundBuffer(inGame)
{	
	m_OGGOpen = false;
}


//////////////////////////////////////////////////////////////////////////
CBSoundOGG::~CBSoundOGG()
{
	OGGClose();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::InitializeBuffer(CBFile *File)
{
	if(!File) return E_FAIL;

	HRESULT res = OGGOpen(File);
	if(FAILED(res)) return res;

	return CreateSoundBuffer(m_TotalDataLength, &m_Format);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::FillBuffer(CBFile *File)
{
	if(!m_SoundBuffer || !File || m_Streamed) return S_OK;

	LPVOID Data1, Data2;
	DWORD  Data1Size, Data2Size;
	HRESULT ret;
	ret = OGGOpen(File);
	if(FAILED(ret)) return ret;
	
	ret = m_SoundBuffer->Lock(0, 0, &Data1, &Data1Size, &Data2, &Data2Size, DSBLOCK_ENTIREBUFFER);
	if(FAILED(ret)) return ret;

	if(Data1Size>0) OGGReadBlock(Data1, Data1Size);
	if(Data2Size>0) OGGReadBlock(Data2, Data2Size);
	
	ret = m_SoundBuffer->Unlock(Data1, Data1Size, Data2, Data2Size);
	if(FAILED(ret)) return ret;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::FillRawBuffer(CBFile *File)
{
	if(!File) return E_FAIL;

	SAFE_DELETE_ARRAY(m_RawBuffer);
	m_RawBuffer = new BYTE[m_TotalDataLength];
	if(!m_RawBuffer) return E_FAIL;

	HRESULT ret;
	ret = OGGOpen(File);
	if(FAILED(ret)){
		SAFE_DELETE_ARRAY(m_RawBuffer);
		return ret;	
	}

	return OGGReadBlock(m_RawBuffer, m_TotalDataLength);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::ResetStream()
{
	if(!m_File) return S_OK;
	m_TotalDataRead = 0;
	return OGGOpen(m_File);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::LoadSegment(int Segment)
{
	LPVOID Data;
	DWORD  DataSize;
	
	HRESULT res = m_SoundBuffer->Lock(Segment*m_StreamBlockSize, m_StreamBlockSize, &Data, &DataSize, 0, 0, 0);
	if(FAILED(res)) return res;

	DWORD DataLeft = m_TotalDataLength - m_TotalDataRead;
	OGGReadBlock(Data, min(DataLeft, DataSize));
	m_TotalDataRead += min(DataLeft, DataSize);

	if(DataLeft<DataSize){
		if(m_Looping){
			if(m_LoopStart)
			{
				ov_pcm_seek(&m_VorbisFile, m_LoopStart);
				m_TotalDataRead = m_LoopStart * m_Format.wf.nBlockAlign;
			}
			else ResetStream();
			OGGReadBlock((BYTE*)Data+DataLeft, DataSize-DataLeft);
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
// low level OGG functions
//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::OGGOpen(CBFile *File)
{
	if(!File) return E_FAIL;
	OGGClose();

	ov_callbacks callbacks;
	callbacks.read_func  = CBSoundOGG::FileRead;
	callbacks.seek_func  = CBSoundOGG::FileSeek;
	callbacks.close_func = CBSoundOGG::FileClose;
	callbacks.tell_func  = CBSoundOGG::FileTell;

	File->Seek(0);

	// open ogg vorbis stream
	int result = ov_open_callbacks(File, &m_VorbisFile, NULL, 0, callbacks);
	if (result!=0)
	{
		Game->LOG(0, "ov_open_callbacks failed, result=%d", result);
		return E_FAIL;
	}
	
	// calculate stream type
	vorbis_info* vi = ov_info(&m_VorbisFile, -1);
	if(!vi){
		Game->LOG(0, "ov_info failed");
		return E_FAIL;
	}

	m_Format.wf.wFormatTag = WAVE_FORMAT_PCM;
	m_Format.wBitsPerSample = 16;
	m_Format.wf.nChannels = vi->channels;
	m_Format.wf.nSamplesPerSec = vi->rate;
	
	m_Format.wf.nBlockAlign = m_Format.wBitsPerSample / 8 * m_Format.wf.nChannels;
	m_Format.wf.nAvgBytesPerSec = m_Format.wf.nSamplesPerSec * m_Format.wf.nBlockAlign;

	m_TotalDataLength=ov_pcm_total(&m_VorbisFile, -1) * m_Format.wf.nBlockAlign;

	m_OGGOpen = true;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::OGGClose()
{
	if(m_OGGOpen) ov_clear(&m_VorbisFile);
	m_OGGOpen = false;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::OGGReadBlock(void *Buffer, DWORD Size)
{
	DWORD DataRead = 0;
	bool eof = false;
	int bitstream;
	while(DataRead<Size && !eof){
		int result = ov_read(&m_VorbisFile, (char*)Buffer+DataRead, Size-DataRead, 0, 2, 1, &bitstream);
		if(result==0) eof = true;
		else if(result > 0)	DataRead+=result;
	}	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundOGG::GetPosition()
{
	if(m_OGGOpen && m_SoundBuffer){

		DWORD PlayPos;
		m_SoundBuffer->GetCurrentPosition(&PlayPos, NULL);
		
		if(m_Streamed){			
			return ov_pcm_tell(&m_VorbisFile) /*- (m_NumStreamBlocks * m_StreamBlockSize) / m_Format.wf.nBlockAlign*/;
		}
		else{
			return PlayPos / m_Format.wf.nBlockAlign;
		}
	}
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundOGG::SetPosition(DWORD Pos)
{
	if(!m_OGGOpen) return E_FAIL;

	if(m_Streamed){
		if(!ov_pcm_seek(&m_VorbisFile, Pos)){
			m_TotalDataRead = Pos * m_Format.wf.nBlockAlign;
			m_CurrentStreamBlock = 0;
			m_LastStreamBlock = -1;
			Update(true, true);
		}
		return S_OK;
	}
	else{
		if(m_SoundBuffer) m_SoundBuffer->SetCurrentPosition(Pos * m_Format.wf.nBlockAlign);
	}

	return S_OK;
}





//////////////////////////////////////////////////////////////////////////
// OGG -> CBFile interface
//////////////////////////////////////////////////////////////////////////
size_t CBSoundOGG::FileRead(void* ptr, size_t size, size_t nmemb, void* datasource)
{
	CBFile* file = (CBFile*)datasource;

	int BytesRead = min(size*nmemb, file->GetSize() - file->GetPos());
	file->Read(ptr, BytesRead);
	return (BytesRead / size);
}


//////////////////////////////////////////////////////////////////////////
int CBSoundOGG::FileSeek(void* datasource, int64_t offset, int whence)
{
	CBFile* file = (CBFile*)datasource;

	if(FAILED(file->Seek(offset, (TSeek)whence))) return 1;
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
int CBSoundOGG::FileClose(void* datasource)
{

	return 1; // we handle closing ourselves
}


//////////////////////////////////////////////////////////////////////////
long CBSoundOGG::FileTell(void* datasource)
{
	CBFile* file = (CBFile*)datasource;
	return file->GetPos();
}
