// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

#include "WScEngine.h"

#ifdef SetCurrentDirectory
	#undef SetCurrentDirectory
#endif

#ifdef CreateFont
	#undef CreateFont
#endif


namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace System::Runtime::InteropServices;
	using namespace System::Reflection;
	using namespace System::IO;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace DeadCode::WME::Core;

	
	public delegate void WEngineLogCallback(String^ Text, int Result);
	public delegate void WScriptElementCallback(int Line, int Type, String^ Text);
	public delegate void WScriptCompilerCallback(int Line, String^ Text);
	
	/// <summary>
	/// Summary for WGame
	/// </summary>
	public ref class WGame : public WObject
	{
	public:
		WGame(void);
		WGame(CBGame* Native);
		virtual ~WGame();

		//////////////////////////////////////////////////////////////////////////
		// Callbacks
		//////////////////////////////////////////////////////////////////////////
		void SetLogCallback(WEngineLogCallback^ Callback);
		void SetScriptCompilerCallback(WScriptCompilerCallback^ Callback);		
		void SetScriptElementCallback(WScriptElementCallback^ Callback);

		//////////////////////////////////////////////////////////////////////////
		// Properties
		//////////////////////////////////////////////////////////////////////////
		property bool EditorMode
		{
			bool get()
			{
				return Native->m_EditorMode;
			}
			void set(bool Value)
			{
				Native->m_EditorMode = Value;
			}
		}

		property WRenderer^ Renderer
		{
			WRenderer^ get()
			{
				if(Native->m_Renderer) return gcnew WRenderer(Native->m_Renderer);
				else return nullptr;
			}
		}

		property String^ RegistryPath
		{
			String^ get()
			{
				return gcnew String(Native->m_Registry->GetBasePath());
			}
			void set(String^ Value)
			{
				const char* S = WUtils::GetString(Value);
				Native->m_Registry->SetBasePath((char*)S);
				WUtils::FreeString(S);
			}
		}

		property bool Interactive
		{
			bool get()
			{
				return Native->m_Interactive;
			}
			void set(bool Value)
			{
				Native->SetInteractive(Value);
			}
		}

		property bool UseD3D
		{
			bool get()
			{
				return Native->m_UseD3D;
			}
			void set(bool Value)
			{
				Native->m_UseD3D = Value;
			}
		}

		property String^ ProjectFile
		{
			String^ get()
			{
				return gcnew String(Native->m_Registry->GetIniName());
			}
			void set(String^ Value)
			{
				const char* S;

				// set project
				S = WUtils::GetString(Value);
				Native->m_Registry->SetIniName((char*)S);
				WUtils::FreeString(S);

				// set working dir
				String^ ProjectPath = Path::GetDirectoryName(Value);
				if(ProjectPath)
				{
					Directory::SetCurrentDirectory(ProjectPath);

					// ???
					S = WUtils::GetString(ProjectPath);
					Native->m_FileManager->SetBasePath((char*)S);
					WUtils::FreeString(S);
				}
			}
		}

		property WScEngine^ ScriptEngine
		{
			WScEngine^ get()
			{
				if(Native->m_ScEngine) return gcnew WScEngine(Native->m_ScEngine);
				else return nullptr;
			}
		}

		property int FPS
		{
			int get()
			{
				return Native->m_Fps;
			}
		}

		property UInt32 CurrentTime
		{
			UInt32 get()
			{
				return Native->m_CurrentTime;
			}
		}

		property UInt32 WindowsTime
		{
			UInt32 get()
			{
				return timeGetTime();
			}
		}

		property Point MousePos
		{
			Point get()
			{
				POINT Pos;
				Native->GetMousePos(&Pos);
				return Point(Pos.x, Pos.y);
			}
		}

		property bool ForceScripts
		{
			bool get()
			{
				return Native->m_EditorForceScripts;
			}
			void set(bool Value)
			{
				Native->m_EditorForceScripts = Value;
			}
		}

		property bool AbsolutePathWarning
		{
			bool get()
			{
				return Native->m_DEBUG_AbsolutePathWarning;
			}
			void set(bool Value)
			{
				Native->m_DEBUG_AbsolutePathWarning = Value;
			}
		}

		property bool ForceNonStreamedSounds
		{
			bool get()
			{
				return Native->m_ForceNonStreamedSounds;
			}
			void set(bool Value)
			{
				Native->m_ForceNonStreamedSounds = Value;
			}
		}

		property WObject^ MainObject
		{
			WObject^ get()
			{
				if(Native->m_MainObject) return gcnew WObject(Native->m_MainObject);
				else return nullptr;
			}
			void set(WObject^ Value)
			{
				if(Value==nullptr) Native->m_MainObject = NULL;
				else Native->m_MainObject = Value->Native;
			}
		}

		property WObject^ ActiveObject
		{
			WObject^ get()
			{
				if(Native->m_ActiveObject) return gcnew WObject(Native->m_ActiveObject);
				else return nullptr;
			}
			void set(WObject^ Value)
			{
				if(Value==nullptr) Native->SetActiveObject(NULL);
				else Native->SetActiveObject(Value->Native);
			}
		}

		property bool Subtitles
		{
			bool get()
			{
				return Native->m_Subtitles;
			}
			void set(bool Value)
			{
				Native->m_Subtitles = Value;
			}
		}

		property bool VideoSubtitles
		{
			bool get()
			{
				return Native->m_VideoSubtitles;
			}
			void set(bool Value)
			{
				Native->m_VideoSubtitles = Value;
			}
		}

		property bool PersonalSavegames
		{
			bool get()
			{
				return Native->m_PersonalizedSave;
			}
			void set(bool Value)
			{
				Native->m_PersonalizedSave = Value;
			}
		}

		property bool DoNotExpandStrings
		{
			bool get()
			{
				return Native->m_DoNotExpandStrings;
			}
			void set(bool Value)
			{
				Native->m_DoNotExpandStrings = Value;
			}
		}

		property int SubtitlesSpeed
		{
			int get()
			{
				return Native->m_SubtitlesSpeed;
			}
			void set(int Value)
			{
				Native->m_SubtitlesSpeed = Value;
			}
		}

		property WUIWindow^ FocusedWindow
		{
			WUIWindow^ get()
			{
				if(Native->m_FocusedWindow) return gcnew WUIWindow(Native->m_FocusedWindow);
				else return nullptr;
			}
			void set(WUIWindow^ Value)
			{
				if(Value==nullptr) Native->m_FocusedWindow = NULL;
				else Native->FocusWindow(Value->Native);
			}
		}

		property int ThumbnailWidth
		{
			int get()
			{
				return Native->m_ThumbnailWidth;
			}
			void set(int Value)
			{
				Native->m_ThumbnailWidth = Value;
			}
		}

		property int ThumbnailHeight
		{
			int get()
			{
				return Native->m_ThumbnailHeight;
			}
			void set(int Value)
			{
				Native->m_ThumbnailHeight = Value;
			}
		}

		property bool IndicatorDisplay
		{
			bool get()
			{
				return Native->m_IndicatorDisplay;
			}
			void set(bool Value)
			{
				Native->m_IndicatorDisplay = Value;
			}
		}

		property int IndicatorX
		{
			int get()
			{
				return Native->m_IndicatorX;
			}
			void set(int Value)
			{
				Native->m_IndicatorX = Value;
			}
		}

		property int IndicatorY
		{
			int get()
			{
				return Native->m_IndicatorY;
			}
			void set(int Value)
			{
				Native->m_IndicatorY = Value;
			}
		}

		property int IndicatorWidth
		{
			int get()
			{
				return Native->m_IndicatorWidth;
			}
			void set(int Value)
			{
				Native->m_IndicatorWidth = Value;
			}
		}

		property int IndicatorHeight
		{
			int get()
			{
				return Native->m_IndicatorHeight;
			}
			void set(int Value)
			{
				Native->m_IndicatorHeight = Value;
			}
		}

		property System::Drawing::Color IndicatorColor
		{
			System::Drawing::Color get()
			{
				return System::Drawing::Color::FromArgb(Native->m_IndicatorColor);
			}
			void set(System::Drawing::Color Value)
			{
				Native->m_IndicatorColor = Value.ToArgb();
			}
		}

		property WSprite^ NoninteractiveCursor
		{
			WSprite^ get()
			{
				if(Native->m_CursorNoninteractive) return gcnew WSprite(Native->m_CursorNoninteractive);
				else return nullptr;
			}
			void set(WSprite^ Value)
			{
				SAFE_DELETE(Native->m_CursorNoninteractive);
				if(Value==nullptr) Native->m_CursorNoninteractive = NULL;
				else Native->m_CursorNoninteractive = Value->Native;
			}
		}

		bool SetOffset(int X, int Y)
		{
			Native->SetOffset(X, Y);
			return true;
		}

		property WFont^ SystemFont
		{
			WFont^ get()
			{
				if(Native->m_SystemFont) return gcnew WFont(Native->m_SystemFont);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(Native->m_SystemFont) Native->m_FontStorage->RemoveFont(Native->m_SystemFont);

				if(Value==nullptr) Native->m_SystemFont = NULL;
				else Native->m_SystemFont = Value->Native;
			}
		}

		property WFont^ VideoFont
		{
			WFont^ get()
			{
				if(Native->m_VideoFont) return gcnew WFont(Native->m_VideoFont);
				else return nullptr;
			}
			void set(WFont^ Value)
			{
				if(Native->m_VideoFont) Native->m_FontStorage->RemoveFont(Native->m_VideoFont);

				if(Value==nullptr) Native->m_VideoFont = NULL;
				else Native->m_VideoFont = Value->Native;
			}
		}

		property IntPtr ResourceModule
		{
			IntPtr get()
			{
				return IntPtr(Native->m_ResourceModule);
			}
			void set(IntPtr Value)
			{
				Native->m_ResourceModule = (HMODULE)Value.ToPointer();
			}
		}

		property String^ LocalSaveDir
		{
			String^ get()
			{
				if(!Native->m_LocalSaveDir) return nullptr;
				else return gcnew String(Native->m_LocalSaveDir);
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_LocalSaveDir, S);
				WUtils::FreeString(S);
			}
		}

		property String^ LoadImageName
		{
			String^ get()
			{
				if(!Native->m_LoadImageName) return nullptr;
				else return gcnew String(Native->m_LoadImageName);
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_LoadImageName, S);
				WUtils::FreeString(S);
			}
		}
		
		property String^ SaveImageName
		{
			String^ get()
			{
				if(!Native->m_SaveImageName) return nullptr;
				else return gcnew String(Native->m_SaveImageName);
			}
			void set(String^ Value)
			{
				char* S = (char*)WUtils::GetString(Value);
				CBUtils::SetString(&Native->m_SaveImageName, S);
				WUtils::FreeString(S);
			}
		}

		property int LoadImageX
		{
			int get()
			{
				return Native->m_LoadImageX;
			}
			void set(int Value)
			{
				Native->m_LoadImageX = Value;
			}
		}

		property int LoadImageY
		{
			int get()
			{
				return Native->m_LoadImageY;
			}
			void set(int Value)
			{
				Native->m_LoadImageY = Value;
			}
		}

		property int SaveImageX
		{
			int get()
			{
				return Native->m_SaveImageX;
			}
			void set(int Value)
			{
				Native->m_SaveImageX = Value;
			}
		}

		property int SaveImageY
		{
			int get()
			{
				return Native->m_SaveImageY;
			}
			void set(int Value)
			{
				Native->m_SaveImageY = Value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// 3D related
		//////////////////////////////////////////////////////////////////////////
		property bool SupportsRealTimeShadows
		{
			bool get()
			{
				return Native->m_SupportsRealTimeShadows;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Font related
		//////////////////////////////////////////////////////////////////////////
		WFont^ CreateFont(String^ Filename)
		{
			if(!Native->m_FontStorage) return nullptr;

			char* S = (char*)WUtils::GetString(Filename);
			CBFont* Font = Native->m_FontStorage->AddFont(S);
			WUtils::FreeString(S);

			if(Font) return gcnew WFont(Font);
			else return nullptr;
		}

		void DestroyFont(WFont^% Font)
		{
			if(Font!=nullptr && Native->m_FontStorage)
			{
				Native->m_FontStorage->RemoveFont(Font->Native);
				Font = nullptr;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// registry
		//////////////////////////////////////////////////////////////////////////		
		String^ GetRegString(String^ Subkey, String^ Key, String^ InitVal)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);
			char* SInitVal = (char*)WUtils::GetString(InitVal);

			char* Buf = new char[1024];
			DWORD Size = 1024;
			Native->m_Registry->ReadString(SSubkey, SKey, Buf, &Size, SInitVal);

			String^ Ret = gcnew String(Buf);

			delete [] Buf;
			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);
			WUtils::FreeString(SInitVal);

			return Ret;
		}

		String^ GetRegString(String^ Subkey, String^ Key)
		{
			return GetRegString(Subkey, Key, nullptr);
		}

		bool SetRegString(String^ Subkey, String^ Key, String^ Value)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);
			char* SValue = (char*)WUtils::GetString(Value);

			bool Ret = Native->m_Registry->WriteString(SSubkey, SKey, SValue);

			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);
			WUtils::FreeString(SValue);

			return Ret;
		}

		int GetRegInt(String^ Subkey, String^ Key, int InitVal)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);

			int Ret = Native->m_Registry->ReadInt(SSubkey, SKey, InitVal);

			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);

			return Ret;	
		}

		int GetRegInt(String^ Subkey, String^ Key)
		{
			return GetRegInt(Subkey, Key, 0);
		}

		bool SetRegInt(String^ Subkey, String^ Key, int Value)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);

			bool Ret = Native->m_Registry->WriteInt(SSubkey, SKey, Value);

			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);

			return Ret;
		}

		bool GetRegBool(String^ Subkey, String^ Key, bool InitVal)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);

			bool Ret = Native->m_Registry->ReadBool(SSubkey, SKey, InitVal);

			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);

			return Ret;
		}

		bool GetRegBool(String^ Subkey, String^ Key)
		{
			return GetRegBool(Subkey, Key, false);
		}

		bool SetRegBool(String^ Subkey, String^ Key, bool Value)
		{
			char* SSubkey = (char*)WUtils::GetString(Subkey);
			char* SKey = (char*)WUtils::GetString(Key);

			bool Ret = Native->m_Registry->WriteBool(SSubkey, SKey, Value);

			WUtils::FreeString(SSubkey);
			WUtils::FreeString(SKey);

			return Ret;
		}


		//////////////////////////////////////////////////////////////////////////
		// initialization
		//////////////////////////////////////////////////////////////////////////
		bool Initialize1()
		{
			return SUCCEEDED(Native->Initialize1());
		}

		bool Initialize2()
		{
			return SUCCEEDED(Native->Initialize2());
		}

		bool Initialize3()
		{
			return SUCCEEDED(Native->Initialize3());
		}

		bool LoadSettings(String^ Filename)
		{
			bool ret;

			char* S = (char*)WUtils::GetString(Filename);
			ret = SUCCEEDED(Native->LoadSettings(S));
			WUtils::FreeString(S);
			return ret;
		}

		bool InitializeWindowed(Control^ Canvas, bool Accelerated, String^ ProjectFile)
		{
			return InitializeWindowed(Canvas, Accelerated, ProjectFile, nullptr);
		}

		bool InitializeWindowed(Control^ Canvas, bool Accelerated, String^ ProjectFile, String^ LogFilename)
		{
			if(!Native) return false;

			if(LogFilename!=nullptr) DebugEnable(LogFilename);

			HWND Window = SAFE_CONTROL_HWND(Canvas);

			// initialize renderer
			Native->m_UseD3D = Accelerated;
			Native->Initialize1();
			Native->Initialize2();
			Native->m_Renderer->m_Windowed = true;
			Native->m_Renderer->m_Instance = NULL;

			this->ProjectFile = ProjectFile;

			CHWManager* m = new CHWManager(Native);
			m->m_Force16bitTextures = false;
			m->m_ForceTextureSubdivision = false;
			m->AutoselectWindowedDevice(Native->m_UseD3D, !Native->m_UseD3D, Window);
			

			//Game->InitInput(Instance, Window);
			Native->m_Renderer->m_Window = Window;
			Native->m_Renderer->m_ClipperWindow = Window;
			Native->m_Renderer->m_Ready = (Native->m_Renderer->InitRenderer(m)==S_OK);

			if(!Native->m_Renderer->m_Ready && Native->m_UseD3D)
			{
				//MessageBox(LOC("/str1056/Error initializing accelerated renderer. Sticking back to software..."), NULL, MB_OK|MB_ICONWARNING);
				SAFE_DELETE(Native->m_Renderer);
				Native->m_UseD3D = false;
				Native->Initialize2();

				m->AutoselectWindowedDevice(false, true, Window);

				Native->m_Renderer->m_Window = Window;
				Native->m_Renderer->m_ClipperWindow = Window;
				Native->m_Renderer->m_Ready = (Native->m_Renderer->InitRenderer(m)==S_OK); 
			}

			// initialize sound manager
			HRESULT ret = Native->m_SoundMgr->Initialize(Window, m);
			if(FAILED(ret))
			{
				Native->LOG(ret, "Sound is NOT available.");
			}
			delete m;

			if(Native->m_Renderer->m_Ready)
			{
				Renderer->Resize(Canvas->ClientSize.Width, Canvas->ClientSize.Height);

				Native->m_SystemFont = Native->m_FontStorage->AddFont("system_font.fnt");
				Native->m_ShadowImage = Native->m_SurfaceStorage->AddSurface("shadow.png");
			}

			return Native->m_Renderer->m_Ready;
		}

		bool InitializeNoRenderer(Control^ SoundWindow, String^ ProjectFile)
		{
			return InitializeNoRenderer(SoundWindow, ProjectFile, nullptr);
		}

		bool InitializeNoRenderer(Control^ SoundWindow, String^ ProjectFile, String^ LogFilename)
		{
			if(!Native) return false;

			if(LogFilename!=nullptr) DebugEnable(LogFilename);
			this->ProjectFile = ProjectFile;

			CHWManager* m = new CHWManager(Native);
			m->EnumDSDevices();
			if(m->m_AudioDevices.GetSize()>0) m->m_SelectedAudioDevice = m->m_AudioDevices[0];

			// initialize sound manager
			HWND Window = SAFE_CONTROL_HWND(SoundWindow);
			HRESULT ret = Native->m_SoundMgr->Initialize(Window, m);
			if(FAILED(ret))
			{
				Native->LOG(ret, "Sound is NOT available.");
			}
			delete m;

			return true;
		}

		bool InitializeSimple(String^ ProjectFile)
		{
			if(!Native) return false;

			bool ret = SUCCEEDED(Native->Initialize1());
			this->ProjectFile = ProjectFile;
			return ret;
		}

		property WDynArray<CBObject*, WObject>^ RegObjects
		{
			WDynArray<CBObject*, WObject>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CBObject*, WObject>(&Native->m_RegObjects);
			}
		}

		property WDynArray<CUIWindow*, WUIWindow>^ Windows
		{
			WDynArray<CUIWindow*, WUIWindow>^ get()
			{
				if(!Native) return nullptr;
				else return gcnew WDynArray<CUIWindow*, WUIWindow>(&Native->m_Windows);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// frame / display
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		bool FrameInit()
		{
			return FrameInit(true, System::Drawing::Color::Black);
		}

		bool FrameInit(bool Update)
		{
			return FrameInit(Update, System::Drawing::Color::Black);
		}

		bool FrameInit(bool Update, System::Drawing::Color BgColor)
		{
			if (Native->m_Renderer && Native->m_Renderer->m_Ready)
			{
				// init loop
				Native->m_Renderer->OnWindowChange();
				if(Update) Native->InitLoop();		

				if(FAILED(Native->m_Renderer->Fill(BgColor.R, BgColor.G, BgColor.B))) return false;
				else return true;
			}
			else return false;
		}

		bool FrameFinish()
		{
			if (Native->m_Renderer && Native->m_Renderer->m_Ready)
			{
				Native->m_Renderer->Setup2D();
				Native->DisplayQuickMsg();

				// flip
				return SUCCEEDED(Native->m_Renderer->Flip());
			}
			else return false;
		}

		bool UpdateSounds()
		{
			if(Native->m_SoundMgr)
			{
				// init loop
				if(FAILED(Native->m_SoundMgr->InitLoop())) return false;
				else return true;
			}
			else return false;
		}

		bool DisplayWindows(bool InGame)
		{
			return SUCCEEDED(Native->DisplayWindows(InGame));
		}

		virtual bool DisplayCursor()
		{
			return SUCCEEDED(Native->ShowCursor());
		}

		virtual bool DisplayContent(bool Update)
		{
			return SUCCEEDED(Native->DisplayContent(Update));
		}

		virtual bool DisplayContent()
		{
			return DisplayContent(true);
		}

		virtual bool DisplayContentSimple()
		{
			return SUCCEEDED(Native->DisplayContentSimple());
		}


		//////////////////////////////////////////////////////////////////////////
		// path related
		//////////////////////////////////////////////////////////////////////////
		bool RestoreCurrentPath()
		{
			if(Native->m_FileManager)
			{
				return SUCCEEDED(Native->m_FileManager->RestoreCurrentDir());
			}
			else return false;
		}

		property int NumSinglePaths
		{
			int get()
			{
				if(Native->m_FileManager) return Native->m_FileManager->m_SinglePaths.GetSize();
				else return 0;
			}
		}

		String^ GetSinglePath(int Index)
		{
			if(!Native->m_FileManager || Index < 0 || Index >= Native->m_FileManager->m_SinglePaths.GetSize()) return nullptr;
			else
			{
				return gcnew String(Native->m_FileManager->m_SinglePaths[Index]);
			}
		}

		String^ MakeRelativePath(String^ FullPath)
		{	
			if(Native->m_FileManager)
			{
				char* S = (char*)WUtils::GetString(FullPath);	
				Native->m_FileManager->MakeRelativePath(S);
				String^ Ret = gcnew String(S);
				WUtils::FreeString(S);
				return Ret;
			}
			else return FullPath;
		}

		String^ MakeAbsolutePath(String^ RelativePath)
		{	
			if(Native->m_FileManager)
			{
				char NewPath[MAX_PATH];
				char* S = (char*)WUtils::GetString(RelativePath);	
				Native->m_FileManager->MakeAbsolutePath(S, NewPath, MAX_PATH);
				String^ Ret = gcnew String(NewPath);
				WUtils::FreeString(S);
				return Ret;
			}
			else return RelativePath;
		}

		//////////////////////////////////////////////////////////////////////////
		// Diagnostics
		//////////////////////////////////////////////////////////////////////////
		void WriteLog(String^ Text)
		{
			const char* S = WUtils::GetString(Text);
			Native->LOG(0, (char*)S);
			WUtils::FreeString(S);
		}

		void DebugEnable(String^ LogFilename)
		{
			char* S = (char*)WUtils::GetString(LogFilename);
			Native->DEBUG_DebugEnable(S);
			WUtils::FreeString(S);
		}

		void DebugDisable()
		{
			Native->DEBUG_DebugDisable();
		}

		void QuickMessage(String^ Text)
		{
			char* S = (char*)WUtils::GetString(Text);
			Native->QuickMessage(S);
			WUtils::FreeString(S);
		}


		//////////////////////////////////////////////////////////////////////////
		// Object management
		//////////////////////////////////////////////////////////////////////////
		bool RegisterObject(WObject^ Object)
		{
			if(Object==nullptr) return false;
			else return SUCCEEDED(Native->RegisterObject(Object->Native));
		}

		bool UnregisterObject(WObject^% Object)
		{
			if(Object==nullptr) return false;
			else
			{
				bool Ret = SUCCEEDED(Native->UnregisterObject(Object->Native));
				if(Ret) Object = nullptr;
				return Ret;
			}
		}

		bool IsRegisteredObject(WObject^ Object)
		{
			if(Object==nullptr) return false;
			else return Native->ValidObject(Object->Native);
		}


		//////////////////////////////////////////////////////////////////////////
		// Loading / saving
		//////////////////////////////////////////////////////////////////////////
		bool LoadFromFile(String^ Filename)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Filename);
			Ret = SUCCEEDED(Native->LoadFile(S));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer, bool Complete)
		{
			bool Ret = false;
			char* S = (char*)WUtils::GetString(Buffer);
			Ret = SUCCEEDED(Native->LoadBuffer((BYTE*)S, Complete));
			WUtils::FreeString(S);

			return Ret;
		}

		bool LoadFromBuffer(String^ Buffer)
		{
			return LoadFromBuffer(Buffer, true);
		}


		//////////////////////////////////////////////////////////////////////////
		// Miscelaneous
		//////////////////////////////////////////////////////////////////////////
		void GetVersion([Out]System::Byte% Major, [Out]System::Byte% Minor, [Out]System::Byte% Build)
		{
			Major = DCGF_VER_MAJOR;
			Minor = DCGF_VER_MINOR;
			Build = DCGF_VER_BUILD;
		}

		//////////////////////////////////////////////////////////////////////////
		// Mouse events routing
		//////////////////////////////////////////////////////////////////////////
		void OnMouseLeftDown()
		{
			Native->OnMouseLeftDown();
		}

		void OnMouseLeftUp()
		{
			Native->OnMouseLeftUp();
		}

		void OnMouseRightDown()
		{
			Native->OnMouseRightDown();
		}

		void OnMouseRightUp()
		{
			Native->OnMouseRightUp();
		}

		void OnMouseMiddleDown()
		{
			Native->OnMouseMiddleDown();
		}

		void OnMouseMiddleUp()
		{
			Native->OnMouseMiddleUp();
		}

	private:
		delegate void EngineLogDelegate(char* Text, HRESULT Result, void* Data);		
		GCHandle ThisGCHandle;
		GCHandle LogCallbackHandle;
		WEngineLogCallback^ EngineLogCallback;
		static void EngineLogStub(char* Text, HRESULT Result, void* Data);

		delegate void ScriptCompilerDelegate(int Line, char* Text , void* Data);
		delegate void ScriptElementDelegate(int Line, int Type, void* ElementData, void* Data);
		GCHandle CompilerCallbackHandle;
		GCHandle ElementCallbackHandle;
		WScriptCompilerCallback^ ScriptCompilerCallback;
		WScriptElementCallback^ ScriptElementCallback;
		static void ScriptCompilerStub(int Line, char* Text , void* Data);
		static void ScriptElementStub(int Line, int Type, void* ElementData, void* Data);

	protected:
		void InitializeInternal()
		{
			// map resources to the current assembly
			Assembly^ A = Assembly::GetExecutingAssembly();
			const char* S = WUtils::GetString(A->GetName()->Name + ".dll");

			HMODULE Mod = GetModuleHandle(S);

			Native->SetResourceModule(Mod);
			WUtils::FreeString(S);

			//ThisGCHandle = GCHandle::Alloc(this);
		}

	internal:
		property CBGame* Native
		{
			CBGame* get() { return (CBGame*)m_Native; };
		}
	};
}}}
