// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BGAME_H__DAA97522_137F_11D4_9F37_8F90855A4202__INCLUDED_)
#define AFX_BGAME_H__DAA97522_137F_11D4_9F37_8F90855A4202__INCLUDED_

#include "BSound.h"	// Added by ClassView
#include "BDebugger.h"
#include "dctypes.h"	// Added by ClassView
#include "wme_plugin.h"
#include "BSaveThumbHelper.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifdef WME_D3D9
	typedef IDirect3D9* (WINAPI* DLL_Direct3DCreate) (UINT SDKVersion);
#else
	typedef IDirect3D8* (WINAPI* DLL_Direct3DCreate) (UINT SDKVersion);	
	typedef HRESULT (WINAPI* DLL_DirectXFileCreate) (LPDIRECTXFILE* lplpDirectXFile);
#endif
typedef HRESULT (WINAPI* DLL_DirectDrawCreateEx) (GUID FAR* lpGuid, LPVOID* lplpDD, REFIID iid, IUnknown FAR* pUnkOuter);
typedef HRESULT (WINAPI* DLL_DirectDrawEnumerateEx) (LPDDENUMCALLBACKEXA lpCallback, LPVOID lpContext, DWORD dwFlags);
typedef HRESULT (WINAPI* DLL_DirectSoundCreate8) (LPCGUID pcGuidDevice, LPDIRECTSOUND8 *ppDS8, LPUNKNOWN pUnkOuter);
typedef HRESULT (WINAPI* DLL_DirectSoundEnumerate) (LPDSENUMCALLBACKA pDSEnumCallback, LPVOID pContext);

typedef void (__stdcall *ENGINE_LOG_CALLBACK)(char* Text, HRESULT Result, void* Data);


#define NUM_MUSIC_CHANNELS 5

class CBGame: public CBObject, public IWmeGame
{
public:
	struct TDirectXBindings
	{
		DLL_Direct3DCreate Direct3DCreate;
		DLL_DirectDrawCreateEx DirectDrawCreateEx;
		DLL_DirectDrawEnumerateEx DirectDrawEnumerateEx;
		DLL_DirectSoundCreate8 DirectSoundCreate8;
		DLL_DirectSoundEnumerate DirectSoundEnumerate;
		HINSTANCE hDDraw;
		HINSTANCE hD3D;
		HINSTANCE hDSound;
#ifndef WME_D3D9
		HINSTANCE hDXof;
		DLL_DirectXFileCreate DirectXFileCreate;
#endif
	};

	__declspec(dllexport) HRESULT BindDirectX();
	HRESULT UnBindDirectX();
	TDirectXBindings m_DirectX;
	__declspec(dllexport) bool IsDirectXBound();

	DECLARE_PERSISTENT(CBGame, CBObject);

	virtual HRESULT OnScriptShutdown(CScScript* Script);

	virtual HRESULT GetLayerSize(int* LayerWidth, int* LayerHeight, RECT* Viewport, bool* CustomViewport);
	virtual DWORD GetAmbientLightColor();
	virtual HRESULT GetFogParams(bool* FogEnabled, DWORD* FogColor, float* Start, float* End);

	virtual CBObject* GetNextAccessObject(CBObject* CurrObject);
	virtual CBObject* GetPrevAccessObject(CBObject* CurrObject);

	virtual HRESULT OnActivate(bool Activate, bool RefreshMouse);
	virtual HRESULT OnMouseLeftDown();
	virtual HRESULT OnMouseLeftUp();
	virtual HRESULT OnMouseLeftDblClick();
	virtual HRESULT OnMouseRightDblClick();
	virtual HRESULT OnMouseRightDown();
	virtual HRESULT OnMouseRightUp();
	virtual HRESULT OnMouseMiddleDown();
	virtual HRESULT OnMouseMiddleUp();
	virtual HRESULT OnPaint();
	virtual HRESULT OnWindowClose();
	virtual HRESULT OnWinsock(WPARAM wparam, LPARAM lparam);

	bool m_AutorunDisabled;
	DWORD m_LastMiniUpdate;
	bool m_MiniUpdateEnabled;
	virtual HRESULT MiniUpdate();

	void GetMousePos(POINT* Pos);
	RECT m_MouseLockRect;

	bool m_ShuttingDown;

	virtual HRESULT DisplayDebugInfo();
	bool m_DEBUG_ShowFPS;

	bool m_SuspendedRendering;
	int m_SoundBufferSizeSec;
	virtual HRESULT RenderShadowGeometry();

	TTextEncoding m_TextEncoding;
	bool m_TextRTL;

	CBSprite* m_LoadingIcon;
	int m_LoadingIconX;
	int m_LoadingIconY;
	int m_LoadingIconPersistent;

	virtual HRESULT ResetContent();

	void DEBUG_DumpClassRegistry();
	HRESULT SetWaitCursor(char* Filename);

	char* m_LocalSaveDir;
	bool m_SaveDirChecked;

	bool m_SupportsRealTimeShadows;
	TShadowType m_MaxShadowType;
	HRESULT SetMaxShadowType(TShadowType MaxShadowType);
	virtual TShadowType GetMaxShadowType(CBObject* Object=NULL);

	bool m_IndicatorDisplay;
	DWORD m_IndicatorColor;
	int m_IndicatorProgress;
	int m_IndicatorX;
	int m_IndicatorY;
	int m_IndicatorWidth;
	int m_IndicatorHeight;

	bool m_RichSavedGames;
	char* m_SavedGameExt;

	int m_EditorResolutionWidth;
	int m_EditorResolutionHeight;

	char* m_LoadImageName;
	char* m_SaveImageName;
	int m_SaveImageX;
	int m_SaveImageY;
	int m_LoadImageX;
	int m_LoadImageY;
	CBSurface* m_SaveLoadImage;

	HRESULT DisplayIndicator();

	int m_ThumbnailWidth;
	int m_ThumbnailHeight;

	bool m_ReportTextureFormat;
	HMODULE m_ResourceModule;
	void SetResourceModule(HMODULE ResModule);

	void SetEngineLogCallback(ENGINE_LOG_CALLBACK Callback=NULL, void* Data=NULL);
	ENGINE_LOG_CALLBACK m_EngineLogCallback;
	void* m_EngineLogCallbackData;

	bool m_EditorMode;
	bool m_DoNotExpandStrings;
	void GetOffset(int* OffsetX, int* OffsetY);
	void SetOffset(int OffsetX, int OffsetY);
	int GetSequence();
	int m_OffsetY;
	int m_OffsetX;
	float m_OffsetPercentX;
	float m_OffsetPercentY;
	CBObject* m_MainObject;
	HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
	HRESULT InitLoop();
	DWORD m_CurrentTime;
	DWORD m_DeltaTime;
	CBFont* m_SystemFont;
	CBFont* m_VideoFont;
	HRESULT Initialize1();
	HRESULT Initialize2();
	HRESULT Initialize3();
	CBFileManager* m_FileManager;
	CBNetworkMgr* m_NetworkMgr;
	CBAccessMgr* m_AccessMgr;
	CBTransitionMgr* m_TransMgr;
	CBPluginMgr* m_PluginMgr;	
	CBDebugger* GetDebugMgr();
	void __cdecl LOG(HRESULT res, LPSTR fmt, ...);
	CBRenderer* m_Renderer;
	CBSoundMgr* m_SoundMgr;
	CScEngine* m_ScEngine;
	CSXMath* m_MathClass;
	CSXDirectory* m_DirectoryClass;
	CBSurfaceStorage* m_SurfaceStorage;
	CBFontStorage* m_FontStorage;
	CBGame();
	virtual ~CBGame();
	void DEBUG_DebugDisable();
	void DEBUG_DebugEnable(const char* Filename=NULL);
	bool m_DEBUG_DebugMode;
	bool m_DEBUG_AbsolutePathWarning;
	FILE* m_DEBUG_LogFile;
	int m_Sequence;
	virtual HRESULT LoadFile(char * Filename);
	virtual HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
	CBArray<CBQuickMsg*, CBQuickMsg*> m_QuickMessages;	
	CBArray<CUIWindow*, CUIWindow*> m_Windows;
	CBArray<CBViewport*, CBViewport*> m_ViewportStack;
	int m_ViewportSP;
	bool m_MouseLeftDown;
	bool m_MouseRightDown;
	bool m_MouseMidlleDown;
	CBStringTable* m_StringTable;

	int m_SettingsResWidth;
	int m_SettingsResHeight;
	bool m_SettingsRequireAcceleration;
	bool m_SettingsAllowWindowed;
	bool m_SettingsAllowAdvanced;
	bool m_SettingsAllowAccessTab;
	bool m_SettingsAllowAboutTab;
	bool m_SettingsRequireSound;
	bool m_SettingsAllowDesktopRes;
	int m_SettingsTLMode;
	char* m_SettingsGameFile;
	CBFader* m_Fader;
	bool m_SuppressScriptErrors;

	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();

	virtual void PublishNatives();
	virtual HRESULT ExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();

	// compatibility bits
	bool m_CompatKillMethodThreads;

	// FPS stuff
private:	
	DWORD m_LastTime;
	DWORD m_FpsTime;
	DWORD m_FramesRendered;	

public:
	DWORD m_SurfaceGCCycleTime;
	bool m_SmartCache;
	bool m_VideoSubtitles;
	bool m_Subtitles;
	DWORD m_MusicStartTime[NUM_MUSIC_CHANNELS];
	bool m_CompressedSavegames;
	int m_ScheduledLoadSlot;
	bool m_Loading;
	bool m_PersonalizedSave;
	HRESULT EmptySaveSlot(int Slot);
	bool IsSaveSlotUsed(int Slot);
	HRESULT GetSaveSlotDescription(int Slot, char* Buffer);
	HRESULT GetSaveSlotFilename(int Slot, char* Buffer);
	void SetWindowTitle();
	virtual bool HandleMouseWheel(int Delta);
	bool m_Quitting;
	virtual HRESULT GetVersion(BYTE* VerMajor, BYTE* VerMinor, BYTE* ExtMajor, BYTE* ExtMinor);
	virtual bool HandleKeypress(bool Printable, DWORD CharCode, DWORD KeyData);
	bool HandleAccessKey(bool Printable, DWORD CharCode, DWORD KeyData);
	int m_FreezeLevel;
	HRESULT Unfreeze();
	HRESULT Freeze(bool IncludingMusic=true);
	HRESULT FocusWindow(CUIWindow* Window);
	CVidPlayer* m_VideoPlayer;
	CVidTheoraPlayer* m_TheoraPlayer;
	bool m_LoadInProgress;
	CUIWindow* m_FocusedWindow;
	bool m_EditorForceScripts;
	static void AfterLoadRegion(void* Region, void* Data);
	static void AfterLoadSubFrame(void* Subframe, void* Data);
	static void AfterLoadSound(void* Sound, void* Data);
	static void AfterLoadTheora(void* Theora, void* Data);
	static void AfterLoadXModel(void* Model, void* Data);
	static void AfterLoadScript(void* script, void* data);
	static void InvalidateValues(void* Value, void* Data);
	HRESULT LoadSettings(char* Filename);
	HRESULT ResumeMusic(int Channel);
	HRESULT SetMusicStartTime(int Channel, DWORD Time);
	HRESULT PauseMusic(int Channel);
	HRESULT StopMusic(int Channel);
	HRESULT PlayMusic(int Channel, char* Filename, bool Looping = true, DWORD LoopStart=0, DWORD PrivVolume=100);
	CBSound* m_Music[NUM_MUSIC_CHANNELS];
	bool m_MusicCrossfadeRunning;
	bool m_MusicCrossfadeSwap;
	DWORD m_MusicCrossfadeStartTime;
	DWORD m_MusicCrossfadeLength;
	int m_MusicCrossfadeChannel1;
	int m_MusicCrossfadeChannel2;
	HRESULT DisplayWindows(bool InGame=false);
	CBRegistry* m_Registry;
	bool m_UseD3D;
	virtual HRESULT Cleanup();
	virtual HRESULT LoadGame(int Slot);
	virtual HRESULT LoadGame(char* Filename);
	virtual HRESULT SaveGame(int Slot, char* Desc, bool quickSave = false);
	virtual HRESULT ShowCursor();
	CBSprite* m_CursorNoninteractive;
	CBObject* m_ActiveObject;
	CBKeyboardState* m_KeyboardState;
	bool m_Interactive;
	TGameState m_State;
	TGameState m_OrigState;
	bool m_OrigInteractive;
	DWORD m_Timer;	
	DWORD m_TimerDelta;
	DWORD m_TimerLast;

	DWORD m_LiveTimer;
	DWORD m_LiveTimerDelta;
	DWORD m_LiveTimerLast;

	CBObject* m_CapturedObject;
	POINT m_MousePos;
	bool ValidObject(CBObject* Object);
	HRESULT UnregisterObject(CBObject* Object);
	HRESULT RegisterObject(CBObject* Object);
	void QuickMessage(char* Text);
	void QuickMessageForm(LPSTR fmt, ...);
	HRESULT DisplayQuickMsg();
	DWORD m_Fps;
	HRESULT UpdateMusicCrossfade();

	bool IsVideoPlaying();
	HRESULT StopVideo();

	CBArray<CBObject*, CBObject*> m_RegObjects;

	// accessibility flags
	bool m_AccessTTSEnabled;
	bool m_AccessTTSTalk;
	bool m_AccessTTSCaptions;
	bool m_AccessTTSKeypress;
	bool m_AccessKeyboardEnabled;
	bool m_AccessKeyboardCursorSkip;
	bool m_AccessKeyboardPause;
	
	bool m_AccessGlobalPaused;

	CUIWindow* m_AccessShieldWin;
	HRESULT AccessPause();
	HRESULT AccessUnpause();
	
public:
	virtual HRESULT DisplayContent(bool Update=true, bool DisplayAll=false);
	virtual HRESULT DisplayContentSimple();
	bool m_ForceNonStreamedSounds;
	void ResetMousePos();
	int m_SubtitlesSpeed;
	void SetInteractive(bool State);
	virtual HRESULT WindowLoadHook(CUIWindow* Win, char** Buf, char** Params);
	virtual HRESULT WindowScriptMethodHook(CUIWindow* Win, CScScript* Script, CScStack* Stack, char* Name);
	HRESULT GetCurrentViewportOffset(int* OffsetX=NULL, int* OffsetY=NULL);
	HRESULT GetCurrentViewportRect(RECT* Rect, bool* Custom=NULL);
	HRESULT PopViewport();
	HRESULT PushViewport(CBViewport* Viewport);
	HRESULT SetActiveObject(CBObject* Obj);	

	CBSprite* m_LastCursor;
	HRESULT DrawCursor(CBSprite* Cursor);

	virtual HRESULT InitAfterLoad();

	CBSaveThumbHelper* m_CachedThumbnail;
	GUID m_GameGUID;

private:
	HRESULT GetSaveDir(char* Buffer);
	CBDebugger* m_DebugMgr;


// IWmeGame
public:
	IWmeValue* CreateValue();
	bool DeleteValue(IWmeValue* Value);
	IWmeParamSet* CreateParamSet();
	bool DeleteParamSet(IWmeParamSet* ParamSet);

	bool SubscribeEvent(IWmeObject* Object, EWmeEvent Event);
	bool UnsubscribeEvent(IWmeObject* Object, EWmeEvent Event);

	IWmeFile* OpenFile(const char* Filename);
	bool CloseFile(IWmeFile* File);

	IWmeObject* CreateObject(const char* ClassName, IWmeParamSet* Params=NULL);

// IWmeObject
public:
	bool SendEvent(const char* EventName);
	IWmeValue* CallMethod(const char* MethodName, IWmeParamSet* Params=0);
	bool SetProperty(const char* PropName, IWmeValue* Value);
	bool SetProperty(const char* PropName, int Value);
	bool SetProperty(const char* PropName, const char* Value);
	bool SetProperty(const char* PropName, double Value);
	bool SetProperty(const char* PropName, bool Value);
	bool SetProperty(const char* PropName, IWmeObject* Value);
	bool SetProperty(const char* PropName);
	IWmeValue* GetProperty(const char* PropName);
	void* GetInterface(const char* ClassName);
};

#endif // !defined(AFX_BGAME_H__DAA97522_137F_11D4_9F37_8F90855A4202__INCLUDED_)
