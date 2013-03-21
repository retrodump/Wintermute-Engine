// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include <rpcsal.h>
#include <gameux.h>

typedef HRESULT (__stdcall *DllSHGetFolderPathW)(HWND hwndOwner, int nFolder,	HANDLE hToken, DWORD dwFlags, LPWSTR pszPath);

class CBGameExHelper : public CBBase
{
public:
	CBGameExHelper(CBGame* inGame);
	virtual ~CBGameExHelper(void);

	HRESULT RegisterGame(bool AllUsers);
	HRESULT UnregisterGame();

private:
	DllSHGetFolderPathW m_SHGetFolderPathW;

	// DX SDK stuff
	HRESULT CreateShortcut( WCHAR* strLaunchPath, WCHAR* strCommandLineArgs, WCHAR* strShortcutFilePath);
	HRESULT CreateShortcutToURL(WCHAR* pszURL, WCHAR* pszLinkFile);
	HRESULT CreateTaskA(GAME_INSTALL_SCOPE InstallScope, GUID* pGameInstanceGUID, BOOL bSupportTask, int nTaskID, CHAR* strTaskName, CHAR* strLaunchPath, CHAR* strCommandLineArgs);
	HRESULT CreateTaskW(GAME_INSTALL_SCOPE InstallScope, GUID* pGameInstanceGUID, BOOL bSupportTask, int nTaskID, WCHAR* strTaskName, WCHAR* strLaunchPath, WCHAR* strCommandLineArgs);
	HRESULT RemoveTasks(GUID* pGUID);
	HRESULT AddToGameExplorerA(CHAR* strGDFBinPath, CHAR* strGameInstallPath, GAME_INSTALL_SCOPE InstallScope, GUID* pInstanceGUID);
	HRESULT AddToGameExplorerW(WCHAR* strGDFBinPath, WCHAR* strGameInstallPath, GAME_INSTALL_SCOPE InstallScope, GUID* pInstanceGUID);
	HRESULT RemoveFromGameExplorer(GUID *pInstanceGUID);
	HRESULT SetupRichSavedGamesW(WCHAR* strSavedGameExtension, WCHAR* strLaunchPath, WCHAR* strCommandLineToLaunchSavedGame);
	HRESULT RemoveRichSavedGamesW(WCHAR* strSavedGameExtension);
	HRESULT SetupRichSavedGamesA(CHAR* strSavedGameExtension, CHAR* strLaunchPath, CHAR* strCommandLineToLaunchSavedGame);
	HRESULT RemoveRichSavedGamesA(CHAR* strSavedGameExtension);	
};
