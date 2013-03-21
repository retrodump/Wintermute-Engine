// ProjectMan.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ProjectMan.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "ProjectDoc.h"
#include "ProjectView.h"
#include "resource.h"
#include "../MFCExt/StatLink.h"
#include "../MFCExt/utils_mfc.h"
#include "NumEncoder.h"
#include "ReminderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp, CDCGFApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CDCGFApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CDCGFApp::OnFileOpen)
	ON_COMMAND(ID_TOOLS_SCENEEDIT, OnToolsSceneEdit)
	ON_COMMAND(ID_TOOLS_SPRITEEDIT, OnToolsSpriteEdit)
	ON_COMMAND(ID_TOOLS_STRINGTABLE, OnToolsStringTable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CApp initialization

BOOL CApp::InitInstance()
{
	if(!CDCGFApp::InitInstance()) return FALSE;
	
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

/*
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
*/
	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("DEAD:CODE"));

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	CDCGFStringTable::Init();

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_PROJECTYPE,
		RUNTIME_CLASS(CProjectDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CProjectView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	//m_pMainWnd->DragAcceptFiles();

	// This code replaces the MFC created menus with 
	// the Ownerdrawn versions 
	static UINT toolbars[]={
		IDR_MAINFRAME, IDR_TOOLBAR_2
	};	
	pDocTemplate->m_hMenuShared=pMainFrame->NewMenu(IDR_PROJECTYPE, toolbars, 2);
	pMainFrame->m_hMenuDefault=pMainFrame->NewDefaultMenu(IDR_MAINFRAME, toolbars, 2);
/*
	CDCGFStringTable::LocMenu(CMenu::FromHandle(pMainFrame->m_hMenuDefault));
	CDCGFStringTable::LocMenu(CMenu::FromHandle(pDocTemplate->m_hMenuShared));
*/
	// This simulates a window being opened if you don't have
	// a default window displayed at startup
	pMainFrame->OnUpdateFrameMenu(pMainFrame->m_hMenuDefault);


	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand==CCommandLineInfo::FileNew)
		cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNothing)
	{
		DoSomeNagging();

		if(GetRegInt(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "AutoCheckUpdates", 1))
		{
			pMainFrame->CheckForUpdates(true);
		}
	}

	pMainFrame->UpdateMenuURL(pMainFrame->GetMenu());


	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CApp::DoSomeNagging()
{
	CString Magic = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "BBID");
	CNumEncoder Enc;
	Enc.DecodeFromString(Magic);

	CTime Time = CTime::GetCurrentTime();
	
	int Year     = Enc.GetNumber(0, Time.GetYear());
	int Month    = Enc.GetNumber(1, Time.GetMonth());
	int Day      = Enc.GetNumber(2, Time.GetDay());
	int DaysUsed = Enc.GetNumber(3, 1);
	int Answer   = Enc.GetNumber(4, 0); // 0..remind later, 1..done already, 2..don't wanna

	// we started a new day
	if(Year != Time.GetYear() || Month != Time.GetMonth() || Day != Time.GetDay())
	{
		DaysUsed++;

		// nag every 10th day
		if(DaysUsed % 10 == 0 && Answer == 0)
		{
			// display dialog
			CReminderDlg Dlg;
			Dlg.DoModal();
			Answer = Dlg.m_SelectedOption;
		}
	}

	Enc.SetNumber(0, Time.GetYear());
	Enc.SetNumber(1, Time.GetMonth());
	Enc.SetNumber(2, Time.GetDay());
	Enc.SetNumber(3, DaysUsed);
	Enc.SetNumber(4, Answer);

	Magic = Enc.EncodeToString();
	SetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "BBID", Magic);
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStaticLink	m_Mail;
	CStaticLink	m_Web;
	CStatic	m_Product;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MAIL, m_Mail);
	DDX_Control(pDX, IDC_WEB, m_Web);
	DDX_Control(pDX, IDC_PRODUCT, m_Product);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


//////////////////////////////////////////////////////////////////////////
void CApp::OnToolsSceneEdit() 
{
	CString Path = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath");
	if(Path[Path.GetLength()-1]!='\\') Path+="\\";

	Path+="SceneEdit.exe";
	WinExec(Path, SW_MAXIMIZE);
}


//////////////////////////////////////////////////////////////////////////
void CApp::OnToolsSpriteEdit() 
{
	CString Path = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath");
	if(Path[Path.GetLength()-1]!='\\') Path+="\\";

	Path+="SpriteEdit.exe";
	WinExec(Path, SW_MAXIMIZE);
}

//////////////////////////////////////////////////////////////////////////
void CApp::OnToolsStringTable() 
{
	CString Path = GetRegString(HKEY_CURRENT_USER, DCGF_TOOLS_REG_PATH, "ToolsPath");
	if(Path[Path.GetLength()-1]!='\\') Path+="\\";

	Path+="StringTableMgr.exe";
	WinExec(Path, SW_NORMAL);
}


/////////////////////////////////////////////////////////////////////////////
// CApp message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOCWIN(this);
	
	m_Mail.m_link = "mailto:wme@wintermute-engine.org?subject=ProjectMan";
	m_Web.m_link = "http://www.wintermute-engine.org/";

	CString str, new_str;
	m_Product.GetWindowText(str);
	new_str.Format("%s  %d.%d.%03d", str, DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD);
	m_Product.SetWindowText(new_str);
	
	return TRUE;
}
