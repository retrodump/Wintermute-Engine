// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "VidPlayer.h"

#pragma comment(lib, "vfw32.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CVidPlayer::CVidPlayer(CBGame* inGame):CBBase(inGame)
{
	SetDefaults();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::SetDefaults()
{
	m_Playing = false;	

	m_AviFile = NULL;

	m_AudioStream = NULL;
	m_AudioFormat = NULL;

	m_VideoStream = NULL;
	m_VideoFormat = NULL;
	m_VideoPGF = NULL;
	m_VideoEndTime = 0;

	m_Sound = NULL;
	m_SoundAvailable = false;

	m_VidRenderer = NULL;

	m_StartTime = 0;
	m_TotalVideoTime = 0;

	m_LastSample = -1;

	m_TargetFormat = NULL;

	m_PlayPosX = m_PlayPosY = 0;
	m_PlayZoom = 0.0f;

	m_Filename = NULL;

	m_SlowRendering = false;

	m_CurrentSubtitle = 0;
	m_ShowSubtitle = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CVidPlayer::~CVidPlayer()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Cleanup()
{
	if(m_Sound) m_Sound->Stop();
	if(m_VideoPGF) AVIStreamGetFrameClose(m_VideoPGF);
	m_VideoPGF = NULL;

	m_Playing = false;


	if(m_AviFile) AVIFileRelease(m_AviFile);
	
	if(m_AudioStream) AVIStreamRelease(m_AudioStream);
	if(m_VideoStream) AVIStreamRelease(m_VideoStream);

	if(m_AudioFormat)  delete [] (BYTE*)m_AudioFormat;
	if(m_VideoFormat)  delete [] (BYTE*)m_VideoFormat;
	if(m_TargetFormat) delete [] (BYTE*)m_TargetFormat;

	SAFE_DELETE(m_Sound);
	SAFE_DELETE(m_VidRenderer);

	SAFE_DELETE_ARRAY(m_Filename);

	for(int i=0; i<m_Subtitles.GetSize(); i++) delete m_Subtitles[i];
	m_Subtitles.RemoveAll();

	return SetDefaults();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Initialize(char *inFilename, char* SubtitleFile)
{
	Cleanup();

	char Filename[MAX_PATH];
	Game->m_FileManager->GetFullPath(inFilename, Filename);

	// open file
	if(AVIFileOpen(&m_AviFile, Filename, OF_READ, NULL) != 0){
		Game->LOG(0, "Error opening AVI file '%s'", Filename);
		return E_FAIL;
	}

	// get video stream
	if(AVIFileGetStream(m_AviFile, &m_VideoStream, streamtypeVIDEO, 0)!=0){
		Game->LOG(0, "Error finding video stream in AVI file '%s'", Filename);
		return E_FAIL;
	}
	m_TotalVideoTime = AVIStreamEndTime(m_VideoStream);
	
	// get audio stream
	if(Game->m_SoundMgr->m_SoundAvailable && AVIFileGetStream(m_AviFile, &m_AudioStream, streamtypeAUDIO, 0)==0)
		m_SoundAvailable = true;
	else
		m_SoundAvailable = false;


	LONG Size;

	// get video format
	if(AVIStreamReadFormat(m_VideoStream, 0, NULL, &Size)){
		Game->LOG(0, "Error obtaining video stream format in AVI file '%s'", Filename);
		return E_FAIL;
	}
	m_VideoFormat = (LPBITMAPINFO)new BYTE[Size];
	AVIStreamReadFormat(m_VideoStream, 0, m_VideoFormat, &Size);

	// initialize optimal target format
	m_TargetFormat = (LPBITMAPV4HEADER)new BYTE[max(Size, sizeof(BITMAPV4HEADER))];
	memset(m_TargetFormat, 0, sizeof(BITMAPV4HEADER));
	memcpy(m_TargetFormat, m_VideoFormat, Size);
	m_TargetFormat->bV4Size = max(Size, sizeof(BITMAPV4HEADER));

	m_TargetFormat->bV4BitCount = 24;
	m_TargetFormat->bV4V4Compression = BI_RGB;

	if(Game->m_UseD3D)
		m_VidRenderer = new CVidRendererD3D(Game);
	else
		m_VidRenderer = new CVidRendererDD(Game);

	if(!m_VidRenderer || FAILED(m_VidRenderer->Initialize(m_VideoFormat, m_TargetFormat))){
		Game->LOG(0, "Error initializing video renderer for AVI file '%s'", Filename);
		SAFE_DELETE(m_VidRenderer);
		return E_FAIL;
	}


	// create sound buffer
	HRESULT res;

	if(m_SoundAvailable){
		m_Sound = new CBSoundAVI(Game);
		if(FAILED(res = m_Sound->InitializeBuffer(m_AudioStream))){
			SAFE_DELETE(m_Sound);
			m_SoundAvailable = false;
			Game->LOG(res, "Error initializing sound buffer for AVI file '%s'", Filename);
		}
	}

	if(Game->m_VideoSubtitles) LoadSubtitles(inFilename, SubtitleFile);

	m_Filename = new char[strlen(Filename)+1];
	if(m_Filename) strcpy(m_Filename, Filename);
	
	return S_OK;	
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Update()
{
	if(!m_Playing) return S_OK;

	HRESULT res;

	if(m_SoundAvailable && m_Sound){
		res = m_Sound->Update();
		if(FAILED(res)) return res;
	}


	DWORD CurrentTime; // current playing time (in ms)
/*
	if(m_SoundAvailable && m_Sound){
		CurrentTime = m_Sound->GetPosition(); // in samples
		CurrentTime /= (m_Sound->m_Format.wf.nSamplesPerSec / 1000);

		if(!m_Sound->IsPlaying()) CurrentTime = m_TotalVideoTime;
	}
	else
		CurrentTime = timeGetTime() - m_StartTime;
*/
	CurrentTime = timeGetTime() - m_StartTime;

	if(CurrentTime >= m_TotalVideoTime){
		Stop();
		return S_OK;
	}


	// get and render frame
	DWORD sample = AVIStreamTimeToSample(m_VideoStream, CurrentTime);
	if(sample != m_LastSample){
		m_LastSample = sample;

		// process subtitles
		m_ShowSubtitle = false;
		while(m_CurrentSubtitle<m_Subtitles.GetSize()){
			int End = m_Subtitles[m_CurrentSubtitle]->m_EndFrame;

			bool NextFrameOK = (m_CurrentSubtitle < m_Subtitles.GetSize()-1 && m_Subtitles[m_CurrentSubtitle+1]->m_StartFrame <= sample);

			if(sample > End){
				if(NextFrameOK){
					m_CurrentSubtitle++;
				}
				else{
					m_ShowSubtitle = (End==0);
					break;
				}
			}
			else{
				m_ShowSubtitle = true;
				break;
			}
		}


		// render frame
		LPBITMAPINFOHEADER FrameData = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_VideoPGF, sample);
		if(FrameData){
			if(m_SlowRendering) return m_VidRenderer->ProcessFrameSlow(FrameData);
			else return m_VidRenderer->ProcessFrame(FrameData);
		}
		else return E_FAIL;
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Display()
{
	if(!m_Playing) return S_OK;

	HRESULT res;
	if(m_VidRenderer) res = m_VidRenderer->Display(m_PlayPosX, m_PlayPosY, m_PlayZoom);
	else res = E_FAIL;

	// display subtitle
	if(m_ShowSubtitle){
		CBFont* font = Game->m_VideoFont?Game->m_VideoFont:Game->m_SystemFont;
		int Height = font->GetTextHeight((BYTE*)m_Subtitles[m_CurrentSubtitle]->m_Text, Game->m_Renderer->m_Width);
		font->DrawText((BYTE*)m_Subtitles[m_CurrentSubtitle]->m_Text, 0, Game->m_Renderer->m_Height-Height-5, Game->m_Renderer->m_Width, TAL_CENTER);
	}

	return res;
}




//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Play(TVideoPlayback Type, int X, int Y, bool FreezeMusic)
{
	if(!m_VideoStream || !m_VidRenderer) return E_FAIL;

	switch(Type){
		case VID_PLAY_POS:
			m_PlayZoom = 100.0f;
			m_PlayPosX = X;
			m_PlayPosY = Y;
		break;

		case VID_PLAY_STRETCH:
		{
			float ZoomX = (float)((float)Game->m_Renderer->m_Width / (float)m_VideoFormat->bmiHeader.biWidth * 100);
			float ZoomY = (float)((float)Game->m_Renderer->m_Height / (float)m_VideoFormat->bmiHeader.biHeight * 100);
			m_PlayZoom = min(ZoomX, ZoomY);
			m_PlayPosX = (Game->m_Renderer->m_Width - m_VideoFormat->bmiHeader.biWidth * (m_PlayZoom / 100)) / 2;
			m_PlayPosY = (Game->m_Renderer->m_Height - m_VideoFormat->bmiHeader.biHeight * (m_PlayZoom / 100)) / 2;
		}
		break;
	
		case VID_PLAY_CENTER:
			m_PlayZoom = 100.0f;
			m_PlayPosX = (Game->m_Renderer->m_Width - m_VideoFormat->bmiHeader.biWidth) / 2;
			m_PlayPosY = (Game->m_Renderer->m_Height - m_VideoFormat->bmiHeader.biHeight) / 2;
		break;
	}

	m_TargetFormat->bV4BitCount = 24;
	m_TargetFormat->bV4V4Compression = BI_RGB;


	m_VideoPGF = AVIStreamGetFrameOpen(m_VideoStream, (LPBITMAPINFOHEADER)m_TargetFormat);
	if(!m_VideoPGF){
		m_VideoPGF = AVIStreamGetFrameOpen(m_VideoStream, NULL);
		if(!m_VideoPGF){
			Game->LOG(0, "Error: Unsupported AVI format (file '%s')", m_Filename);
			Cleanup();
			return E_FAIL;
		}
		else{
			Game->LOG(0, "Performance warning: non-optimal AVI format, using generic (i.e. slow) rendering routines (file '%s')", m_Filename);
			m_SlowRendering = true;
		}
	}
	else m_SlowRendering = false;

	// HACK!!!
	m_SlowRendering = true;


	m_CurrentSubtitle = 0;
	
	Game->Freeze(FreezeMusic);

	m_Playing = true;
	if(m_Sound) m_Sound->Play();
	m_StartTime = timeGetTime();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::Stop()
{
	if(!m_Playing) return S_OK;

	Cleanup();

	Game->Unfreeze();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CVidPlayer::IsPlaying()
{
	return m_Playing;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidPlayer::LoadSubtitles(char* Filename, char* SubtitleFile)
{	
	if(!Filename) return S_OK;

	char NewFile[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];

	if(SubtitleFile)
	{
		strcpy(NewFile, SubtitleFile);
	}
	else
	{
		_splitpath(Filename, drive, dir, fname, NULL);
		_makepath(NewFile, drive, dir, fname, ".SUB");
	}

	DWORD Size;
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(NewFile, &Size, false);
	if(Buffer==NULL) return S_OK; // no subtitles


	LONG Start, End;
	bool InToken;
	char* TokenStart;
	int TokenLength;
	int TokenPos;
	int TextLength;

	int Pos = 0;
	int LineLength = 0;
	while(Pos<Size){
		Start = End = -1;
		InToken = false;
		TokenPos = -1;
		TextLength = 0;
		
		LineLength = 0;
		while(Pos+LineLength < Size && Buffer[Pos+LineLength]!='\n' && Buffer[Pos+LineLength]!='\0') LineLength++;
		
		int RealLength = LineLength - (Pos+LineLength>=Size?0:1);
		char* Text = new char[RealLength+1];
		char* line = (char*)&Buffer[Pos];

		for(int i=0; i<RealLength; i++){
			if(line[i]=='{'){
				if(!InToken){
					InToken = true;
					TokenStart = line+i+1;
					TokenLength = 0;
					TokenPos++;
				}
				else TokenLength++;
			}
			else if(line[i]=='}'){
				if(InToken){
					InToken = false;
					char* Token = new char[TokenLength+1];
					strncpy(Token, TokenStart, TokenLength);
					Token[TokenLength] = '\0';
					if(TokenPos==0) Start = atoi(Token);
					else if(TokenPos==1) End = atoi(Token);

					delete [] Token;
				}
				else{
					Text[TextLength] = line[i];
					TextLength++;
				}
			}
			else{
				if(InToken){
					TokenLength++;
				}
				else{
					Text[TextLength] = line[i];
					if(Text[TextLength]=='|') Text[TextLength] = '\n';
					TextLength++;
				}
			}
		}
		Text[TextLength] = '\0';

		if(Start!=-1 && TextLength>0) m_Subtitles.Add(new CVidSubtitle(Game, Text, Start, End));

		delete [] Text;

		Pos+=LineLength+1;
	}

	delete [] Buffer;
	return S_OK;
}
