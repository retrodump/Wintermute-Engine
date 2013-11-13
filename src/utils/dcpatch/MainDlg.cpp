// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dcpatch.h"
#include "MainDlg.h"
#include "InstallDlg.h"
#include "StatLink.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	CStaticLink	m_wndLink1;
	CStaticLink	m_wndLink2;
	CStaticLink	m_wndLink4;

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
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
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Dragging = false;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_INST, m_InstButton);
	DDX_Control(pDX, IDC_README, m_Readme);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_INST, OnInstall)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	char pth[MAX_PATH];
	_getcwd (pth,MAX_PATH);
	m_Readme.Navigate(CString("file://") + CString(pth) + "\\" + CString("readme.htm"), NULL, NULL, NULL, NULL);	
	m_Readme.SetFocus();
	
	
	/*
	VERIFY(m_BackBmp.m_dib.Load(AfxGetResourceHandle(), IDB_BACK));
	m_BackBmp.SubclassDlgItem(IDB_BACK, this);
	m_palMsgHandler.Install(this, m_BackBmp.m_dib.GetPalette());

	HRGN region = ::BitmapToRegion((HBITMAP)m_BackBmp.m_dib, RGB(255, 0, 255));
	if(region){
		SetWindowRgn(region, TRUE);
	}


	m_InstButton.SetBitmaps(IDB_INST_N, IDB_INST_P, IDB_INST_H, 152, 203);
	m_AboutButton.SetBitmaps(IDB_ABOUT_N, IDB_ABOUT_P, IDB_ABOUT_H, 68, 136);
	m_CloseButton.SetBitmaps(IDB_CLOSE_N, IDB_CLOSE_P, IDB_CLOSE_H, 238, 135);
*/
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
			
	return FALSE;  // return TRUE  unless you set the focus to a control
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		/*
		m_CloseButton.Invalidate(FALSE);
		m_InstButton.Invalidate(FALSE);
		m_AboutButton.Invalidate(FALSE);
		*/
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//////////////////////////////////////////////////////////////////////////
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnClose() 
{
	OnOK();
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if(rcClient.PtInRect(point)){
		SetCapture();
		m_Dragging = true;
		m_DragFrom = point;
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_Dragging){
		ReleaseCapture();
		m_Dragging = false;
	}

	CDialog::OnLButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_Dragging){
		int xdiff = 0;
		int ydiff = 0;
		CRect rcWnd;
		GetWindowRect(&rcWnd);

		if(point.x != 0 && point.y != 0){
			xdiff = point.x - m_DragFrom.x;
			ydiff = point.y - m_DragFrom.y;
		}

		CRect rcNew(rcWnd.left + xdiff, rcWnd.top + ydiff,rcWnd.right + xdiff, rcWnd.bottom + ydiff);

		SetWindowPos(NULL, rcNew.left, rcNew.top, rcWnd.Width(), rcWnd.Height(), SWP_SHOWWINDOW);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnInstall() 
{
	CInstallDlg dlg;
	dlg.Create(IDD_INSTALL, this);
	dlg.CenterWindow();
	dlg.ShowWindow(SW_SHOW);
	EnableWindow(FALSE);
	bool success = dlg.Install();
	dlg.DestroyWindow();
	EnableWindow(TRUE);
	SetActiveWindow();

	if(success){
		m_InstButton.EnableWindow(FALSE);		
	}
	
	Invalidate(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CAboutDlg dlg;
	dlg.DoModal();
	CDialog::OnLButtonDblClk(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnOK() 
{
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
BOOL CMainDlg::OnEraseBkgnd(CDC* pDC) 
{
	//return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}


//////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	m_wndLink1.SubclassDlgItem(IDC_PART_WEB, this);
	m_wndLink1.m_link = "http://www.wintermute-engine.org";

	m_wndLink2.SubclassDlgItem(IDC_PART_MAIL,  this);
	m_wndLink2.m_link = "mailto:mnemonic@wintermute-engine.org";

	m_wndLink4.SubclassDlgItem(IDC_PART_LOGO, this);
	m_wndLink4.m_link = "http://www.wintermute-engine.org";

	return CDialog::OnInitDialog();
}

BEGIN_EVENTSINK_MAP(CMainDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMainDlg)
	ON_EVENT(CMainDlg, IDC_README, 113 /* TitleChange */, OnTitleChangeReadme, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


//////////////////////////////////////////////////////////////////////////
void CMainDlg::OnTitleChangeReadme(LPCTSTR Text) 
{
	SetWindowText(Text);
}
