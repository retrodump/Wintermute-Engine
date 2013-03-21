// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include <afxinet.h>
#include "ProjectMan.h"


#include "MainFrm.h"
#include "PropSheetEx.h"
#include "../MFCExt/PageGeneral.h"
#include "../MFCExt/utils_mfc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CDCGFFrameWindow)

BEGIN_MESSAGE_MAP(CMainFrame, CDCGFFrameWindow)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_HELP_DOCS, OnHelpDocs)
	ON_COMMAND(ID_HELP_UPDATES, OnHelpUpdates)
	ON_COMMAND(ID_HELP_DONATE, OnHelpDonate)
	ON_COMMAND_EX_RANGE(ID_HELP_WEB1, ID_HELP_WEB10, OnOpenHelpURL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDCGFFrameWindow::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CDCGFFrameWindow::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CDCGFFrameWindow::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CDCGFFrameWindow::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnToolsOptions() 
{
	CPropSheetEx dlgPropertySheet(LOC("/str0107/Shared tools settings"));
	CPageGeneral PageGeneral;

	dlgPropertySheet.AddPage(&PageGeneral);
	dlgPropertySheet.DoModal();	
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnHelpDocs() 
{
	char full_path[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), full_path, MAX_PATH);


	::HtmlHelp(NULL, GetPath(full_path) + "wme.chm", HH_DISPLAY_TOPIC, 0);
}


//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnHelpUpdates() 
{	
	CheckForUpdates(false);
}

//////////////////////////////////////////////////////////////////////////
void CMainFrame::OnHelpDonate() 
{	
	::ShellExecute(GetSafeHwnd(), "open", "http://dead-code.org/redir.php?target=donate", "", "", SW_SHOWNORMAL);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CMainFrame::CheckForUpdates(bool Silent)
{
	// check if internet connection is active
	if(Silent)
	{
		DWORD nFlags = 0;
		if(!InternetGetConnectedState(&nFlags, 0) || (nFlags & INTERNET_CONNECTION_OFFLINE))
		{
			return S_OK;
		}
	}


	CWaitCursor wait;
	
	CString WMEVersion;
	HRESULT Ret = GetWMEVersionHttp(WMEVersion);
	if(FAILED(Ret)){
		if(!Silent) MessageBox(LOC("/str0145/Error querying latest available version."), NULL, MB_OK|MB_ICONERROR);
		return Ret;
	}
	else{
		DWORD VerMajor = atoi(Entry(1, WMEVersion, '.'));
		DWORD VerMinor = atoi(Entry(2, WMEVersion, '.'));
		DWORD VerBuild = atoi(Entry(3, WMEVersion, '.'));

		bool UpdateAvailable = false;
		
		if(VerMajor > DCGF_VER_MAJOR) UpdateAvailable = true;
		else if(VerMajor == DCGF_VER_MAJOR && VerMinor > DCGF_VER_MINOR) UpdateAvailable = true;
		else if(VerMajor == DCGF_VER_MAJOR && VerMinor == DCGF_VER_MINOR && VerBuild > DCGF_VER_BUILD) UpdateAvailable = true;
		
		if(!UpdateAvailable){
			if(!Silent) MessageBox(LOC("/str0146/Your WME version is up to date."), NULL, MB_OK|MB_ICONINFORMATION);
		}
		else{
			if(IDYES==MessageBox(LOC("/str0147/There is an updated version of WME available. Do you want to display the download page now?"), "", MB_OK|MB_ICONQUESTION|MB_YESNO)){
				ShellExecute(NULL, "open", LOC("/str1085/http://dead-code.org/redir.php?target=updates"), NULL, NULL, SW_SHOWMAXIMIZED);
			}
		}
	}


	return Ret;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CMainFrame::GetWMEVersionHttp(CString& WMEVersion)
{
	HRESULT RetCode = S_OK;
	WMEVersion = "0.0.0";

	CString Magic = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "BBID");

	CInternetSession Session;
	CHttpConnection* Server = NULL;
	CHttpFile* File = NULL;

	DWORD HttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;
	const TCHAR Headers[] = _T("Accept: text/*\r\nUser-Agent: WME ProjectMan\r\n");


	CString Url = LOC("/str1086/http://www.dead-code.org/vercheck.php");
	
	CString CurrVer;
	CurrVer.Format("%d.%d.%03d", DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD);
	
	Url += "?product=wme&ver=" + CurrVer;
	Url += "&bbid=" + Magic;
	if(DCGF_VER_BETA) Url += "&beta=1";

	bool DotNet = RegKeyExists(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\.NETFramework\\policy\\v2.0");
	Url += "&dotnet=" + CString(DotNet?"1":"0");
		
	CString ServerName;
	CString Object;
	INTERNET_PORT Port;
	DWORD ServiceType;

	try{
		if(!AfxParseURL(Url, ServiceType, ServerName, Object, Port) || ServiceType != INTERNET_SERVICE_HTTP){
			return E_FAIL;
		}


		Server = Session.GetHttpConnection(ServerName, Port);

		File = Server->OpenRequest(CHttpConnection::HTTP_VERB_GET, Object, NULL, 1, NULL, NULL, HttpRequestFlags);
		File->AddRequestHeaders(Headers);
		if(File->SendRequest()){
			TCHAR sz[1024];
			if(File->ReadString(sz, 1023)){
				WMEVersion = Entry(1, CString(sz), '\n');
			}			
		}

		File->Close();
		Server->Close();
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet
		//TCHAR szErr[1024];
		//pEx->GetErrorMessage(szErr, 1024);
		//MessageBox(szErr, LOC("/str1002/Error"), MB_OK|MB_ICONERROR);

		RetCode = E_FAIL;
		pEx->Delete();
	}

	if (File != NULL) delete File;
	if (Server != NULL) delete Server;
	Session.Close();

	return RetCode;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CMainFrame::UpdateMenuURL(CMenu* Menu)
{
	m_ListURL.RemoveAll();

	// read from registry
	HKEY key;
	DWORD type, size, data_size;
	LONG lr;

	CString Path = CString(DCGF_TOOLS_REG_PATH) + "\\MenuURL";

	size = MAX_PATH;
	data_size = 1024;
	char Name[MAX_PATH];
	BYTE Data[1024];

	//CMenu* Menu = CMenu::FromHandle(m_hMenuDefault);
	

	int Index = 0;
	lr = RegOpenKeyEx(HKEY_CURRENT_USER, Path, 0, KEY_READ, &key);
	if(lr==ERROR_SUCCESS){

		while(ERROR_SUCCESS==RegEnumValue(key, Index, Name, &size, NULL, &type, Data, &data_size)){			

			if(type==REG_SZ){
				m_ListURL.Add(Data);
				Menu->ModifyMenu(ID_HELP_WEB1 + Index, MF_BYCOMMAND|MF_STRING, ID_HELP_WEB1 + Index, Name);
			}

			if(m_ListURL.GetSize()==10) break;


			Index++;
			size = MAX_PATH;
			data_size = 1024;
		}

	}


	if(m_ListURL.GetSize()==0){
		SetRegString(HKEY_CURRENT_USER, Path, "WME Homepage", "http://dead-code.org/");
		SetRegString(HKEY_CURRENT_USER, Path, "WME Forum", "http://forum.dead-code.org/");

		return UpdateMenuURL(Menu);
	}
	else{
		for(int i=Index; i<10; i++){
			Menu->RemoveMenu(ID_HELP_WEB1 + i, MF_BYCOMMAND);
		}
	}

	return S_OK;
}



//////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::OnOpenHelpURL(UINT nID)
{
	int Index = nID - ID_HELP_WEB1;

	if(Index >= 0 && Index < m_ListURL.GetSize()) ShellExecute(NULL, "open", m_ListURL[Index], NULL, NULL, SW_SHOWMAXIMIZED);
	return TRUE;
}
