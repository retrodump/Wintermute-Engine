// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\VidSubtitler.h"


//////////////////////////////////////////////////////////////////////////
CVidSubtitler::CVidSubtitler(CBGame* inGame):CBBase(inGame)
{
	m_LastSample = -1;
	m_CurrentSubtitle = 0;
	m_ShowSubtitle = false;
}

//////////////////////////////////////////////////////////////////////////
CVidSubtitler::~CVidSubtitler(void)
{
	for(int i=0; i<m_Subtitles.GetSize(); i++) delete m_Subtitles[i];
	m_Subtitles.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CVidSubtitler::LoadSubtitles(char* Filename, char* SubtitleFile)
{	
	if(!Filename) return S_OK;

	for(int i=0; i<m_Subtitles.GetSize(); i++) delete m_Subtitles[i];
	m_Subtitles.RemoveAll();

	m_LastSample = -1;
	m_CurrentSubtitle = 0;
	m_ShowSubtitle = false;


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

		if(Start!=-1 && TextLength>0 && (Start!=1 || End!=1)) m_Subtitles.Add(new CVidSubtitle(Game, Text, Start, End));

		delete [] Text;

		Pos+=LineLength+1;
	}

	delete [] Buffer;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CVidSubtitler::Display()
{
	if(m_ShowSubtitle)
	{
		CBFont* font = Game->m_VideoFont?Game->m_VideoFont:Game->m_SystemFont;
		int Height = font->GetTextHeight((BYTE*)m_Subtitles[m_CurrentSubtitle]->m_Text, Game->m_Renderer->m_Width);
		font->DrawText((BYTE*)m_Subtitles[m_CurrentSubtitle]->m_Text, 0, Game->m_Renderer->m_Height-Height-5, Game->m_Renderer->m_Width, TAL_CENTER);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CVidSubtitler::Update(LONG Frame)
{
	if(Frame != m_LastSample)
	{
		m_LastSample = Frame;

		// process subtitles
		m_ShowSubtitle = false;
		while(m_CurrentSubtitle<m_Subtitles.GetSize()){
			int End = m_Subtitles[m_CurrentSubtitle]->m_EndFrame;

			bool NextFrameOK = (m_CurrentSubtitle < m_Subtitles.GetSize()-1 && m_Subtitles[m_CurrentSubtitle+1]->m_StartFrame <= Frame);

			if(Frame > End){
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
	}
	return S_OK;
}
