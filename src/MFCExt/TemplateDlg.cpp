// TemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TemplateDlg.h"
#include "objects.h"
#include "BrowseForFolder.h"
#include "DCGFStringTable.h"
#include "utils_mfc.h"
#include <Shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "Shlwapi.lib")

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg dialog


//////////////////////////////////////////////////////////////////////////
CTemplateDlg::CTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTemplateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_SelectDir = false;

	m_Source = m_Target = "";
	m_Subdir = "";
	m_InitName = "";
	m_Title = "";
	Game = NULL;
	m_ProjectRoot = "";
	m_SelectedName = "";
	m_SelectedObject = "";
}


//////////////////////////////////////////////////////////////////////////
CTemplateDlg::~CTemplateDlg()
{
	m_RepPaths.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemplateDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_DIR_LABEL, m_DirLabel);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_NAME_LABEL, m_NameLabel);
	DDX_Control(pDX, IDC_DIR, m_Dir);
	DDX_Control(pDX, IDC_NAME, m_Name);
	DDX_Control(pDX, IDC_TEMPLATES, m_Templates);
	DDX_Control(pDX, IDC_REPOSITORY, m_Repository);
	DDX_Control(pDX, IDC_PROPS, m_PropList);
	DDX_Control(pDX, IDC_DESC, m_Desc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CTemplateDlg)
	ON_CBN_SELCHANGE(IDC_REPOSITORY, OnSelchangeRepository)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_CHANGE(IDC_DIR, OnChangeDir)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TEMPLATES, OnChangeTemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LOCWIN(this);
	
	m_Templates.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	m_Templates.InsertColumn(0,LOC("/str1055/Template"), LVCFMT_LEFT, 235);
	
	m_ImageList.Create(IDB_NODES, 16, 1, RGB(255, 0, 255));
	m_Templates.SetImageList(&m_ImageList, LVSIL_SMALL);


	// setup repository paths

	// local repository
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char full_path[MAX_PATH];
	CString Path;
	GetModuleFileName(AfxGetInstanceHandle(), full_path, MAX_PATH);
	_splitpath(full_path, drive, dir, NULL, NULL);
	Path.Format("%s%s", drive, dir);
	if(Path=="" || Path[Path.GetLength()-1]!='\\') Path+= "\\";
	AddRepositoryPath(Path + "template");

	CString InitPath = AfxGetApp()->GetProfileString("Settings", "LastRepository", Path);

	// common repositories
	for(int i=0; i<10; i++){
		CString RepKey;
		RepKey.Format("RepositoryPath%d", i+1);

		
		HKEY key;
		DWORD type, size = MAX_PATH;
		char val[MAX_PATH];
		LONG lr = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\DEAD:CODE\\Wintermute Tools\\Settings", 0, KEY_READ, &key);
		if(lr == ERROR_SUCCESS){
			lr = RegQueryValueEx(key, RepKey, 0, &type, (BYTE*)val, &size);
			RegCloseKey(key);
		}
		if(lr == ERROR_SUCCESS) AddRepositoryPath(val);
	}

	bool Found = false;
	for(int i=0; i<m_RepPaths.GetSize(); i++){
		m_Repository.AddString(m_RepPaths[i]);

		if(m_RepPaths[i]==InitPath){
			m_Repository.SelectString(-1, InitPath);
			Found = true;
		}
	}
	if(!Found) m_Repository.SelectString(-1, m_RepPaths[0]);
	ReadTemplates();


	if(!m_SelectDir){
		CRect rect1, rect2;

		m_Browse.ShowWindow(SW_HIDE);
		m_Dir.ShowWindow(SW_HIDE);
		m_DirLabel.ShowWindow(SW_HIDE);

		m_Templates.GetWindowRect(&rect1);
		ScreenToClient(&rect1);
		m_PropList.GetWindowRect(&rect2);
		ScreenToClient(&rect2);
		rect2.top = rect1.top;
		m_PropList.MoveWindow(&rect2);
		
		m_NameLabel.SetWindowText(LOC("/str1035/Name:"));
	}
	else m_NameLabel.SetWindowText(LOC("/str1037/Project name:"));

	m_LastName = "";
	m_OK.EnableWindow(FALSE);

	if(m_SelectDir){
		CString Dir = AfxGetApp()->GetProfileString("Settings", "LastProjectDir", "");
		m_Dir.SetWindowText(Dir);
	}


	m_PropList.SetPropChangeCallback(HookPropChange, this);

	if(m_InitName!=""){
		m_Name.SetWindowText(m_InitName);
		OnChangeName();
	}

	m_Name.SetSel(0, -1);
	m_Name.SetFocus();

	if(m_Title!=""){
		SetWindowText(m_Title);
	}
	else{
		if(m_Subdir!=""){
			CString Title;
			GetWindowText(Title);
			SetWindowText(Title + " (" + m_Subdir + ")");
		}
	}


	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::AddRepositoryPath(CString Path)
{
	if(Path=="") return;

	if(Path[Path.GetLength()-1]!='\\') Path+= "\\";

	for(int i=0; i<m_RepPaths.GetSize(); i++){
		if(m_RepPaths[i]==Path) return;
	}

	m_RepPaths.Add(Path);
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnSelchangeRepository() 
{
	int i = m_Repository.GetCurSel();
	if(i>=0 && i<m_RepPaths.GetSize()) AfxGetApp()->WriteProfileString("Settings", "LastRepository", m_RepPaths[i]);
	ReadTemplates();
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnBrowse() 
{
	CBrowseForFolder bf;	
	bf.hWndOwner = this->GetSafeHwnd();
	bf.strTitle = LOC("/str1038/Select a target folder...");

	CString Dir="";
	m_Dir.GetWindowText(Dir);

	bf.strStartupDir = Dir;

	if(bf.GetFolder(Dir)){
		if(Dir=="" || Dir[Dir.GetLength()-1]!='\\') Dir+="\\";
		CString Name;
		m_Name.GetWindowText(Name);
		Dir+=Name;

		m_Dir.SetWindowText(Dir);
	}
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnChangeName() 
{
	CString NewName;
	m_Name.GetWindowText(NewName);

	CString Dir;
	m_Dir.GetWindowText(Dir);
	
	if(m_SelectDir){
		if(Dir.Right(m_LastName.GetLength()+1)=="\\"+m_LastName){
			Dir = Dir.Left(Dir.GetLength() - m_LastName.GetLength());
		}

		if(Dir=="" || Dir[Dir.GetLength()-1]!='\\') Dir+="\\";
		Dir += NewName;


		m_Dir.SetWindowText(Dir);		
	}
	m_LastName = NewName;

	SetOKState();
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnChangeDir() 
{
	SetOKState();
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnOK() 
{
	// check name
	CString Name;
	m_Name.GetWindowText(Name);

	if(Name=="") return;
	if(Name.FindOneOf("./\\:*?<>\"| ")!=-1){
		MessageBox(LOC("/str1039/The specified name contains illegal characters.\n\n(Illegal characters are: ./\\:*?<>\"| and space)"), NULL, MB_OK|MB_ICONWARNING);
		return;
	}

	// check dir
	CString Dir;
	m_Dir.GetWindowText(Dir);

	if(m_SelectDir){
		if(Dir=="") return;
		m_Target = Dir;
	}

	if(m_Target=="" || m_Target[m_Target.GetLength()-1]!='\\') m_Target+="\\";
	
	CFileFind finder;
	if(!finder.FindFile(m_Source)){
		MessageBox(CString(LOC("/str1040/Cannot find the source file/folder")) + " '" + m_Source + "'.", NULL, MB_OK|MB_ICONWARNING);
		return;
	}

	CWaitCursor wait;
	finder.FindNextFile();
	if(finder.IsDirectory()){
		if(!ProcessDir(m_Source, m_Target)) return;
	}
	else{
		if(!ProcessFile(m_Source, m_Target)) return;
	}
	

	// save settings
	if(m_SelectDir){
		if(Dir.Right(m_LastName.GetLength()+1)=="\\"+m_LastName){
			Dir = Dir.Left(Dir.GetLength() - m_LastName.GetLength());
		}		
		AfxGetApp()->WriteProfileString("Settings", "LastProjectDir", Dir);
	}

	m_SelectedName = m_LastName;

	// and quit
	CDialog::OnOK();
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ReadTemplates()
{
	m_Templates.DeleteAllItems();

	CString Dir;
	int i = m_Repository.GetCurSel();
	if(i>=0 && i<m_RepPaths.GetSize()) Dir = m_RepPaths[i];
	else return false;

	if(Dir=="" || Dir[Dir.GetLength()-1]!='\\') Dir+="\\";
	Dir+=m_Subdir+"\\";

	CFileFind finder;
	BOOL working = finder.FindFile(Dir + "*.*");
	i=0;
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots()) continue;

		if(finder.IsDirectory()){
			CFileFind finder2;
			if(finder2.FindFile(finder.GetFilePath() + "\\_template"))
				m_Templates.InsertItem(i++, finder.GetFileName(), ICON_FOLDER);
		}
		else m_Templates.InsertItem(i++, finder.GetFileName(), ICON_FILE);
	}

	if(m_Templates.GetItemCount()>0) m_Templates.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	else HandleTemplate("");
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnChangeTemplate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if(pNMListView->iItem!=-1){

		// getting selected?
		BOOL bPrevSel = (BOOL)((pNMListView->uOldState & LVIS_SELECTED));
		if(bPrevSel < 0) bPrevSel = 0;
	
		BOOL bSel=(BOOL)((pNMListView->uNewState & LVIS_SELECTED)); 
		if (bSel < 0) bSel = 0;

		if(bSel){
			HandleTemplate(m_Templates.GetItemText(pNMListView->iItem, 0));
		}
		else HandleTemplate("");
	}
	
	*pResult = 0;
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::HandleTemplate(CString Name)
{
	CString RepPath="";
	int i = m_Repository.GetCurSel();
	if(i>=0 && i<m_RepPaths.GetSize()) RepPath = m_RepPaths[i];


	if(Name=="" || RepPath==""){
		m_Source = "";		
	}
	else{
		if(RepPath[RepPath.GetLength()-1]!='\\') RepPath+="\\";
		RepPath+=m_Subdir+"\\";
		m_Source = RepPath + Name;
		
		CFileFind finder;
		CString Source;
		
		finder.FindFile(m_Source);
		finder.FindNextFile();
		
		if(finder.IsDirectory()){
			Source = m_Source;
			if(Source[Source.GetLength()-1]!='\\') Source+="\\";
			Source+="_template";

			if(!finder.FindFile(Source)) m_Source = "";
		}
		else Source = m_Source;

		if(m_Source!=""){
			CWaitCursor wait;
			if(!m_TemplateMgr.LoadDefinitions(Source)){
				MessageBox(m_TemplateMgr.GetErrorString(), NULL, MB_OK|MB_ICONERROR);
				m_Source = "";
			}
			else{
				CString Desc = m_TemplateMgr.m_Description;
				Desc.Replace("\n", "\r\n");
				m_Desc.SetWindowText(Desc);
								
			}
		}
	}


	if(m_Source==""){
		m_Desc.SetWindowText("");		
		m_TemplateMgr.Cleanup();
	}

	DisplayProps();

	SetOKState();

	return (m_Source!="");
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::SetOKState()
{
	BOOL Enable = TRUE;

	CString Name;
	m_Name.GetWindowText(Name);

	CString Dir;
	m_Dir.GetWindowText(Dir);

	if(m_SelectDir && Dir=="") Enable = FALSE;
	if(m_Source=="") Enable = FALSE;
	if(Name=="") Enable = FALSE;

	m_OK.EnableWindow(Enable);
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::HookPropChange(CPLProperty *Prop, CPLCategory *Cat, void *Data)
{
	((CTemplateDlg*)Data)->OnPropChange(Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
void CTemplateDlg::OnPropChange(CPLProperty *Prop, CPLCategory *Cat)
{
	//////////////////////////////////////////////////////////////////////////
	if(Cat->GetID()=="default"){
		m_TemplateMgr.m_Dict[Prop->GetID()] = Prop->GetValue();
	}
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::DisplayProps()
{
	m_PropList.Cleanup();
	
		
	CPLPropertyEdit* pEdit;
	CPLPropertyCombo* pCombo;
	CPLCategory* cat;
	
	cat = m_PropList.AddCategory("default", LOC("/str1041/Template properties"));
	
	
	for(int i=0; i<m_TemplateMgr.m_Props.GetSize(); i++){
		CTemplateProp* prop = m_TemplateMgr.m_Props[i];

		// combo
		if(prop->m_Choices.GetSize()>0){
			pCombo = new CPLPropertyCombo(prop->m_Name, prop->m_Caption);
			for(int j=0; j<prop->m_Choices.GetSize(); j++){
				pCombo->AddValue(prop->m_Choices[j]);
			}
			m_PropList.AddProperty(cat,  pCombo);
			pCombo->SetValue(prop->m_Value);
		}
		// normal
		else{
			pEdit = new CPLPropertyEdit(prop->m_Name, prop->m_Caption);
			m_PropList.AddProperty(cat, pEdit);
			pEdit->SetValue(prop->m_Value);
			
			CPLPropertyEdit::TPropertyEditType Type;
			if(prop->m_Type=="signed") Type = CPLPropertyEdit::EDIT_SIGNED;
			else if(prop->m_Type=="unsigned") Type = CPLPropertyEdit::EDIT_UNSIGNED;
			else Type = CPLPropertyEdit::EDIT_ANY;

			if(prop->m_Validate) pEdit->SetValidation(Type, true, prop->m_MinValue, prop->m_MaxValue);
			else pEdit->SetValidation(Type);
		}
	}

	
	m_PropList.Populate(true);
	m_PropList.Redraw(true);

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ProcessFile(CString SourceFile, CString TargetPath)
{
	// get relative path
	CString RelTarget = TargetPath;
	RelTarget.TrimRight("\\");
	RelTarget = GetRelativePath(Game, RelTarget);
	
	
	CString Name;
	m_Name.GetWindowText(Name);
	CString TargetFile = TargetPath + Name + "." + GetExt(SourceFile);

	CFileFind finder;
	if(finder.FindFile(TargetFile)){
		if(MessageBox(CString(LOC("/str1042/The target file")) + " '" + TargetFile + "' " + LOC("/str1043/already exists.\nDo you want to overwrite it?"), NULL, MB_YESNO|MB_ICONQUESTION)!=IDYES)
			return false;
	}

	m_TemplateMgr.m_Dict["NAME"] = Name;
	m_TemplateMgr.m_Dict["BASE_PATH"] = RelTarget;


	if(!ParseFile(SourceFile, TargetFile)){
		MessageBox(m_TemplateMgr.GetErrorString(), NULL, MB_OK|MB_ICONERROR);
		return false;
	}
	else{
		m_SelectedObject = TargetFile;
		return true;
	}
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ProcessDir(CString SourceDir, CString TargetPath)
{
	CString Name;
	m_Name.GetWindowText(Name);


	if(!m_SelectDir && !m_TemplateMgr.m_NoDir) TargetPath+=Name+"\\";
	if(SourceDir[SourceDir.GetLength()-1]!='\\') SourceDir+="\\";


	if(!m_TemplateMgr.m_NoDir){
		CFileFind finder;
		CString Targ = TargetPath;
		Targ.TrimRight("\\");
		if(finder.FindFile(Targ)){
			finder.FindNextFile();
			if(!finder.IsDirectory()){
				MessageBox(CString(LOC("/str1044/The destination folder already contains file named")) + " '" + Name + "'.\n" + LOC("/str1045/Please select another name."), NULL, MB_OK|MB_ICONWARNING);
				return false;
			}
			else {
				if(MessageBox(CString(LOC("/str1046/The destination folder already contains subfolder")) + " '" + Name + "'. " + LOC("/str1047/Some of its contents may be overwritten.\nDo you want to continue?"), NULL, MB_OKCANCEL|MB_ICONWARNING)!=IDOK)
					return false;
			}
		}
	}

	// get relative path
	CString RelTarget = TargetPath;
	RelTarget.TrimRight("\\");
	RelTarget = GetRelativePath(Game, RelTarget);


	m_TemplateMgr.m_Dict["NAME"] = Name;
	m_TemplateMgr.m_Dict["BASE_PATH"] = RelTarget;

	if(!ParseDir(SourceDir, TargetPath)){
		MessageBox(m_TemplateMgr.GetErrorString(), NULL, MB_OK|MB_ICONERROR);
		return false;
	}
	else{
		m_SelectedObject = TargetPath;
		return true;
	}
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ParseFile(CString SourceFile, CString TargetFile)
{
	// expand macros in filename
	m_TemplateMgr.ExpandMacros(TargetFile, TargetFile, true);
	
	CString RelFilename = GetRelativePath(Game, TargetFile);
	m_TemplateMgr.m_Dict["FILENAME"] = RelFilename;

	char RelPath[MAX_PATH];
	if(!PathRelativePathTo(RelPath, TargetFile, FILE_ATTRIBUTE_NORMAL, LPCSTR(m_ProjectRoot), FILE_ATTRIBUTE_NORMAL)){
		strcpy(RelPath, LPCSTR(m_ProjectRoot));
	}
	m_TemplateMgr.m_Dict["PROJECT_ROOT"] = RelPath;
	

	CString ShortName;
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(TargetFile, drive, path, fname, ext);
	ShortName.Format("%s%s", fname, ext);
	m_TemplateMgr.m_Dict["SHORT_FILENAME"] = ShortName;


	// reset errors
	m_TemplateMgr.m_Errors.RemoveAll();


	// create target path
	if(MakePath(TargetFile)==""){
		m_TemplateMgr.AddError(CString(LOC("/str1048/Error creating folder for file")) + " '" + TargetFile + "'");
		return false;
	}


	if(IsBinary(SourceFile)){
		CopyFile(SourceFile, TargetFile, FALSE);
	}
	else{

		CString RelPath;
		RelPath.Format("%s%s", drive, path);
		RelPath = GetRelativePath(Game, RelPath);
		RelPath.TrimRight("\\");
		m_TemplateMgr.m_Dict["PATH"] = RelPath;

		CString Error;
		Error.Format(CString(LOC("/str1049/Parsing file")) + " '%s'...", SourceFile);
		m_TemplateMgr.AddError(Error);


		CStringArray SourceArray;
		CStringArray TargetArray;

		CStdioFile f;
		CString line;
		if(f.Open(SourceFile, CFile::modeRead | CFile::typeText)){
			while(f.ReadString(line)){
				SourceArray.Add(line);
			}
			f.Close();
		}
		else{		
			Error.Format(CString(LOC("/str1050/Error opening file")) + " '%s' " + LOC("/str1051/for reading"), SourceFile);
			m_TemplateMgr.AddError(Error);
			return false;
		}

		if(!m_TemplateMgr.ParseTemplate(SourceArray, TargetArray)) return false;

		if(f.Open(TargetFile, CFile::modeWrite | CFile::modeCreate | CFile::typeText)){
			for(int i=0; i<TargetArray.GetSize(); i++){
				f.WriteString(TargetArray[i] + "\n");
			}
			f.Close();
		}
		else{
			Error.Format(CString(LOC("/str1050/Error opening file")) + " '%s' " + LOC("/str1052/for writing"), TargetFile);
			m_TemplateMgr.AddError(Error);
			return false;
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ParseDir(CString SourceDir, CString TargetDir)
{
	CFileFind finder;

	BOOL working = finder.FindFile(SourceDir+"*.*");
	while(working){
		working = finder.FindNextFile();
		if(finder.IsDots()) continue;

		if(finder.IsDirectory()){
			CString DirName;
			m_TemplateMgr.ExpandMacros(TargetDir+finder.GetFileName()+"\\", DirName, true);
			MakePath(DirName);
			if(!ParseDir(SourceDir+finder.GetFileName()+"\\", TargetDir+finder.GetFileName()+"\\"))
				return false;
		}
		else{
			if(finder.GetFileName().CompareNoCase("_template")==0) continue;
			else if(finder.GetFileName().CompareNoCase("_merge")==0){
				if(!ParseMergeFile(SourceDir, TargetDir, finder.GetFilePath())) return false;

			}
			else if(!ParseFile(SourceDir+finder.GetFileName(), TargetDir+finder.GetFileName()))
				return false;
		}
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::ParseMergeFile(CString SourceDir, CString TargetDir, CString MergeFile)
{
	m_TemplateMgr.m_Errors.RemoveAll();
	CString Error;
	Error.Format(CString(LOC("/str1053/Parsing merge file")) + " '%s'...", MergeFile);
	m_TemplateMgr.AddError(Error);

	CStdioFile f;
	CString line;
	if(f.Open(MergeFile, CFile::modeRead | CFile::typeText)){
		while(f.ReadString(line)){
			if(line=="" || line[0]==';') continue;

			char SourceFileTemp[MAX_PATH];
			char SourceFile[MAX_PATH];
			CString TargetFile;
			sprintf(SourceFileTemp, "%s%s", SourceDir, line);
			PathCanonicalize(SourceFile, SourceFileTemp);

			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];
			_splitpath(SourceFile, NULL, NULL, fname, ext);
			TargetFile.Format("%s%s%s", TargetDir, fname, ext);

			m_TemplateMgr.ExpandMacros(TargetFile, TargetFile, true);

			if(!ParseFile(SourceFile, TargetFile)){
				f.Close();
				return false;
			}
			/*
			if(MakePath(CString(TargetFile))==""){
				m_TemplateMgr.AddError("Error creating folder for file '" + TargetFile + "'");
				return false;
			}

			CopyFile(SourceFile, TargetFile, FALSE);
			*/
		}
		f.Close();
	}
	else{
		m_TemplateMgr.AddError(LOC("/str1054/Error opening merge file"));
		return false;
	}

	
	return true;
}


#define BUFFER_SIZE 16384
//////////////////////////////////////////////////////////////////////////
bool CTemplateDlg::IsBinary(CString Filename)
{
	FILE* f = fopen(Filename, "rb");
	if(!f) return true;

	bool IsBinary = false;
	BYTE* buffer = new BYTE[BUFFER_SIZE];

	DWORD read = fread(buffer, 1, BUFFER_SIZE, f);
	while(read>0 && !IsBinary){
		for(int i=0; i<read; i++){
			if(buffer[i]<0x09){
				IsBinary = true;
				break;
			}
		}
		read = fread(buffer, 1, BUFFER_SIZE, f);
	}
	fclose(f);
	delete [] buffer;

	return IsBinary;
}
