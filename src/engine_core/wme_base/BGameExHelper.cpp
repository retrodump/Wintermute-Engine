// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BGameExHelper.h"
#include <shlwapi.h>
#include <shlobj.h>
#include <intshcut.h>

#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
CBGameExHelper::CBGameExHelper(CBGame* inGame):CBBase(inGame)
{
	m_SHGetFolderPathW = NULL;

	HMODULE hDll = LoadLibrary("shell32.dll");
	if(hDll) m_SHGetFolderPathW = (DllSHGetFolderPathW)GetProcAddress(hDll, "SHGetFolderPathW");
}

//////////////////////////////////////////////////////////////////////////
CBGameExHelper::~CBGameExHelper(void)
{
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::RegisterGame(bool AllUsers)
{
	char ExeFilename[MAX_PATH];
	::GetModuleFileName(NULL, ExeFilename, MAX_PATH);

	char* ExePath =	CBUtils::GetPath(ExeFilename);

	GAME_INSTALL_SCOPE Scope = AllUsers?GIS_ALL_USERS:GIS_CURRENT_USER;

	GUID Guid = Game->m_GameGUID;
	if(Guid == GUID_NULL) return E_FAIL;

	HRESULT Res = AddToGameExplorerA(ExeFilename, ExePath, Scope, &Guid);


	wchar_t ExeFilenameW[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, ExeFilename, MAX_PATH, ExeFilenameW, MAX_PATH);

	wchar_t* PlayString = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0060/Play"));

	wchar_t* SettingsString = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0061/Settings"));

	wchar_t* SupportLabel1 = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0062/Website 1"));

	wchar_t* SupportUrl1 = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0063/http://dead-code.org"));

	wchar_t* SupportLabel2 = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0064/Website 2"));

	wchar_t* SupportUrl2 = CBTextUtils::NativeToUnicode(Game->m_TextEncoding,
		Game->m_StringTable->ExpandStatic("/SYSENG0065/http://dead-code.org"));

	CreateTaskW(Scope, &Game->m_GameGUID, false, 0, PlayString, ExeFilenameW, NULL);
	CreateTaskW(Scope, &Game->m_GameGUID, false, 1, SettingsString, ExeFilenameW, L"-detect -settingsonly");
	
	if(wcslen(SupportLabel1) > 0)
		CreateTaskW(Scope, &Game->m_GameGUID, true, 0, SupportLabel1, SupportUrl1, L"");
	if(wcslen(SupportLabel2) > 0)
		CreateTaskW(Scope, &Game->m_GameGUID, true, 1, SupportLabel2, SupportUrl2, L"");


	if(Game->m_RichSavedGames)
	{
		char QuotedExeFilename[MAX_PATH];
		sprintf(QuotedExeFilename, "\"%s\"", ExeFilename);

		SetupRichSavedGamesA(Game->m_SavedGameExt, QuotedExeFilename, "-savegame \"%1\"");
	}
	
	delete [] ExePath;
	delete [] PlayString;
	delete [] SettingsString;
	delete [] SupportLabel1;
	delete [] SupportUrl1;
	delete [] SupportLabel2;
	delete [] SupportUrl2;

	return Res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::UnregisterGame()
{
	GUID Guid = Game->m_GameGUID;
	if(Guid == GUID_NULL) return E_FAIL;

	RemoveFromGameExplorer(&Guid);
	RemoveTasks(&Guid);
	RemoveRichSavedGamesA(Game->m_SavedGameExt);
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::AddToGameExplorerA( CHAR* strGDFBinPath, CHAR* strGameInstallPath, 
						  GAME_INSTALL_SCOPE InstallScope, GUID* pInstanceGUID )
{
	WCHAR wstrBinPath[MAX_PATH] = {0};
	WCHAR wstrInstallPath[MAX_PATH] = {0};

	MultiByteToWideChar(CP_ACP, 0, strGDFBinPath, MAX_PATH, wstrBinPath, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, strGameInstallPath, MAX_PATH, wstrInstallPath, MAX_PATH);

	return AddToGameExplorerW( wstrBinPath, wstrInstallPath, InstallScope, pInstanceGUID );
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::AddToGameExplorerW( WCHAR* strGDFBinPath, WCHAR* strGameInstallPath, 
						  GAME_INSTALL_SCOPE InstallScope, GUID* pInstanceGUID )
{
	HRESULT hr = E_FAIL;
	bool    bCleanupCOM = false;
	BOOL    bHasAccess = FALSE;
	BSTR    bstrGDFBinPath = NULL;
	BSTR    bstrGameInstallPath = NULL;
	IGameExplorer* pFwGameExplorer = NULL;

	if( strGDFBinPath == NULL || strGameInstallPath == NULL || pInstanceGUID == NULL )
	{
		assert( false );
		return E_INVALIDARG;
	}

	bstrGDFBinPath = SysAllocString( strGDFBinPath );
	bstrGameInstallPath = SysAllocString( strGameInstallPath );
	if( bstrGDFBinPath == NULL || bstrGameInstallPath == NULL )
	{
		hr = E_OUTOFMEMORY;
		goto LCleanup;
	}

	hr = CoInitialize( 0 );
	bCleanupCOM = SUCCEEDED(hr); 

	// Create an instance of the Game Explorer Interface
	hr = CoCreateInstance( __uuidof(GameExplorer), NULL, CLSCTX_INPROC_SERVER, 
		__uuidof(IGameExplorer), (void**) &pFwGameExplorer );
	if( FAILED(hr) || pFwGameExplorer == NULL )
	{
		// On Windows XP or eariler, write registry keys to known location 
		// so that if the machine is upgraded to Windows Vista or later, these games will 
		// be automatically found.
		// 
		// Depending on GAME_INSTALL_SCOPE, write to:
		//      HKLM\Software\Microsoft\Windows\CurrentVersion\GameUX\GamesToFindOnWindowsUpgrade\{GUID}\
		// or
		//      HKCU\Software\Classes\Software\Microsoft\Windows\CurrentVersion\GameUX\GamesToFindOnWindowsUpgrade\{GUID}\
		// and write there these 2 string values: GDFBinaryPath and GameInstallPath 
		//
		HKEY hKeyGamesToFind = NULL, hKeyGame = NULL;
		LONG lResult;
		DWORD dwDisposition;
		if( InstallScope == GIS_CURRENT_USER )
			lResult = RegCreateKeyExW( HKEY_CURRENT_USER, L"Software\\Classes\\Software\\Microsoft\\Windows\\CurrentVersion\\GameUX\\GamesToFindOnWindowsUpgrade", 
			0, NULL, 0, KEY_WRITE, NULL, &hKeyGamesToFind, &dwDisposition );
		else
			lResult = RegCreateKeyExW( HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\GameUX\\GamesToFindOnWindowsUpgrade", 
			0, NULL, 0, KEY_WRITE, NULL, &hKeyGamesToFind, &dwDisposition );


		if( ERROR_SUCCESS == lResult ) 
		{
			WCHAR strGameInstanceGUID[128] = {0};
			hr = StringFromGUID2( *pInstanceGUID, strGameInstanceGUID, 128 );

			if( SUCCEEDED(hr) )
			{
				lResult = RegCreateKeyExW( hKeyGamesToFind, strGameInstanceGUID, 0, NULL, 0, KEY_WRITE, NULL, &hKeyGame, &dwDisposition );
				if( ERROR_SUCCESS == lResult ) 
				{
					size_t nGDFBinPath = 0, nGameInstallPath = 0;
					nGDFBinPath = wcslen(strGDFBinPath);
					nGameInstallPath = wcslen(strGameInstallPath);
					RegSetValueExW( hKeyGame, L"GDFBinaryPath", 0, REG_SZ, (BYTE*)strGDFBinPath, (DWORD)((nGDFBinPath + 1)*sizeof(WCHAR)) );
					RegSetValueExW( hKeyGame, L"GameInstallPath", 0, REG_SZ, (BYTE*)strGameInstallPath, (DWORD)((nGameInstallPath + 1)*sizeof(WCHAR)) );
				}
				if( hKeyGame ) RegCloseKey( hKeyGame );
			}
		}
		if( hKeyGamesToFind ) RegCloseKey( hKeyGamesToFind );
	}
	else
	{
		hr = pFwGameExplorer->VerifyAccess( bstrGDFBinPath, &bHasAccess );
		if( FAILED(hr) || !bHasAccess )
			goto LCleanup;

		hr = pFwGameExplorer->AddGame( bstrGDFBinPath, bstrGameInstallPath, 
			InstallScope, pInstanceGUID );
		/*
		if( FAILED(hr) )
		{
			// If AddGame() failed, then the game may have been already added.
			// So try to retrieve existing game instance GUID
			RetrieveGUIDForApplicationW( bstrGDFBinPath, pInstanceGUID );
		}
		*/
	}

LCleanup:
	if( bstrGDFBinPath ) SysFreeString( bstrGDFBinPath );
	if( bstrGameInstallPath ) SysFreeString( bstrGameInstallPath );
	if( pFwGameExplorer ) pFwGameExplorer->Release();
	if( bCleanupCOM ) CoUninitialize();

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::RemoveFromGameExplorer( GUID *pInstanceGUID )
{   
	HRESULT hr = E_FAIL;
	IGameExplorer* pFwGameExplorer = NULL;

	hr = CoInitialize( 0 );
	bool bCleanupCOM = SUCCEEDED(hr); 
	bool bVistaPath = false;

	// Create an instance of the Game Explorer Interface
	hr = CoCreateInstance( __uuidof(GameExplorer), NULL, CLSCTX_INPROC_SERVER, 
					  __uuidof(IGameExplorer), (void**) &pFwGameExplorer );
	if( SUCCEEDED(hr) ) 
	{
		bVistaPath = true;
		// Remove the game from the Game Explorer
		hr = pFwGameExplorer->RemoveGame( *pInstanceGUID );
	}
	else 
	{
		// On Windows XP remove reg keys
		if( pInstanceGUID == NULL )
			goto LCleanup;

		WCHAR strGameInstanceGUID[128] = {0};
		hr = StringFromGUID2( *pInstanceGUID, strGameInstanceGUID, 128 );
		if( FAILED(hr) )
			goto LCleanup;

		WCHAR szKeyPath[1024];
		swprintf(szKeyPath, L"Software\\Classes\\Software\\Microsoft\\Windows\\CurrentVersion\\GameUX\\GamesToFindOnWindowsUpgrade\\%s", strGameInstanceGUID );
		SHDeleteKeyW( HKEY_CURRENT_USER, szKeyPath );

		swprintf(szKeyPath, L"Software\\Microsoft\\Windows\\CurrentVersion\\GameUX\\GamesToFindOnWindowsUpgrade\\%s", strGameInstanceGUID );
		SHDeleteKeyW( HKEY_LOCAL_MACHINE, szKeyPath );

		hr = S_OK;
		goto LCleanup;
	}

LCleanup:
	if( pFwGameExplorer ) pFwGameExplorer->Release();
	if( bCleanupCOM ) CoUninitialize();

	return hr;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::CreateTaskA(GAME_INSTALL_SCOPE InstallScope, GUID* pGameInstanceGUID, BOOL bSupportTask,
				   int nTaskID, CHAR* strTaskName, CHAR* strLaunchPath, CHAR* strCommandLineArgs )         
{
	WCHAR wstrTaskName[MAX_PATH] = {0};
	WCHAR wstrLaunchPath[MAX_PATH] = {0};
	WCHAR wstrCommandLineArgs[MAX_PATH] = {0};

	MultiByteToWideChar(CP_ACP, 0, strTaskName, MAX_PATH, wstrTaskName, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, strLaunchPath, MAX_PATH, wstrLaunchPath, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, strCommandLineArgs, MAX_PATH, wstrCommandLineArgs, MAX_PATH);

	return CreateTaskW( InstallScope, pGameInstanceGUID, bSupportTask, nTaskID, 
		wstrTaskName, wstrLaunchPath, wstrCommandLineArgs );
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::CreateTaskW( GAME_INSTALL_SCOPE InstallScope,   // Either GIS_CURRENT_USER or GIS_ALL_USERS 
				   GUID* pGameInstanceGUID,           // valid GameInstance GUID that was passed to AddGame()
				   BOOL bSupportTask,                 // if TRUE, this is a support task otherwise it is a play task
				   int nTaskID,                       // ID of task
				   WCHAR* strTaskName,                // Name of task.  Ex "Play"
				   WCHAR* strLaunchPath,              // Path to exe.  Example: "C:\Program Files\Microsoft\MyGame.exe"
				   WCHAR* strCommandLineArgs )        // Can be NULL.  Example: "-windowed"
{
	HRESULT hr;
	WCHAR strPath[512];
	WCHAR strGUID[256];
	WCHAR strCommonFolder[MAX_PATH];
	WCHAR strShortcutFilePath[512];

	if(!m_SHGetFolderPathW) return E_FAIL;

	// Get base path based on install scope
	if( InstallScope == GIS_CURRENT_USER )
		m_SHGetFolderPathW( NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, strCommonFolder );
	else
		m_SHGetFolderPathW( NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, strCommonFolder );

	// Convert GUID to string
	hr = StringFromGUID2( *pGameInstanceGUID, strGUID, 256 );
	if( FAILED(hr) )
		return hr;

	// Create dir path for shortcut	
	swprintf( strPath, L"%s\\Microsoft\\Windows\\GameExplorer\\%s\\%s\\%d", 
		strCommonFolder, strGUID, (bSupportTask) ? L"SupportTasks" : L"PlayTasks", nTaskID );

	// Create the directory and all intermediate directories
	char strPathA[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, strPath, MAX_PATH, strPathA, MAX_PATH, NULL, NULL);
	strcat(strPathA, "\\");
	CBUtils::CreatePath(strPathA, true);


	// Create shortcut
	swprintf( strShortcutFilePath, L"%s\\%s.lnk", strPath, strTaskName );
	CreateShortcut( strLaunchPath, strCommandLineArgs, strShortcutFilePath );


	//swprintf( strShortcutFilePath, L"%s\\%s.url", strPath, strTaskName );
	//CreateShortcutToURL(strLaunchPath, strShortcutFilePath);


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::RemoveTasks( GUID* pGUID ) // valid GameInstance GUID that was passed to AddGame()
{
	HRESULT hr;
	WCHAR strPath[512] = {0};
	WCHAR strGUID[256];
	WCHAR strLocalAppData[MAX_PATH];
	WCHAR strCommonAppData[MAX_PATH];

	if(!m_SHGetFolderPathW) return E_FAIL;

	// Get base path based on install scope
	if( FAILED( hr = m_SHGetFolderPathW( NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, strLocalAppData ) ) )
		return hr;

	if( FAILED( hr = m_SHGetFolderPathW( NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, strCommonAppData ) ) )
		return hr;

	// Convert GUID to string
	if( FAILED( hr = StringFromGUID2( *pGUID, strGUID, 256 ) ) )
		return hr;

	swprintf(strPath, L"%s\\Microsoft\\Windows\\GameExplorer\\%s", strLocalAppData, strGUID );

	SHFILEOPSTRUCTW fileOp;
	ZeroMemory( &fileOp, sizeof(SHFILEOPSTRUCTW) );
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = strPath;
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	SHFileOperationW( &fileOp );

	swprintf(strPath, L"%s\\Microsoft\\Windows\\GameExplorer\\%s", strCommonAppData, strGUID );

	ZeroMemory( &fileOp, sizeof(SHFILEOPSTRUCTW) );
	fileOp.wFunc = FO_DELETE;
	fileOp.pFrom = strPath;
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
	SHFileOperationW( &fileOp );

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::CreateShortcut( WCHAR* strLaunchPath, WCHAR* strCommandLineArgs, WCHAR* strShortcutFilePath )
{
	HRESULT hr;

	hr = CoInitialize( 0 );
	bool bCleanupCOM = SUCCEEDED(hr); 

	IShellLinkW* psl; 
	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellLinkW, (LPVOID*)&psl); 
	if (SUCCEEDED(hr)) 
	{ 
		// Setup shortcut
		psl->SetPath( strLaunchPath ); 
		if( strCommandLineArgs ) psl->SetArguments( strCommandLineArgs ); 

		// These shortcut settings aren't needed for tasks
		// if( strIconPath ) psl->SetIconLocation( strIconPath, nIcon );
		// if( wHotkey ) psl->SetHotkey( wHotkey );
		// if( nShowCmd ) psl->SetShowCmd( nShowCmd );
		// if( strDescription ) psl->SetDescription( strDescription );

		// Set working dir to path of launch exe
		WCHAR strFullPath[512];
		WCHAR* strExePart; 
		GetFullPathNameW( strLaunchPath, 512, strFullPath, &strExePart );
		if( strExePart ) *strExePart = 0;
		psl->SetWorkingDirectory( strFullPath );

		// Save shortcut to file
		IPersistFile* ppf; 
		hr = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
		if (SUCCEEDED(hr)) 
		{ 
			hr = ppf->Save( strShortcutFilePath, TRUE ); 
			ppf->Release(); 
		} 
		psl->Release(); 
	} 

	if( bCleanupCOM ) CoUninitialize();

	return hr;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::CreateShortcutToURL(WCHAR* pszURL, WCHAR* pszLinkFile)
{
	HRESULT hr;

	hr = CoInitialize( 0 );
	bool bCleanupCOM = SUCCEEDED(hr); 


	IUniformResourceLocatorW *pURL = NULL;

	// Create an IUniformResourceLocator object
	hr = CoCreateInstance (CLSID_InternetShortcut, NULL, 
		CLSCTX_INPROC_SERVER, IID_IUniformResourceLocatorW, (LPVOID*) &pURL);
	if (SUCCEEDED(hr))
	{
		IPersistFile *pPF = NULL;

		hr = pURL->SetURL(pszURL, 0);

		if (SUCCEEDED(hr))
		{
			hr = pURL->QueryInterface (IID_IPersistFile, (void **)&pPF);
			if (SUCCEEDED(hr))
			{   
				// Save the shortcut via the IPersistFile::Save member function.
				hr = pPF->Save (pszLinkFile, TRUE);

				// Release the pointer to IPersistFile.
				pPF->Release ();
			}
		}
		// Release the pointer to IUniformResourceLocator
		pURL->Release();
	}

	if( bCleanupCOM ) CoUninitialize();

	return hr;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::SetupRichSavedGamesW( WCHAR* strSavedGameExtension, WCHAR* strLaunchPath, 
							WCHAR* strCommandLineToLaunchSavedGame )
{
	HKEY hKey = NULL;
	LONG lResult;
	DWORD dwDisposition;
	WCHAR strExt[256];
	WCHAR strType[256];
	WCHAR strCmdLine[256];
	WCHAR strTemp[512];
	size_t nStrLength = 0;

	// Validate args 
	if( strLaunchPath == NULL || strSavedGameExtension == NULL )
	{
		assert( false );
		return E_INVALIDARG;
	}

	// Setup saved game extension arg - make sure there's a period at the start
	if( strSavedGameExtension[0] == L'.' )
	{
		wcscpy(strExt, strSavedGameExtension);
		swprintf(strType, L"%sType", strSavedGameExtension+1);
	}
	else
	{
		swprintf(strExt, L".%s", strSavedGameExtension);
		swprintf(strType, L"%sType", strSavedGameExtension);
	}

	// Create default command line arg if none supplied
	if( strCommandLineToLaunchSavedGame )
		wcscpy(strCmdLine, strCommandLineToLaunchSavedGame );
	else
		wcscpy(strCmdLine, L"\"%1\"" );

	// Create file association & metadata regkeys
	lResult = RegCreateKeyExW( HKEY_CLASSES_ROOT, strExt, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition );
	if( ERROR_SUCCESS == lResult ) 
	{
		// Create the following regkeys:
		//
		// [HKEY_CLASSES_ROOT\.ExampleGameSave]
		// (Default)="ExampleGameSaveFileType"
		//
		nStrLength = wcslen( strType);
		RegSetValueExW( hKey, L"", 0, REG_SZ, (BYTE*)strType, (DWORD)((nStrLength + 1)*sizeof(WCHAR)) );

		// Create the following regkeys:
		//
		// [HKEY_CLASSES_ROOT\.ExampleGameSave\ShellEx\{BB2E617C-0920-11d1-9A0B-00C04FC2D6C1}]
		// (Default)="{4E5BFBF8-F59A-4e87-9805-1F9B42CC254A}"
		//
		HKEY hSubKey = NULL;
		lResult = RegCreateKeyExW( hKey, L"ShellEx\\{BB2E617C-0920-11d1-9A0B-00C04FC2D6C1}", 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, &dwDisposition );
		if( ERROR_SUCCESS == lResult ) 
		{
			swprintf( strTemp, L"{4E5BFBF8-F59A-4e87-9805-1F9B42CC254A}" );
			nStrLength = wcslen(strTemp);
			RegSetValueExW( hSubKey, L"", 0, REG_SZ, (BYTE*)strTemp, (DWORD)((nStrLength + 1)*sizeof(WCHAR)) );
		}
		if( hSubKey ) RegCloseKey( hSubKey );
	}
	if( hKey ) RegCloseKey( hKey );

	lResult = RegCreateKeyExW( HKEY_CLASSES_ROOT, strType, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition );
	if( ERROR_SUCCESS == lResult ) 
	{
		// Create the following regkeys:
		//
		// [HKEY_CLASSES_ROOT\ExampleGameSaveFileType]
		// PreviewTitle="prop:System.Game.RichSaveName;System.Game.RichApplicationName"
		// PreviewDetails="prop:System.Game.RichLevel;System.DateChanged;System.Game.RichComment;System.DisplayName;System.DisplayType"
		//
		size_t nPreviewDetails = 0, nPreviewTitle = 0;
		WCHAR* strPreviewTitle = L"prop:System.Game.RichSaveName;System.Game.RichApplicationName";
		WCHAR* strPreviewDetails = L"prop:System.Game.RichLevel;System.DateChanged;System.Game.RichComment;System.DisplayName;System.DisplayType";
		nPreviewTitle = wcslen( strPreviewTitle);
		nPreviewDetails = wcslen(strPreviewDetails);
		RegSetValueExW( hKey, L"PreviewTitle", 0, REG_SZ, (BYTE*)strPreviewTitle, (DWORD)((nPreviewTitle + 1)*sizeof(WCHAR)) );
		RegSetValueExW( hKey, L"PreviewDetails", 0, REG_SZ, (BYTE*)strPreviewDetails, (DWORD)((nPreviewDetails + 1)*sizeof(WCHAR)) );

		// Create the following regkeys:
		//
		// [HKEY_CLASSES_ROOT\ExampleGameSaveFileType\Shell\Open\Command]
		// (Default)=""%ProgramFiles%\ExampleGame.exe" "%1""
		//
		HKEY hSubKey = NULL;
		lResult = RegCreateKeyExW( hKey, L"Shell\\Open\\Command", 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, &dwDisposition );
		if( ERROR_SUCCESS == lResult ) 
		{
			swprintf( strTemp, L"%s %s", strLaunchPath, strCmdLine );
			nStrLength = wcslen(strTemp);
			RegSetValueExW( hSubKey, L"", 0, REG_SZ, (BYTE*)strTemp, (DWORD)((nStrLength + 1)*sizeof(WCHAR)) );
		}
		if( hSubKey ) RegCloseKey( hSubKey );
	}
	if( hKey ) RegCloseKey( hKey );

	// Create the following regkeys:
	//
	// [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers\.ExampleGameSave]
	// (Default)="{ECDD6472-2B9B-4b4b-AE36-F316DF3C8D60}"
	//
	swprintf( strTemp, L"Software\\Microsoft\\Windows\\CurrentVersion\\PropertySystem\\PropertyHandlers\\%s", strExt );
	lResult = RegCreateKeyExW( HKEY_LOCAL_MACHINE, strTemp, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition );
	if( ERROR_SUCCESS == lResult ) 
	{
		wcscpy(strTemp, L"{ECDD6472-2B9B-4B4B-AE36-F316DF3C8D60}" );
		nStrLength = wcslen(strTemp);
		RegSetValueExW( hKey, L"", 0, REG_SZ, (BYTE*)strTemp, (DWORD)((nStrLength + 1)*sizeof(WCHAR)) );
	}
	if( hKey ) RegCloseKey( hKey );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::RemoveRichSavedGamesW( WCHAR* strSavedGameExtension ) 
{
	WCHAR strExt[256];
	WCHAR strType[256];
	WCHAR strTemp[512];

	// Validate args 
	if( strSavedGameExtension == NULL )
	{
		assert( false );
		return E_INVALIDARG;
	}

	// Setup saved game extension arg - make sure there's a period at the start
	if( strSavedGameExtension[0] == L'.' )
	{
		wcscpy(strExt, strSavedGameExtension);
		swprintf( strType, L"%sType", strSavedGameExtension+1 );
	}
	else
	{
		swprintf( strExt, L".%s", strSavedGameExtension );
		swprintf( strType, L"%sType", strSavedGameExtension );
	}

	// Delete the following regkeys:
	//
	// [HKEY_CLASSES_ROOT\.ExampleGameSave]
	// [HKEY_CLASSES_ROOT\ExampleGameSaveFileType]
	// [HKEY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\PropertySystem\PropertyHandlers\.ExampleGameSave]
	SHDeleteKeyW( HKEY_CLASSES_ROOT, strExt );
	SHDeleteKeyW( HKEY_CLASSES_ROOT, strType );
	swprintf( strTemp, L"Software\\Microsoft\\Windows\\CurrentVersion\\PropertySystem\\PropertyHandlers\\%s", strExt );
	SHDeleteKeyW( HKEY_LOCAL_MACHINE, strTemp );

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::SetupRichSavedGamesA( CHAR* strSavedGameExtension, CHAR* strLaunchPath, CHAR* strCommandLineToLaunchSavedGame )
{
	WCHAR wstrSavedGameExtension[MAX_PATH] = {0};
	WCHAR wstrLaunchPath[MAX_PATH] = {0};
	WCHAR wstrCommandLineToLaunchSavedGame[MAX_PATH] = {0};

	MultiByteToWideChar(CP_ACP, 0, strSavedGameExtension, MAX_PATH, wstrSavedGameExtension, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, strLaunchPath, MAX_PATH, wstrLaunchPath, MAX_PATH);
	MultiByteToWideChar(CP_ACP, 0, strCommandLineToLaunchSavedGame, MAX_PATH, wstrCommandLineToLaunchSavedGame, MAX_PATH);

	return SetupRichSavedGamesW( wstrSavedGameExtension, wstrLaunchPath, wstrCommandLineToLaunchSavedGame );
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBGameExHelper::RemoveRichSavedGamesA( CHAR* strSavedGameExtension ) 
{
	WCHAR wstrSavedGameExtension[MAX_PATH] = {0};
	MultiByteToWideChar(CP_ACP, 0, strSavedGameExtension, MAX_PATH, wstrSavedGameExtension, MAX_PATH);
	return RemoveRichSavedGamesW( wstrSavedGameExtension );
}
