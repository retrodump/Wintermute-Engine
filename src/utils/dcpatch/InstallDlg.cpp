// InstallDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dcpatch.h"
#include "InstallDlg.h"
#include "BrowseForFolder.h"
#include <direct.h>
#include "../../engine_core/wme_base/dcgf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define FILES_PATH "patch"

/////////////////////////////////////////////////////////////////////////////
// CInstallDlg dialog


CInstallDlg::CInstallDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInstallDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInstallDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_INFO, m_InfoText);
	DDX_Control(pDX, IDC_ACTIONS, m_ActionsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInstallDlg, CDialog)
	//{{AFX_MSG_MAP(CInstallDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstallDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CInstallDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageList = new CImageList;
	m_ImageList->Create(IDB_ICONS, 16, 1, 0x00008080);
	m_ActionsList.SetImageList(m_ImageList, LVSIL_SMALL);

	
	m_ActionsList.DeleteAllItems();
	m_ActionsList.InsertItem(0, "Locating installation",   0);
	m_ActionsList.InsertItem(1, "Preparing files",         0);
	m_ActionsList.InsertItem(2, "Copying files",           0);
	m_ActionsList.InsertItem(3, "Finalizing installation", 0);

	m_InfoText.SetWindowText("");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
BOOL CInstallDlg::DestroyWindow() 
{
	delete m_ImageList;

	return CDialog::DestroyWindow();
}


//////////////////////////////////////////////////////////////////////////
bool CInstallDlg::Install()
{
	CWaitCursor wait;
	
	SetItemIcon(0, 0);
	SetItemIcon(2, 0);
	SetItemIcon(3, 0);

	if(!InstInit(0))    return false;
	if(!InstPreCopy(1)) return false;
	if(!InstCopy(2))    return false;
	if(!InstFinish(3))  return false;

	MessageBox("Patch has been applied successfuly.", "Done", MB_ICONINFORMATION);
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CInstallDlg::InstInit(int Index)
{
	SetItemIcon(Index, 3);
	ProcessMessages();
	Sleep(500);
	

	bool ret=false;

	// check version
	CString WMEVersion = GetRegString(HKEY_CURRENT_USER, "Software\\DEAD:CODE\\Wintermute Tools\\Settings", "InstallVersion");

	DWORD VerMajor = atoi(Entry(1, WMEVersion, '.'));
	DWORD VerMinor = atoi(Entry(2, WMEVersion, '.'));
	DWORD VerBuild = atoi(Entry(3, WMEVersion, '.'));

	bool VerOK = true;
	if(VerMajor < 1) VerOK = false;
	else if(VerMajor == 1 && VerMinor < 8) VerOK = false;
	
	if(!VerOK)
	{
		SetItemIcon(Index, 2);
		MessageBox("WME 1.8 must be installed prior to installing this update.\nPlease download the complete installer from www.wintermute-engine.org", "Error", MB_ICONERROR);
		return false;
	}


	// check running processes
	bool WMERunning = true;
	do
	{
		m_WMEWindowTitles.RemoveAll();
		EnumWindows(EnumWindowsCallback, (LPARAM)this);
		if(m_WMEWindowTitles.GetSize()>0)
		{
			CString Titles = "";
			for(int i=0; i<m_WMEWindowTitles.GetSize(); i++)
			{
				Titles += "\n" + m_WMEWindowTitles[i];
			}
			int ID = MessageBox("The update process requires you to quit the following applications first:" + CString("\n") + Titles, "Update", MB_ICONWARNING|MB_ABORTRETRYIGNORE);
			if(ID==IDABORT)
			{
				SetItemIcon(Index, 2);
				MessageBox("Update has been cancelled.", "Error", MB_ICONERROR);
				return false;
			}
			else if(ID==IDIGNORE) WMERunning = false;
			else WMERunning = true;
		}
		else WMERunning = false;
	}
	while(WMERunning);
	


	HKEY key;
	DWORD type, size = MAX_PATH;
	char val[MAX_PATH];
	LONG lr = RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\DEAD:CODE\\Wintermute Tools\\Settings", 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS){
		lr = RegQueryValueEx(key, "ToolsPath", 0, &type, (BYTE*)val, &size);
		RegCloseKey(key);
	}
	if(lr != ERROR_SUCCESS){
		lr = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\DEAD:CODE\\Wintermute Tools\\Settings", 0, KEY_READ, &key);
		if(lr == ERROR_SUCCESS){
			lr = RegQueryValueEx(key, "ToolsPath", 0, &type, (BYTE*)val, &size);
			RegCloseKey(key);
		}
	}

	if(lr == ERROR_SUCCESS){
		m_GameDir = CString(val);
		if(m_GameDir=="") ret = false;
		else if(m_GameDir[m_GameDir.GetLength()-1] != '\\'){
			m_GameDir += '\\';
			ret = true;
		}
	}
	
	CFileFind finder;
	ret = true;
	while(ret && !finder.FindFile(m_GameDir + "wme.exe")){
		CBrowseForFolder bf;	
		bf.hWndOwner = this->m_hWnd;
		bf.strTitle = "Please specify the WME DevKit installation folder.";
		bf.strStartupDir = m_GameDir;

		if (!bf.GetFolder(m_GameDir)) ret = false;
		else if(m_GameDir[m_GameDir.GetLength()-1] != '\\') m_GameDir += '\\';
	}
	
	if(!ret){
		SetItemIcon(Index, 2);
		MessageBox("Unable to locate the previous WME DevKit installation.", "Error", MB_ICONERROR);
	}
	/*
	else{
		DWORD FileSize = 0;
		FILE* f = fopen(LPCSTR(m_GameDir + "wa.exe"), "rb");
		if(f){
			fseek(f, 0, SEEK_END);
			FileSize = ftell(f);
			fclose(f);
		}

		if(FileSize!=5112320 && FileSize!=2099200){
			ret = false;
			SetItemIcon(Index, 2);
			MessageBox("Neodpovídá velikost souboru. Ujistìte se, že byla hra správnì nainstalována z originálního CD.", "Chyba", MB_ICONERROR);
		}
	}
	*/


	SetItemIcon(Index, ret?1:2);
	return ret;

}

//////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CInstallDlg::EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
	CInstallDlg* _this = (CInstallDlg*)lParam;
	if(!_this) return FALSE;

	char ClassName[MAX_PATH];
	::GetClassNameA(hwnd, ClassName, MAX_PATH);

	char WindowName[MAX_PATH];
	::GetWindowText(hwnd, WindowName, MAX_PATH);

	if(_stricmp(ClassName, "GF_FRAME")==0 || strstr(WindowName, "WME Project Manager") || strstr(WindowName, "SpriteEdit")  || strstr(WindowName, "SceneEdit"))
	{
		::GetWindowText(hwnd, ClassName, MAX_PATH);
		_this->m_WMEWindowTitles.Add(ClassName);
	}

	return TRUE;
}



//////////////////////////////////////////////////////////////////////////
bool CInstallDlg::InstPatch(int Index)
{
	SetItemIcon(Index, 3);
	ProcessMessages();
	Sleep(500);
	SetItemIcon(Index, 1);
	ProcessMessages();
	Sleep(500);
	
	return true;
}


/////////////////////////////////////////////////////////////////////////////
bool CInstallDlg::InstPreCopy(int Index)
{
	SetItemIcon(Index, 3);
	ProcessMessages();

	Sleep(500);

	m_CopyFiles.RemoveAll();
	ScanDir(".\\" + CString(FILES_PATH));

	m_Package.ReadPackage(".\\patch.dcp");

	SetItemIcon(Index, 1);
	ProcessMessages();
	Sleep(500);
	
	return true;
}


/////////////////////////////////////////////////////////////////////////////
bool CInstallDlg::InstCopy(int Index)
{
	SetItemIcon(Index, 3);
	ProcessMessages();
	Sleep(500);

	bool ret=true;

	int i;
	m_Progress.SetRange32(0, m_CopyFiles.GetSize() + m_Package.GetNumEntries());
	if(m_CopyFiles.GetSize() + m_Package.GetNumEntries()==0) ret = false;
	
	// single files
	if(ret){
		for(i=0; i<m_CopyFiles.GetSize(); i++){
			m_InfoText.SetWindowText("Copying \"" + m_CopyFiles[i] + "\"...");
			m_Progress.SetPos(i);
			ProcessMessages();
		
			CString source = ".\\" + CString(FILES_PATH) + "\\" + m_CopyFiles[i];
			CString target = m_GameDir + m_CopyFiles[i];

			if(MakePath(target)==""){
				ret = false;
				break;
			}
			else{
				SetFileAttributes(LPCSTR(target), FILE_ATTRIBUTE_NORMAL);
				ret = (::CopyFile(LPCSTR(source), LPCSTR(target), FALSE)!=0);
				if(!ret) break;
			}
		}
	}

	// package files
	if(ret){
		for(i=0; i<m_Package.GetNumEntries(); i++){
			m_InfoText.SetWindowText("Copying \"" + m_Package.GetEntryName(i) + "\"...");
			m_Progress.SetPos(i + m_CopyFiles.GetSize());
			ProcessMessages();

			if(MakePath(m_GameDir + m_Package.GetEntryName(i))==""){
				ret = false;
				break;
			}
			else{
				ret = m_Package.ExtractEntry(i, m_GameDir);
				if(!ret) break;
			}
		}
	}


	
	m_Progress.SetPos(0);
	m_InfoText.SetWindowText("");
	SetItemIcon(Index, ret?1:2);

	if(!ret) MessageBox("Error copying files.", "Error", MB_ICONERROR);

	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool CInstallDlg::InstFinish(int Index)
{
	SetItemIcon(Index, 3);
	ProcessMessages();

	char String[MAX_PATH];
	sprintf(String, "%d.%d.%d", DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD);


	HKEY key;
	LONG lr = RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\DEAD:CODE\\Wintermute Tools\\Settings", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, "InstallVersion", 0, REG_SZ, (BYTE*)String, strlen(String)+1);
		lr = RegSetValueEx(key, "InstallPath", 0, REG_SZ, (BYTE*)LPCSTR(m_GameDir), m_GameDir.GetLength()+1);
		RegCloseKey(key);
	}
	
	
	Sleep(500);
	SetItemIcon(Index, 1);
	ProcessMessages();
	Sleep(500);
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
void CInstallDlg::ProcessMessages()
{
	MSG message;
	while(::PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
		::TranslateMessage(&message);
		::DispatchMessage(&message);
	}
}


//////////////////////////////////////////////////////////////////////////
void CInstallDlg::SetItemIcon(int Item, int Icon)
{
	CString label = m_ActionsList.GetItemText(Item, 0);
	m_ActionsList.DeleteItem(Item);
	m_ActionsList.InsertItem(Item, label, Icon);
}


////////////////////////////////////////////////////////////////////////////////
void ReadString(FILE* f, char* str){
	BYTE len;
	fread(&len, 1, 1, f);
	fread(str, len, 1, f);
}


//////////////////////////////////////////////////////////////////////////
void CInstallDlg::ScanDir(CString BaseDir)
{
	CFileFind finder;

	BOOL Working = finder.FindFile(BaseDir + "\\*.*");
	while (Working) {
		Working = finder.FindNextFile();
		if(!finder.IsDots()){
			if(finder.IsDirectory()) ScanDir(BaseDir + "\\" + finder.GetFileName());
			else AddFile(finder.GetFilePath());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void CInstallDlg::AddFile(CString Filename)
{
	char pth[MAX_PATH];
	_getcwd (pth,MAX_PATH);
	CString CurrentDir = CString(pth);
	if(CurrentDir[CurrentDir.GetLength()-1] != '\\') CurrentDir += '\\';
	CurrentDir += CString(FILES_PATH) + "\\";

	m_CopyFiles.Add(Filename.Right(Filename.GetLength() - CurrentDir.GetLength()));
}
