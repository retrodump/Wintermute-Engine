// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#define WIN32_LEAN_AND_MEAN

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include <ShellAPI.h>
#include <shlobj.h>
#include <mmsystem.h>
#include <vfw.h>
#include <winsock2.h>
#include <dbghelp.h>

class CBGame;

typedef GUID *REFKNOWNFOLDERID;
typedef ITEMIDLIST *PIDLIST_ABSOLUTE;
typedef HRESULT (__stdcall *SHGetKnownFolderIDList)(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PIDLIST_ABSOLUTE *ppidl);
typedef int (WINAPI *FONTAPI)(LPCSTR, DWORD, PVOID);
#define FR_PRIVATE     0x10

//////////////////////////////////////////////////////////////////////////
class CBPlatform
{
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static bool Initialize(CBGame* inGame, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow);
	static int MessageLoop();


	// high level functions
	static bool GetPrivateSaveDirectory(char* Buffer);
	static bool GetSafeLogFilename(char* Buffer);


	// debugging
	static LONG HandleCrashReport(CBGame* Game, EXCEPTION_POINTERS* pExPtrs);

	// Win32 API bindings
	static HINSTANCE ShellExecute(HWND hwnd, LPCSTR lpOperation, LPCSTR lpFile, LPCSTR lpParameters, LPCSTR lpDirectory, INT nShowCmd);
	static HRESULT CoInitialize(LPVOID pvReserved);
	static void CoUninitialize(void);
	static void OutputDebugString(LPCSTR lpOutputString);
	static void GetLocalTime(LPSYSTEMTIME lpSystemTime);
	static DWORD timeGetTime(VOID);
	static BOOL GetCursorPos(LPPOINT lpPoint);
	static BOOL SetCursorPos(int X, int Y);
	static BOOL ScreenToClient(HWND hWnd, LPPOINT lpPoint);
	static BOOL ClientToScreen(HWND hWnd, LPPOINT lpPoint);
	static BOOL ShowWindow(HWND hWnd, int nCmdShow);
	static BOOL DeleteFile(LPCSTR lpFileName);
	static DWORD GetModuleFileName(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
	static HMODULE LoadLibrary(LPCSTR lpFileName);
	static BOOL FreeLibrary(HMODULE hModule);
	static FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static HWND SetCapture(HWND hWnd);
	static BOOL ReleaseCapture(VOID);
	static BOOL SetRectEmpty(LPRECT lprc);
	static BOOL IsRectEmpty(LPRECT lprc);
	static BOOL PtInRect(LPRECT lprc, POINT p);
	static BOOL SetRect(LPRECT lprc, int left, int top, int right, int bottom);
	static BOOL IntersectRect(LPRECT lprcDst, CONST RECT* lprcSrc1, CONST RECT* lprcSrc2);
	static BOOL SetForegroundWindow(HWND hWnd);

	static bool IsWindowsVistaOrHigher();
	static bool IsWindowsXPOrHigher();

	static bool AddFontResource(LPCSTR lpszFilename);
	static bool RemoveFontResource(LPCSTR lpszFilename);
	
private:
	static CBGame* Game;
	static SHGetKnownFolderIDList s_SHGetKnownFolderList;
	static UINT s_QueryCancelAutoPlay;
};
