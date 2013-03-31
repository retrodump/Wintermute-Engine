// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSound.h"


IMPLEMENT_PERSISTENT(CBSound, false);

//////////////////////////////////////////////////////////////////////////
CBSound::CBSound(CBGame* inGame):CBBase(inGame)
{
	m_Sound = NULL;
	m_SoundFilename = NULL;

	m_SoundType = SOUND_SFX;
	m_SoundStreamed = false;
	m_SoundLooping = false;
	m_SoundPlaying = false;
	m_SoundPaused = false;
	m_SoundFreezePaused = false;
	m_SoundPosition = 0;
	m_SoundPrivateVolume = 0;
	m_SoundLoopStart = 0;

	m_SFXType = SFX_NONE;
	m_SFXParam1 = m_SFXParam2 = m_SFXParam3 = m_SFXParam4 = 0;
}


//////////////////////////////////////////////////////////////////////////
CBSound::~CBSound()
{
	if(m_Sound) Game->m_SoundMgr->RemoveSound(m_Sound);
	m_Sound = NULL;

	SAFE_DELETE_ARRAY(m_SoundFilename);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetSound(char *Filename, TSoundType Type, bool Streamed, DWORD initialPrivateVolume)
{
	if(m_Sound){
		Game->m_SoundMgr->RemoveSound(m_Sound);
		m_Sound = NULL;
	}
	SAFE_DELETE_ARRAY(m_SoundFilename);

	m_Sound = Game->m_SoundMgr->AddSound(Filename, Type, Streamed, initialPrivateVolume);
	if(m_Sound){
		m_SoundFilename = new char[strlen(Filename)+1];
		strcpy(m_SoundFilename, Filename);

		m_SoundType = Type;
		m_SoundStreamed = Streamed;

		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetSoundSimple()
{
	m_Sound = Game->m_SoundMgr->AddSound(m_SoundFilename, m_SoundType, m_SoundStreamed);
	if(m_Sound){
		if(m_SoundPosition) m_Sound->SetPosition(m_SoundPosition);
		m_Sound->m_Looping = m_SoundLooping;
		m_Sound->SetPrivateVolume(m_SoundPrivateVolume);
		m_Sound->m_LoopStart = m_SoundLoopStart;
		m_Sound->m_FreezePaused = m_SoundFreezePaused;
		if(m_SoundPlaying) return m_Sound->Resume();
		else return S_OK;
	}
	else return E_FAIL;
}



//////////////////////////////////////////////////////////////////////////
DWORD CBSound::GetLength()
{
	if(m_Sound) return m_Sound->GetLength();
	else return 0;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::Play(bool Looping)
{	
	if(m_Sound){
		m_SoundPaused = false;		
		return m_Sound->Play(Looping, m_SoundPosition);
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::Stop()
{
	if(m_Sound){
		m_SoundPaused = false;
		return m_Sound->Stop();
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::Pause(bool FreezePaused)
{
	if(m_Sound){
		m_SoundPaused = true;
		if(FreezePaused) m_Sound->m_FreezePaused = true;
		return m_Sound->Pause();
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::Resume()
{
	if(m_Sound && m_SoundPaused){
		m_SoundPaused = false;
		return m_Sound->Resume();
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::Persist(CBPersistMgr *PersistMgr)
{
	if(PersistMgr->m_Saving && m_Sound){
		m_SoundPlaying = m_Sound->IsPlaying();
		m_SoundLooping = m_Sound->m_Looping;
		m_SoundPrivateVolume = m_Sound->m_PrivateVolume;
		if(m_SoundPlaying) m_SoundPosition = m_Sound->GetPosition();
		m_SoundLoopStart = m_Sound->m_LoopStart;
		m_SoundFreezePaused = m_Sound->m_FreezePaused;
	}

	if(PersistMgr->m_Saving)
	{
		m_SFXType = SFX_NONE;
		m_SFXParam1 = m_SFXParam2 = m_SFXParam3 = m_SFXParam4 = 0;
	}

	PersistMgr->Transfer(TMEMBER(Game));

	PersistMgr->Transfer(TMEMBER(m_SoundFilename));
	PersistMgr->Transfer(TMEMBER(m_SoundLooping));
	PersistMgr->Transfer(TMEMBER(m_SoundPaused));
	PersistMgr->Transfer(TMEMBER(m_SoundFreezePaused));
	PersistMgr->Transfer(TMEMBER(m_SoundPlaying));
	PersistMgr->Transfer(TMEMBER(m_SoundPosition));
	PersistMgr->Transfer(TMEMBER(m_SoundPrivateVolume));
	PersistMgr->Transfer(TMEMBER(m_SoundStreamed));
	PersistMgr->Transfer(TMEMBER_INT(m_SoundType));

	// 1.1.41
	if(PersistMgr->CheckVersion(1, 1, 41)){
		PersistMgr->Transfer(TMEMBER(m_SoundLoopStart));
	}
	else{
		m_SoundLoopStart = 0;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBSound::IsPlaying()
{
	return m_Sound && m_Sound->IsPlaying();
}


//////////////////////////////////////////////////////////////////////////
bool CBSound::IsPaused()
{
	return m_Sound && m_SoundPaused;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetPositionTime(DWORD Time)
{
	if(!m_Sound) return E_FAIL;
	m_SoundPosition = m_Sound->m_Format.wf.nSamplesPerSec / 1000.0f * Time;
	HRESULT ret = m_Sound->SetPosition(m_SoundPosition);
	if(m_Sound->IsPlaying()) m_SoundPosition = 0;
	return ret;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBSound::GetPositionTime()
{
	if(!m_Sound) return 0;

	if(!m_Sound->IsPlaying()) return 0;
	else return m_Sound->GetPosition() / (m_Sound->m_Format.wf.nSamplesPerSec / 1000.0f);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetVolume(int Volume)
{
	if(!m_Sound) return E_FAIL;
	else return m_Sound->SetPrivateVolume(Volume);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetPrivateVolume(int Volume)
{
	if(!m_Sound) return E_FAIL;
	else return m_Sound->m_PrivateVolume = Volume;
}


//////////////////////////////////////////////////////////////////////////
int CBSound::GetVolume()
{
	if(!m_Sound) return 0;
	else return m_Sound->m_PrivateVolume;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetLoopStart(DWORD Pos)
{
	if(!m_Sound) return E_FAIL;
	else {
		m_Sound->m_LoopStart = m_Sound->m_Format.wf.nSamplesPerSec / 1000.0f * Pos;
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::SetPan(LONG Pan)
{
	if(m_Sound) return m_Sound->SetPan(Pan);
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::LoadRawData()
{
	if(!m_Sound || !m_SoundFilename) return E_FAIL;

	Stop();
	CBFile* File = Game->m_FileManager->OpenFile(m_SoundFilename);
	if(!File){
		Game->LOG(0, "Error reading raw sound data for file '%s'", m_SoundFilename);
		return E_FAIL;
	}

	HRESULT ret = m_Sound->FillRawBuffer(File);
	Game->m_FileManager->CloseFile(File);
	File = NULL;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::GetRawData(DWORD Time, int* LeftChannel, int* RightChannel)
{
	if(LeftChannel) *LeftChannel = 0;
	if(RightChannel) *RightChannel = 0;

	if(!m_Sound) return E_FAIL;
	if(m_Sound->m_RawBuffer == NULL){
		LoadRawData();
		if(m_Sound->m_RawBuffer == NULL) return E_FAIL;
	}

	// seek the right position
	float BytesPerMSec = m_Sound->m_Format.wf.nSamplesPerSec * m_Sound->m_Format.wf.nBlockAlign / 1000;
	DWORD Pos = BytesPerMSec * Time;
	if(Pos%m_Sound->m_Format.wf.nBlockAlign!=0) Pos-=Pos%m_Sound->m_Format.wf.nBlockAlign;

	// for each channel
	for(int i=0; i<min(2, m_Sound->m_Format.wf.nChannels); i++){
		DWORD BytesPerSample = m_Sound->m_Format.wBitsPerSample / 8;
		
		if(Pos>m_Sound->m_TotalDataLength) continue;
		
		DWORD buffer;
		if(i==0 && LeftChannel){			
			memcpy(&buffer, &m_Sound->m_RawBuffer[Pos], min(sizeof(DWORD), BytesPerSample));
			if(m_Sound->m_Format.wBitsPerSample == 8) *LeftChannel = (char)buffer;
			else *LeftChannel = (short)buffer;
		}
		if(i==1 && RightChannel){
			memcpy(&buffer, &m_Sound->m_RawBuffer[Pos + BytesPerSample], min(sizeof(DWORD), BytesPerSample));
			if(m_Sound->m_Format.wBitsPerSample == 8) *RightChannel = (char)buffer;
			else *RightChannel = (short)buffer;
		}
	}
	
	return S_OK;
	//return m_Sound->GetRawData(Time, LeftChannel, RightChannel);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSound::ApplyFX(TSFXType Type, float Param1, float Param2, float Param3, float Param4)
{
	if(!m_Sound) return S_OK;

	if(Type!=m_SFXType || Param1!=m_SFXParam1 || Param2!=m_SFXParam2 || Param3!=m_SFXParam3 || Param4!=m_SFXParam4)
	{
		HRESULT Ret = m_Sound->ApplyFX(Type, Param1, Param2, Param3, Param4);

		m_SFXType = Type;
		m_SFXParam1 = Param1;
		m_SFXParam2 = Param2;
		m_SFXParam3 = Param3;
		m_SFXParam4 = Param4;

		return Ret;
	}
	return S_OK;
}
