// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSoundBuffer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_NONSTREAMED_FILE_SIZE 1024*1024

//////////////////////////////////////////////////////////////////////////
CBSoundBuffer::CBSoundBuffer(CBGame* inGame):CBBase(inGame)
{
	m_SoundBuffer = NULL;
	m_Streamed = false;
	m_Filename = NULL;
	m_File = NULL;
	m_PrivateVolume = 100;

	m_StreamBlockSize = 0;
	m_NumStreamBlocks = 0;
	m_CurrentStreamBlock = 0;
	m_LastStreamBlock = -1;

	m_StopNow = false;
	m_Looping = false;
	m_LoopStart = 0;

	m_TotalDataLength = m_TotalDataRead = 0;

	memset(&m_Format, 0, sizeof(m_Format));

	m_Type = SOUND_SFX;

	m_RawBuffer = NULL;
	m_FreezePaused = false;

	m_BufferSize = 0;
	m_NoUpdate = false;
}


//////////////////////////////////////////////////////////////////////////
CBSoundBuffer::~CBSoundBuffer()
{
	Stop();

	if(m_File){
		Game->m_FileManager->CloseFile(m_File);
		m_File = NULL;
	}

	RELEASE(m_SoundBuffer);
	SAFE_DELETE_ARRAY(m_Filename);
	SAFE_DELETE_ARRAY(m_RawBuffer);
}


//////////////////////////////////////////////////////////////////////////
void CBSoundBuffer::SetStreaming(bool Streamed, DWORD NumBlocks, DWORD BlockSize)
{
	m_Streamed = Streamed;

	m_NumStreamBlocks = m_Streamed?NumBlocks:0;
	m_StreamBlockSize = m_Streamed?BlockSize:0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::LoadFromFile(char *Filename, bool ForceReload)
{
	HRESULT res;
	bool NewlyCreated = false;

	if(!m_SoundBuffer || ForceReload || m_Streamed){
		if(!m_File) m_File = Game->m_FileManager->OpenFile(Filename);
		if(!m_File){
			Game->LOG(0, "Error opening sound file '%s'", Filename);
			return E_FAIL;
		}
		// switch to streamed for big files
		if(!m_Streamed && (m_File->GetSize() > MAX_NONSTREAMED_FILE_SIZE && !Game->m_ForceNonStreamedSounds)) SetStreaming(true);
	}

	// create buffer
	if(!m_SoundBuffer){
		NewlyCreated = true;
		
		res = InitializeBuffer(m_File);
		if(FAILED(res)){
			Game->LOG(res, "Error creating sound buffer for file '%s'", Filename);
			return res;
		}
	}


	// fill buffer
	if(NewlyCreated || ForceReload){
		res = Restore(NULL);
		if(FAILED(res)) return res;

		if(!m_Streamed) res = FillBuffer(m_File);
		else res = Update(true);

		if(FAILED(res)){
			Game->LOG(res, "Error loading sound data from file '%s'", Filename);
			return res;
		}
	}


	// store filename
	if(!m_Filename){
		m_Filename = new char[strlen(Filename)+1];
		strcpy(m_Filename, Filename);
	}

	// close file (if not streaming)
	if(!m_Streamed && m_File){
		Game->m_FileManager->CloseFile(m_File);
		m_File = NULL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::CreateSoundBuffer(DWORD BufSize, PCMWAVEFORMAT *Format, bool AdoptFormat)
{
	if(AdoptFormat){
		memcpy(&m_Format, Format, sizeof(PCMWAVEFORMAT));
		m_Format.wf.wFormatTag = WAVE_FORMAT_PCM;
	}

	// set defaults for streamed buffer
	if(m_Streamed)
	{
		if(m_NumStreamBlocks==0) m_NumStreamBlocks = 16;
		if(m_StreamBlockSize==0)
		{
			m_StreamBlockSize = Format->wf.nSamplesPerSec * Game->m_SoundBufferSizeSec * Format->wf.nBlockAlign / m_NumStreamBlocks;
			m_StreamBlockSize -= m_StreamBlockSize % Format->wf.nBlockAlign;
		}

		if(m_StreamBlockSize > BufSize) SetStreaming(false); // no need to stream
	}

	DSBUFFERDESC dsbdesc;
	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	
	dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
	dsbdesc.dwBufferBytes = m_Streamed?m_StreamBlockSize*m_NumStreamBlocks:BufSize; 
	dsbdesc.lpwfxFormat   = (LPWAVEFORMATEX)Format;

	m_BufferSize = dsbdesc.dwBufferBytes;
	
	if(m_Streamed) dsbdesc.dwFlags=DSBCAPS_GETCURRENTPOSITION2/*|DSBCAPS_CTRLPOSITIONNOTIFY*/|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;
	else dsbdesc.dwFlags=DSBCAPS_GETCURRENTPOSITION2|/*DSBCAPS_STATIC|*/DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;

	DWORD OrigFlags = dsbdesc.dwFlags;

	dsbdesc.dwFlags |= DSBCAPS_CTRLFX;	
	HRESULT Ret = Game->m_SoundMgr->m_DirectSound->CreateSoundBuffer(&dsbdesc, &m_SoundBuffer, NULL);

	// if the buffer's too small, disable SFX
	if(Ret==DSERR_BUFFERTOOSMALL)
	//if(FAILED(Ret))
	{
		dsbdesc.dwFlags = OrigFlags;
		Ret = Game->m_SoundMgr->m_DirectSound->CreateSoundBuffer(&dsbdesc, &m_SoundBuffer, NULL);
	}
	
	//Game->QuickMessageForm("%s %d %s", m_Filename, BufSize, m_Streamed?"yes":"no");

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Play(bool Looping, DWORD StartSample)
{
	if(!m_SoundBuffer) return S_OK;

	m_Looping = Looping;

	bool Restored;
	HRESULT res = Restore(&Restored);
	if(FAILED(res)) return res;

	if(Restored){
		res = LoadFromFile(m_Filename, true);
		if(FAILED(res)) return res;
	}

	DWORD Flags;
	res = Stop();
	if(FAILED(res)) return res;


	if(m_Streamed) Update(true);
	if(StartSample) SetPosition(StartSample);


	if(m_Streamed || m_Looping) Flags = DSBPLAY_LOOPING;
	else Flags = 0;

	return m_SoundBuffer->Play(0, 0, Flags);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Resume()
{
	if(!m_SoundBuffer) return S_OK;

	bool Restored;
	HRESULT res = Restore(&Restored);
	if(FAILED(res)) return res;

	if(Restored){
		res = LoadFromFile(m_Filename, true);
		if(FAILED(res)) return res;
	}

	DWORD Flags;

	if(m_Streamed || m_Looping) Flags = DSBPLAY_LOOPING;
	else Flags = 0;

	return m_SoundBuffer->Play(0, 0, Flags);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Stop()
{
	if(!m_SoundBuffer) return S_OK;

	HRESULT ret = m_SoundBuffer->Stop();
	if(FAILED(ret)) return ret;

	m_StopNow = false;

	if(!m_NoUpdate)
	{
		m_LastStreamBlock = -1;
		m_CurrentStreamBlock = 0;
	}

	return m_SoundBuffer->SetCurrentPosition(0);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Pause()
{
	if(!m_SoundBuffer) return S_OK;
	return m_SoundBuffer->Stop();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Restore(bool *Restored)
{
	if(!m_SoundBuffer) return S_OK;

    HRESULT res;

    if(Restored) *Restored = false;

    DWORD Status;
    if(FAILED(res = m_SoundBuffer->GetStatus(&Status))) return res;

    if(Status & DSBSTATUS_BUFFERLOST){
        do{
            res = m_SoundBuffer->Restore();
            if(res==DSERR_BUFFERLOST) Sleep(10);
        } while(res = m_SoundBuffer->Restore());

        if(Restored) *Restored = true;

        return S_OK;
    }
    else return S_FALSE;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::Update(bool FillAll, bool NoReset)
{
	if(m_NoUpdate) return S_OK;
	if(!m_SoundBuffer || !m_Streamed) return S_OK;

	DWORD pos;
	HRESULT res = m_SoundBuffer->GetCurrentPosition(&pos, NULL);
	if(FAILED(res)) return res;
	
	pos/=m_StreamBlockSize;

	if(pos==m_LastStreamBlock){
		m_StopNow = true;
	}
	else if(m_StopNow){
		Stop();
		return S_OK;
	}


	if(FillAll && !NoReset) ResetStream();

	do {
		if(m_CurrentStreamBlock != pos || FillAll){
			res = LoadSegment(m_CurrentStreamBlock);
			if(FAILED(res)) return res;

			m_CurrentStreamBlock++;
			if(m_CurrentStreamBlock >= m_NumStreamBlocks) m_CurrentStreamBlock = 0;
		}
	} while(FillAll && m_CurrentStreamBlock != pos);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundBuffer::GetLength()
{
	float BytesPerSec = m_Format.wf.nSamplesPerSec * m_Format.wf.nBlockAlign;
	if(BytesPerSec>0) return (float)(m_TotalDataLength / BytesPerSec) * 1000;	
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
void CBSoundBuffer::SetType(TSoundType Type)
{
	m_Type = Type;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::SetVolume(int Volume)
{
	if(!m_SoundBuffer) return S_OK;

	Volume = (float)Volume / 100.0f * m_PrivateVolume;

	// convert linear ramp to decibels
	double fv = Volume / 200.0;  // range: 0-1
	double attenuate = pow(1 - fv, 3);
	Volume = int(-10000 * attenuate);

	HRESULT res = m_SoundBuffer->SetVolume(Volume);
	if(FAILED(res)) Game->LOG(res, "Error setting sound volume (%s)", m_Filename);
	
	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::SetPrivateVolume(int Volume)
{
	m_PrivateVolume = Volume;
	int RampVolume=100;
	switch(m_Type){
		case SOUND_SFX:    RampVolume = Game->m_SoundMgr->m_VolumeSFX; break;
		case SOUND_SPEECH: RampVolume = Game->m_SoundMgr->m_VolumeSpeech; break;
		case SOUND_MUSIC:  RampVolume = Game->m_SoundMgr->m_VolumeMusic; break;		
	}
	return SetVolume(RampVolume);
}


//////////////////////////////////////////////////////////////////////////
bool CBSoundBuffer::IsPlaying()
{
	if(!m_SoundBuffer) return false;
	else{
		DWORD status;
		m_SoundBuffer->GetStatus(&status);
		return (status & DSBSTATUS_PLAYING);
	}
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSoundBuffer::GetPosition()
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::SetPosition(DWORD Pos)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::SetPan(LONG Pan)
{
	if(m_SoundBuffer) return m_SoundBuffer->SetPan(Pan);
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::GetRawData(DWORD Time, DWORD* LeftChannel, DWORD* RightChannel)
{
	if(!m_RawBuffer){
		Game->LOG(0, "CBSoundBuffer::GetRawData - attempting to read raw data from uninitialized sound buffer");
		return E_FAIL;
	}

	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundBuffer::ApplyFX(TSFXType Type, float Param1, float Param2, float Param3, float Param4)
{
	if(!m_SoundBuffer) return S_OK;

	bool NeedsResume = false;
	if(IsPlaying())
	{
		NeedsResume = true;
		Pause();
	}


	HRESULT Res = E_FAIL;
	LPDIRECTSOUNDBUFFER8 Buf8;

	if(FAILED( Res = m_SoundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&Buf8)))
		return Res;


	switch(Type)
	{
	case SFX_ECHO:
		{
			DSEFFECTDESC desc;
			memset(&desc, 0, sizeof(desc));
			desc.dwSize = sizeof(DSEFFECTDESC);
			desc.guidDSFXClass = GUID_DSFX_STANDARD_ECHO;

			Buf8->SetFX(1, &desc, (DWORD*)&Res);
			if(SUCCEEDED(Res))
			{
				IDirectSoundFXEcho8* DMO;
				if(SUCCEEDED(Res=Buf8->GetObjectInPath(GUID_All_Objects, 0, IID_IDirectSoundFXEcho8, (LPVOID*) &DMO)))
				{
					DSFXEcho Params;
					DMO->GetAllParameters(&Params);
					Params.fWetDryMix = Param1;
					Params.fFeedback = Param2;
					Params.fLeftDelay = Param3;
					Params.fRightDelay = Param4;
					Res = DMO->SetAllParameters(&Params);
				}
			}
		}
		break;

	case SFX_REVERB:
		{
			DSEFFECTDESC desc;
			memset(&desc, 0, sizeof(desc));
			desc.dwSize = sizeof(DSEFFECTDESC);
			desc.guidDSFXClass = GUID_DSFX_WAVES_REVERB;


			Buf8->SetFX(1, &desc, (DWORD*)&Res);
			if(SUCCEEDED(Res))
			{
				IDirectSoundFXWavesReverb8* DMO;
				if(SUCCEEDED(Res=Buf8->GetObjectInPath(GUID_All_Objects, 0, IID_IDirectSoundFXWavesReverb8, (LPVOID*) &DMO)))
				{
					DSFXWavesReverb Params;
					DMO->GetAllParameters(&Params);
					Params.fInGain = Param1;
					Params.fReverbMix = Param2;
					Params.fReverbTime = Param3;
					Params.fHighFreqRTRatio = Param4;
					Res = DMO->SetAllParameters(&Params);
				}
			}
		}
		break;

	default:
		Buf8->SetFX(0, NULL, NULL);
	}

	if(NeedsResume) Resume();

	Buf8->Release();

	return E_FAIL;
}