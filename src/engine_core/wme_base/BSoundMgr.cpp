// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSoundMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_PERSISTENT(CBSoundMgr, true);

//////////////////////////////////////////////////////////////////////////
CBSoundMgr::CBSoundMgr(CBGame* inGame):CBBase(inGame)
{
	m_DirectSound = NULL;
	m_SoundAvailable = false;

	m_VolumeSFX = m_VolumeSpeech = m_VolumeMusic = 200;
	m_VolumeOriginal = m_VolumeWave = MAKELONG(32767, 32767);
}


//////////////////////////////////////////////////////////////////////////
CBSoundMgr::~CBSoundMgr()
{
	if(m_SoundAvailable){
		Game->m_Registry->WriteInt("Audio", "MasterVolume", m_VolumeWave);
		
		Game->m_Registry->WriteInt("Audio", "SFXVolume",    m_VolumeSFX);
		Game->m_Registry->WriteInt("Audio", "SpeechVolume", m_VolumeSpeech);
		Game->m_Registry->WriteInt("Audio", "MusicVolume",  m_VolumeMusic);

		SetVolumeWave(m_VolumeOriginal);		
	}

	Cleanup();

	RELEASE(m_DirectSound);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::Cleanup()
{
	for(int i=0; i<m_Sounds.GetSize(); i++) delete m_Sounds[i];
	m_Sounds.RemoveAll();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::Initialize(HWND Window, CHWManager* hwManager)
{
	m_SoundAvailable = false;

	if(!hwManager->m_SelectedAudioDevice || hwManager->m_SelectedAudioDevice->m_NoSound) return S_OK;

	HRESULT ret;
	if(FAILED(ret=Game->m_DirectX.DirectSoundCreate8(&hwManager->m_SelectedAudioDevice->m_Guid, &m_DirectSound, NULL))) return ret;

	if(FAILED(ret=m_DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY))){
		Game->LOG(ret, "CBSoundMgr::Initialize - Error setting cooperative level");
		RELEASE(m_DirectSound);
		return ret;
	}

	// get original wave volume
	WAVEOUTCAPS waveCaps;
	if(FAILED(ret=waveOutGetDevCaps(0, &waveCaps, sizeof(waveCaps)))){
		Game->LOG(ret, "CBSoundMgr::Initialize - Error getting wave device caps");
		RELEASE(m_DirectSound);
		return ret;
	}
	if(waveCaps.dwSupport&WAVECAPS_VOLUME) waveOutGetVolume(0, &m_VolumeOriginal);

	m_VolumeWave = (DWORD)Game->m_Registry->ReadInt("Audio", "MasterVolume", m_VolumeOriginal);

	m_VolumeSFX    = Game->m_Registry->ReadInt("Audio", "SFXVolume",    200);
	m_VolumeSpeech = Game->m_Registry->ReadInt("Audio", "SpeechVolume", 200);
	m_VolumeMusic  = Game->m_Registry->ReadInt("Audio", "MusicVolume",  200);


	m_SoundAvailable = true;

	SetVolumeWave(m_VolumeWave);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::InitLoop()
{
	if(!m_SoundAvailable) return S_OK;

	for(int i=0; i<m_Sounds.GetSize(); i++){
		m_Sounds[i]->Update();
	}
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CBSoundBuffer* CBSoundMgr::AddSound(char *Filename, TSoundType Type, bool Streamed, DWORD initialPrivateVolume)
{
	if(!m_SoundAvailable) return NULL;

	CBSoundBuffer* sound;
	
	char NewFile[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	// try to switch WAV to OGG file (if available)
	_splitpath(Filename, drive, dir, fname, ext);
	if(_stricmp(ext, ".WAV")==0){
		_makepath(NewFile, drive, dir, fname, ".OGG");
		CBFile* file = Game->m_FileManager->OpenFile(NewFile);
		if(file){
			Filename = NewFile;
			Game->m_FileManager->CloseFile(file);
		}
	}

	_splitpath(Filename, NULL, NULL, NULL, ext);

	if(_stricmp(ext, ".OGG")==0) sound = new CBSoundOGG(Game);
	else sound = new CBSoundWAV(Game);

	if(!sound) return NULL;

	sound->SetStreaming(Streamed);
	sound->SetType(Type);


	HRESULT res = sound->LoadFromFile(Filename);
	if(FAILED(res)){
		Game->LOG(res, "Error loading sound '%s'", Filename);
		delete sound;
		return NULL;
	}

	// sound starts with user defined instead of 100% volume (of the global setting)
	sound->m_PrivateVolume = initialPrivateVolume;

	// set volume appropriately (global volume setting)
	switch(Type){
		case SOUND_SFX:    sound->SetVolume(m_VolumeSFX);    break;
		case SOUND_SPEECH: sound->SetVolume(m_VolumeSpeech); break;
		case SOUND_MUSIC:  sound->SetVolume(m_VolumeMusic);  break;
	}

	// register sound
	m_Sounds.Add(sound);

	return sound;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::AddSound(CBSoundBuffer* Sound, TSoundType Type)
{
	if(!Sound) return E_FAIL;

	// set volume appropriately
	switch(Type)
	{
		case SOUND_SFX:    Sound->SetVolume(m_VolumeSFX);    break;
		case SOUND_SPEECH: Sound->SetVolume(m_VolumeSpeech); break;
		case SOUND_MUSIC:  Sound->SetVolume(m_VolumeMusic);  break;
	}

	// register sound
	m_Sounds.Add(Sound);
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::RemoveSound(CBSoundBuffer* Sound)
{
	for(int i=0; i<m_Sounds.GetSize(); i++){
		if(m_Sounds[i]==Sound){
			delete m_Sounds[i];
			m_Sounds.RemoveAt(i);
			return S_OK;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetVolumeWave(DWORD Volume)
{
	if(!m_SoundAvailable) return S_OK;

	WAVEOUTCAPS waveCaps;
	if(FAILED(waveOutGetDevCaps(0, &waveCaps, sizeof(waveCaps)))) return DSERR_NODRIVER;
	m_VolumeWave = Volume;

	if(waveCaps.dwSupport&WAVECAPS_VOLUME && !Game->m_EditorMode) return waveOutSetVolume(0, Volume);
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetVolume(TSoundType Type, int Volume)
{
	if(!m_SoundAvailable) return S_OK;

	switch(Type){
		case SOUND_SFX:    m_VolumeSFX    = Volume; break;
		case SOUND_SPEECH: m_VolumeSpeech = Volume; break;
		case SOUND_MUSIC:  m_VolumeMusic  = Volume; break;
	}

	for(int i=0; i<m_Sounds.GetSize(); i++){
		if(m_Sounds[i]->m_Type==Type) m_Sounds[i]->SetVolume(Volume);
	}

	return S_OK;
}

#define MIN_VOLUME 0
#define MAX_VOLUME 200
//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetVolumePercent(TSoundType Type, BYTE Percent)
{
	int Volume = (MAX_VOLUME - MIN_VOLUME) / 100 * Percent + MIN_VOLUME;
	return SetVolume(Type, Volume);
}


//////////////////////////////////////////////////////////////////////////
BYTE CBSoundMgr::GetVolumePercent(TSoundType Type)
{
	int Volume;
	switch(Type){
		case SOUND_SFX:    Volume = m_VolumeSFX;    break;
		case SOUND_SPEECH: Volume = m_VolumeSpeech; break;
		case SOUND_MUSIC:  Volume = m_VolumeMusic;  break;
	}

	Volume -= MIN_VOLUME;
	Volume /= (MAX_VOLUME - MIN_VOLUME) / 100;
	
	return (BYTE)Volume;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::SetMasterVolumePercent(BYTE Percent)
{
	int Volume = (0xFFFF) / 100 * Percent;
	return SetVolumeWave(MAKELONG(Volume, Volume));
}


//////////////////////////////////////////////////////////////////////////
BYTE CBSoundMgr::GetMasterVolumePercent()
{
	WORD Volume = HIWORD(m_VolumeWave) / (0xFFFF / 100);
	return (BYTE)Volume;
}


/*
//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::Persist(CBPersistMgr *PersistMgr)
{
	if(!PersistMgr->m_Saving) Cleanup();

	PersistMgr->Transfer(TMEMBER(Game));
	//m_Sounds.Persist(PersistMgr);

	return S_OK;
}
*/


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::PauseAll(bool IncludingMusic)
{
	for(int i=0; i<m_Sounds.GetSize(); i++){
		if(m_Sounds[i]->IsPlaying() && (m_Sounds[i]->m_Type!=SOUND_MUSIC||IncludingMusic)){
			m_Sounds[i]->Pause();
			m_Sounds[i]->m_FreezePaused = true;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSoundMgr::ResumeAll()
{
	for(int i=0; i<m_Sounds.GetSize(); i++){
		if(m_Sounds[i]->m_FreezePaused){
			m_Sounds[i]->Resume();
			m_Sounds[i]->m_FreezePaused = false;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
LONG CBSoundMgr::PosToPan(int X, int Y)
{
	int Margin = Game->m_Renderer->m_Width*5;

	int MinPos = -Margin;
	int MaxPos = Game->m_Renderer->m_Width + Margin;

	return DSBPAN_LEFT + (float)(X + Margin) / (float)(MaxPos - MinPos) * (DSBPAN_RIGHT - DSBPAN_LEFT);
}
