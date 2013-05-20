// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BGame.h"
#include "SXString.h"
#include "SXDate.h"
#include "SXArray.h"
#include "SXObject.h"
#include "SXMemBuffer.h"
#include "SXFile.h"
#include "SXTcpClient.h"
#include "XModel.h"
#include <time.h>
#include "crc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(CBGame, true);

static char _dcgf_credits[] = "*** Wintermute Engine, (c) 2010 DEAD:CODE ***";


//////////////////////////////////////////////////////////////////////
CBGame::CBGame():CBObject(this)
{	
	m_ShuttingDown = false;
	
	m_State = GAME_RUNNING;
	m_OrigState = GAME_RUNNING;
	m_FreezeLevel = 0;

	m_Interactive = true;
	m_OrigInteractive = false;

	m_SurfaceStorage = NULL;
	m_FontStorage = NULL;
	m_Renderer = NULL;
	m_SoundMgr = NULL;
	m_FileManager = NULL;
	m_NetworkMgr = NULL;
	m_AccessMgr = NULL;
	m_TransMgr = NULL;
	m_PluginMgr = NULL;
	m_DebugMgr = NULL;
	m_ScEngine = NULL;
	m_KeyboardState = NULL;
	
	m_MathClass = NULL;
	m_DirectoryClass = NULL;

	m_DEBUG_LogFile = NULL;
	m_DEBUG_DebugMode = false;
	m_DEBUG_AbsolutePathWarning = true;
	m_DEBUG_ShowFPS = false;

	m_SystemFont = NULL;
	m_VideoFont = NULL;

	m_VideoPlayer = NULL;
	m_TheoraPlayer = NULL;

	m_MainObject = NULL;
	m_ActiveObject = NULL;

	m_Fader = NULL;

	m_OffsetX = m_OffsetY = 0;
	m_OffsetPercentX = m_OffsetPercentY = 0.0f;

	m_Subtitles = true;
	m_VideoSubtitles = true;

	m_Timer = 0;	
	m_TimerDelta = 0;
	m_TimerLast = 0;

	m_LiveTimer = 0;	
	m_LiveTimerDelta = 0;
	m_LiveTimerLast = 0;

	m_Sequence = 0;

	m_MousePos.x = m_MousePos.y = 0;
	m_MouseLeftDown = m_MouseRightDown = m_MouseMidlleDown = false;
	m_CapturedObject = NULL;

	// FPS counters
	m_LastTime = m_FpsTime = m_DeltaTime = m_FramesRendered = m_Fps = 0;

	m_CursorNoninteractive = NULL;

	m_UseD3D = false;
	
	srand((unsigned)time(NULL));

	m_Registry = new CBRegistry(this);
	m_StringTable = new CBStringTable(this);

	for(int i=0; i<NUM_MUSIC_CHANNELS; i++){
		m_Music[i] = NULL;
		m_MusicStartTime[i] = 0;
	}

	m_SettingsResWidth = 800;
	m_SettingsResHeight = 600;
	m_SettingsRequireAcceleration = false;
	m_SettingsRequireSound = false;
	m_SettingsTLMode = 0;
	m_SettingsAllowWindowed = true;
	m_SettingsGameFile = NULL;
	m_SettingsAllowAdvanced = false;
	m_SettingsAllowAccessTab = true;
	m_SettingsAllowAboutTab = true;
	m_SettingsAllowDesktopRes = false;

	m_EditorForceScripts = false;
	m_EditorAlwaysRegister = false;

	m_FocusedWindow = NULL;

	m_LoadInProgress = false;

	m_Quitting = false;
	m_Loading = false;
	m_ScheduledLoadSlot = -1;

	m_PersonalizedSave = false;
	m_CompressedSavegames = true;

	m_EditorMode = false;
	m_DoNotExpandStrings = false;

	m_EngineLogCallback = NULL;
	m_EngineLogCallbackData = NULL;	

	m_SmartCache = false;
	m_SurfaceGCCycleTime = 10000;

	m_ReportTextureFormat = false;

	m_ViewportSP = -1;

	m_SubtitlesSpeed = 70;

	m_ResourceModule = NULL;

	m_ForceNonStreamedSounds = false;

	m_ThumbnailWidth = m_ThumbnailHeight = 0;

	m_IndicatorDisplay = false;
	m_IndicatorColor = DRGBA(255, 0, 0, 128);
	m_IndicatorProgress = 0;
	m_IndicatorX = -1;
	m_IndicatorY = -1;
	m_IndicatorWidth = -1;
	m_IndicatorHeight = 8;
	m_RichSavedGames = false;
	m_SavedGameExt = NULL;
	CBUtils::SetString(&m_SavedGameExt, "dsv");

	m_MusicCrossfadeRunning = false;
	m_MusicCrossfadeStartTime = 0;
	m_MusicCrossfadeLength = 0;
	m_MusicCrossfadeChannel1 = -1;
	m_MusicCrossfadeChannel2 = -1;
	m_MusicCrossfadeSwap = false;

	m_LoadImageName = NULL;
	m_SaveImageName = NULL;
	m_SaveLoadImage = NULL;

	m_SaveImageX = m_SaveImageY = 0;
	m_LoadImageX = m_LoadImageY = 0;

	m_MaxShadowType = SHADOW_STENCIL;
	m_SupportsRealTimeShadows = false;

	m_EditorResolutionWidth = 0;
	m_EditorResolutionHeight = 0;
	
	m_LocalSaveDir = NULL;
	CBUtils::SetString(&m_LocalSaveDir, "saves");
	m_SaveDirChecked = false;

	m_LoadingIcon = NULL;
	m_LoadingIconX = m_LoadingIconY = 0;
	m_LoadingIconPersistent = false;

	m_TextEncoding = TEXT_ANSI;
	m_TextRTL = false;

	m_SoundBufferSizeSec = 3;
	m_SuspendedRendering = false;

	m_LastCursor = NULL;

	// accessibility flags
	m_AccessTTSEnabled = false;
	m_AccessTTSTalk = true;
	m_AccessTTSCaptions = true;
	m_AccessTTSKeypress = true;
	m_AccessKeyboardEnabled = false;
	m_AccessKeyboardCursorSkip = true;
	m_AccessKeyboardPause = false;

	m_AccessGlobalPaused = false;
	m_AccessShieldWin = NULL;

	CBPlatform::SetRectEmpty(&m_MouseLockRect);

	m_SuppressScriptErrors = false;
	m_LastMiniUpdate = 0;
	m_MiniUpdateEnabled = false;

	m_CachedThumbnail = NULL;
	m_GameGUID = GUID_NULL;

	m_AutorunDisabled = false;

	// compatibility bits
	m_CompatKillMethodThreads = false;


	BindDirectX();
	CBPlatform::CoInitialize(NULL);
}


//////////////////////////////////////////////////////////////////////
CBGame::~CBGame()
{	
	m_ShuttingDown = true;

	LOG(0, "");
	LOG(0, "Shutting down...");
	
	GetDebugMgr()->OnGameShutdown();

	m_Registry->WriteBool("System", "LastRun", true);
	Cleanup();

	SAFE_DELETE_ARRAY(m_LocalSaveDir);
	SAFE_DELETE_ARRAY(m_SettingsGameFile);
	SAFE_DELETE_ARRAY(m_SavedGameExt);
	
	SAFE_DELETE(m_CachedThumbnail);
	
	SAFE_DELETE(m_SaveLoadImage);
	SAFE_DELETE(m_MathClass);
	SAFE_DELETE(m_DirectoryClass);

	SAFE_DELETE(m_TransMgr);	
	SAFE_DELETE(m_ScEngine);
	SAFE_DELETE(m_FontStorage);
	SAFE_DELETE(m_SurfaceStorage);
	SAFE_DELETE(m_VideoPlayer);
	SAFE_DELETE(m_TheoraPlayer);
	SAFE_DELETE(m_SoundMgr);
	SAFE_DELETE(m_PluginMgr);
	SAFE_DELETE(m_DebugMgr);
	//SAFE_DELETE(m_KeyboardState);
	
	SAFE_DELETE(m_Renderer);
	SAFE_DELETE(m_FileManager);
	SAFE_DELETE(m_NetworkMgr);
	SAFE_DELETE(m_AccessMgr);
	SAFE_DELETE(m_Registry);
	SAFE_DELETE(m_StringTable);

	CBPlatform::CoUninitialize();
	UnBindDirectX();

	DEBUG_DebugDisable();
	CBPlatform::OutputDebugString("--- shutting down normally ---\n");
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::Cleanup()
{
	int i;

	SAFE_DELETE(m_LoadingIcon);

	m_EngineLogCallback = NULL;
	m_EngineLogCallbackData = NULL;	

	for(i=0; i<NUM_MUSIC_CHANNELS; i++)
	{
		SAFE_DELETE(m_Music[i]);
		m_MusicStartTime[i] = 0;
	}

	UnregisterObject(m_Fader);
	m_Fader = NULL;

	for(i=0; i<m_RegObjects.GetSize(); i++){
		SAFE_DELETE(m_RegObjects[i]);
	}
	m_RegObjects.RemoveAll();

	m_Windows.RemoveAll(); // refs only
	m_FocusedWindow = NULL; // ref only
	m_AccessShieldWin = NULL;
	
	SAFE_DELETE_ARRAY(m_SaveImageName);
	SAFE_DELETE_ARRAY(m_LoadImageName);

	SAFE_DELETE(m_CursorNoninteractive);
	SAFE_DELETE(m_Cursor);
	SAFE_DELETE(m_ActiveCursor);

	SAFE_DELETE(m_ScValue);
	SAFE_DELETE(m_SFX);

	for(i=0; i<m_Scripts.GetSize(); i++)
	{
		m_Scripts[i]->m_Owner = NULL;
		m_Scripts[i]->Finish();
	}
	m_Scripts.RemoveAll();

	m_FontStorage->RemoveFont(m_SystemFont);
	m_SystemFont = NULL;
	
	m_FontStorage->RemoveFont(m_VideoFont);
	m_VideoFont = NULL;

	if(m_ShadowImage)
	{
		m_SurfaceStorage->RemoveSurface(m_ShadowImage);
		m_ShadowImage = NULL;
	}

	for(i=0; i<m_QuickMessages.GetSize(); i++) delete m_QuickMessages[i];
	m_QuickMessages.RemoveAll();

	m_ViewportStack.RemoveAll();
	m_ViewportSP = -1;


	SAFE_DELETE_ARRAY(m_Name);
	SAFE_DELETE_ARRAY(m_Filename);
	for(int i=0; i<7; i++)
	{
		SAFE_DELETE_ARRAY(m_Caption[i]);
	}

	m_LastCursor = NULL;

	SAFE_DELETE(m_KeyboardState);

	if(m_AccessMgr) m_AccessMgr->SetActiveObject(NULL);
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBGame::Initialize1()
{	
	m_SurfaceStorage = new CBSurfaceStorage(this);
	if(m_SurfaceStorage==NULL) goto init_fail;

	m_FontStorage = new CBFontStorage(this);
	if(m_FontStorage==NULL) goto init_fail;

	m_FileManager = new CBFileManager(this);
	if(m_FileManager==NULL) goto init_fail;

	m_NetworkMgr = new CBNetworkMgr(this);
	if(m_NetworkMgr==NULL) goto init_fail;

	m_AccessMgr = new CBAccessMgr(this);
	if(m_AccessMgr==NULL) goto init_fail;

	m_SoundMgr = new CBSoundMgr(this);
	if(m_SoundMgr==NULL) goto init_fail;

	m_PluginMgr = new CBPluginMgr(this);
	if(m_PluginMgr==NULL) goto init_fail;

	m_DebugMgr = new CBDebugger(this);
	if(m_DebugMgr==NULL) goto init_fail;

	m_MathClass = new CSXMath(this);
	if(m_MathClass==NULL) goto init_fail;

	m_DirectoryClass = new CSXDirectory(this);
	if(m_DirectoryClass==NULL) goto init_fail;

	m_ScEngine = new CScEngine(this);
	if(m_ScEngine==NULL) goto init_fail;

	m_VideoPlayer = new CVidPlayer(this);
	if(m_VideoPlayer==NULL) goto init_fail;

	m_TransMgr = new CBTransitionMgr(this);
	if(m_TransMgr==NULL) goto init_fail;

	m_KeyboardState = new CBKeyboardState(this);
	if(m_KeyboardState==NULL) goto init_fail;

	m_Fader = new CBFader(this);
	if(m_Fader==NULL) goto init_fail;
	RegisterObject(m_Fader);

	m_PluginMgr->RegisterPlugins();


	return S_OK;

init_fail:
	if(m_DirectoryClass) delete m_DirectoryClass;
	if(m_MathClass) delete m_MathClass;
	if(m_KeyboardState) delete m_KeyboardState;
	if(m_TransMgr) delete m_TransMgr;
	if(m_PluginMgr) delete m_PluginMgr;
	if(m_DebugMgr) delete m_DebugMgr;
	if(m_SurfaceStorage) delete m_SurfaceStorage;
	if(m_FontStorage) delete m_FontStorage;
	if(m_SoundMgr) delete m_SoundMgr;
	if(m_FileManager) delete m_FileManager;
	if(m_NetworkMgr) delete m_NetworkMgr;
	if(m_AccessMgr) delete m_AccessMgr;
	if(m_ScEngine) delete m_ScEngine;
	if(m_VideoPlayer) delete m_VideoPlayer;
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBGame::Initialize2() // we know whether we are going to be accelerated
{	
	if(m_UseD3D) m_Renderer = new CBRenderD3D(this);
	else m_Renderer = new CBRenderDD(this);
	if(m_Renderer==NULL) goto init_fail;

	return S_OK;

init_fail:
	if(m_Renderer) delete m_Renderer;
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBGame::Initialize3() // renderer is initialized
{	
	m_PosX = m_Renderer->m_Width / 2;
	m_PosY = m_Renderer->m_Height / 2;

	if(m_IndicatorY==-1) m_IndicatorY = m_Renderer->m_Height - m_IndicatorHeight;
	if(m_IndicatorX==-1) m_IndicatorX = 0;
	if(m_IndicatorWidth==-1) m_IndicatorWidth = m_Renderer->m_Width;

	if(m_AccessMgr) Game->m_AccessMgr->Initialize();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
void CBGame::DEBUG_DebugEnable(const char* Filename)
{
	m_DEBUG_DebugMode = true;

	if(Filename)m_DEBUG_LogFile = fopen(Filename, "a+");
	else m_DEBUG_LogFile = fopen(".\\zz_debug.log", "a+");

	if(!m_DEBUG_LogFile)
	{
		char SafeLogFilename[MAX_PATH];
		CBPlatform::GetSafeLogFilename(SafeLogFilename);
		m_DEBUG_LogFile = fopen(SafeLogFilename, "a+");
	}

	if(m_DEBUG_LogFile!=NULL) fprintf(m_DEBUG_LogFile, "\n");

	SYSTEMTIME st;
	GetLocalTime(&st);
#ifdef _DEBUG
	LOG(0, "********** DEBUG LOG OPENED %02d-%02d-%04d (Debug Build) *******************", st.wDay, st.wMonth, st.wYear);
#else
	LOG(0, "********** DEBUG LOG OPENED %02d-%02d-%04d (Release Build) *****************", st.wDay, st.wMonth, st.wYear);
#endif

	LOG(0, "%s ver %d.%d.%d%s, Compiled on " __DATE__ ", " __TIME__, DCGF_NAME, DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD, DCGF_VER_SUFFIX);
	//LOG(0, "Extensions: %s ver %d.%02d", EXT_NAME, EXT_VER_MAJOR, EXT_VER_MINOR);

	CBUtils::DebugMessage(NULL, "About to get Windows version...");
	char ver_string[MAX_PATH];
	CBUtils::GetWindowsVersion(ver_string);
	LOG(0, "Platform: %s", ver_string);
	CBUtils::DebugMessage(NULL, "About to get DirectX version...");
	CBUtils::GetDXVersion(NULL, ver_string, MAX_PATH);
	LOG(0, "DirectX version: %s", ver_string);
	LOG(0, "");
}


//////////////////////////////////////////////////////////////////////
void CBGame::DEBUG_DebugDisable()
{	
	if(m_DEBUG_LogFile != NULL)
	{
		LOG(0, "********** DEBUG LOG CLOSED ********************************************");
		fclose(m_DEBUG_LogFile);
		m_DEBUG_LogFile = NULL;
	}
	m_DEBUG_DebugMode = false;
}


//////////////////////////////////////////////////////////////////////
void __cdecl CBGame::LOG(HRESULT res, LPSTR fmt, ...)
{
	if(!m_DEBUG_DebugMode || m_DEBUG_LogFile == NULL) return;

	char buff[512];
	va_list va;

	va_start(va, fmt);
	vsprintf(buff, fmt, va);
	va_end(va);

	// redirect to an engine's own callback
	if(m_EngineLogCallback)
	{
		m_EngineLogCallback(buff, res, m_EngineLogCallbackData);
	}
	if(m_DebugMgr) m_DebugMgr->OnLog(res, buff);

	SYSTEMTIME st;
	CBPlatform::GetLocalTime(&st);
	fprintf(m_DEBUG_LogFile, "%02d:%02d: %s\n", st.wHour, st.wMinute, buff);
	if(res!=S_OK) fprintf(m_DEBUG_LogFile, "       returned: %s\n", DXGetErrorString(res));
	fflush(m_DEBUG_LogFile);

	//QuickMessage(buff);
}


//////////////////////////////////////////////////////////////////////////
void CBGame::SetEngineLogCallback(ENGINE_LOG_CALLBACK Callback, void *Data)
{
	m_EngineLogCallback = Callback;
	m_EngineLogCallbackData = Data;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBGame::InitLoop()
{
	m_ViewportSP = -1;
	
	m_CurrentTime = CBPlatform::timeGetTime();
	
	GetDebugMgr()->OnGameTick();
	m_Renderer->InitLoop();
	m_SoundMgr->InitLoop();
	UpdateMusicCrossfade();

	m_NetworkMgr->InitLoop();
	
	if(m_AccessMgr) m_AccessMgr->InitLoop();

	m_SurfaceStorage->InitLoop();
	

	//m_ActiveObject = NULL;

	// count FPS
	m_DeltaTime = m_CurrentTime - m_LastTime;
	m_LastTime  = m_CurrentTime;
	m_FpsTime += m_DeltaTime;

	m_LiveTimerDelta = m_LiveTimer - m_LiveTimerLast;
	m_LiveTimerLast = m_LiveTimer;
	m_LiveTimer += min(1000, m_DeltaTime);	

	if(m_State!=GAME_FROZEN)
	{
		m_TimerDelta = m_Timer - m_TimerLast;
		m_TimerLast = m_Timer;
		m_Timer += min(1000, m_DeltaTime);	
	}
	else m_TimerDelta = 0;

	m_FramesRendered++;
	if (m_FpsTime > 1000)
	{
		m_Fps = m_FramesRendered;
		m_FramesRendered  = 0;
		m_FpsTime = 0;
	}
	//Game->LOG(0, "%d", m_Fps);

	GetMousePos(&m_MousePos);
	
	m_FocusedWindow = NULL;
	for(int i=m_Windows.GetSize()-1; i>=0; i--)
	{
		if(m_Windows[i]->m_Visible)
		{
			m_FocusedWindow = m_Windows[i];
			break;
		}
	}

	UpdateSounds();
	
	if(m_Fader) m_Fader->Update();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBGame::InitInput(HINSTANCE hInst, HWND hWnd)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CBGame::GetSequence()
{
	return ++m_Sequence;
}


//////////////////////////////////////////////////////////////////////////
void CBGame::SetOffset(int OffsetX, int OffsetY)
{
	m_OffsetX = OffsetX;
	m_OffsetY = OffsetY;
}

//////////////////////////////////////////////////////////////////////////
void CBGame::GetOffset(int* OffsetX, int* OffsetY)
{
	if(OffsetX!=NULL) *OffsetX = m_OffsetX;
	if(OffsetY!=NULL) *OffsetY = m_OffsetY;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL)
	{
		Game->LOG(0, "CBGame::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing GAME file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (GAME)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (NAME)
	TOKEN_DEF (SYSTEM_FONT)
	TOKEN_DEF (VIDEO_FONT)
	TOKEN_DEF (EVENTS)
	TOKEN_DEF (CURSOR)
	TOKEN_DEF (ACTIVE_CURSOR)
	TOKEN_DEF (NONINTERACTIVE_CURSOR)
	TOKEN_DEF (STRING_TABLE)
	TOKEN_DEF (RESOLUTION)
	TOKEN_DEF (SETTINGS)
	TOKEN_DEF (REQUIRE_3D_ACCELERATION)
	TOKEN_DEF (REQUIRE_SOUND)
	TOKEN_DEF (HWTL_MODE)
	TOKEN_DEF (ALLOW_WINDOWED_MODE)
	TOKEN_DEF (ALLOW_ACCESSIBILITY_TAB)
	TOKEN_DEF (ALLOW_ABOUT_TAB)
	TOKEN_DEF (ALLOW_ADVANCED)
	TOKEN_DEF (ALLOW_DESKTOP_RES)
	TOKEN_DEF (REGISTRY_PATH)
	TOKEN_DEF (PERSONAL_SAVEGAMES)
	TOKEN_DEF (SCRIPT)
	TOKEN_DEF (CAPTION)
	TOKEN_DEF (PROPERTY)
	TOKEN_DEF (SUBTITLES_SPEED)
	TOKEN_DEF (SUBTITLES)
	TOKEN_DEF (VIDEO_SUBTITLES)
	TOKEN_DEF (EDITOR_PROPERTY)
	TOKEN_DEF (THUMBNAIL_WIDTH)
	TOKEN_DEF (THUMBNAIL_HEIGHT)
	TOKEN_DEF (INDICATOR_X)
	TOKEN_DEF (INDICATOR_Y)
	TOKEN_DEF (INDICATOR_WIDTH)
	TOKEN_DEF (INDICATOR_HEIGHT)
	TOKEN_DEF (INDICATOR_COLOR)
	TOKEN_DEF (SAVE_IMAGE_X)
	TOKEN_DEF (SAVE_IMAGE_Y)
	TOKEN_DEF (SAVE_IMAGE)
	TOKEN_DEF (LOAD_IMAGE_X)
	TOKEN_DEF (LOAD_IMAGE_Y)
	TOKEN_DEF (LOAD_IMAGE)
	TOKEN_DEF (SHADOW_IMAGE)
	TOKEN_DEF (LOCAL_SAVE_DIR)
	TOKEN_DEF (RICH_SAVED_GAMES)
	TOKEN_DEF (SAVED_GAME_EXT)
	TOKEN_DEF (GUID)
	TOKEN_DEF (COMPAT_KILL_METHOD_THREADS)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (GAME)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (SYSTEM_FONT)
		TOKEN_TABLE (VIDEO_FONT)
		TOKEN_TABLE (EVENTS)
		TOKEN_TABLE (CURSOR)
		TOKEN_TABLE (ACTIVE_CURSOR)
		TOKEN_TABLE (NONINTERACTIVE_CURSOR)
		TOKEN_TABLE (PERSONAL_SAVEGAMES)
		TOKEN_TABLE (SCRIPT)
		TOKEN_TABLE (CAPTION)
		TOKEN_TABLE (PROPERTY)
		TOKEN_TABLE (SUBTITLES_SPEED)
		TOKEN_TABLE (SUBTITLES)
		TOKEN_TABLE (VIDEO_SUBTITLES)
		TOKEN_TABLE (EDITOR_PROPERTY)
		TOKEN_TABLE (THUMBNAIL_WIDTH)
		TOKEN_TABLE (THUMBNAIL_HEIGHT)
		TOKEN_TABLE (INDICATOR_X)
		TOKEN_TABLE (INDICATOR_Y)
		TOKEN_TABLE (INDICATOR_WIDTH)
		TOKEN_TABLE (INDICATOR_HEIGHT)
		TOKEN_TABLE (INDICATOR_COLOR)
		TOKEN_TABLE (SAVE_IMAGE_X)
		TOKEN_TABLE (SAVE_IMAGE_Y)
		TOKEN_TABLE (SAVE_IMAGE)
		TOKEN_TABLE (LOAD_IMAGE_X)
		TOKEN_TABLE (LOAD_IMAGE_Y)
		TOKEN_TABLE (LOAD_IMAGE)
		TOKEN_TABLE (SHADOW_IMAGE)
		TOKEN_TABLE (LOCAL_SAVE_DIR)
		TOKEN_TABLE (COMPAT_KILL_METHOD_THREADS)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete)
	{
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_GAME)
		{
			Game->LOG(0, "'GAME' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_TEMPLATE:
				if(FAILED(LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_CAPTION:
				SetCaption((char*)params);
			break;

			case TOKEN_SYSTEM_FONT:
				if(m_SystemFont) m_FontStorage->RemoveFont(m_SystemFont);
				m_SystemFont = NULL;

				m_SystemFont = Game->m_FontStorage->AddFont((char*)params);
			break;

			case TOKEN_VIDEO_FONT:
				if(m_VideoFont) m_FontStorage->RemoveFont(m_VideoFont);
				m_VideoFont = NULL;

				m_VideoFont = Game->m_FontStorage->AddFont((char*)params);
			break;

			case TOKEN_SHADOW_IMAGE:
				if(m_ShadowImage) m_SurfaceStorage->RemoveSurface(m_ShadowImage);
				m_ShadowImage = NULL;

				m_ShadowImage = Game->m_SurfaceStorage->AddSurface((char*)params);
			break;


			case TOKEN_CURSOR:
				SAFE_DELETE(m_Cursor);
				m_Cursor = new CBSprite(Game);
				if(!m_Cursor || FAILED(m_Cursor->LoadFile((char*)params))){
					SAFE_DELETE(m_Cursor);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_ACTIVE_CURSOR:
				SAFE_DELETE(m_ActiveCursor);
				m_ActiveCursor = new CBSprite(Game);
				if(!m_ActiveCursor || FAILED(m_ActiveCursor->LoadFile((char*)params))){
					SAFE_DELETE(m_ActiveCursor);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_NONINTERACTIVE_CURSOR:
				SAFE_DELETE(m_CursorNoninteractive);
				m_CursorNoninteractive = new CBSprite(Game);
				if(!m_CursorNoninteractive || FAILED(m_CursorNoninteractive->LoadFile((char*)params))){
					SAFE_DELETE(m_CursorNoninteractive);
					cmd = PARSERR_GENERIC;
				}
			break;
		
			case TOKEN_SCRIPT:
				AddScript((char*)params);
			break;		

			case TOKEN_PERSONAL_SAVEGAMES:
				parser.ScanStr((char*)params, "%b", &m_PersonalizedSave);
			break;

			case TOKEN_SUBTITLES:
				parser.ScanStr((char*)params, "%b", &m_Subtitles);
			break;

			case TOKEN_SUBTITLES_SPEED:
				parser.ScanStr((char*)params, "%d", &m_SubtitlesSpeed);
			break;

			case TOKEN_VIDEO_SUBTITLES:
				parser.ScanStr((char*)params, "%b", &m_VideoSubtitles);
			break;

			case TOKEN_PROPERTY:
				ParseProperty(params, false);
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;

			case TOKEN_THUMBNAIL_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_ThumbnailWidth);
			break;

			case TOKEN_THUMBNAIL_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_ThumbnailHeight);
			break;

			case TOKEN_INDICATOR_X:
				parser.ScanStr((char*)params, "%d", &m_IndicatorX);
			break;

			case TOKEN_INDICATOR_Y:
				parser.ScanStr((char*)params, "%d", &m_IndicatorY);
			break;

			case TOKEN_INDICATOR_COLOR:
			{
				int r, g, b, a;
				parser.ScanStr((char*)params, "%d,%d,%d,%d", &r, &g, &b, &a);
				m_IndicatorColor = DRGBA(r, g, b, a);
			}
			break;

			case TOKEN_INDICATOR_WIDTH:
				parser.ScanStr((char*)params, "%d", &m_IndicatorWidth);
			break;

			case TOKEN_INDICATOR_HEIGHT:
				parser.ScanStr((char*)params, "%d", &m_IndicatorHeight);
			break;

			case TOKEN_SAVE_IMAGE:
				CBUtils::SetString(&m_SaveImageName, (char*)params);
			break;

			case TOKEN_SAVE_IMAGE_X:
				parser.ScanStr((char*)params, "%d", &m_SaveImageX);
			break;

			case TOKEN_SAVE_IMAGE_Y:
				parser.ScanStr((char*)params, "%d", &m_SaveImageY);
			break;

			case TOKEN_LOAD_IMAGE:
				CBUtils::SetString(&m_LoadImageName, (char*)params);
			break;

			case TOKEN_LOAD_IMAGE_X:
				parser.ScanStr((char*)params, "%d", &m_LoadImageX);
			break;

			case TOKEN_LOAD_IMAGE_Y:
				parser.ScanStr((char*)params, "%d", &m_LoadImageY);
			break;

			case TOKEN_LOCAL_SAVE_DIR:
				CBUtils::SetString(&m_LocalSaveDir, (char*)params);
			break;

			case TOKEN_COMPAT_KILL_METHOD_THREADS:
				parser.ScanStr((char*)params, "%b", &m_CompatKillMethodThreads);
			break;
		}
	}

	if(!m_SystemFont) m_SystemFont = Game->m_FontStorage->AddFont("system_font.fnt");
	if(!m_ShadowImage) m_ShadowImage = Game->m_SurfaceStorage->AddSurface("shadow.png");


	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in GAME definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading GAME definition");
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// LOG
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "LOG")==0)
	{
		Stack->CorrectParams(1);
		LOG(0, Stack->Pop()->GetString());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Caption
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Caption")==0)
	{
		HRESULT res = CBObject::ScCallMethod(Script, Stack, ThisStack, Name);
		SetWindowTitle();
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	// Msg
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Msg")==0)
	{
		Stack->CorrectParams(1);
		QuickMessage(Stack->Pop()->GetString());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RunScript
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RunScript")==0)
	{
		Game->LOG(0, "**Warning** The 'RunScript' method is now obsolete. Use 'AttachScript' instead (same syntax)");
		Stack->CorrectParams(1);
		if(FAILED(AddScript(Stack->Pop()->GetString())))
			Stack->PushBool(false);
		else
			Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// LoadStringTable
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "LoadStringTable")==0)
	{
		Stack->CorrectParams(2);
		char* Filename = Stack->Pop()->GetString();
		CScValue* Val = Stack->Pop();

		bool ClearOld;
		if(Val->IsNULL()) ClearOld = true;
		else ClearOld = Val->GetBool();
		
		if(FAILED(m_StringTable->LoadFile(Filename, ClearOld)))
			Stack->PushBool(false);
		else
			Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ValidObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ValidObject")==0)
	{
		Stack->CorrectParams(1);
		CBScriptable* obj = Stack->Pop()->GetNative();
		if(ValidObject((CBObject*) obj)) Stack->PushBool(true);
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Reset
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Reset")==0)
	{
		Stack->CorrectParams(0);
		ResetContent();
		Stack->PushNULL();

		return S_OK;
	}


	//////////////////////////////////////////////////////////////////////////
	// UnloadObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "UnloadObject")==0)
	{
		Stack->CorrectParams(1);
		CScValue* val = Stack->Pop();
		CBObject* obj = (CBObject*)val->GetNative();
		UnregisterObject(obj);
		if(val->GetType()==VAL_VARIABLE_REF) val->SetNULL();

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// LoadWindow
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "LoadWindow")==0)
	{
		Stack->CorrectParams(1);
		CUIWindow* win = new CUIWindow(Game);
		if(win && SUCCEEDED(win->LoadFile(Stack->Pop()->GetString())))
		{
			m_Windows.Add(win);
			RegisterObject(win);
			Stack->PushNative(win, true);
		}
		else
		{
			SAFE_DELETE(win);
			Stack->PushNULL();
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ExpandString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ExpandString")==0)
	{
		Stack->CorrectParams(1);
		CScValue* val = Stack->Pop();
		char* str = new char[strlen(val->GetString())+1];
		strcpy(str, val->GetString());
		m_StringTable->Expand(&str);
		Stack->PushString(str);
		delete [] str;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayMusic / PlayMusicChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayMusic")==0 || strcmp(Name, "PlayMusicChannel")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "PlayMusic")==0) Stack->CorrectParams(4);
		else
		{
			Stack->CorrectParams(5);
			Channel = Stack->Pop()->GetInt();
		}
		
		char* Filename = Stack->Pop()->GetString();
		CScValue* ValLooping = Stack->Pop();
		bool Looping = ValLooping->IsNULL()?true:ValLooping->GetBool();

		CScValue* ValLoopStart = Stack->Pop();
		DWORD LoopStart = (DWORD)(ValLoopStart->IsNULL()?0:ValLoopStart->GetInt());

		CScValue* ValPrivVolume = Stack->Pop();
		DWORD PrivVolume = (DWORD)(ValPrivVolume->IsNULL()?100:ValPrivVolume->GetInt());

		if(FAILED(PlayMusic(Channel, Filename, Looping, LoopStart, PrivVolume))) Stack->PushBool(false);
		else Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopMusic / StopMusicChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopMusic")==0 || strcmp(Name, "StopMusicChannel")==0)
	{
		int Channel = 0;

		if(strcmp(Name, "StopMusic")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}
		
		if(FAILED(StopMusic(Channel))) Stack->PushBool(false);
		else Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PauseMusic / PauseMusicChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PauseMusic")==0 || strcmp(Name, "PauseMusicChannel")==0)
	{
		int Channel = 0;

		if(strcmp(Name, "PauseMusic")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}
		
		if(FAILED(PauseMusic(Channel))) Stack->PushBool(false);
		else Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ResumeMusic / ResumeMusicChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ResumeMusic")==0 || strcmp(Name, "ResumeMusicChannel")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "ResumeMusic")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}
		
		if(FAILED(ResumeMusic(Channel))) Stack->PushBool(false);
		else Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetMusic / GetMusicChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetMusic")==0 || strcmp(Name, "GetMusicChannel")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "GetMusic")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}
		if(Channel<0 || Channel>=NUM_MUSIC_CHANNELS) Stack->PushNULL();
		else
		{
			if(!m_Music[Channel] || !m_Music[Channel]->m_SoundFilename) Stack->PushNULL();
			else Stack->PushString(m_Music[Channel]->m_SoundFilename);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetMusicPosition / SetMusicChannelPosition
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetMusicPosition")==0 || strcmp(Name, "SetMusicChannelPosition")==0 || strcmp(Name, "SetMusicPositionChannel")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "SetMusicPosition")==0) Stack->CorrectParams(1);
		else
		{
			Stack->CorrectParams(2);
			Channel = Stack->Pop()->GetInt();
		}
		
		DWORD Time = Stack->Pop()->GetInt();
		
		if(FAILED(SetMusicStartTime(Channel, Time))) Stack->PushBool(false);
		else Stack->PushBool(true);
		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetMusicPosition / GetMusicChannelPosition
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetMusicPosition")==0 || strcmp(Name, "GetMusicChannelPosition")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "GetMusicPosition")==0)	Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}

		if(Channel<0 || Channel>=NUM_MUSIC_CHANNELS || !m_Music[Channel]) Stack->PushInt(0);
		else Stack->PushInt(m_Music[Channel]->GetPositionTime());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsMusicPlaying / IsMusicChannelPlaying
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsMusicPlaying")==0 || strcmp(Name, "IsMusicChannelPlaying")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "IsMusicPlaying")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}

		if(Channel<0 || Channel>=NUM_MUSIC_CHANNELS || !m_Music[Channel]) Stack->PushBool(false);
		else Stack->PushBool(m_Music[Channel]->IsPlaying());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetMusicVolume / SetMusicChannelVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetMusicVolume")==0 || strcmp(Name, "SetMusicChannelVolume")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "SetMusicVolume")==0) Stack->CorrectParams(1);
		else
		{
			Stack->CorrectParams(2);
			Channel = Stack->Pop()->GetInt();
		}

		int Volume = Stack->Pop()->GetInt();
		if(Channel<0 || Channel>=NUM_MUSIC_CHANNELS || !m_Music[Channel]) Stack->PushBool(false);
		else
		{
			if(FAILED(m_Music[Channel]->SetVolume(Volume))) Stack->PushBool(false);
			else Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetMusicVolume / GetMusicChannelVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetMusicVolume")==0 || strcmp(Name, "GetMusicChannelVolume")==0)
	{
		int Channel = 0;
		if(strcmp(Name, "GetMusicVolume")==0) Stack->CorrectParams(0);
		else
		{
			Stack->CorrectParams(1);
			Channel = Stack->Pop()->GetInt();
		}

		if(Channel<0 || Channel>=NUM_MUSIC_CHANNELS || !m_Music[Channel]) Stack->PushInt(0);
		else Stack->PushInt(m_Music[Channel]->GetVolume());

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MusicCrossfade
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MusicCrossfade")==0)
	{
		Stack->CorrectParams(4);
		int Channel1 = Stack->Pop()->GetInt(0);
		int Channel2 = Stack->Pop()->GetInt(0);
		DWORD FadeLength = (DWORD)Stack->Pop()->GetInt(0);
		bool Swap = Stack->Pop()->GetBool(true);

		if(m_MusicCrossfadeRunning)
		{
			Script->RuntimeError("Game.MusicCrossfade: Music crossfade is already in progress.");
			Stack->PushBool(false);
			return S_OK;
		}

		m_MusicCrossfadeStartTime = m_LiveTimer;
		m_MusicCrossfadeChannel1 = Channel1;
		m_MusicCrossfadeChannel2 = Channel2;
		m_MusicCrossfadeLength = FadeLength;
		m_MusicCrossfadeSwap = Swap;

		m_MusicCrossfadeRunning = true;

		Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetSoundLength
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetSoundLength")==0)
	{
		Stack->CorrectParams(1);

		int Length = 0;
		char* Filename = Stack->Pop()->GetString();
		
		CBSound* Sound = new CBSound(Game);
		if(Sound && SUCCEEDED(Sound->SetSound(Filename, SOUND_MUSIC, true)))
		{
			Length = Sound->GetLength();
			SAFE_DELETE(Sound);
		}
		Stack->PushInt(Length);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetMousePos
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetMousePos")==0)
	{
		Stack->CorrectParams(2);
		int x = Stack->Pop()->GetInt();
		int y = Stack->Pop()->GetInt();
		x = max(x, 0); x = min(x, m_Renderer->m_Width);
		y = max(y, 0); y = min(y, m_Renderer->m_Height);
		POINT p;
		p.x = x + m_Renderer->m_DrawOffsetX;
		p.y = y + m_Renderer->m_DrawOffsetY;

		CBPlatform::ClientToScreen(m_Renderer->m_Window, &p);
		CBPlatform::SetCursorPos(p.x, p.y);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// LockMouseRect
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "LockMouseRect")==0)
	{
		Stack->CorrectParams(4);
		int left = Stack->Pop()->GetInt();
		int top = Stack->Pop()->GetInt();
		int right = Stack->Pop()->GetInt();
		int bottom = Stack->Pop()->GetInt();

		if(right < left) CBUtils::Swap(&left, &right);
		if(bottom < top) CBUtils::Swap(&top, &bottom);

		CBPlatform::SetRect(&m_MouseLockRect, left, top, right, bottom);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayVideo
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayVideo")==0)
	{
		Game->LOG(0, "Warning: Game.PlayVideo() is now deprecated. Use Game.PlayTheora() instead.");

		Stack->CorrectParams(6);
		char* Filename = Stack->Pop()->GetString();
		CScValue* valType = Stack->Pop();
		int Type;
		if(valType->IsNULL()) Type = (int)VID_PLAY_STRETCH;
		else Type = valType->GetInt();

		int X = Stack->Pop()->GetInt();
		int Y = Stack->Pop()->GetInt();
		bool FreezeMusic = Stack->Pop()->GetBool(true);

		CScValue* valSub = Stack->Pop();
		char* SubtitleFile = valSub->IsNULL()?NULL:valSub->GetString();

		if(Type < (int)VID_PLAY_POS || Type > (int)VID_PLAY_CENTER) Type = (int)VID_PLAY_STRETCH;

		if(SUCCEEDED(Game->m_VideoPlayer->Initialize(Filename, SubtitleFile)))
		{
			if(SUCCEEDED(Game->m_VideoPlayer->Play((TVideoPlayback)Type, X, Y, FreezeMusic)))
			{
				Stack->PushBool(true);
				Script->Sleep(0);
			}
			else Stack->PushBool(false);
		}
		else Stack->PushBool(false);
		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayTheora
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayTheora")==0)
	{
		Stack->CorrectParams(7);
		char* Filename = Stack->Pop()->GetString();
		CScValue* valType = Stack->Pop();
		int Type;
		if(valType->IsNULL()) Type = (int)VID_PLAY_STRETCH;
		else Type = valType->GetInt();

		int X = Stack->Pop()->GetInt();
		int Y = Stack->Pop()->GetInt();
		bool FreezeMusic = Stack->Pop()->GetBool(true);
		bool DropFrames = Stack->Pop()->GetBool(true);

		CScValue* valSub = Stack->Pop();
		char* SubtitleFile = valSub->IsNULL()?NULL:valSub->GetString();


		if(Type < (int)VID_PLAY_POS || Type > (int)VID_PLAY_CENTER) Type = (int)VID_PLAY_STRETCH;

		SAFE_DELETE(m_TheoraPlayer);
		m_TheoraPlayer = new CVidTheoraPlayer(this);
		if(m_TheoraPlayer && SUCCEEDED(m_TheoraPlayer->Initialize(Filename, SubtitleFile)))
		{
			m_TheoraPlayer->m_DontDropFrames = !DropFrames;
			if(SUCCEEDED(m_TheoraPlayer->Play((TVideoPlayback)Type, X, Y, true, FreezeMusic)))
			{
				Stack->PushBool(true);
				Script->Sleep(0);
			}
			else Stack->PushBool(false);
		}
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// QuitGame
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "QuitGame")==0)
	{
		Stack->CorrectParams(0);		
		Stack->PushNULL();
		m_Quitting = true;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RegWriteNumber
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RegWriteNumber")==0)
	{
		Stack->CorrectParams(2);
		char* Key = Stack->Pop()->GetString();
		int Val = Stack->Pop()->GetInt();
		m_Registry->WriteInt("PrivateSettings", Key, Val);
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RegReadNumber
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RegReadNumber")==0)
	{
		Stack->CorrectParams(2);
		char* Key = Stack->Pop()->GetString();
		int InitVal = Stack->Pop()->GetInt();
		Stack->PushInt(m_Registry->ReadInt("PrivateSettings", Key, InitVal));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RegWriteString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RegWriteString")==0)
	{
		Stack->CorrectParams(2);
		char* Key = Stack->Pop()->GetString();
		char* Val = Stack->Pop()->GetString();
		m_Registry->WriteString("PrivateSettings", Key, Val);
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RegReadString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RegReadString")==0)
	{
		Stack->CorrectParams(2);
		char* Key = Stack->Pop()->GetString();
		char* InitVal = Stack->Pop()->GetString();
		DWORD len = MAX_PATH;
		char str[MAX_PATH+1];
		m_Registry->ReadString("PrivateSettings", Key, str, &len, InitVal);
		Stack->PushString(str);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SaveGame
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SaveGame")==0)
	{
		Stack->CorrectParams(3);
		int Slot = Stack->Pop()->GetInt();
		char* xdesc = Stack->Pop()->GetString();
		char* Desc = new char[strlen(xdesc)+1];
		strcpy(Desc, xdesc);
		bool quick = Stack->Pop()->GetBool(false);

		Stack->PushBool(true);
		if(FAILED(SaveGame(Slot, Desc, quick))){
			Stack->Pop();
			Stack->PushBool(false);
		}
		delete [] Desc;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// LoadGame
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "LoadGame")==0)
	{
		Stack->CorrectParams(1);
		m_ScheduledLoadSlot = Stack->Pop()->GetInt();
		m_Loading = true;
		Stack->PushBool(false);
		Script->Sleep(0);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsSaveSlotUsed
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsSaveSlotUsed")==0)
	{
		Stack->CorrectParams(1);
		int Slot = Stack->Pop()->GetInt();
		Stack->PushBool(IsSaveSlotUsed(Slot));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetSaveSlotDescription
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetSaveSlotDescription")==0)
	{
		Stack->CorrectParams(1);
		int Slot = Stack->Pop()->GetInt();
		char Desc[512];
		Desc[0] = '\0';
		GetSaveSlotDescription(Slot, Desc);
		Stack->PushString(Desc);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// EmptySaveSlot
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "EmptySaveSlot")==0)
	{
		Stack->CorrectParams(1);
		int Slot = Stack->Pop()->GetInt();
		EmptySaveSlot(Slot);
		Stack->PushNULL();
		return S_OK;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// SetGlobalSFXVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetGlobalSFXVolume")==0)
	{
		Stack->CorrectParams(1);
		Game->m_SoundMgr->SetVolumePercent(SOUND_SFX, (BYTE)Stack->Pop()->GetInt());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetGlobalSpeechVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetGlobalSpeechVolume")==0)
	{
		Stack->CorrectParams(1);
		Game->m_SoundMgr->SetVolumePercent(SOUND_SPEECH, (BYTE)Stack->Pop()->GetInt());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetGlobalMusicVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetGlobalMusicVolume")==0)
	{
		Stack->CorrectParams(1);
		Game->m_SoundMgr->SetVolumePercent(SOUND_MUSIC, (BYTE)Stack->Pop()->GetInt());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetGlobalMasterVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetGlobalMasterVolume")==0)
	{
		Stack->CorrectParams(1);
		Game->m_SoundMgr->SetMasterVolumePercent((BYTE)Stack->Pop()->GetInt());
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetGlobalSFXVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetGlobalSFXVolume")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushInt(m_SoundMgr->GetVolumePercent(SOUND_SFX));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetGlobalSpeechVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetGlobalSpeechVolume")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushInt(m_SoundMgr->GetVolumePercent(SOUND_SPEECH));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetGlobalMusicVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetGlobalMusicVolume")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushInt(m_SoundMgr->GetVolumePercent(SOUND_MUSIC));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetGlobalMasterVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetGlobalMasterVolume")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushInt(m_SoundMgr->GetMasterVolumePercent());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetActiveCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetActiveCursor")==0)
	{
		Stack->CorrectParams(1);
		if(SUCCEEDED(SetActiveCursor(Stack->Pop()->GetString()))) Stack->PushBool(true);
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetActiveCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetActiveCursor")==0)
	{
		Stack->CorrectParams(0);
		if(!m_ActiveCursor || !m_ActiveCursor->m_Filename) Stack->PushNULL();
		else Stack->PushString(m_ActiveCursor->m_Filename);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetActiveCursorObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetActiveCursorObject")==0)
	{
		Stack->CorrectParams(0);
		if(!m_ActiveCursor) Stack->PushNULL();
		else Stack->PushNative(m_ActiveCursor, true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RemoveActiveCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveActiveCursor")==0)
	{
		Stack->CorrectParams(0);
		SAFE_DELETE(m_ActiveCursor);
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// HasActiveCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "HasActiveCursor")==0)
	{
		Stack->CorrectParams(0);

		if(m_ActiveCursor) Stack->PushBool(true);
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// FileExists
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FileExists")==0)
	{
		Stack->CorrectParams(1);
		char* Filename = Stack->Pop()->GetString();

		CBFile* File = m_FileManager->OpenFile(Filename, false);
		if(!File) Stack->PushBool(false);
		else
		{
			m_FileManager->CloseFile(File);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// FadeOut / FadeOutAsync / SystemFadeOut / SystemFadeOutAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FadeOut")==0 || strcmp(Name, "FadeOutAsync")==0 || strcmp(Name, "SystemFadeOut")==0 || strcmp(Name, "SystemFadeOutAsync")==0)
	{
		Stack->CorrectParams(5);
		DWORD Duration = Stack->Pop()->GetInt(500);
		BYTE Red = Stack->Pop()->GetInt(0);
		BYTE Green = Stack->Pop()->GetInt(0);
		BYTE Blue = Stack->Pop()->GetInt(0);
		BYTE Alpha = Stack->Pop()->GetInt(0xFF);

		bool System = (strcmp(Name, "SystemFadeOut")==0 || strcmp(Name, "SystemFadeOutAsync")==0);

		m_Fader->FadeOut(DRGBA(Red, Green, Blue, Alpha), Duration, System);
		if(strcmp(Name, "FadeOutAsync")!=0 && strcmp(Name, "SystemFadeOutAsync")!=0) Script->WaitFor(m_Fader);
				
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// FadeIn / FadeInAsync / SystemFadeIn / SystemFadeInAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FadeIn")==0 || strcmp(Name, "FadeInAsync")==0 || strcmp(Name, "SystemFadeIn")==0 || strcmp(Name, "SystemFadeInAsync")==0)
	{
		Stack->CorrectParams(5);
		DWORD Duration = Stack->Pop()->GetInt(500);
		BYTE Red = Stack->Pop()->GetInt(0);
		BYTE Green = Stack->Pop()->GetInt(0);
		BYTE Blue = Stack->Pop()->GetInt(0);
		BYTE Alpha = Stack->Pop()->GetInt(0xFF);

		bool System = (strcmp(Name, "SystemFadeIn")==0 || strcmp(Name, "SystemFadeInAsync")==0);

		m_Fader->FadeIn(DRGBA(Red, Green, Blue, Alpha), Duration, System);
		if(strcmp(Name, "FadeInAsync")!=0 && strcmp(Name, "SystemFadeInAsync")!=0) Script->WaitFor(m_Fader);
				
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFadeColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetFadeColor")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushInt(m_Fader->GetCurrentColor());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Screenshot
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Screenshot")==0)
	{
		Stack->CorrectParams(1);
		char Filename[MAX_PATH];

		CScValue* Val = Stack->Pop();

		int FileNum = 0;
		while(true)
		{
			sprintf(Filename, "%s%03d.bmp", Val->IsNULL()?m_Name:Val->GetString(), FileNum);
			FILE* f = fopen(Filename, "rb");
			if(!f) break;
			else fclose(f);
			FileNum++;
		}

		bool ret = false;
		CBImage* Image = Game->m_Renderer->TakeScreenshot();
		if(Image)
		{
			ret = SUCCEEDED(Image->SaveBMPFile(Filename));
			delete Image;
		}
		else ret = false;
		
		Stack->PushBool(ret);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ScreenshotEx
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ScreenshotEx")==0)
	{
		Stack->CorrectParams(3);
		char* Filename = Stack->Pop()->GetString();
		int SizeX = Stack->Pop()->GetInt(m_Renderer->m_Width);
		int SizeY = Stack->Pop()->GetInt(m_Renderer->m_Height);

		bool ret = false;
		CBImage* Image = Game->m_Renderer->TakeScreenshot();
		if(Image)
		{
			ret = SUCCEEDED(Image->Resize(SizeX, SizeY));
			if(ret) ret = SUCCEEDED(Image->SaveBMPFile(Filename));
			delete Image;
		}
		else ret = false;

		Stack->PushBool(ret);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// CreateWindow
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CreateWindow")==0)
	{
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();
		
		CUIWindow* Win = new CUIWindow(Game);
		m_Windows.Add(Win);
		RegisterObject(Win);
		if(!Val->IsNULL()) Win->SetName(Val->GetString());
		Stack->PushNative(Win, true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DeleteWindow
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DeleteWindow")==0)
	{
		Stack->CorrectParams(1);
		CBObject* Obj = (CBObject*)Stack->Pop()->GetNative();
		for(int i=0; i<m_Windows.GetSize(); i++)
		{
			if(m_Windows[i]==Obj)
			{
				UnregisterObject(m_Windows[i]);
				Stack->PushBool(true);
				return S_OK;
			}
		}		
		Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// OpenDocument
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "OpenDocument")==0)
	{
		Stack->CorrectParams(1);
		if(!m_Renderer->m_Windowed) CBPlatform::ShowWindow(m_Renderer->m_Window, SW_MINIMIZE);
		HINSTANCE i = CBPlatform::ShellExecute(m_Renderer->m_Window, "open", Stack->Pop()->GetString(), NULL, NULL, SW_MAXIMIZE);
		Stack->PushBool((int)i>32);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DEBUG_DumpClassRegistry
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DEBUG_DumpClassRegistry")==0)
	{
		Stack->CorrectParams(0);
		DEBUG_DumpClassRegistry();
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetLoadingScreen
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetLoadingScreen")==0)
	{
		Stack->CorrectParams(3);
		CScValue* Val = Stack->Pop();
		m_LoadImageX = Stack->Pop()->GetInt();
		m_LoadImageY = Stack->Pop()->GetInt();
		
		if(Val->IsNULL()) SAFE_DELETE_ARRAY(m_LoadImageName);
		else CBUtils::SetString(&m_LoadImageName, Val->GetString());

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetSavingScreen
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetSavingScreen")==0)
	{
		Stack->CorrectParams(3);
		CScValue* Val = Stack->Pop();
		m_SaveImageX = Stack->Pop()->GetInt();
		m_SaveImageY = Stack->Pop()->GetInt();

		if(Val->IsNULL()) SAFE_DELETE_ARRAY(m_SaveImageName);
		else CBUtils::SetString(&m_SaveImageName, Val->GetString());

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetWaitCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetWaitCursor")==0)
	{
		Stack->CorrectParams(1);
		if(SUCCEEDED(SetWaitCursor(Stack->Pop()->GetString()))) Stack->PushBool(true);
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RemoveWaitCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveWaitCursor")==0)
	{
		Stack->CorrectParams(0);
		SAFE_DELETE(m_CursorNoninteractive);		
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetWaitCursor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetWaitCursor")==0)
	{
		Stack->CorrectParams(0);
		if(!m_CursorNoninteractive || !m_CursorNoninteractive->m_Filename) Stack->PushNULL();
		else Stack->PushString(m_CursorNoninteractive->m_Filename);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetWaitCursorObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetWaitCursorObject")==0)
	{
		Stack->CorrectParams(0);
		if(!m_CursorNoninteractive) Stack->PushNULL();
		else Stack->PushNative(m_CursorNoninteractive, true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ClearScriptCache
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ClearScriptCache")==0)
	{
		Stack->CorrectParams(0);		
		Stack->PushBool(SUCCEEDED(m_ScEngine->EmptyScriptCache()));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DisplayLoadingIcon
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DisplayLoadingIcon")==0)
	{
		Stack->CorrectParams(4);

		char* Filename = Stack->Pop()->GetString();
		m_LoadingIconX = Stack->Pop()->GetInt();
		m_LoadingIconY = Stack->Pop()->GetInt();
		m_LoadingIconPersistent = Stack->Pop()->GetBool();

		SAFE_DELETE(m_LoadingIcon);
		m_LoadingIcon = new CBSprite(this);
		if(!m_LoadingIcon || FAILED(m_LoadingIcon->LoadFile(Filename)))
		{
			SAFE_DELETE(m_LoadingIcon);
		}
		else
		{
			DisplayContent(false, true);
			Game->m_Renderer->Flip();
			Game->m_Renderer->InitLoop();
		}
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// HideLoadingIcon
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "HideLoadingIcon")==0)
	{
		Stack->CorrectParams(0);		
		SAFE_DELETE(m_LoadingIcon);
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DumpTextureStats
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DumpTextureStats")==0)
	{
		Stack->CorrectParams(1);
		char* Filename = Stack->Pop()->GetString();

		if(!m_UseD3D) Script->RuntimeError("Game.DumpTextureStats is only available in accelerated mode");
		else m_Renderer->DumpData(Filename);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccOutputText
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccOutputText")==0)
	{
		Stack->CorrectParams(2);
		char* Str = Stack->Pop()->GetString();
		int Type = Stack->Pop()->GetInt();
		m_AccessMgr->Speak(Str, (TTTSType)Type);
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsShadowTypeSupported
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsShadowTypeSupported")==0)
	{
		Stack->CorrectParams(1);
		TShadowType Type = (TShadowType)Stack->Pop()->GetInt();

		switch(Type)
		{
			case SHADOW_NONE:
			case SHADOW_SIMPLE:
				Stack->PushBool(true);
				break;

			case SHADOW_FLAT:
				Game->m_Renderer->EnableShadows();
				Stack->PushBool(m_SupportsRealTimeShadows);
				break;

			case SHADOW_STENCIL:
				Stack->PushBool(m_Renderer->StencilSupported());
				break;

			default:
				Stack->PushBool(false);
		}
		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StoreSaveThumbnail
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StoreSaveThumbnail")==0)
	{
		Stack->CorrectParams(0);
		SAFE_DELETE(m_CachedThumbnail);
		m_CachedThumbnail = new CBSaveThumbHelper(this);
		if(FAILED(m_CachedThumbnail->StoreThumbnail()))
		{
			SAFE_DELETE(m_CachedThumbnail);
			Stack->PushBool(false);
		}
		else Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DeleteSaveThumbnail
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DeleteSaveThumbnail")==0)
	{
		Stack->CorrectParams(0);
		SAFE_DELETE(m_CachedThumbnail);
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFileChecksum
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetFileChecksum")==0)
	{
		Stack->CorrectParams(2);
		char* Filename = Stack->Pop()->GetString();
		bool AsHex = Stack->Pop()->GetBool(false);

		CBFile* File = m_FileManager->OpenFile(Filename, false);
		if(File)
		{
			crc remainder = crc_initialize();
			BYTE Buf[1024];
			int BytesRead = 0;

			while(BytesRead < File->GetSize())
			{
				int BufSize = min(1024, File->GetSize() - BytesRead);
				BytesRead += BufSize;

				File->Read(Buf, BufSize);
				for(int i=0; i<BufSize; i++)
				{
					remainder = crc_process_byte(Buf[i], remainder);
				}
			}
			crc checksum = crc_finalize(remainder);

			if(AsHex)
			{
				char Hex[100];
				sprintf(Hex, "%x", checksum);
				Stack->PushString(Hex);
			}
			else
				Stack->PushInt(checksum);

			m_FileManager->CloseFile(File);
			File = NULL;
		}
		else Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// EnableScriptProfiling
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "EnableScriptProfiling")==0)
	{
		Stack->CorrectParams(0);
		m_ScEngine->EnableProfiling();
		Stack->PushNULL();

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DisableScriptProfiling
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DisableScriptProfiling")==0)
	{
		Stack->CorrectParams(0);
		m_ScEngine->DisableProfiling();
		Stack->PushNULL();

		return S_OK;
	}

	else return CBObject::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CBGame::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0)
	{
		m_ScValue->SetString("game");
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Name")==0)
	{
		m_ScValue->SetString(m_Name);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Hwnd (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Hwnd")==0)
	{
		m_ScValue->SetInt((int)m_Renderer->m_Window);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// CurrentTime (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CurrentTime")==0)
	{
		m_ScValue->SetInt((int)m_Timer);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// WindowsTime (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WindowsTime")==0)
	{
		m_ScValue->SetInt((int)CBPlatform::timeGetTime());
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// WindowedMode (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WindowedMode")==0)
	{
		m_ScValue->SetBool(m_Renderer->m_Windowed);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MouseX
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MouseX")==0)
	{
		m_ScValue->SetInt(m_MousePos.x);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MouseY
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MouseY")==0)
	{
		m_ScValue->SetInt(m_MousePos.y);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MainObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MainObject")==0)
	{
		m_ScValue->SetNative(m_MainObject, true);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ActiveObject (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ActiveObject")==0)
	{
		m_ScValue->SetNative(m_ActiveObject, true);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ScreenWidth (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ScreenWidth")==0)
	{
		m_ScValue->SetInt(m_Renderer->m_Width);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ScreenHeight (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ScreenHeight")==0)
	{
		m_ScValue->SetInt(m_Renderer->m_Height);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Interactive
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Interactive")==0)
	{
		m_ScValue->SetBool(m_Interactive);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// DebugMode (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DebugMode")==0)
	{
		m_ScValue->SetBool(m_DEBUG_DebugMode);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SoundAvailable (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SoundAvailable")==0)
	{
		m_ScValue->SetBool(m_SoundMgr->m_SoundAvailable);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SFXVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SFXVolume")==0)
	{
		Game->LOG(0, "**Warning** The SFXVolume attribute is obsolete");
		m_ScValue->SetInt(m_SoundMgr->GetVolumePercent(SOUND_SFX));
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SpeechVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SpeechVolume")==0)
	{
		Game->LOG(0, "**Warning** The SpeechVolume attribute is obsolete");
		m_ScValue->SetInt(m_SoundMgr->GetVolumePercent(SOUND_SPEECH));
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MusicVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MusicVolume")==0)
	{
		Game->LOG(0, "**Warning** The MusicVolume attribute is obsolete");
		m_ScValue->SetInt(m_SoundMgr->GetVolumePercent(SOUND_MUSIC));
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MasterVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MasterVolume")==0)
	{
		Game->LOG(0, "**Warning** The MasterVolume attribute is obsolete");
		m_ScValue->SetInt(m_SoundMgr->GetMasterVolumePercent());
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Keyboard (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Keyboard")==0)
	{
		if(m_KeyboardState) m_ScValue->SetNative(m_KeyboardState, true);
		else m_ScValue->SetNULL();

		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Subtitles
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Subtitles")==0)
	{
		m_ScValue->SetBool(m_Subtitles);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SubtitlesSpeed
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SubtitlesSpeed")==0)
	{
		m_ScValue->SetInt(m_SubtitlesSpeed);
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// VideoSubtitles
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "VideoSubtitles")==0)
	{
		m_ScValue->SetBool(m_VideoSubtitles);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// FPS (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "FPS")==0)
	{
		m_ScValue->SetInt(m_Fps);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Shadows (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Shadows")==0)
	{
		m_ScValue->SetBool(m_MaxShadowType > SHADOW_NONE);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SimpleShadows (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SimpleShadows")==0)
	{
		m_ScValue->SetBool(m_MaxShadowType == SHADOW_SIMPLE);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SupportsRealTimeShadows (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SupportsRealTimeShadows")==0)
	{
		Game->m_Renderer->EnableShadows();
		m_ScValue->SetBool(m_SupportsRealTimeShadows);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaxShadowType
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MaxShadowType")==0)
	{
		m_ScValue->SetInt(m_MaxShadowType);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AcceleratedMode / Accelerated (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AcceleratedMode")==0 || strcmp(Name, "Accelerated")==0)
	{
		m_ScValue->SetBool(m_UseD3D);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TextEncoding
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TextEncoding")==0)
	{
		m_ScValue->SetInt(m_TextEncoding);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TextRTL
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TextRTL")==0)
	{
		m_ScValue->SetBool(m_TextRTL);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SoundBufferSize
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SoundBufferSize")==0)
	{
		m_ScValue->SetInt(m_SoundBufferSizeSec);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SuspendedRendering
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SuspendedRendering")==0)
	{
		m_ScValue->SetBool(m_SuspendedRendering);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SuppressScriptErrors
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SuppressScriptErrors")==0)
	{
		m_ScValue->SetBool(m_SuppressScriptErrors);
		return m_ScValue;
	}


	//////////////////////////////////////////////////////////////////////////
	// Frozen
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Frozen")==0)
	{
		m_ScValue->SetBool(m_State==GAME_FROZEN);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Direct3DDevice
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Direct3DDevice")==0)
	{
		if(Game->m_UseD3D)
		{
			m_ScValue->SetInt((int)((CBRenderD3D*)m_Renderer)->m_Device);
		}
		else m_ScValue->SetNULL();
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectDrawInterface
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectDrawInterface")==0)
	{
		if(!Game->m_UseD3D)
		{
			m_ScValue->SetInt((int)((CBRenderDD*)m_Renderer)->m_DirectDraw);
		}
		else m_ScValue->SetNULL();
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccTTSEnabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccTTSEnabled")==0)
	{
		m_ScValue->SetBool(m_AccessTTSEnabled);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccTTSTalk
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccTTSTalk")==0)
	{
		m_ScValue->SetBool(m_AccessTTSTalk);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccTTSCaptions
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccTTSCaptions")==0)
	{
		m_ScValue->SetBool(m_AccessTTSCaptions);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccTTSKeypress
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccTTSKeypress")==0)
	{
		m_ScValue->SetBool(m_AccessTTSKeypress);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccKeyboardEnabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccKeyboardEnabled")==0)
	{
		m_ScValue->SetBool(m_AccessKeyboardEnabled);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccKeyboardCursorSkip
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccKeyboardCursorSkip")==0)
	{
		m_ScValue->SetBool(m_AccessKeyboardCursorSkip);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AccKeyboardPause
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AccKeyboardPause")==0)
	{
		m_ScValue->SetBool(m_AccessKeyboardPause);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// UsedMemory (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "UsedMemory")==0)
	{
		m_ScValue->SetInt(CBUtils::GetUsedMemMB());
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaxActiveLights (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MaxActiveLights")==0)
	{
		if(m_UseD3D)
		{
			CBRenderD3D* Rend = (CBRenderD3D*)m_Renderer;
			m_ScValue->SetInt(Rend->GetMaxActiveLights());
		}
		else m_ScValue->SetInt(0);

		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// HardwareTL (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "HardwareTL")==0)
	{
		if(m_UseD3D)
		{
			CBRenderD3D* Rend = (CBRenderD3D*)m_Renderer;
			m_ScValue->SetBool(Rend->m_HardwareTL);
		}
		else m_ScValue->SetBool(false);

		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AutorunDisabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AutorunDisabled")==0)
	{
		m_ScValue->SetBool(m_AutorunDisabled);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// SaveDirectory (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SaveDirectory")==0)
	{
		char SaveDir[MAX_PATH+1];
		GetSaveDir(SaveDir);
		m_ScValue->SetString(SaveDir);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// MostRecentSaveSlot (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MostRecentSaveSlot")==0)
	{
		m_ScValue->SetInt(m_Registry->ReadInt("System", "MostRecentSaveSlot", -1));
		return m_ScValue;
	}

	else return CBObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Name")==0)
	{
		SetName(Value->GetString());

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MouseX
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MouseX")==0)
	{
		m_MousePos.x = Value->GetInt();
		ResetMousePos();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MouseY
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MouseY")==0)
	{
		m_MousePos.y = Value->GetInt();
		ResetMousePos();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Caption
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Name")==0)
	{
		HRESULT res = CBObject::ScSetProperty(Name, Value);
		SetWindowTitle();
		return res;
	}

	//////////////////////////////////////////////////////////////////////////
	// MainObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MainObject")==0)
	{
		CBScriptable* obj = Value->GetNative();
		if(obj==NULL || ValidObject((CBObject*)obj)) m_MainObject = (CBObject*)obj;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Interactive
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Interactive")==0)
	{
		SetInteractive(Value->GetBool());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SFXVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SFXVolume")==0)
	{
		Game->LOG(0, "**Warning** The SFXVolume attribute is obsolete");
		Game->m_SoundMgr->SetVolumePercent(SOUND_SFX, (BYTE)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SpeechVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SpeechVolume")==0)
	{
		Game->LOG(0, "**Warning** The SpeechVolume attribute is obsolete");
		Game->m_SoundMgr->SetVolumePercent(SOUND_SPEECH, (BYTE)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MusicVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MusicVolume")==0)
	{
		Game->LOG(0, "**Warning** The MusicVolume attribute is obsolete");
		Game->m_SoundMgr->SetVolumePercent(SOUND_MUSIC, (BYTE)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MasterVolume
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MasterVolume")==0)
	{
		Game->LOG(0, "**Warning** The MasterVolume attribute is obsolete");
		Game->m_SoundMgr->SetMasterVolumePercent((BYTE)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Subtitles
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Subtitles")==0)
	{
		m_Subtitles = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SubtitlesSpeed
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SubtitlesSpeed")==0)
	{
		m_SubtitlesSpeed = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// VideoSubtitles
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "VideoSubtitles")==0)
	{
		m_VideoSubtitles = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Shadows (obsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Shadows")==0)
	{
		if(Value->GetBool()) SetMaxShadowType(SHADOW_STENCIL);
		else SetMaxShadowType(SHADOW_NONE);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SimpleShadows (onsolete)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SimpleShadows")==0)
	{
		if(Value->GetBool()) SetMaxShadowType(SHADOW_SIMPLE);
		else SetMaxShadowType(SHADOW_STENCIL);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MaxShadowType
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MaxShadowType")==0)
	{
		SetMaxShadowType((TShadowType)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TextEncoding
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TextEncoding")==0)
	{
		int Enc = Value->GetInt();
		if(Enc<0) Enc = 0;
		if(Enc>=NUM_TEXT_ENCODINGS) Enc = NUM_TEXT_ENCODINGS - 1;
		m_TextEncoding = (TTextEncoding)Enc;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TextRTL
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TextRTL")==0)
	{
		m_TextRTL = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SoundBufferSize
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SoundBufferSize")==0)
	{
		m_SoundBufferSizeSec = Value->GetInt();
		m_SoundBufferSizeSec = max(3, m_SoundBufferSizeSec);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SuspendedRendering
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SuspendedRendering")==0)
	{
		m_SuspendedRendering = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SuppressScriptErrors
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SuppressScriptErrors")==0)
	{
		m_SuppressScriptErrors = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AutorunDisabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AutorunDisabled")==0)
	{
		m_AutorunDisabled = Value->GetBool();
		return S_OK;
	}

	else return CBObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CBGame::ScToString()
{
	return "[game object]";
}



#define QUICK_MSG_DURATION 3000
//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayQuickMsg()
{
	if(m_QuickMessages.GetSize()==0 || !m_SystemFont) return S_OK;

	int i;

	// update
	for(i=0; i<m_QuickMessages.GetSize(); i++)
	{
		if(m_CurrentTime - m_QuickMessages[i]->m_StartTime >= QUICK_MSG_DURATION)
		{
			delete m_QuickMessages[i];
			m_QuickMessages.RemoveAt(i);
			i--;
		}
	}

	int PosY=20;

	// display
	for(i=0; i<m_QuickMessages.GetSize(); i++)
	{
		m_SystemFont->DrawText((BYTE*)m_QuickMessages[i]->GetText(), 0, PosY, m_Renderer->m_Width);
		PosY+=m_SystemFont->GetTextHeight((BYTE*)m_QuickMessages[i]->GetText(), m_Renderer->m_Width);
	}
	return S_OK;
}


#define MAX_QUICK_MSG 5
//////////////////////////////////////////////////////////////////////////
void CBGame::QuickMessage(char *Text)
{
	if(m_QuickMessages.GetSize()>=MAX_QUICK_MSG)
	{
		delete m_QuickMessages[0];
		m_QuickMessages.RemoveAt(0);
	}
	m_QuickMessages.Add(new CBQuickMsg(Game, Text));
}


//////////////////////////////////////////////////////////////////////////
void CBGame::QuickMessageForm(LPSTR fmt, ...)
{
	char buff[256];
	va_list va;

	va_start(va, fmt);
	vsprintf(buff, fmt, va);
	va_end(va);
	
	QuickMessage(buff);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::RegisterObject(CBObject *Object)
{
	m_RegObjects.Add(Object);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::UnregisterObject(CBObject *Object)
{
	if(!Object) return S_OK;

	int i;

	// is it a window?
	for(i=0; i<m_Windows.GetSize(); i++)
	{
		if((CBObject*)m_Windows[i] == Object)
		{
			m_Windows.RemoveAt(i);

			// get new focused window
			if(m_FocusedWindow==Object) m_FocusedWindow = NULL;

			break;
		}
	}

	// is it active object?
	if(m_ActiveObject==Object) m_ActiveObject = NULL;

	// is it main object?
	if(m_MainObject==Object) m_MainObject = NULL;

	// is it active accessibility object?
	if(m_AccessMgr && m_AccessMgr->GetActiveObject()==Object) m_AccessMgr->SetActiveObject(NULL);

	// destroy object
	for(i=0; i<m_RegObjects.GetSize(); i++)
	{
		if(m_RegObjects[i] == Object)
		{
			m_RegObjects.RemoveAt(i);
			if(!m_LoadInProgress) CSysClassRegistry::GetInstance()->EnumInstances(InvalidateValues, "CScValue", (void*)Object);
			delete Object;
			return S_OK;
		}
	}

	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
void CBGame::InvalidateValues(void* Value, void* Data)
{
	CScValue* val = (CScValue*)Value;
	if(val->IsNative() && val->GetNative()==Data)
	{
		if(!val->m_Persistent && ((CBScriptable*)Data)->m_RefCount==1)
		{
			((CBScriptable*)Data)->m_RefCount++;
		}
		val->SetNative(NULL);
		val->SetNULL();
	}
}



//////////////////////////////////////////////////////////////////////////
bool CBGame::ValidObject(CBObject *Object)
{
	if(!Object) return false;
	if(Object==this) return true;
	
	for(int i=0; i<m_RegObjects.GetSize(); i++)
	{
		if(m_RegObjects[i] == Object) return true;
	}
	return m_PluginMgr->ValidObject(Object);
}


//////////////////////////////////////////////////////////////////////////
void CBGame::PublishNatives()
{
	if(!m_ScEngine || !m_ScEngine->m_CompilerAvailable) return;

	m_ScEngine->ExtDefineFunction("LOG");
	m_ScEngine->ExtDefineFunction("String");
	m_ScEngine->ExtDefineFunction("MemBuffer");
	m_ScEngine->ExtDefineFunction("File");
	m_ScEngine->ExtDefineFunction("Date");
	m_ScEngine->ExtDefineFunction("Array");
	m_ScEngine->ExtDefineFunction("TcpClient");
	m_ScEngine->ExtDefineFunction("Object");
	//m_ScEngine->ExtDefineFunction("Game");
	m_ScEngine->ExtDefineFunction("Sleep");
	m_ScEngine->ExtDefineFunction("WaitFor");
	m_ScEngine->ExtDefineFunction("Random");
	m_ScEngine->ExtDefineFunction("SetScriptTimeSlice");
	m_ScEngine->ExtDefineFunction("MakeRGBA");
	m_ScEngine->ExtDefineFunction("MakeRGB");
	m_ScEngine->ExtDefineFunction("MakeHSL");
	m_ScEngine->ExtDefineFunction("RGB");
	m_ScEngine->ExtDefineFunction("GetRValue");
	m_ScEngine->ExtDefineFunction("GetGValue");
	m_ScEngine->ExtDefineFunction("GetBValue");
	m_ScEngine->ExtDefineFunction("GetAValue");
	m_ScEngine->ExtDefineFunction("GetHValue");
	m_ScEngine->ExtDefineFunction("GetSValue");
	m_ScEngine->ExtDefineFunction("GetLValue");
	m_ScEngine->ExtDefineFunction("Debug");

	m_ScEngine->ExtDefineFunction("ToString");
	m_ScEngine->ExtDefineFunction("ToInt");
	m_ScEngine->ExtDefineFunction("ToBool");
	m_ScEngine->ExtDefineFunction("ToFloat");

	m_ScEngine->ExtDefineVariable("Game");
	m_ScEngine->ExtDefineVariable("Math");
	m_ScEngine->ExtDefineVariable("Directory");
	m_ScEngine->ExtDefineVariable("self");
	m_ScEngine->ExtDefineVariable("this");

	// register plugin classes
	m_PluginMgr->RegisterClasses();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name)
{
	CScValue* this_obj;

	//////////////////////////////////////////////////////////////////////////
	// LOG
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "LOG")==0)
	{
		Stack->CorrectParams(1);
		Game->LOG(0, "sc: %s", Stack->Pop()->GetString());
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// String
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "String")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXString(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// MemBuffer
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MemBuffer")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXMemBuffer(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// File
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "File")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXFile(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// Date
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Date")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXDate(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// Array
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Array")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXArray(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// TcpClient
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TcpClient")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXTcpClient(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// Object
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Object")==0)
	{
		this_obj = ThisStack->GetTop();

		this_obj->SetNative(new CSXObject(Game, Stack));
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// Sleep
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Sleep")==0)
	{
		Stack->CorrectParams(1);
		
		Script->Sleep((DWORD)Stack->Pop()->GetInt());
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// WaitFor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WaitFor")==0)
	{
		Stack->CorrectParams(1);

		CBScriptable* obj = Stack->Pop()->GetNative();
		if(ValidObject((CBObject*)obj)) Script->WaitForExclusive((CBObject*)obj);
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// Random
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Random")==0)
	{
		Stack->CorrectParams(2);
		
		int from = Stack->Pop()->GetInt();
		int to   = Stack->Pop()->GetInt();

		Stack->PushInt(CBUtils::RandomInt(from, to));
	}

	//////////////////////////////////////////////////////////////////////////
	// SetScriptTimeSlice
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetScriptTimeSlice")==0)
	{
		Stack->CorrectParams(1);
		
		Script->m_TimeSlice = (DWORD)Stack->Pop()->GetInt();
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// MakeRGBA / MakeRGB / RGB
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MakeRGBA")==0 || strcmp(Name, "MakeRGB")==0 || strcmp(Name, "RGB")==0)
	{
		Stack->CorrectParams(4);
		int r = Stack->Pop()->GetInt();
		int g = Stack->Pop()->GetInt();
		int b = Stack->Pop()->GetInt();
		int a;
		CScValue* val = Stack->Pop();
		if(val->IsNULL()) a = 255;
		else a = val->GetInt();

		Stack->PushInt(DRGBA(r, g, b, a));
	}

	//////////////////////////////////////////////////////////////////////////
	// MakeHSL
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MakeHSL")==0)
	{
		Stack->CorrectParams(3);
		int h = Stack->Pop()->GetInt();
		int s = Stack->Pop()->GetInt();
		int l = Stack->Pop()->GetInt();

		Stack->PushInt(CBUtils::HSLtoRGB(h, s, l));
	}

	//////////////////////////////////////////////////////////////////////////
	// GetRValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetRValue")==0)
	{
		Stack->CorrectParams(1);
		
		DWORD rgba = (DWORD)Stack->Pop()->GetInt();
		Stack->PushInt(D3DCOLGetR(rgba));
	}

	//////////////////////////////////////////////////////////////////////////
	// GetGValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetGValue")==0)
	{
		Stack->CorrectParams(1);
		
		DWORD rgba = (DWORD)Stack->Pop()->GetInt();
		Stack->PushInt(D3DCOLGetG(rgba));
	}

	//////////////////////////////////////////////////////////////////////////
	// GetBValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetBValue")==0)
	{
		Stack->CorrectParams(1);
		
		DWORD rgba = (DWORD)Stack->Pop()->GetInt();
		Stack->PushInt(D3DCOLGetB(rgba));
	}

	//////////////////////////////////////////////////////////////////////////
	// GetAValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetAValue")==0)
	{
		Stack->CorrectParams(1);
		
		DWORD rgba = (DWORD)Stack->Pop()->GetInt();
		Stack->PushInt(D3DCOLGetA(rgba));
	}

	//////////////////////////////////////////////////////////////////////////
	// GetHValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetHValue")==0)
	{
		Stack->CorrectParams(1);
		DWORD rgb = (DWORD)Stack->Pop()->GetInt();

		BYTE H, S, L;
		CBUtils::RGBtoHSL(rgb, &H, &S, &L);
		Stack->PushInt(H);
	}

	//////////////////////////////////////////////////////////////////////////
	// GetSValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetSValue")==0)
	{
		Stack->CorrectParams(1);
		DWORD rgb = (DWORD)Stack->Pop()->GetInt();

		BYTE H, S, L;
		CBUtils::RGBtoHSL(rgb, &H, &S, &L);
		Stack->PushInt(S);
	}

	//////////////////////////////////////////////////////////////////////////
	// GetLValue
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetLValue")==0)
	{
		Stack->CorrectParams(1);
		DWORD rgb = (DWORD)Stack->Pop()->GetInt();

		BYTE H, S, L;
		CBUtils::RGBtoHSL(rgb, &H, &S, &L);
		Stack->PushInt(L);
	}

	//////////////////////////////////////////////////////////////////////////
	// Debug
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Debug")==0)
	{
		Stack->CorrectParams(0);
		
		if(Game->GetDebugMgr()->m_Enabled)
		{
			Game->GetDebugMgr()->OnScriptHitBreakpoint(Script);
			Script->Sleep(0);
		}
		Stack->PushNULL();
	}

	//////////////////////////////////////////////////////////////////////////
	// ToString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToString")==0)
	{
		Stack->CorrectParams(1);
		char* Str = Stack->Pop()->GetString();
		char* Str2 = new char[strlen(Str)+1];
		strcpy(Str2, Str);
		Stack->PushString(Str2);
		delete [] Str2;
	}

	//////////////////////////////////////////////////////////////////////////
	// ToInt
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToInt")==0)
	{
		Stack->CorrectParams(1);
		int Val = Stack->Pop()->GetInt();
		Stack->PushInt(Val);
	}

	//////////////////////////////////////////////////////////////////////////
	// ToFloat
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToFloat")==0)
	{
		Stack->CorrectParams(1);
		double Val = Stack->Pop()->GetFloat();
		Stack->PushFloat(Val);
	}

	//////////////////////////////////////////////////////////////////////////
	// ToBool
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToBool")==0)
	{
		Stack->CorrectParams(1);
		bool Val = Stack->Pop()->GetBool();
		Stack->PushBool(Val);
	}

	//////////////////////////////////////////////////////////////////////////
	// plugins/failure
	else
	{
		if(FAILED(m_PluginMgr->HandleExternalCall(Script, Stack, ThisStack, Name)))
		{
			Script->RuntimeError("Call to undefined function '%s'. Ignored.", Name);
			Stack->CorrectParams(0);
			Stack->PushNULL();
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ShowCursor()
{
	if(!m_Interactive && Game->m_State==GAME_RUNNING)
	{
		if(m_CursorNoninteractive) return DrawCursor(m_CursorNoninteractive);
	}
	else
	{
		if(m_ActiveObject && !FAILED(m_ActiveObject->ShowCursor())) return S_OK;
		else
		{
			if(m_ActiveObject && m_ActiveCursor && m_ActiveObject->GetExtendedFlag("usable")) return DrawCursor(m_ActiveCursor);
			else if(m_Cursor) return DrawCursor(m_Cursor);
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::SaveGame(int Slot, char* Desc, bool quickSave)
{
	char Filename[MAX_PATH+1];
	GetSaveSlotFilename(Slot, Filename);

	LOG(0, "Saving game '%s'...", Filename);

	Game->m_PluginMgr->ApplyEvent(WME_EVENT_GAME_BEFORE_SAVE, NULL, NULL);
	Game->ApplyEvent("BeforeSave", true);

	HRESULT ret;
	
	m_IndicatorDisplay = true;
	m_IndicatorProgress = 0;
	CBPersistMgr* pm = new CBPersistMgr(Game);
	if(FAILED(ret = pm->InitSave(Desc))) goto save_finish;

	if (!quickSave)
	{
		SAFE_DELETE(m_SaveLoadImage);
		if(m_SaveImageName)
		{
			if(m_UseD3D)
				m_SaveLoadImage = new CBSurfaceD3D(this);
			else
				m_SaveLoadImage = new CBSurfaceDD(this);
		
			if(!m_SaveLoadImage || FAILED(m_SaveLoadImage->Create(m_SaveImageName, true, 0, 0, 0)))
			{
				SAFE_DELETE(m_SaveLoadImage);
			}
		}
	}

	if(FAILED(ret = CSysClassRegistry::GetInstance()->SaveTable(Game, pm, quickSave))) goto save_finish;
	if(FAILED(ret = CSysClassRegistry::GetInstance()->SaveInstances(Game, pm, quickSave))) goto save_finish;
	if(FAILED(ret = pm->SaveFile(Filename))) goto save_finish;

	m_Registry->WriteInt("System", "MostRecentSaveSlot", Slot);

save_finish:
	delete pm;
	m_IndicatorDisplay = false;

	SAFE_DELETE(m_SaveLoadImage);

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::LoadGame(int Slot)
{
	//Game->LOG(0, "Load start %d", CBUtils::GetUsedMemMB());

	m_Loading = false;
	m_ScheduledLoadSlot = -1;

	char Filename[MAX_PATH+1];
	GetSaveSlotFilename(Slot, Filename);

	return LoadGame(Filename);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::LoadGame(char* Filename)
{
	LOG(0, "Loading game '%s'...", Filename);
	GetDebugMgr()->OnGameShutdown();

	HRESULT ret;

	SAFE_DELETE(m_SaveLoadImage);
	if(m_LoadImageName)
	{
		if(m_UseD3D)
			m_SaveLoadImage = new CBSurfaceD3D(this);
		else
			m_SaveLoadImage = new CBSurfaceDD(this);

		if(!m_SaveLoadImage || FAILED(m_SaveLoadImage->Create(m_LoadImageName, true, 0, 0, 0)))
		{
			SAFE_DELETE(m_SaveLoadImage);
		}
	}


	m_LoadInProgress = true;
	m_IndicatorDisplay = true;
	m_IndicatorProgress = 0;
	CBPersistMgr* pm = new CBPersistMgr(Game);
	m_DEBUG_AbsolutePathWarning = false;
	if(FAILED(ret = pm->InitLoad(Filename))) goto load_finish;

	//if(FAILED(ret = Cleanup())) goto load_finish;
	if(FAILED(ret = CSysClassRegistry::GetInstance()->LoadTable(Game, pm))) goto load_finish;
	if(FAILED(ret = CSysClassRegistry::GetInstance()->LoadInstances(Game, pm))) goto load_finish;

	// data initialization after load
	InitAfterLoad();
	
	Game->m_PluginMgr->ApplyEvent(WME_EVENT_GAME_AFTER_LOAD, NULL, NULL);
	Game->ApplyEvent("AfterLoad", true);

	DisplayContent(true, false);
	//m_Renderer->Flip();

	AccessUnpause();
	GetDebugMgr()->OnGameInit();

load_finish:
	m_DEBUG_AbsolutePathWarning = true;

	m_IndicatorDisplay = false;
	delete pm;
	m_LoadInProgress = false;

	SAFE_DELETE(m_SaveLoadImage);

	//Game->LOG(0, "Load end %d", CBUtils::GetUsedMemMB());

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::InitAfterLoad()
{
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadRegion,   "CBRegion",   NULL);
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadSubFrame, "CBSubFrame", NULL);
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadSound,    "CBSound",    NULL);
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadTheora,   "CVidTheoraPlayer", NULL);
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadXModel,   "CXModel",    NULL);
	CSysClassRegistry::GetInstance()->EnumInstances(AfterLoadScript,   "CScScript",  NULL);

	m_ScEngine->RefreshScriptBreakpoints();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadRegion(void* Region, void* Data)
{
	((CBRegion*)Region)->CreateRegion();
}


//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadSubFrame(void* Subframe, void* Data)
{
	((CBSubFrame*)Subframe)->SetSurfaceSimple();
}


//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadSound(void* Sound, void* Data)
{
	((CBSound*)Sound)->SetSoundSimple();
}

//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadTheora(void* Theora, void* Data)
{
	((CVidTheoraPlayer*)Theora)->InitializeSimple();
}

//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadXModel(void* Model, void* Data)
{
	((CXModel*)Model)->InitializeSimple();
}

//////////////////////////////////////////////////////////////////////////
void CBGame::AfterLoadScript(void* script, void* data)
{
	((CScScript*)script)->AfterLoad();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayWindows(bool InGame)
{
	HRESULT res;

	int i;

	// did we lose focus? focus topmost window
	if(m_FocusedWindow==NULL || !m_FocusedWindow->m_Visible || m_FocusedWindow->m_Disable)
	{
		m_FocusedWindow = NULL;
		for(i=m_Windows.GetSize()-1; i>=0; i--)
		{
			if(m_Windows[i]->m_Visible && !m_Windows[i]->m_Disable)
			{
				m_FocusedWindow = m_Windows[i];
				break;
			}
		}
	}

	// display all windows
	for(i=0; i<m_Windows.GetSize(); i++)
	{
		if(m_Windows[i]->m_Visible && m_Windows[i]->m_InGame==InGame){		

			res = m_Windows[i]->Display();
			if(FAILED(res)) return res;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::PlayMusic(int Channel, char *Filename, bool Looping, DWORD LoopStart, DWORD PrivVolume)
{
	if(Channel>=NUM_MUSIC_CHANNELS)
	{
		Game->LOG(0, "**Error** Attempting to use music channel %d (max num channels: %d)", Channel, NUM_MUSIC_CHANNELS);
		return E_FAIL;
	}

	SAFE_DELETE(m_Music[Channel]);

	m_Music[Channel] = new CBSound(Game);
	if(m_Music[Channel] && SUCCEEDED(m_Music[Channel]->SetSound(Filename, SOUND_MUSIC, true, PrivVolume)))
	{
		if(m_MusicStartTime[Channel])
		{
			m_Music[Channel]->SetPositionTime(m_MusicStartTime[Channel]);
			m_MusicStartTime[Channel] = 0;
		}
		if(LoopStart) m_Music[Channel]->SetLoopStart(LoopStart);
		return m_Music[Channel]->Play(Looping);
	}
	else
	{
		SAFE_DELETE(m_Music[Channel]);
		return E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::StopMusic(int Channel)
{
	if(Channel>=NUM_MUSIC_CHANNELS)
	{
		Game->LOG(0, "**Error** Attempting to use music channel %d (max num channels: %d)", Channel, NUM_MUSIC_CHANNELS);
		return E_FAIL;
	}

	if(m_Music[Channel])
	{
		m_Music[Channel]->Stop();
		SAFE_DELETE(m_Music[Channel]);
		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::PauseMusic(int Channel)
{
	if(Channel>=NUM_MUSIC_CHANNELS)
	{
		Game->LOG(0, "**Error** Attempting to use music channel %d (max num channels: %d)", Channel, NUM_MUSIC_CHANNELS);
		return E_FAIL;
	}

	if(m_Music[Channel]) return m_Music[Channel]->Pause();
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ResumeMusic(int Channel)
{
	if(Channel>=NUM_MUSIC_CHANNELS)
	{
		Game->LOG(0, "**Error** Attempting to use music channel %d (max num channels: %d)", Channel, NUM_MUSIC_CHANNELS);
		return E_FAIL;
	}

	if(m_Music[Channel]) return m_Music[Channel]->Resume();
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::SetMusicStartTime(int Channel, DWORD Time)
{
	
	if(Channel>=NUM_MUSIC_CHANNELS)
	{
		Game->LOG(0, "**Error** Attempting to use music channel %d (max num channels: %d)", Channel, NUM_MUSIC_CHANNELS);
		return E_FAIL;
	}

	m_MusicStartTime[Channel] = Time;
	if(m_Music[Channel] && m_Music[Channel]->IsPlaying()) return m_Music[Channel]->SetPositionTime(Time);
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::LoadSettings(char *Filename)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (SETTINGS)
		TOKEN_TABLE (GAME)
		TOKEN_TABLE (STRING_TABLE)
		TOKEN_TABLE (RESOLUTION)
		TOKEN_TABLE (REQUIRE_3D_ACCELERATION)
		TOKEN_TABLE (REQUIRE_SOUND)
		TOKEN_TABLE (HWTL_MODE)
		TOKEN_TABLE (ALLOW_WINDOWED_MODE)
		TOKEN_TABLE (ALLOW_ACCESSIBILITY_TAB)
		TOKEN_TABLE (ALLOW_ABOUT_TAB)
		TOKEN_TABLE (ALLOW_ADVANCED)
		TOKEN_TABLE (ALLOW_DESKTOP_RES)
		TOKEN_TABLE (REGISTRY_PATH)
		TOKEN_TABLE (RICH_SAVED_GAMES)
		TOKEN_TABLE (SAVED_GAME_EXT)
		TOKEN_TABLE (GUID)
	TOKEN_TABLE_END

	
	BYTE* OrigBuffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(OrigBuffer==NULL)
	{
		Game->LOG(0, "CBGame::LoadSettings failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret = S_OK;

	BYTE* Buffer = OrigBuffer;
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_SETTINGS)
	{
		Game->LOG(0, "'SETTINGS' keyword expected in game settings file.");
		return E_FAIL;
	}
	Buffer = params;
	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_GAME:
				SAFE_DELETE_ARRAY(m_SettingsGameFile);
				m_SettingsGameFile = new char[strlen((char*)params)+1];
				if(m_SettingsGameFile) strcpy(m_SettingsGameFile, (char*)params);
			break;

			case TOKEN_STRING_TABLE:
				if(FAILED(m_StringTable->LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;
			
			case TOKEN_RESOLUTION:
				parser.ScanStr((char*)params, "%d,%d", &m_SettingsResWidth, &m_SettingsResHeight);
			break;

			case TOKEN_REQUIRE_3D_ACCELERATION:
				parser.ScanStr((char*)params, "%b", &m_SettingsRequireAcceleration);
			break;

			case TOKEN_REQUIRE_SOUND:
				parser.ScanStr((char*)params, "%b", &m_SettingsRequireSound);
			break;

			case TOKEN_HWTL_MODE:
				parser.ScanStr((char*)params, "%d", &m_SettingsTLMode);
			break;

			case TOKEN_ALLOW_WINDOWED_MODE:
				parser.ScanStr((char*)params, "%b", &m_SettingsAllowWindowed);
			break;

			case TOKEN_ALLOW_DESKTOP_RES:
				parser.ScanStr((char*)params, "%b", &m_SettingsAllowDesktopRes);
			break;

			case TOKEN_ALLOW_ADVANCED:
				parser.ScanStr((char*)params, "%b", &m_SettingsAllowAdvanced);
			break;

			case TOKEN_ALLOW_ACCESSIBILITY_TAB:
				parser.ScanStr((char*)params, "%b", &m_SettingsAllowAccessTab);
			break;

			case TOKEN_ALLOW_ABOUT_TAB:
				parser.ScanStr((char*)params, "%b", &m_SettingsAllowAboutTab);
			break;

			case TOKEN_REGISTRY_PATH:
				m_Registry->SetBasePath((char*)params);
			break;
			
			case TOKEN_RICH_SAVED_GAMES:
				parser.ScanStr((char*)params, "%b", &m_RichSavedGames);
			break;

			case TOKEN_SAVED_GAME_EXT:
				CBUtils::SetString(&m_SavedGameExt, (char*)params);
			break;

			case TOKEN_GUID:
			{
				OLECHAR olestr[50];
				mbstowcs(olestr, (char*)params, 50);
				IIDFromString(olestr, &m_GameGUID);
			}
			break;
		}
	}
	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in game settings '%s'", Filename);
		ret = E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading game settings '%s'", Filename);
		ret = E_FAIL;
	}

	m_SettingsAllowWindowed = m_Registry->ReadBool("Debug", "AllowWindowed", m_SettingsAllowWindowed);
	m_CompressedSavegames = m_Registry->ReadBool("Debug", "CompressedSavegames", m_CompressedSavegames);

	delete [] OrigBuffer;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::Persist(CBPersistMgr *PersistMgr)
{
	if(!PersistMgr->m_Saving) Cleanup();

	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_ActiveObject));
	PersistMgr->Transfer(TMEMBER(m_CapturedObject));
	PersistMgr->Transfer(TMEMBER(m_CursorNoninteractive));
	PersistMgr->Transfer(TMEMBER(m_DoNotExpandStrings));
	PersistMgr->Transfer(TMEMBER(m_EditorMode));
	PersistMgr->Transfer(TMEMBER(m_Fader));
	PersistMgr->Transfer(TMEMBER(m_FreezeLevel));
	PersistMgr->Transfer(TMEMBER(m_FocusedWindow));
	PersistMgr->Transfer(TMEMBER(m_FontStorage));
	PersistMgr->Transfer(TMEMBER(m_Interactive));
	PersistMgr->Transfer(TMEMBER(m_KeyboardState));
	PersistMgr->Transfer(TMEMBER(m_LastTime));
	PersistMgr->Transfer(TMEMBER(m_MainObject));
	for(int i=0; i<NUM_MUSIC_CHANNELS; i++)
	{
		PersistMgr->Transfer(TMEMBER(m_Music[i]));
		PersistMgr->Transfer(TMEMBER(m_MusicStartTime[i]));
	}

	PersistMgr->Transfer(TMEMBER(m_OffsetX));
	PersistMgr->Transfer(TMEMBER(m_OffsetY));
	PersistMgr->Transfer(TMEMBER(m_OffsetPercentX));
	PersistMgr->Transfer(TMEMBER(m_OffsetPercentY));

	PersistMgr->Transfer(TMEMBER(m_OrigInteractive));
	PersistMgr->Transfer(TMEMBER_INT(m_OrigState));
	PersistMgr->Transfer(TMEMBER(m_PersonalizedSave));
	PersistMgr->Transfer(TMEMBER(m_Quitting));

	m_RegObjects.Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_ScEngine));
	//PersistMgr->Transfer(TMEMBER(m_SoundMgr));
	PersistMgr->Transfer(TMEMBER_INT(m_State));
	//PersistMgr->Transfer(TMEMBER(m_SurfaceStorage));
	PersistMgr->Transfer(TMEMBER(m_Subtitles));
	PersistMgr->Transfer(TMEMBER(m_SubtitlesSpeed));
	PersistMgr->Transfer(TMEMBER(m_SystemFont));
	PersistMgr->Transfer(TMEMBER(m_VideoFont));
	PersistMgr->Transfer(TMEMBER(m_VideoSubtitles));

	PersistMgr->Transfer(TMEMBER(m_Timer));
	PersistMgr->Transfer(TMEMBER(m_TimerDelta));
	PersistMgr->Transfer(TMEMBER(m_TimerLast));

	PersistMgr->Transfer(TMEMBER(m_LiveTimer));
	PersistMgr->Transfer(TMEMBER(m_LiveTimerDelta));
	PersistMgr->Transfer(TMEMBER(m_LiveTimerLast));

	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeRunning));
	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeStartTime));
	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeLength));
	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeChannel1));
	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeChannel2));
	PersistMgr->Transfer(TMEMBER(m_MusicCrossfadeSwap));

	PersistMgr->Transfer(TMEMBER(m_LoadImageName));
	PersistMgr->Transfer(TMEMBER(m_SaveImageName));
	PersistMgr->Transfer(TMEMBER(m_SaveImageX));
	PersistMgr->Transfer(TMEMBER(m_SaveImageY));
	PersistMgr->Transfer(TMEMBER(m_LoadImageX));
	PersistMgr->Transfer(TMEMBER(m_LoadImageY));

	PersistMgr->Transfer(TMEMBER_INT(m_MaxShadowType));

	PersistMgr->Transfer(TMEMBER(m_EditorResolutionWidth));
	PersistMgr->Transfer(TMEMBER(m_EditorResolutionHeight));

	PersistMgr->Transfer(TMEMBER_INT(m_TextEncoding));
	PersistMgr->Transfer(TMEMBER(m_TextRTL));

	PersistMgr->Transfer(TMEMBER(m_SoundBufferSizeSec));
	PersistMgr->Transfer(TMEMBER(m_SuspendedRendering));

	PersistMgr->Transfer(TMEMBER(m_AccessShieldWin));

	PersistMgr->Transfer(TMEMBER(m_MouseLockRect));
	
	m_Windows.Persist(PersistMgr);

	if(PersistMgr->CheckVersion(1, 7, 1))
	{
		PersistMgr->Transfer(TMEMBER(m_SuppressScriptErrors));
	}
	else m_SuppressScriptErrors = false;

	if(PersistMgr->CheckVersion(1, 8, 2))
	{
		PersistMgr->Transfer(TMEMBER(m_AutorunDisabled));
	}
	else m_AutorunDisabled = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::FocusWindow(CUIWindow *Window)
{
	CUIWindow* Prev = m_FocusedWindow;

	int i;
	for(i=0; i<m_Windows.GetSize(); i++)
	{
		if(m_Windows[i]==Window)
		{
			if(i<m_Windows.GetSize()-1)
			{
				m_Windows.RemoveAt(i);
				m_Windows.Add(Window);

				Game->m_FocusedWindow = Window;
			}

			if(Window->m_Mode==WINDOW_NORMAL && Prev!=Window && Game->ValidObject(Prev) && (Prev->m_Mode==WINDOW_EXCLUSIVE || Prev->m_Mode==WINDOW_SYSTEM_EXCLUSIVE))
				return FocusWindow(Prev);
			else return S_OK;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::Freeze(bool IncludingMusic)
{
	if(m_FreezeLevel==0)
	{
		m_ScEngine->PauseAll();
		m_SoundMgr->PauseAll(IncludingMusic);
		m_OrigState = m_State;
		m_OrigInteractive = m_Interactive;
		m_Interactive = true;
	}
	m_State = GAME_FROZEN;
	m_FreezeLevel++;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::Unfreeze()
{
	if(m_FreezeLevel==0) return S_OK;

	m_FreezeLevel--;
	if(m_FreezeLevel==0)
	{
		m_State = m_OrigState;
		m_Interactive = m_OrigInteractive;
		m_ScEngine->ResumeAll();
		m_SoundMgr->ResumeAll();
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData)
{
	if(IsVideoPlaying())
	{
		if(CharCode==VK_ESCAPE) StopVideo();
		return true;
	}
	/*
	if (CharCode == 9)
	{
		DEBUG_DumpClassRegistry();
	}
	*/
	if(HandleAccessKey(Printable, CharCode, KeyData)) return true;

	m_KeyboardState->ReadKey(Printable, CharCode, KeyData);

	if(m_FocusedWindow)
	{
		if(!Game->m_FocusedWindow->HandleKeypress(Printable, CharCode, KeyData)){
			if(!Printable)
			{
				if(Game->m_FocusedWindow->CanHandleEvent("Keypress"))
					Game->m_FocusedWindow->ApplyEvent("Keypress");
				else
					ApplyEvent("Keypress");
			}
		}
		return true;
	}
	else if(!Printable)
	{
		ApplyEvent("Keypress");
		return true;
	}
	else return true;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::HandleAccessKey(bool Printable, DWORD CharCode, DWORD KeyData)
{
	if(m_AccessKeyboardEnabled)
	{
		if(CharCode==VK_TAB && (CBUtils::IsKeyDown(VK_CONTROL) || CBUtils::IsKeyDown(VK_RCONTROL)))
		{
			CBObject* obj = NULL;
			if(CBUtils::IsKeyDown(VK_SHIFT) || CBUtils::IsKeyDown(VK_RSHIFT))
			{
				obj = m_AccessMgr->GetPrevObject();
			}
			else
			{
				obj = m_AccessMgr->GetNextObject();
			}
			return true;
		}
	}
	if(Printable && m_AccessKeyboardPause)
	{
		if(CharCode==VK_SPACE && (CBUtils::IsKeyDown(VK_CONTROL) || CBUtils::IsKeyDown(VK_RCONTROL)))
		{
			m_AccessGlobalPaused = !m_AccessGlobalPaused;
			if(m_AccessGlobalPaused) AccessPause();
			else AccessUnpause();
			return true;
		}
	}
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::AccessPause()
{
	m_AccessGlobalPaused = true;

	if(m_AccessShieldWin) UnregisterObject(m_AccessShieldWin);

	m_AccessShieldWin = new CUIWindow(this);
	m_Windows.Add(m_AccessShieldWin);
	RegisterObject(m_AccessShieldWin);

	m_AccessShieldWin->m_PosX = m_AccessShieldWin->m_PosY = 0;
	m_AccessShieldWin->m_Width = m_Renderer->m_Width;
	m_AccessShieldWin->m_Height = m_Renderer->m_Height;

	CUIText* Sta = new CUIText(Game);
	Sta->m_Parent = m_AccessShieldWin;
	m_AccessShieldWin->m_Widgets.Add(Sta);
	Sta->SetText((char*)m_StringTable->ExpandStatic("/SYSENG0040/Game paused. Press Ctrl+Space to resume."));
	Sta->m_SharedFonts = true;
	Sta->m_Font = m_SystemFont;
	Sta->SizeToFit();
	Sta->m_PosY = m_AccessShieldWin->m_Height - Sta->m_Height;
	Sta->m_PosX = (m_AccessShieldWin->m_Width - Sta->m_Width) / 2;


	m_AccessShieldWin->m_Visible = true;
	m_AccessShieldWin->GoSystemExclusive();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::AccessUnpause()
{
	m_AccessGlobalPaused = false;
	if(m_AccessShieldWin)
	{
		m_AccessShieldWin->Close();
		UnregisterObject(m_AccessShieldWin);
		m_AccessShieldWin = NULL;
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::HandleMouseWheel(int Delta)
{
	bool Handled = false;
	if(m_FocusedWindow)
	{
		Handled = Game->m_FocusedWindow->HandleMouseWheel(Delta);

		if(!Handled)
		{
			if(Delta<0 && Game->m_FocusedWindow->CanHandleEvent("MouseWheelDown")){
				Game->m_FocusedWindow->ApplyEvent("MouseWheelDown");
				Handled = true;
			}
			else if(Game->m_FocusedWindow->CanHandleEvent("MouseWheelUp"))
			{
				Game->m_FocusedWindow->ApplyEvent("MouseWheelUp");
				Handled = true;
			}

		}
	}

	if(!Handled)
	{
		if(Delta<0)
		{
			ApplyEvent("MouseWheelDown");
		}
		else
		{
			ApplyEvent("MouseWheelUp");
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetVersion(BYTE *VerMajor, BYTE *VerMinor, BYTE *ExtMajor, BYTE *ExtMinor)
{
	if(VerMajor) *VerMajor = DCGF_VER_MAJOR;
	if(VerMinor) *VerMinor = DCGF_VER_MINOR;

	if(ExtMajor) *ExtMajor = 0;
	if(ExtMinor) *ExtMinor = 0;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CBGame::SetWindowTitle()
{
	if(m_Renderer)
	{
		char Title[512];
		strcpy(Title, m_Caption[0]);
		if(Title[0]!='\0') strcat(Title, " - ");
		strcat(Title, "Wintermute Engine");

		//SetWindowText(m_Renderer->m_Window, Title);
		CBTextUtils::SetWindowText(this->m_TextEncoding, m_Renderer->m_Window, (BYTE*)Title);
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetSaveSlotFilename(int Slot, char *Buffer)
{
	char Folder[MAX_PATH+1];
	GetSaveDir(Folder);
	sprintf(Buffer, "%s\\save%03d.%s", Folder, Slot, m_SavedGameExt);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetSaveDir(char* Buffer)
{
	bool Handled = false;

	// check if local dir is writable
	if(!m_PersonalizedSave && !m_SaveDirChecked)
	{
		char TestFilename[MAX_PATH+1];
		sprintf(TestFilename, "%s\\writest.$$$", m_LocalSaveDir);
		CBUtils::CreatePath(TestFilename, false);

		FILE* TestFile = fopen(TestFilename, "wb");
		if(!TestFile)
		{
			Game->LOG(0, "Save directory is not writable. Switching to personal savegames.");
			m_PersonalizedSave = true;
		}
		else
		{
			fclose(TestFile);
			DeleteFile(TestFilename);
		}
		m_SaveDirChecked = true;
	}


	if(m_PersonalizedSave)
	{
		Handled = CBPlatform::GetPrivateSaveDirectory(Buffer);
		if(Handled)
		{
			if(Buffer[strlen(Buffer)-1]!='\\') strcat(Buffer, "\\");
			int FolderLen = strlen(Buffer);
			if(Game->m_Caption[0] && Game->m_Caption[0][0]!='\0') strcat(Buffer, Game->m_Caption[0]);
			else strcat(Buffer, Game->m_Name);

			// remove invalid chars from filename
			for(int i=FolderLen; i<strlen(Buffer); i++)
			{
				if(strchr("\\|*?:\"<>/", Buffer[i])) Buffer[i] = '_';
			}
		}		
	}
	if(!m_PersonalizedSave || !Handled) strcpy(Buffer, m_LocalSaveDir);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetSaveSlotDescription(int Slot, char *Buffer)
{
	Buffer[0] = '\0';

	char Filename[MAX_PATH+1];
	GetSaveSlotFilename(Slot, Filename);
	CBPersistMgr* pm = new CBPersistMgr(Game);
	if(!pm) return E_FAIL;

	m_DEBUG_AbsolutePathWarning = false;
	if(FAILED(pm->InitLoad(Filename)))
	{
		m_DEBUG_AbsolutePathWarning = true;
		delete pm;
		return E_FAIL;
	}

	m_DEBUG_AbsolutePathWarning = true;
	strcpy(Buffer, pm->m_SavedDescription);
	delete pm;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::IsSaveSlotUsed(int Slot)
{
	char Filename[MAX_PATH+1];
	GetSaveSlotFilename(Slot, Filename);

	CBFile* File = m_FileManager->OpenFile(Filename, false);
	if(!File) return false;

	m_FileManager->CloseFile(File);
	return true;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::EmptySaveSlot(int Slot)
{
	char Filename[MAX_PATH+1];
	GetSaveSlotFilename(Slot, Filename);

	CBPlatform::DeleteFile(Filename);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::SetActiveObject(CBObject *Obj)
{
	// not-active when game is frozen
	if(Obj && !Game->m_Interactive && !Obj->m_NonIntMouseEvents)
	{
		Obj = NULL;
	}

	if(Obj==m_ActiveObject) return S_OK;
	
	if(m_ActiveObject) m_ActiveObject->ApplyEvent("MouseLeave");
	//if(ValidObject(m_ActiveObject)) m_ActiveObject->ApplyEvent("MouseLeave");
	m_ActiveObject = Obj;
	if(m_ActiveObject)
	{
		m_AccessMgr->Speak(m_ActiveObject->GetAccessCaption(), TTS_CAPTION);
		m_ActiveObject->ApplyEvent("MouseEntry");
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::PushViewport(CBViewport *Viewport)
{
	m_ViewportSP++;
	if(m_ViewportSP>=m_ViewportStack.GetSize()) m_ViewportStack.Add(Viewport);
	else m_ViewportStack[m_ViewportSP] = Viewport;

	m_Renderer->SetViewport(Viewport->GetRect());

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::PopViewport()
{
	m_ViewportSP--;
	if(m_ViewportSP<-1) Game->LOG(0, "Fatal: Viewport stack underflow!");

	if(m_ViewportSP>=0 && m_ViewportSP<m_ViewportStack.GetSize()) m_Renderer->SetViewport(m_ViewportStack[m_ViewportSP]->GetRect());
	else m_Renderer->SetViewport(	m_Renderer->m_DrawOffsetX,
									m_Renderer->m_DrawOffsetY,
									m_Renderer->m_Width + m_Renderer->m_DrawOffsetX,
									m_Renderer->m_Height + m_Renderer->m_DrawOffsetY);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetCurrentViewportRect(RECT *Rect, bool* Custom)
{
	if(Rect==NULL) return E_FAIL;
	else
	{
		if(m_ViewportSP>=0)
		{
			CopyRect(Rect, m_ViewportStack[m_ViewportSP]->GetRect());
			if(Custom) *Custom = true;
		}
		else
		{
			//SetRect(Rect, 0, 0, m_Renderer->m_Width, m_Renderer->m_Height);
			SetRect(Rect,	m_Renderer->m_DrawOffsetX,
							m_Renderer->m_DrawOffsetY,
							m_Renderer->m_Width + m_Renderer->m_DrawOffsetX,
							m_Renderer->m_Height + m_Renderer->m_DrawOffsetY);
			if(Custom) *Custom = false;
		}

		return S_OK;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetCurrentViewportOffset(int* OffsetX, int* OffsetY)
{
	if(m_ViewportSP>=0)
	{
		if(OffsetX) *OffsetX = m_ViewportStack[m_ViewportSP]->m_OffsetX;
		if(OffsetY) *OffsetY = m_ViewportStack[m_ViewportSP]->m_OffsetY;
	}
	else
	{
		if(OffsetX) *OffsetX = 0;
		if(OffsetY) *OffsetY = 0;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::WindowLoadHook(CUIWindow* Win, char** Buf, char** Params)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::WindowScriptMethodHook(CUIWindow* Win, CScScript* Script, CScStack* Stack, char* Name)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
void CBGame::SetInteractive(bool State)
{
	m_Interactive = State;
	if(m_TransMgr) m_TransMgr->m_OrigInteractive = State;
}


//////////////////////////////////////////////////////////////////////////
void CBGame::ResetMousePos()
{
	POINT p;
	p.x = m_MousePos.x + m_Renderer->m_DrawOffsetX;
	p.y = m_MousePos.y + m_Renderer->m_DrawOffsetY;
	
	CBPlatform::ClientToScreen(Game->m_Renderer->m_Window, &p);	
	CBPlatform::SetCursorPos(p.x, p.y);
}


//////////////////////////////////////////////////////////////////////////
void CBGame::SetResourceModule(HMODULE ResModule)
{
	m_ResourceModule = ResModule;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayContent(bool Update, bool DisplayAll)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayContentSimple()
{
	// fill black
	m_Renderer->Fill(0,0,0);
	if(m_IndicatorDisplay) DisplayIndicator();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayIndicator()
{
	if(m_SaveLoadImage)
	{
		RECT rc;
		SetRect(&rc, 0, 0, m_SaveLoadImage->GetWidth(), m_SaveLoadImage->GetHeight());
		if(m_LoadInProgress) m_SaveLoadImage->DisplayTrans(m_LoadImageX, m_LoadImageY, rc);
		else m_SaveLoadImage->DisplayTrans(m_SaveImageX, m_SaveImageY, rc);
	}

	if(!m_IndicatorDisplay && m_IndicatorWidth<=0 || m_IndicatorHeight<=0) return S_OK;
	m_Renderer->SetupLines();
	for(int i=0; i<m_IndicatorHeight; i++)
		m_Renderer->DrawLine(m_IndicatorX, m_IndicatorY + i, m_IndicatorX + (int)(m_IndicatorWidth * (float)((float)m_IndicatorProgress / 100.0f)), m_IndicatorY + i, m_IndicatorColor);

	m_Renderer->Setup2D();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::UpdateMusicCrossfade()
{
	BYTE GlobMusicVol = m_SoundMgr->GetVolumePercent(SOUND_MUSIC);

	if(!m_MusicCrossfadeRunning) return S_OK;
	if(m_State==GAME_FROZEN) return S_OK;

	if(m_MusicCrossfadeChannel1<0 || m_MusicCrossfadeChannel1>=NUM_MUSIC_CHANNELS || !m_Music[m_MusicCrossfadeChannel1])
	{
		m_MusicCrossfadeRunning = false;
		return S_OK;
	}
	if(m_MusicCrossfadeChannel2<0 || m_MusicCrossfadeChannel2>=NUM_MUSIC_CHANNELS || !m_Music[m_MusicCrossfadeChannel2])
	{
		m_MusicCrossfadeRunning = false;
		return S_OK;
	}

	if(!m_Music[m_MusicCrossfadeChannel1]->IsPlaying()) m_Music[m_MusicCrossfadeChannel1]->Play();
	if(!m_Music[m_MusicCrossfadeChannel2]->IsPlaying()) m_Music[m_MusicCrossfadeChannel2]->Play();

	DWORD CurrentTime = Game->m_LiveTimer - m_MusicCrossfadeStartTime;

	if(CurrentTime >= m_MusicCrossfadeLength)
	{
		m_MusicCrossfadeRunning = false;
		//m_Music[m_MusicCrossfadeChannel2]->SetVolume(GlobMusicVol);
		m_Music[m_MusicCrossfadeChannel2]->SetVolume(100);
		
		m_Music[m_MusicCrossfadeChannel1]->Stop();
		//m_Music[m_MusicCrossfadeChannel1]->SetVolume(GlobMusicVol);
		m_Music[m_MusicCrossfadeChannel1]->SetVolume(100);
		

		if(m_MusicCrossfadeSwap)
		{
			// swap channels
			CBSound* Dummy = m_Music[m_MusicCrossfadeChannel1];
			int DummyInt = m_MusicStartTime[m_MusicCrossfadeChannel1];
			
			m_Music[m_MusicCrossfadeChannel1] = m_Music[m_MusicCrossfadeChannel2];
			m_MusicStartTime[m_MusicCrossfadeChannel1] = m_MusicStartTime[m_MusicCrossfadeChannel2];
			
			m_Music[m_MusicCrossfadeChannel2] = Dummy;
			m_MusicStartTime[m_MusicCrossfadeChannel2] = DummyInt;
		}
	}
	else
	{
		//m_Music[m_MusicCrossfadeChannel1]->SetVolume(GlobMusicVol - (float)CurrentTime / (float)m_MusicCrossfadeLength * GlobMusicVol);
		//m_Music[m_MusicCrossfadeChannel2]->SetVolume((float)CurrentTime / (float)m_MusicCrossfadeLength * GlobMusicVol);
		m_Music[m_MusicCrossfadeChannel1]->SetVolume(100 - (float)CurrentTime / (float)m_MusicCrossfadeLength * 100);
		m_Music[m_MusicCrossfadeChannel2]->SetVolume((float)CurrentTime / (float)m_MusicCrossfadeLength * 100);

		//Game->QuickMessageForm("%d %d", m_Music[m_MusicCrossfadeChannel1]->GetVolume(), m_Music[m_MusicCrossfadeChannel2]->GetVolume());
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::ResetContent()
{
	m_ScEngine->ClearGlobals();
	//m_Timer = 0;
	//m_LiveTimer = 0;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void CBGame::DEBUG_DumpClassRegistry()
{
	FILE* f = fopen(".\\zz_class_reg_dump.log", "wt");
	CSysClassRegistry::GetInstance()->DumpClasses(f);
	fclose(f);
	Game->QuickMessage("Classes dump completed.");
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::InvalidateDeviceObjects()
{
	for(int i=0; i<m_RegObjects.GetSize(); i++)
	{
		m_RegObjects[i]->InvalidateDeviceObjects();
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::RestoreDeviceObjects()
{
	for(int i=0; i<m_RegObjects.GetSize(); i++)
	{
		m_RegObjects[i]->RestoreDeviceObjects();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::SetWaitCursor(char* Filename)
{
	SAFE_DELETE(m_CursorNoninteractive);

	m_CursorNoninteractive = new CBSprite(Game);
	if(!m_CursorNoninteractive || FAILED(m_CursorNoninteractive->LoadFile(Filename)))
	{
		SAFE_DELETE(m_CursorNoninteractive);
		return E_FAIL;
	}
	else return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::BindDirectX()
{
	memset(&m_DirectX, 0, sizeof(TDirectXBindings));

	// load libraries
	m_DirectX.hDDraw  = CBPlatform::LoadLibrary("ddraw.dll");
#ifdef WME_D3D9
	m_DirectX.hD3D    = CBPlatform::LoadLibrary("d3d9.dll");
#else
	m_DirectX.hD3D    = CBPlatform::LoadLibrary("d3d8.dll");
	m_DirectX.hDXof   = CBPlatform::LoadLibrary("d3dxof.dll");
#endif
	m_DirectX.hDSound = CBPlatform::LoadLibrary("dsound.dll");
	

	// bind DirectDraw
	if(m_DirectX.hDDraw)
	{
		m_DirectX.DirectDrawCreateEx    = (DLL_DirectDrawCreateEx)CBPlatform::GetProcAddress(m_DirectX.hDDraw, "DirectDrawCreateEx");
		m_DirectX.DirectDrawEnumerateEx = (DLL_DirectDrawEnumerateEx)CBPlatform::GetProcAddress(m_DirectX.hDDraw, "DirectDrawEnumerateExA");
	}

	// bind Direct3D
	if(m_DirectX.hD3D)
	{
#ifdef WME_D3D9
		m_DirectX.Direct3DCreate = (DLL_Direct3DCreate)CBPlatform::GetProcAddress(m_DirectX.hD3D, "Direct3DCreate9");
#else
		m_DirectX.Direct3DCreate = (DLL_Direct3DCreate)CBPlatform::GetProcAddress(m_DirectX.hD3D, "Direct3DCreate8");
#endif
	}

	// bind DirectSound
	if(m_DirectX.hDSound)
	{
		m_DirectX.DirectSoundCreate8   = (DLL_DirectSoundCreate8)CBPlatform::GetProcAddress(m_DirectX.hDSound, "DirectSoundCreate8");
		m_DirectX.DirectSoundEnumerate = (DLL_DirectSoundEnumerate)CBPlatform::GetProcAddress(m_DirectX.hDSound, "DirectSoundEnumerateA");
	}

#ifndef WME_D3D9
	// bind DirectXFile
	if(m_DirectX.hDXof)
	{
		m_DirectX.DirectXFileCreate = (DLL_DirectXFileCreate)CBPlatform::GetProcAddress(m_DirectX.hDXof, "DirectXFileCreate");
	}
#endif
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::UnBindDirectX()
{
	if(m_DirectX.hDDraw)  CBPlatform::FreeLibrary(m_DirectX.hDDraw);
	if(m_DirectX.hD3D)    CBPlatform::FreeLibrary(m_DirectX.hD3D);
	if(m_DirectX.hDSound) CBPlatform::FreeLibrary(m_DirectX.hDSound);
#ifndef WME_D3D9
	if(m_DirectX.hDXof)   CBPlatform::FreeLibrary(m_DirectX.hDXof);
#endif
	memset(&m_DirectX, 0, sizeof(TDirectXBindings));
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::IsDirectXBound()
{
	if(!m_DirectX.hDDraw)  return false;
	if(!m_DirectX.hD3D)    return false;
	if(!m_DirectX.hDSound) return false;

	if(!m_DirectX.Direct3DCreate)        return false;
	if(!m_DirectX.DirectDrawCreateEx)    return false;
	if(!m_DirectX.DirectDrawEnumerateEx) return false;
	if(!m_DirectX.DirectSoundCreate8)    return false;
	if(!m_DirectX.DirectSoundEnumerate)  return false;

#ifndef WME_D3D9
	if(!m_DirectX.hDXof)   return false;
	if(!m_DirectX.DirectXFileCreate)     return false;
#endif

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CBGame::IsVideoPlaying()
{
	if(m_VideoPlayer->IsPlaying()) return true;
	if(m_TheoraPlayer && m_TheoraPlayer->IsPlaying()) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::StopVideo()
{
	if(m_VideoPlayer->IsPlaying()) m_VideoPlayer->Stop();
	if(m_TheoraPlayer && m_TheoraPlayer->IsPlaying())
	{
		m_TheoraPlayer->Stop();
		SAFE_DELETE(m_TheoraPlayer);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DrawCursor(CBSprite* Cursor)
{
	if(!Cursor) return E_FAIL;
	if(Cursor!=m_LastCursor)
	{
		Cursor->Reset();
		m_LastCursor = Cursor;
	}
	return Cursor->Draw(m_MousePos.x, m_MousePos.y);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::RenderShadowGeometry()
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
// IWmeGame
//////////////////////////////////////////////////////////////////////////
IWmeValue* CBGame::CreateValue()
{
	return new CScValue(this);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::DeleteValue(IWmeValue* Value)
{
	if(Value) delete (CScValue*)Value;
	return true;
}

//////////////////////////////////////////////////////////////////////////
IWmeParamSet* CBGame::CreateParamSet()
{
	return new CBParamSet(this);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::DeleteParamSet(IWmeParamSet* ParamSet)
{
	if(ParamSet) delete (CBParamSet*)ParamSet;
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SubscribeEvent(IWmeObject* Object, EWmeEvent Event)
{
	return m_PluginMgr->SubscribeEvent(Object, Event);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::UnsubscribeEvent(IWmeObject* Object, EWmeEvent Event)
{
	return m_PluginMgr->UnsubscribeEvent(Object, Event);
}

//////////////////////////////////////////////////////////////////////////
IWmeFile* CBGame::OpenFile(const char* Filename)
{
	return (IWmeFile*)m_FileManager->OpenFile((char*)Filename);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::CloseFile(IWmeFile* File)
{
	return SUCCEEDED(m_FileManager->CloseFile((CBFile*)File));
}

//////////////////////////////////////////////////////////////////////////
IWmeObject* CBGame::CreateObject(const char* ClassName, IWmeParamSet* Params)
{
	//CScValue* Ret = new CScValue(Game);
	IWmeObject* Ret = NULL;

	CScStack* Stack = new CScStack(Game);
	if(Params) ((CBParamSet*)Params)->PushToStack(Stack);
	else Stack->PushInt(0);

	CScStack* ThisStack = new CScStack(Game);
	ThisStack->PushNULL();

	CScScript* Script = new CScScript(Game, Game->m_ScEngine);
	CBUtils::SetString(&Script->m_Filename, "<temp script>");

	if(SUCCEEDED(ExternalCall(Script, Stack, ThisStack, (char*)ClassName)))
	{
		CBScriptable* Obj = ThisStack->Pop()->GetNative();
		if(Obj)
		{
			//Ret->SetNative(Obj);
			Obj->m_RefCount++;
			Ret = Obj;
		}
	}

	delete Stack;
	delete ThisStack;
	delete Script;

	if(Ret) ((CBScriptable*)Ret)->m_RefCount--;
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
// IWmeObject
//////////////////////////////////////////////////////////////////////////
bool CBGame::SendEvent(const char* EventName)
{
	return CBScriptHolder::SendEvent(EventName);
}

//////////////////////////////////////////////////////////////////////////
IWmeValue* CBGame::CallMethod(const char* MethodName, IWmeParamSet* Params)
{
	return CBScriptHolder::CallMethod(MethodName, Params);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, IWmeValue* Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, int Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, const char* Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, double Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, bool Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName, IWmeObject* Value)
{
	return CBScriptHolder::SetProperty(PropName, Value);
}

//////////////////////////////////////////////////////////////////////////
bool CBGame::SetProperty(const char* PropName)
{
	return CBScriptHolder::SetProperty(PropName);
}

//////////////////////////////////////////////////////////////////////////
IWmeValue* CBGame::GetProperty(const char* PropName)
{
	return CBScriptHolder::GetProperty(PropName);
}

//////////////////////////////////////////////////////////////////////////
void* CBGame::GetInterface(const char* ClassName)
{
	return CBScriptHolder::GetInterface(ClassName);
}


//////////////////////////////////////////////////////////////////////////
CBObject* CBGame::GetNextAccessObject(CBObject* CurrObject)
{
	if(m_FocusedWindow)
	{
		return m_FocusedWindow->GetNextAccessObject(CurrObject);
	}
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
CBObject* CBGame::GetPrevAccessObject(CBObject* CurrObject)
{
	if(m_FocusedWindow)
	{
		return m_FocusedWindow->GetPrevAccessObject(CurrObject);
	}
	else return NULL;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnActivate(bool Activate, bool RefreshMouse)
{
	if(m_ShuttingDown || !m_Renderer) return S_OK;

	m_Renderer->m_Active = Activate;

	if(RefreshMouse)
	{
		POINT p;
		GetMousePos(&p);
		SetActiveObject(m_Renderer->GetObjectAt(p.x, p.y));
	}

	if(Activate) m_SoundMgr->ResumeAll();
	else m_SoundMgr->PauseAll();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseLeftDown()
{
	if(IsVideoPlaying())
	{
		StopVideo();
		return S_OK;
	}

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_CLICK, MOUSE_BUTTON_LEFT);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("LeftClick"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("LeftClick");							
		}
	}

	if(m_ActiveObject != NULL) m_CapturedObject = m_ActiveObject;
	m_MouseLeftDown = true;
	CBPlatform::SetCapture(m_Renderer->m_Window);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseLeftUp()
{
	if(IsVideoPlaying()) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_RELEASE, MOUSE_BUTTON_LEFT);

	ReleaseCapture();
	m_CapturedObject = NULL;
	m_MouseLeftDown = false;

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("LeftRelease"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("LeftRelease");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseLeftDblClick() 
{
	if(IsVideoPlaying()) return S_OK;

	if(m_State==GAME_RUNNING && !m_Interactive) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_DBLCLICK, MOUSE_BUTTON_LEFT);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("LeftDoubleClick"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("LeftDoubleClick");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseRightDblClick()
{
	if(IsVideoPlaying()) return S_OK;

	if(m_State==GAME_RUNNING && !m_Interactive) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_DBLCLICK, MOUSE_BUTTON_RIGHT);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("RightDoubleClick"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("RightDoubleClick");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseRightDown()
{
	if(IsVideoPlaying()) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_CLICK, MOUSE_BUTTON_RIGHT);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("RightClick"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("RightClick");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseRightUp() 
{
	if(IsVideoPlaying()) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_RELEASE, MOUSE_BUTTON_RIGHT);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("RightRelease"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("RightRelease");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseMiddleDown() 
{
	if(IsVideoPlaying()) return S_OK;

	if(m_State==GAME_RUNNING && !m_Interactive) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_CLICK, MOUSE_BUTTON_MIDDLE);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("MiddleClick"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("MiddleClick");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnMouseMiddleUp() 
{
	if(IsVideoPlaying()) return S_OK;

	if(m_ActiveObject) m_ActiveObject->HandleMouse(MOUSE_RELEASE, MOUSE_BUTTON_MIDDLE);

	bool Handled = m_State==GAME_RUNNING && SUCCEEDED(ApplyEvent("MiddleRelease"));
	if(!Handled)
	{
		if(m_ActiveObject!=NULL)
		{
			m_ActiveObject->ApplyEvent("MiddleRelease");
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnPaint()
{
	if(m_Renderer && m_Renderer->m_Windowed && m_Renderer->m_Ready)
	{
		m_Renderer->InitLoop();
		DisplayContent(false, true);
		DisplayDebugInfo();
		m_Renderer->WindowedBlt();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnWindowClose()
{
	if(CanHandleEvent("QuitGame"))
	{
		if(m_State!=GAME_FROZEN) Game->ApplyEvent("QuitGame");
		return S_OK;
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnWinsock(WPARAM wparam, LPARAM lparam)
{
	if(Game->m_NetworkMgr)
	{
		if(SUCCEEDED(m_NetworkMgr->OnWinsockEvent(wparam, lparam))) return S_OK;
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::DisplayDebugInfo() 
{
	char str[100];

	if(m_DEBUG_ShowFPS)
	{
		sprintf(str, "FPS: %d", Game->m_Fps);
		m_SystemFont->DrawText((BYTE*)str, 0, 0, 100, TAL_LEFT);
	}

	if(Game->m_DEBUG_DebugMode)
	{
		if(!Game->m_Renderer->m_Windowed)
			sprintf(str, "Mode: %dx%dx%d", m_Renderer->m_Width, m_Renderer->m_Height, m_Renderer->m_BPP);
		else
			sprintf(str, "Mode: %dx%d windowed", m_Renderer->m_Width, m_Renderer->m_Height);

		strcat(str, " (");
		strcat(str, m_Renderer->GetName());
		strcat(str, ")");
		m_SystemFont->DrawText((BYTE*)str, 0, 0, m_Renderer->m_Width, TAL_RIGHT);

		m_Renderer->DisplayDebugInfo();

		int ScrTotal, ScrRunning, ScrWaiting, ScrPersistent;
		ScrTotal = m_ScEngine->GetNumScripts(&ScrRunning, &ScrWaiting, &ScrPersistent);
		sprintf(str, "Running scripts: %d (r:%d w:%d p:%d)", ScrTotal, ScrRunning, ScrWaiting, ScrPersistent);
		m_SystemFont->DrawText((BYTE*)str, 0, 70, m_Renderer->m_Width, TAL_RIGHT);


		sprintf(str, "Timer: %d", m_Timer);
		Game->m_SystemFont->DrawText((BYTE*)str, 0, 130, m_Renderer->m_Width, TAL_RIGHT);

		if(m_ActiveObject!=NULL) m_SystemFont->DrawText((BYTE*)m_ActiveObject->m_Name, 0, 150, m_Renderer->m_Width, TAL_RIGHT);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::SetMaxShadowType(TShadowType MaxShadowType)
{
	if(MaxShadowType > SHADOW_STENCIL) MaxShadowType = SHADOW_STENCIL;
	if(MaxShadowType < 0) MaxShadowType = SHADOW_NONE;

	if(MaxShadowType==SHADOW_FLAT && !m_SupportsRealTimeShadows) MaxShadowType = SHADOW_SIMPLE;
	m_MaxShadowType = MaxShadowType;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
TShadowType CBGame::GetMaxShadowType(CBObject* Object)
{
	if(!Object) return m_MaxShadowType;
	else return min(m_MaxShadowType, Object->m_ShadowType);
}

//////////////////////////////////////////////////////////////////////////
CBDebugger* CBGame::GetDebugMgr()
{
	if(!m_DebugMgr) m_DebugMgr = new CBDebugger(this);
	return m_DebugMgr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetLayerSize(int* LayerWidth, int* LayerHeight, RECT* Viewport, bool* CustomViewport)
{
	if(m_Renderer)
	{
		*LayerWidth = m_Renderer->m_Width;
		*LayerHeight = m_Renderer->m_Height;
		*CustomViewport = false;
		CBPlatform::SetRect(Viewport, 0, 0, m_Renderer->m_Width, m_Renderer->m_Height);
		return S_OK;
	}
	else return E_FAIL;

}

//////////////////////////////////////////////////////////////////////////
DWORD CBGame::GetAmbientLightColor()
{
	return 0x00000000;
}

//////////////////////////////////////////////////////////////////////////
void CBGame::GetMousePos(POINT* Pos)
{
	CBPlatform::GetCursorPos(Pos);
	CBPlatform::ScreenToClient(Game->m_Renderer->m_Window, Pos);

	Pos->x -= m_Renderer->m_DrawOffsetX;
	Pos->y -= m_Renderer->m_DrawOffsetY;

	// Windows can squish maximized window if it's larger than desktop
	// so we need to modify mouse position appropriately (tnx mRax)
	if(m_Renderer->m_Windowed && ::IsZoomed(m_Renderer->m_Window))
	{
		RECT rc;
		::GetClientRect(m_Renderer->m_Window, &rc);
		Pos->x *= Game->m_Renderer->m_RealWidth;
		Pos->x /= (rc.right - rc.left);
		Pos->y *= Game->m_Renderer->m_RealHeight;
		Pos->y /= (rc.bottom - rc.top);
	}

	if(m_MouseLockRect.left != 0 && m_MouseLockRect.right != 0 && m_MouseLockRect.top != 0 && m_MouseLockRect.bottom != 0)
	{
		if(!CBPlatform::PtInRect(&m_MouseLockRect, *Pos))
		{
			Pos->x = max(m_MouseLockRect.left, Pos->x);
			Pos->y = max(m_MouseLockRect.top, Pos->y);

			Pos->x = min(m_MouseLockRect.right, Pos->x);
			Pos->y = min(m_MouseLockRect.bottom, Pos->y);

			POINT NewPos = *Pos;

			NewPos.x += m_Renderer->m_DrawOffsetX;
			NewPos.y += m_Renderer->m_DrawOffsetY;

			CBPlatform::ClientToScreen(Game->m_Renderer->m_Window, &NewPos);
			CBPlatform::SetCursorPos(NewPos.x, NewPos.y);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::GetFogParams(bool* FogEnabled, DWORD* FogColor, float* Start, float* End)
{
	*FogEnabled = false;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::MiniUpdate()
{
	if(!m_MiniUpdateEnabled) return S_OK;

	if(timeGetTime() - m_LastMiniUpdate > 200)
	{
		if(m_SoundMgr) m_SoundMgr->InitLoop();
		m_LastMiniUpdate = timeGetTime();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGame::OnScriptShutdown(CScScript* Script)
{
	return S_OK;
}