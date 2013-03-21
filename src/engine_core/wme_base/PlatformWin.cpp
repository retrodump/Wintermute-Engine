// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\PlatformWin.h"
#include <dbghelp.h>
#include "BGameExHelper.h"
#include "3DEffect.h"

CBGame* CBPlatform::Game = NULL;
SHGetKnownFolderIDList CBPlatform::s_SHGetKnownFolderList = NULL;
UINT CBPlatform::s_QueryCancelAutoPlay = 0;

int g_MinWidth = 0;
int g_MinHeight = 0;

//////////////////////////////////////////////////////////////////////////
LRESULT CBPlatform::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_ACTIVATE:
		{
			if(!Game) break;
			WORD state = LOWORD(wParam);
			Game->OnActivate(state != WA_INACTIVE, state==WA_CLICKACTIVE);
			return 0;
		}

		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}

		case WM_GETMINMAXINFO:
		{
			MINMAXINFO* pMinMax;
			int Width;
			int Height;

			if(!Game || !Game->m_Renderer || !Game->m_Renderer->m_Ready || !Game->m_Renderer->m_Active)
			{
				Width = g_MinWidth;
				Height = g_MinHeight;
			}
			else
			{
				Width = Game->m_Renderer->m_RealWidth;
				Height = Game->m_Renderer->m_RealHeight;
			}
			
			// Fix the size of the window according to client size
			pMinMax = (MINMAXINFO*)lParam;
			pMinMax->ptMinTrackSize.x = Width + ::GetSystemMetrics(SM_CXSIZEFRAME)*2;
			pMinMax->ptMinTrackSize.y = Height +  + ::GetSystemMetrics(SM_CYSIZEFRAME)*2  + ::GetSystemMetrics(SM_CYCAPTION);
			pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
			pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			if(Game) Game->OnMouseLeftDown();
			break;
		}

		case WM_LBUTTONUP:
		{
			if(Game) Game->OnMouseLeftUp();
			break;
		}

		case WM_LBUTTONDBLCLK:
		{
			if(Game) Game->OnMouseLeftDblClick();
			break;
		}

		case WM_RBUTTONDBLCLK:
		{
			if(Game) Game->OnMouseRightDblClick();
			break;
		}		

		case WM_RBUTTONDOWN:
		{
			if(Game) Game->OnMouseRightDown();
			break;
		}

		case WM_RBUTTONUP:
		{
			if(Game) Game->OnMouseRightUp();
			break;
		}

		case WM_MBUTTONDOWN:
		{
			if(Game) Game->OnMouseMiddleDown();
			break;
		}

		case WM_MBUTTONUP:
		{
			if(Game) Game->OnMouseMiddleUp();
			break;
		}

		case WM_SYSKEYDOWN:
		{
			if(wParam==VK_F10)
			{
				if(Game) Game->HandleKeypress(false, wParam, lParam);
				return 0;
			}
			break;
		}

		case WM_MOUSEWHEEL:
		{
			if(Game)
			{
				Game->HandleMouseWheel((int)wParam);
				return 0;
			}
		}

		case WM_KEYDOWN:
		{
			if(Game) Game->HandleKeypress(false, wParam, lParam);
			break;
		}

		case WM_CHAR:
		{
			if(Game && Game->IsVideoPlaying()) break;
			if(Game) Game->HandleKeypress(true, wParam, lParam);
			break;
		}

		case WM_MOVE:
		{
			// Retrieve the window position after a move
			if (Game && Game->m_Renderer->m_Active && Game->m_Renderer->m_Ready && Game->m_Renderer->m_Windowed)
			{
				::GetWindowRect(hWnd, &Game->m_Renderer->m_WindowRect);
				::GetClientRect(hWnd, &Game->m_Renderer->m_ViewportRect);
				::GetClientRect(hWnd, &Game->m_Renderer->m_ScreenRect);
				::ClientToScreen(hWnd, (POINT*)&Game->m_Renderer->m_ScreenRect.left);
				::ClientToScreen(hWnd, (POINT*)&Game->m_Renderer->m_ScreenRect.right);

				Game->m_Renderer->ClipCursor();
			}
			break;
		}

		case WM_PAINT:
		{
			if(Game) Game->OnPaint();
			break;
		}

		case WM_SIZE:
		{
			if(Game) Game->OnActivate(SIZE_MAXHIDE!=wParam && SIZE_MINIMIZED!=wParam, false);
			break;
		}

		case WM_SYSCOMMAND:
		{
			if(wParam == SC_KEYMENU) return 0;
			else if(wParam == SC_MONITORPOWER) return 0;
			else if(wParam == SC_SCREENSAVE) return 0;
			break;
		}		

		/*
		case WM_SETFOCUS:
		{
			::ShowCursor(FALSE);
			return 0;
		}

		case WM_KILLFOCUS:
		{
			::ShowCursor(TRUE);
			return 0;
		}
		*/

		case WM_SETCURSOR:
			if(Game && Game->m_Renderer && Game->m_Renderer->m_Active)
			{
				if(LOWORD(lParam)==HTCLIENT)
				{
					::SetCursor(NULL);
					return TRUE;
				}
			}
			break;

		case WM_ENTERMENULOOP:
			if(Game) Game->OnActivate(false, false);
			break;

		case WM_EXITMENULOOP:
			if(Game) Game->OnActivate(true, false);
			break;

		case WM_CLOSE:
		{
			if(Game && SUCCEEDED(Game->OnWindowClose())) return 0;
			break;
		}		

		case WM_WINSOCK:
		{
			if(Game && SUCCEEDED(Game->OnWinsock(wParam, lParam))) return 0;
			break;
		}
		default:
		{
			if (!s_QueryCancelAutoPlay)
			{ 
				s_QueryCancelAutoPlay = ::RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));
			} 
			if (Game && Game->m_AutorunDisabled && msg && msg == s_QueryCancelAutoPlay)
			{ 
				return TRUE;	// cancel auto-play
			}
		}
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}


#define CLASS_NAME "GF_FRAME"
//////////////////////////////////////////////////////////////////////////
bool CBPlatform::Initialize(CBGame* inGame, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	Game = inGame;
	if(!Game) return false;

	// make sure FPU exceptions are masked (can be messed up by some drivers)
	_controlfp(_CW_DEFAULT, 0xfffff);



	WNDCLASS		wc;	
	HWND			hWnd;
	HRESULT         ret;

#ifdef _DEBUG
	Game->m_Registry->SetBasePath("Software\\DEAD:CODE\\Wintermute\\Debug");
#else
	Game->m_Registry->SetBasePath("Software\\DEAD:CODE\\Wintermute\\Retail");
#endif

	bool DetectHW = false;
	bool SettingsOnly = false;
	bool SilentMode = false;
	int GameExplorerMode = 0; // 0..not, 1..add current user, 2..add all users, 3..remove


	// parse command line
	char* SaveGame = NULL;
	char param[MAX_PATH];
	for(int i = 0; i < CBUtils::GetArgCount(lpszCmdLine); i++)
	{
		CBUtils::GetArg(i, lpszCmdLine, param);

		if(stricmp(param, "-detect")==0) DetectHW = true;
		else if(stricmp(param, "-settingsonly")==0) SettingsOnly = true;
		else if(stricmp(param, "-project")==0)
		{
			CBUtils::GetArg(i+1, lpszCmdLine, param);
			if(strcmp(param, "")!=0)
			{
				char* IniDir = CBUtils::GetPath(param);
				char* IniName = CBUtils::GetFilename(param);

				// switch to ini's dir
				::SetCurrentDirectory(IniDir);

				// set ini name
				sprintf(param, ".\\%s", IniName);
				Game->m_Registry->SetIniName(param);

				delete [] IniDir;
				delete [] IniName;
			}
		}
		else if(stricmp(param, "-savegame")==0)
		{
			CBUtils::GetArg(i+1, lpszCmdLine, param);
			if(strcmp(param, "")!=0)
			{				
				char ModuleName[MAX_PATH];
				GetModuleFileName(NULL, ModuleName, MAX_PATH);

				// switch to exe's dir
				char* ExeDir = CBUtils::GetPath(ModuleName);
				::SetCurrentDirectory(ExeDir);

				delete [] ExeDir;

				CBUtils::SetString(&SaveGame, param);
			}
		}
		else if(stricmp(param, "-gameexplorer")==0)
		{
			CBUtils::GetArg(i+1, lpszCmdLine, param);
			if(strcmp(param, "")!=0)
			{
				if(stricmp(param, "add")==0) GameExplorerMode = 2;
				else if(stricmp(param, "add-current")==0) GameExplorerMode = 1;
				else if(stricmp(param, "remove")==0) GameExplorerMode = 3;

				if(GameExplorerMode > 0) SilentMode = true;
			}
		}
	}
	if(SettingsOnly) DetectHW = true;


	if(Game->m_Registry->ReadBool("Debug", "DebugMode")) Game->DEBUG_DebugEnable(".\\wme.log");
	Game->m_DEBUG_ShowFPS = Game->m_Registry->ReadBool("Debug", "ShowFPS");

	if(Game->m_Registry->ReadBool("Debug", "DisableSmartCache"))
	{
		Game->LOG(0, "Smart cache is DISABLED");
		Game->m_SmartCache = false;
	}

	bool AllowDirectDraw = Game->m_Registry->ReadBool("Debug", "AllowDirectDraw", false);

	// load general game settings
	Game->Initialize1();


	if(FAILED(Game->LoadSettings("startup.settings")))
	{
		Game->LOG(0, "Error loading game settings.");
		SAFE_DELETE(Game);

		if(!SilentMode)
		{
			WORD LangId = PRIMARYLANGID(::GetUserDefaultLangID());
			int StringID;
			switch(LangId)
			{
				case LANG_CZECH:   StringID = IDS_MISSING_FILES_CZE; break;
				case LANG_DANISH:  StringID = IDS_MISSING_FILES_DAN; break;
				case LANG_GERMAN:  StringID = IDS_MISSING_FILES_GER; break;
				case LANG_DUTCH:   StringID = IDS_MISSING_FILES_DUT; break;
				case LANG_FRENCH:  StringID = IDS_MISSING_FILES_FRE; break;
				case LANG_SPANISH: StringID = IDS_MISSING_FILES_SPA; break;
				case LANG_GREEK:   StringID = IDS_MISSING_FILES_GRE; break;
				default:           StringID = IDS_MISSING_FILES_ENG;
			}

			char str[MAX_PATH];
			::LoadString((HINSTANCE)::GetModuleHandle(NULL), StringID, str, MAX_PATH);
			::MessageBox(NULL, str, NULL, MB_OK|MB_ICONERROR);
		}

		return false;
	}

	// Game Explorer integration
	if(GameExplorerMode > 0)
	{
		if(CBPlatform::IsWindowsXPOrHigher())
		{
			char ModuleName[MAX_PATH];
			GetModuleFileName(NULL, ModuleName, MAX_PATH);

			// switch to exe's dir
			char* ExeDir = CBUtils::GetPath(ModuleName);
			::SetCurrentDirectory(ExeDir);
			delete [] ExeDir;

			HRESULT GeRes = E_FAIL;
			CBGameExHelper* GeHelper = new CBGameExHelper(Game);
			if(GeHelper)
			{
				switch(GameExplorerMode)
				{
					case 1:
						Game->LOG(0, "Registering game with Game Explorer for current user.");
						GeRes = GeHelper->RegisterGame(false);
						break;
					case 2:
						Game->LOG(0, "Registering game with Game Explorer for all users.");
						GeRes = GeHelper->RegisterGame(true);
						break;
					case 3:
						Game->LOG(0, "Unregistering game with Game Explorer.");
						GeRes = GeHelper->UnregisterGame();
						break;
				}
			}
			if(FAILED(GeRes)) Game->LOG(GeRes, "Error integrating game with Game Explorer");

			SAFE_DELETE(GeHelper);			
		}
		else Game->LOG(0, "Integrating game with Game Explorer is not supported on this Windows version");
		
		SAFE_DELETE(Game);
		return false;
	}

	// test if DirectX is initialized properly
	if(!Game->IsDirectXBound())
	{
		Game->LOG(0, "Error: DirectX binding failed");
		if(IDYES==::MessageBox(NULL, Game->m_StringTable->ExpandStatic("/SYSENG0026/This game requires Microsoft DirectX 8.0 or later which is not installed on your computer. Please install DirectX 8.0 or later and try again. Do you want to display the DirectX download page now?"), NULL, MB_ICONERROR|MB_YESNO))
		{
			CBPlatform::ShellExecute(NULL, "open", "http://www.microsoft.com/directx/", "", "", SW_MAXIMIZE);
		}

		SAFE_DELETE(Game);
		return false;
	}


	if (!hPrevInstance)
	{
		// Register the Window Class
		wc.lpszClassName = CLASS_NAME;
		wc.lpfnWndProc = CBPlatform::WindowProc;
		wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		wc.hInstance = hInstance;
		wc.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground=(HBRUSH)::GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		::RegisterClass(&wc);
	}

	// query hardware
	CHWManager* m = new CHWManager(Game);
	if(!m->QueryDevices(Game->m_SettingsAllowWindowed?VIDEO_ANY:VIDEO_FULLSCREEN,
		!AllowDirectDraw, // require D3D
		Game->m_SettingsResWidth,
		Game->m_SettingsResHeight,
		DetectHW,
		Game->m_SettingsAllowAdvanced,
		Game->m_SettingsRequireSound,
		Game->m_SettingsTLMode,
		Game->m_SettingsAllowDesktopRes))
	{
		Game->LOG(0, "Fatal: error querying available hardware. Exiting.");
		SAFE_DELETE(m);
		SAFE_DELETE(Game);
		return false;
	}

	// settings only - just quit
	if(SettingsOnly)
	{
		SAFE_DELETE(m);
		SAFE_DELETE(Game);
		return false;
	}

	Game->m_UseD3D = m->m_SelectedVideoDevice->m_Accelerated;

	Game->Initialize2();
	Game->m_Renderer->m_Instance = hInstance;


	// initialize debugger	
	if(Game->m_DEBUG_DebugMode)
	{
		int ConsoleMode = Game->m_Registry->ReadInt("Debug", "DebugConsole", 1);
		if(ConsoleMode==2 || (ConsoleMode==1 && m->m_SelectedWindowed))
			Game->GetDebugMgr()->Initialize();

		if(Game->m_Registry->ReadBool("Debug", "ScriptProfiling")) Game->m_ScEngine->EnableProfiling();
	}
	Game->GetDebugMgr()->OnGameInit();
	Game->m_ScEngine->LoadBreakpoints();



	// Calculate the proper size for the window given an appropriate client size
	int cx = m->m_ReqWidth	+ ::GetSystemMetrics(SM_CXSIZEFRAME)*2;
	int cy = m->m_ReqHeight + ::GetSystemMetrics(SM_CYSIZEFRAME)*2 + ::GetSystemMetrics(SM_CYCAPTION);

	DWORD ExFlags = 0;	
	if(!m->m_SelectedWindowed) ExFlags |= WS_EX_TOPMOST;

	DWORD Flags = m->m_SelectedWindowed?WS_OVERLAPPEDWINDOW:WS_POPUP;
	
	// this doesn't work for some reason
	//if(Game->m_TextRTL) ExFlags |= WS_EX_LAYOUTRTL;

	// restore previous window postition
	int PosX = Game->m_Registry->ReadInt("Video", "WindowPosX", CW_USEDEFAULT);
	int PosY = Game->m_Registry->ReadInt("Video", "WindowPosY", CW_USEDEFAULT);

	g_MinWidth = m->m_ReqWidth;
	g_MinHeight = m->m_ReqHeight;

	// Create and Show the Main Window
	hWnd = ::CreateWindowEx(ExFlags,
			CLASS_NAME,
			"Wintermute Engine",
			Flags,
			PosX,
			PosY,
			CW_USEDEFAULT, //cx,
			CW_USEDEFAULT, //cy,
			NULL,
			NULL,
			hInstance,
			NULL);


	if (hWnd == NULL)
	{
		Game->LOG(0, "Error creating main window. Exiting.");
		SAFE_DELETE(Game);
		return false;
	}



	// move the window to the correct monitor
	RECT WindowRect;
	::GetWindowRect(hWnd, &WindowRect);
	::OffsetRect(&WindowRect, m->m_SelectedVideoDevice->m_MonitorRect.left, m->m_SelectedVideoDevice->m_MonitorRect.top);

	// make sure the window is visible
	RECT Intersect;
	if(!::IntersectRect(&Intersect, &WindowRect, &m->m_SelectedVideoDevice->m_MonitorRect))
	{
		::OffsetRect(&WindowRect, -WindowRect.left, -WindowRect.top);
	}
	
	::SetWindowPos(hWnd, NULL, WindowRect.left, WindowRect.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW);

	// and display
	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);


	// loading...
	const char* ld_string = Game->m_StringTable->ExpandStatic("/SYSENG0027/Loading...");
	HFONT hfnt, hOldFont; 
	hfnt = (HFONT)::GetStockObject(ANSI_VAR_FONT);
	HDC hdc = ::GetWindowDC(hWnd);
	if (hOldFont = (HFONT)::SelectObject(hdc, hfnt))
	{
		SIZE size;		
		::SetBkColor(hdc, 0x000000);
		::SetTextColor(hdc, 0xFFFFFF);

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			::GetTextExtentPoint(hdc, ld_string, strlen(ld_string), &size);
			::TextOut(hdc, (cx-size.cx)/2, (cy-size.cy)/2, ld_string, strlen(ld_string)); 
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode((char*)ld_string);
			if(Temp)
			{
				::GetTextExtentPointW(hdc, Temp, wcslen(Temp), &size);
				::TextOutW(hdc, (cx-size.cx)/2, (cy-size.cy)/2, Temp, wcslen(Temp)); 
				delete [] Temp;
			}
		}

		::SelectObject(hdc, hOldFont); 
	}
	::ReleaseDC(hWnd, hdc);


	Game->m_Renderer->m_Window = Game->m_Renderer->m_ClipperWindow = hWnd;
	Game->m_Renderer->m_MonitorRect = m->m_SelectedVideoDevice->m_MonitorRect;

	// Save the window size/pos for switching modes
	::GetWindowRect(hWnd, &Game->m_Renderer->m_WindowRect);

	// initialize the renderer
	ret = Game->m_Renderer->InitRenderer(m);
	if (FAILED(ret))
	{
		Game->LOG(ret, "Error initializing renderer. Exiting.");
		SAFE_DELETE(m);
		SAFE_DELETE(Game);
		::DestroyWindow(hWnd);
		return false;
	}

	Game->Initialize3();	

	// initialize sound manager (non-fatal if we fail)
	ret = Game->m_SoundMgr->Initialize(hWnd, m);
	if(FAILED(ret))
	{
		Game->LOG(ret, "Sound is NOT available.");
	}


	// no need for device manager anymore
	SAFE_DELETE(m);

	// load game
	DWORD DataInitStart = ::timeGetTime();

	if(FAILED(Game->LoadFile(Game->m_SettingsGameFile?Game->m_SettingsGameFile:"default.game")))
	{
		Game->LOG(ret, "Error loading game file. Exiting.");
		SAFE_DELETE(Game);
		::DestroyWindow(hWnd);
		return false;
	}
	Game->SetWindowTitle();
	Game->m_Renderer->m_Ready = true;
	Game->m_MiniUpdateEnabled = true;

	Game->LOG(0, "Engine initialized in %d ms", ::timeGetTime() - DataInitStart);
	Game->LOG(0, "");


/*
	C3DEffect* E = new C3DEffect(Game);
	E->CreateFromFile("F:\\Program Files\\NVIDIA Corporation\\FX Composer 2\\MEDIA\\HLSL\\test.fx");
	delete E;
*/
	/*
	CBSocket* Sock = Game->m_NetworkMgr->Connect("dead-code.org", 80);
	Sleep(200);
	char txt[] = "GET http://dead-code.org/todo.txt HTTP/1.0\n\n";
	Sock->Send((BYTE*)txt, strlen(txt));
	*/
	
	//CBSocket* Sock2 = Game->m_NetworkMgr->CreateServer(333);
	//Sock2->Listen();

	
	if(SaveGame)
	{
		Game->LoadGame(SaveGame);
		delete [] SaveGame;
	}

	// all set, ready to go
	return true;
}


//////////////////////////////////////////////////////////////////////////
int CBPlatform::MessageLoop()
{
	MSG 			msg;
	HRESULT 		ret;

#ifndef _DEBUG
	__try
	{
#endif
		while (TRUE)
		{
			if (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if(!::GetMessage(&msg, NULL, 0, 0 )) break;
				::TranslateMessage(&msg); 
				::DispatchMessage(&msg);
			}
			else if(Game && Game->m_Renderer->m_Active && Game->m_Renderer->m_Ready)
			{

				Game->DisplayContent();
				Game->DisplayQuickMsg();

				Game->DisplayDebugInfo();

				// ***** flip
				if(!Game->m_SuspendedRendering)
					ret = Game->m_Renderer->Flip();
				else
					ret = S_OK;

				if(Game->m_Quitting) break;
				if(Game->m_Loading) Game->LoadGame(Game->m_ScheduledLoadSlot);
			}
			else 
			{
				// Make sure we go to sleep if we have nothing else to do
				::WaitMessage();
			}
		}

		if(Game)
		{
			// remember previous window position
			if(Game->m_Renderer && Game->m_Renderer->m_Windowed)
			{
				if(!::IsIconic(Game->m_Renderer->m_Window))
				{
					int PosX = Game->m_Renderer->m_WindowRect.left;
					int PosY = Game->m_Renderer->m_WindowRect.top;
					PosX -= Game->m_Renderer->m_MonitorRect.left;
					PosY -= Game->m_Renderer->m_MonitorRect.top;

					Game->m_Registry->WriteInt("Video", "WindowPosX", PosX);
					Game->m_Registry->WriteInt("Video", "WindowPosY", PosY);
				}
			}

			SAFE_DELETE(Game);
		}

		return msg.wParam;
#ifndef _DEBUG
	}
	__except(CBPlatform::HandleCrashReport(Game, GetExceptionInformation()))
	{
		return FALSE;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// high-level functions
//////////////////////////////////////////////////////////////////////////
bool CBPlatform::GetPrivateSaveDirectory(char* Buffer)
{	
	LPITEMIDLIST pidl = NULL;

	LPMALLOC pMalloc;
	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		// Vista - get SHGetKnownFolderIDList
		if(IsWindowsVistaOrHigher())
		{
			if(s_SHGetKnownFolderList==NULL)
			{
				HMODULE hDll = LoadLibrary("shell32.dll");
				if(hDll)
				{
					s_SHGetKnownFolderList = (SHGetKnownFolderIDList)GetProcAddress(hDll, "SHGetKnownFolderIDList");
				}
			}
			if(s_SHGetKnownFolderList)
			{
				GUID FOLDERID_SavedGames = { 0x4c5c32ff, 0xbb9d, 0x43b0, { 0xb5, 0xb4, 0x2d, 0x72, 0xe5, 0x4e, 0xaa, 0xa4 } };
				s_SHGetKnownFolderList(&FOLDERID_SavedGames, 0, NULL, &pidl);
			}
		}
		// otherwise get Docs&Settings folder
		if(!pidl) SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
		
		if(pidl)
		{
			SHGetPathFromIDList(pidl, Buffer);
			pMalloc->Free(pidl);
			return true;
		}
		else pMalloc->Free(pidl);
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CBPlatform::GetSafeLogFilename(char* Buffer)
{
	Buffer[0] = '\0';
	LPITEMIDLIST pidl = NULL;
	LPMALLOC pMalloc;
	if(SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl);
		if(pidl)
		{
			SHGetPathFromIDList(pidl, Buffer);
		}
		pMalloc->Free(pidl);
	}
	char ModuleName[MAX_PATH];
	char Fname[MAX_PATH];
	::GetModuleFileName(NULL, ModuleName, MAX_PATH);
	_splitpath(ModuleName, NULL, NULL, Fname, NULL);

	strcat(Buffer, "\\Wintermute Engine\\Logs\\");
	strcat(Buffer, Fname);
	strcat(Buffer, ".log");

	CBUtils::CreatePath(Buffer);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// debugging
//////////////////////////////////////////////////////////////////////////
typedef void (*DllCreateReport)(const char* AppName, int VerMajor, int VerMinor, int VerBuild, EXCEPTION_POINTERS* pExPtrs, bool SendReport);
LONG CBPlatform::HandleCrashReport(CBGame* Game, EXCEPTION_POINTERS* pExPtrs)
{	
	LONG Ret = EXCEPTION_CONTINUE_SEARCH;
	if(IsDebuggerPresent()) return Ret;

	HMODULE  hMod = LoadLibrary("wme_report.dll");
	if(hMod)
	{
		DllCreateReport CreateReport = (DllCreateReport)GetProcAddress(hMod, "CreateReport");
		if(CreateReport)
		{
			if(Game && Game->m_Renderer && Game->m_Renderer->m_Window)
			{
				ShowWindow(Game->m_Renderer->m_Window, SW_HIDE);
			}
			CreateReport("wme", DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD, pExPtrs, true);
			Ret = EXCEPTION_EXECUTE_HANDLER;
		}		
		FreeLibrary(hMod);
	}
	return Ret;
}


//////////////////////////////////////////////////////////////////////////
// Win32 API bindings
//////////////////////////////////////////////////////////////////////////
HINSTANCE CBPlatform::ShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd)
{
	return ::ShellExecute(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBPlatform::CoInitialize(LPVOID pvReserved)
{
	return ::CoInitialize(pvReserved);
}

//////////////////////////////////////////////////////////////////////////
void CBPlatform::CoUninitialize(void)
{
	::CoUninitialize();
}

//////////////////////////////////////////////////////////////////////////
void CBPlatform::OutputDebugString(LPCSTR lpOutputString)
{
	::OutputDebugString(lpOutputString);
}

//////////////////////////////////////////////////////////////////////////
void CBPlatform::GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
	::GetLocalTime(lpSystemTime);
}

//////////////////////////////////////////////////////////////////////////
DWORD CBPlatform::timeGetTime(VOID)
{
	return ::timeGetTime();
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::GetCursorPos(LPPOINT lpPoint)
{
	return ::GetCursorPos(lpPoint);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::SetCursorPos(int X, int Y)
{
	return ::SetCursorPos(X, Y);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::ScreenToClient(HWND hWnd, LPPOINT lpPoint)
{
	return ::ScreenToClient(hWnd, lpPoint);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::ClientToScreen(HWND hWnd, LPPOINT lpPoint)
{
	return ::ClientToScreen(hWnd, lpPoint);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::ShowWindow(HWND hWnd, int nCmdShow)
{
	return ::ShowWindow(hWnd, nCmdShow);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::DeleteFile(LPCSTR lpFileName)
{
	return ::DeleteFile(lpFileName);
}

//////////////////////////////////////////////////////////////////////////
DWORD CBPlatform::GetModuleFileName(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
	return ::GetModuleFileName(hModule, lpFilename, nSize);
}

//////////////////////////////////////////////////////////////////////////
HMODULE CBPlatform::LoadLibrary(LPCSTR lpFileName)
{
	return ::LoadLibrary(lpFileName);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::FreeLibrary(HMODULE hModule)
{
	return ::FreeLibrary(hModule);
}

//////////////////////////////////////////////////////////////////////////
FARPROC CBPlatform::GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	return ::GetProcAddress(hModule, lpProcName);
}

//////////////////////////////////////////////////////////////////////////
HWND CBPlatform::SetCapture(HWND hWnd)
{
	return ::SetCapture(hWnd);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::ReleaseCapture(VOID)
{
	return ::ReleaseCapture();
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::SetRectEmpty(LPRECT lprc)
{
	return ::SetRectEmpty(lprc);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::IsRectEmpty(LPRECT lprc)
{
	return ::IsRectEmpty(lprc);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::PtInRect(LPRECT lprc, POINT p)
{
	return ::PtInRect(lprc, p);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::SetRect(LPRECT lprc, int left, int top, int right, int bottom)
{
	return ::SetRect(lprc, left, top, right, bottom);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::IntersectRect(LPRECT lprcDst, CONST RECT* lprcSrc1, CONST RECT* lprcSrc2)
{
	return ::IntersectRect(lprcDst, lprcSrc1, lprcSrc2);
}

//////////////////////////////////////////////////////////////////////////
BOOL CBPlatform::SetForegroundWindow(HWND hWnd)
{
	return ::SetForegroundWindow(hWnd);
}

//////////////////////////////////////////////////////////////////////////
bool CBPlatform::IsWindowsVistaOrHigher()
{
	OSVERSIONINFO os;
	memset(&os, 0, sizeof(OSVERSIONINFO));
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&os);

	if(os.dwMajorVersion > 5) return true;
	else return false;
}


//////////////////////////////////////////////////////////////////////////
bool CBPlatform::IsWindowsXPOrHigher()
{
	OSVERSIONINFO os;
	memset(&os, 0, sizeof(OSVERSIONINFO));
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&os);

	if(os.dwMajorVersion >= 5) return true;
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool CBPlatform::AddFontResource(LPCSTR lpszFilename)
{
	int Ret;

	HMODULE hDLL = LoadLibrary("gdi32.dll");
	FONTAPI pAddFontResourceEx = (FONTAPI)GetProcAddress(hDLL, "AddFontResourceExA");
	if(pAddFontResourceEx)
	{
		Ret = pAddFontResourceEx(lpszFilename, FR_PRIVATE, 0);
		FreeLibrary(hDLL);
	}
	else Ret = ::AddFontResource(lpszFilename);

	return Ret != 0;
}

//////////////////////////////////////////////////////////////////////////
bool CBPlatform::RemoveFontResource(LPCSTR lpszFilename)
{
	int Ret;

	HMODULE hDLL = LoadLibrary("gdi32.dll");
	FONTAPI pRemoveFontResourceEx = (FONTAPI)GetProcAddress(hDLL, "RemoveFontResourceExA");
	if(pRemoveFontResourceEx)
	{
		Ret = pRemoveFontResourceEx(lpszFilename, FR_PRIVATE, 0);
		FreeLibrary(hDLL);
	}
	else Ret = ::RemoveFontResourceA(lpszFilename);

	return Ret != 0;
}
