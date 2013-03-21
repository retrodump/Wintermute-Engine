// DCGFDoc.cpp : implementation file
//

#include "stdafx.h"
#include "DCGFApp.h"
#include "DCGFDoc.h"
#include "RootDlg.h"
#include <Shlwapi.h>
#include <direct.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Shlwapi.lib")


#define PROJECT_ROOT_STRING "; $EDITOR_PROJECT_ROOT_DIR$ "

/////////////////////////////////////////////////////////////////////////////
// CDCGFDoc

IMPLEMENT_DYNCREATE(CDCGFDoc, CDocument)


//////////////////////////////////////////////////////////////////////////
CDCGFDoc::CDCGFDoc()
{
}


//////////////////////////////////////////////////////////////////////////
CDCGFDoc::~CDCGFDoc()
{
}


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFDoc::OnNewDocument()
{
	if(!QueryProjectRoot()) return FALSE;

	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}



BEGIN_MESSAGE_MAP(CDCGFDoc, CDocument)
	//{{AFX_MSG_MAP(CDCGFDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCGFDoc diagnostics

#ifdef _DEBUG
void CDCGFDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDCGFDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDCGFDoc serialization

void CDCGFDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDCGFDoc commands


//////////////////////////////////////////////////////////////////////////
bool CDCGFDoc::QueryProjectRoot(CString Filename)
{
	CFrameWnd* frame = ((CMDIFrameWnd*)AfxGetMainWnd())->GetActiveFrame();
		
	CRootDlg dlg;
	CDCGFApp* app = (CDCGFApp*)AfxGetApp();
	for(int i=0; i<app->m_ProjectRoots.GetSize(); i++){
		dlg.AddPrevFolder(app->m_ProjectRoots[i]);
	}
	
	dlg.m_Filename = Filename;
	dlg.m_SelectedFolder = m_ProjectRoot;
	
	if(dlg.DoModal()==IDOK){
		//MessageBox(NULL, dlg.m_SelectedFolder, "", MB_OK);
		if(frame) frame->ShowWindow(SW_SHOW);
		m_ProjectRoot = dlg.m_SelectedFolder;
		app->AddProjectRoot(dlg.m_SelectedFolder);
		return true;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CStdioFile f;
	CString line;
	bool RootFound = false;
	if(f.Open(lpszPathName, CFile::modeRead | CFile::typeText)){
		while(f.ReadString(line)){
			if(line.Find(PROJECT_ROOT_STRING)==0){
				CString root = line.Right(line.GetLength() - strlen(PROJECT_ROOT_STRING));
				root.TrimLeft();
				root.TrimRight();


				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				_splitpath(lpszPathName, drive, dir, NULL, NULL);
				
				char AbsPath[MAX_PATH];
				sprintf(AbsPath, "%s%s", drive, dir);
				chdir(AbsPath);
				_fullpath(AbsPath, LPCSTR(root), MAX_PATH);

				m_ProjectRoot = CString(AbsPath);

				DWORD attr = GetFileAttributes(LPCSTR(AbsPath));
				if(attr == 0xFFFFFFFF || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0){
					RootFound = false;
				}
				else{					
					RootFound = true;
				}

				break;
			}
		}
		f.Close();
	}

	if(RootFound){
		((CDCGFApp*)AfxGetApp())->AddProjectRoot(m_ProjectRoot);
		return TRUE;
	}
	else return QueryProjectRoot(lpszPathName);
}


//////////////////////////////////////////////////////////////////////////
BOOL CDCGFDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// we don't want MFC to screw up our file
	return TRUE;

	//return CDocument::OnSaveDocument(lpszPathName);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CDCGFDoc::WriteEditorData(LPCTSTR lpszPathName, CBDynBuffer *Buffer)
{
	char RelPath[MAX_PATH];
	//PathRelativePathTo(RelPath, LPCSTR(m_ProjectRoot), FILE_ATTRIBUTE_NORMAL, lpszPathName, FILE_ATTRIBUTE_NORMAL);
	if(!PathRelativePathTo(RelPath, lpszPathName, FILE_ATTRIBUTE_NORMAL, LPCSTR(m_ProjectRoot), FILE_ATTRIBUTE_NORMAL)){
		strcpy(RelPath, LPCSTR(m_ProjectRoot));
	}

	Buffer->PutTextIndent(0, "%s%s\n\n", PROJECT_ROOT_STRING, RelPath);
	return S_OK;
}
