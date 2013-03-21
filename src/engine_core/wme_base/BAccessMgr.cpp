// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BAccessMgr.h"


//////////////////////////////////////////////////////////////////////////
CBAccessMgr::CBAccessMgr(CBGame* inGame) : CBBase(inGame)
{
	m_Voice = NULL;
	m_TTSAvailable = false;
	m_ActiveObject = NULL;
	m_PrevActiveObject = NULL;
	SetRectEmpty(&m_HintRect);
	m_HintAfterGUI = false;
	m_PlayingType = TTS_CAPTION;
}


//////////////////////////////////////////////////////////////////////////
CBAccessMgr::~CBAccessMgr(void)
{
	m_ActiveObject = NULL; // ref only
	RELEASE(m_Voice);
	m_TTSAvailable = false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::Initialize()
{
	RELEASE(m_Voice);
	m_TTSAvailable = false;

	if(!Game->m_AccessTTSEnabled) return S_OK;

	HRESULT Res = ::CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&m_Voice);
	if(SUCCEEDED(Res)) m_TTSAvailable = true;

	return Res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::InitLoop()
{
	SetRectEmpty(&m_HintRect);
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::DisplayBeforeGUI()
{
	if(!Game->m_AccessKeyboardEnabled) return S_OK;

	if(!m_HintAfterGUI)	return DisplayInternal();
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::DisplayAfterGUI()
{
	if(!Game->m_AccessKeyboardEnabled) return S_OK;

	if(m_HintAfterGUI)return DisplayInternal();
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::DisplayInternal()
{
	if(!CBUtils::IsKeyDown(VK_CONTROL) && !CBUtils::IsKeyDown(VK_RCONTROL)) return S_OK;

	if(!IsRectEmpty(&m_HintRect))
	{
		Game->m_Renderer->SetupLines();
		Game->m_Renderer->DrawRect(m_HintRect.left, m_HintRect.top, m_HintRect.right, m_HintRect.bottom, 0xFFFF0000, 4);
		Game->m_Renderer->Setup2D();

		// reposition mouse pointer
		if(Game->m_AccessKeyboardCursorSkip && m_ActiveObject != m_PrevActiveObject)
		{
			m_PrevActiveObject = m_ActiveObject;
			POINT p;
			p.x = m_HintRect.left + (m_HintRect.right - m_HintRect.left) / 2;
			p.y = m_HintRect.top + (m_HintRect.bottom - m_HintRect.top) / 2;

			p.x += Game->m_Renderer->m_DrawOffsetX;
			p.y += Game->m_Renderer->m_DrawOffsetY;

			p.x = max(0, p.x);
			p.y = max(0, p.y);
			p.x = min(Game->m_Renderer->m_Width - 1, p.x);
			p.y = min(Game->m_Renderer->m_Height - 1, p.y);

			ClientToScreen(Game->m_Renderer->m_Window, &p);
			SetCursorPos(p.x, p.y);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::SetHintRect(RECT* HintRect, bool AfterGUI)
{
	if(!HintRect) SetRectEmpty(&m_HintRect);
	else m_HintRect = *HintRect;

	m_HintAfterGUI = AfterGUI;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::SetActiveObject(CBObject* ActiveObj)
{
	if(!Game->m_AccessKeyboardEnabled) return S_OK;

	m_ActiveObject = ActiveObj;
	if(!ActiveObj) m_PrevActiveObject = NULL;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
CBObject* CBAccessMgr::GetActiveObject()
{
	return m_ActiveObject;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::Speak(char* Str, TTTSType Type)
{
	if(!m_TTSAvailable) return S_OK;
	if(Type==TTS_CAPTION && !Game->m_AccessTTSCaptions) return S_OK;
	if(Type==TTS_TALK && !Game->m_AccessTTSTalk) return S_OK;
	if(Type==TTS_KEYPRESS && !Game->m_AccessTTSKeypress) return S_OK;


	if(!Str || Str[0]=='\0') return S_OK;

	wchar_t* Temp;
	if(Game->m_TextEncoding==TEXT_ANSI) Temp = CBTextUtils::AnsiToUnicode(Str);
	else Temp = CBTextUtils::Utf8ToUnicode(Str);

	if(Temp)
	{
		DWORD Flags = SPF_ASYNC | SPF_IS_NOT_XML;
		if(m_PlayingType!=TTS_TALK) Flags |= SPF_PURGEBEFORESPEAK;

		HRESULT Res = m_Voice->Speak(Temp, Flags, NULL);
		delete [] Temp;

		m_PlayingType = Type;

		return Res;
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBAccessMgr::Stop()
{
	if(!m_TTSAvailable) return S_OK;
	m_PlayingType = TTS_CAPTION;
	return m_Voice->Speak(L"", SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
}

//////////////////////////////////////////////////////////////////////////
CBObject* CBAccessMgr::GetNextObject()
{
	m_ActiveObject = Game->GetNextAccessObject(m_ActiveObject);
	return m_ActiveObject;
}

//////////////////////////////////////////////////////////////////////////
CBObject* CBAccessMgr::GetPrevObject()
{
	m_ActiveObject = Game->GetPrevAccessObject(m_ActiveObject);
	return m_ActiveObject;
}
