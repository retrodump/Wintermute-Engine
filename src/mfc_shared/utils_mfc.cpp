#include "stdafx.h"
#include "utils_mfc.h"



//////////////////////////////////////////////////////////////////////////
bool UpdateIntEdit(CWnd* Owner, CEdit* Edit, int* Var){
	
	CString str;
	int i;
	Edit->GetWindowText(str);
	i = atoi(str);
	if(i!=*Var || str==""){
		*Var = i;
		str.Format("%d", i);
		Edit->SetWindowText(str);
		if(Owner->GetFocus()==Edit) Edit->SetSel(0, -1);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
bool UpdateFloatEdit(CWnd* Owner, CEdit* Edit, float* Var)
{
	CString str;
	float f;
	Edit->GetWindowText(str);
	f = atof(str);
	if(f!=*Var || str==""){
		*Var = f;
		str.Format("%f", f);
		Edit->SetWindowText(str);
		if(Owner->GetFocus()==Edit) Edit->SetSel(0, -1);
		return true;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
void SetupButton(CBCGButton* Button, LPCTSTR Tooltip, int ID)
{
	Button->m_nFlatStyle = CBCGButton::BUTTONSTYLE_FLAT;
	Button->m_bDrawFocus = FALSE;
	Button->SetTooltip(Tooltip);
	Button->SetImage(ID);

}


//////////////////////////////////////////////////////////////////////////
CString GetRelativeFilename(CBGame *Game, CString InitFile, CString Filter, CString& Dir, CWnd* Parent, bool AllowCreate)
{
	CString ret = "";

	// make absolute path
	Game->m_FileManager->RestoreCurrentDir();
	for(int i=0; i<Game->m_FileManager->m_SinglePaths.GetSize(); i++){
		CString NewPath = CString(Game->m_FileManager->m_SinglePaths[i]) + InitFile;
		CFileFind finder;
		if(finder.FindFile(NewPath)){
			InitFile = NewPath;
			break;
		}
	}


	Filter += "|All Files (*.*)|*.*||";
	DWORD Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
	if(!AllowCreate) Flags |= OFN_FILEMUSTEXIST;
	
	CFileDialog dlg(TRUE, NULL, InitFile, Flags, Filter, Parent);
	if(Dir=="") Dir = CString(Game->m_FileManager->m_BasePath);
	dlg.m_ofn.lpstrInitialDir = LPCSTR(Dir);
	
	if(dlg.DoModal()==IDOK){

		CFileFind finder;
		if(!finder.FindFile(dlg.GetPathName())){
			if(Parent->MessageBox(dlg.GetPathName() + "\nThe specified file doesn't exist.\nDo you want to create it?", NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
				CStdioFile File;
				if(!File.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText)){
					Parent->MessageBox("Error creating file '" + dlg.GetPathName() + "'.", NULL, MB_OK|MB_ICONERROR);
					return "";
				}
				File.Close();

			}
			else return "";
		}

		
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath(LPCSTR(dlg.GetPathName()), drive, dir, NULL, NULL);
		Dir.Format("%s%s", drive, dir);

		ret = dlg.GetPathName();
		char* str = new char[ret.GetLength()+1];
		strcpy(str, LPCSTR(ret));
		Game->m_FileManager->MakeRelativePath(str);
		ret = CString(str);
		delete [] str;
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString MakePath(CString Path){

	char drive[_MAX_DRIVE+1];
	char path[_MAX_PATH+1];
	int i;
	CString result;

	_splitpath(LPCSTR(Path), drive, path, NULL, NULL);
	Path = CString(drive) + "\\" + CString(path);

	if(Path[Path.GetLength()-1] != '\\') Path += '\\';


	for(i = 0; i<Path.GetLength(); i++)
		if(Path[i] == '/') Path.SetAt(i, '\\');

	for(i = 0; i<Path.GetLength(); i++)
		if(i == 0 || Path[i] != '\\' || (Path[i] == '\\' && Path[i-1] != '\\')){
			result += Path[i];
			if(Path[i] == '\\'){
				CreateDirectory(LPCSTR(result), NULL);
				DWORD attr = GetFileAttributes(LPCSTR(result));
				if(attr == 0xFFFFFFFF || (attr & FILE_ATTRIBUTE_DIRECTORY) == 0) return "";
			}

		}

	return result;
}


//////////////////////////////////////////////////////////////////////////
CString MakeAbsolutePath(CString RelativePath, CBGame* Game)
{
	if(!Game) return "";

	CString AbsPath;

	Game->m_FileManager->RestoreCurrentDir();
	for(int i=0; i<Game->m_FileManager->m_SinglePaths.GetSize(); i++){
		CString NewPath = CString(Game->m_FileManager->m_SinglePaths[i]) + RelativePath;
		CFileFind finder;
		if(finder.FindFile(NewPath)){
			AbsPath = NewPath;
			break;
		}
	}
	return AbsPath;
}


//////////////////////////////////////////////////////////////////////////
CString GetPath(CString Filename)
{
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	_splitpath(Filename, drive, path, NULL, NULL);
	
	CString ret;
	ret.Format("%s%s", drive, path);
	return ret;
}


//////////////////////////////////////////////////////////////////////////
int NumEntries(CString List, char Delimiter)
{
	int NumEntries=1;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]==Delimiter) NumEntries++;
	}
			
	return NumEntries;
}


//////////////////////////////////////////////////////////////////////////
int NumEntriesEx(CString List, char Delimiter)
{
	int NumEntries=1;
	bool InQuote = false;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]=='\"'){
			InQuote = !InQuote; 
			continue;
		}		
		if(List[i]==Delimiter && !InQuote) NumEntries++;
	}
			
	return NumEntries;
}


//////////////////////////////////////////////////////////////////////////
CString Entry(int Entry, CString List, char Delimiter)
{
	int CurrEntry=1;
	CString ret = "";

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]==Delimiter) CurrEntry++;
		else if(CurrEntry==Entry){
			ret += List[i];
		}
		if(CurrEntry>Entry) break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString EntryEx(int Entry, CString List, char Delimiter)
{
	int CurrEntry=1;
	CString ret = "";
	bool InQuote = false;

	for(int i=0; i<List.GetLength(); i++){
		if(List[i]=='\"'){
			InQuote = !InQuote; 
			continue;
		}
		if(List[i]==Delimiter && !InQuote) CurrEntry++;
		else if(CurrEntry==Entry){
			ret += List[i];
		}
		if(CurrEntry>Entry) break;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
CString GetExt(CString Filename)
{
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);
	return CString(ext+1);
}


//////////////////////////////////////////////////////////////////////////
CString GetRelativePath(CBGame* Game, CString Filename)
{
	if(!Game || !Game->m_FileManager) return Filename;

	CString RelFilename;
	char* temp = new char[Filename.GetLength()+1];
	strcpy(temp, Filename);
	Game->m_FileManager->MakeRelativePath(temp);
	RelFilename = CString(temp);
	delete [] temp;

	return RelFilename;
}


//////////////////////////////////////////////////////////////////////////
CString GetAbsolutePath(CString Base, CString RelPath)
{
	Base.Replace("/", "\\");
	RelPath.Replace("/", "\\");

	Base.TrimRight("\\");
	RelPath.TrimRight("\\");

	int NumUp=0;
	for(int i=1; i<=NumEntries(RelPath, '\\'); i++){
		if(Entry(i, RelPath, '\\')=="..") NumUp++;
		else break;
	}

	if(NumUp>NumEntries(Base, '\\')) return RelPath;

	CString Result = "";
	if(NumUp>0){
		for(int i=1; i<=NumEntries(Base, '\\')-NumUp; i++){
			Result+=Entry(i, Base, '\\') + "\\";
		}
	}

	for(int i=NumUp+1; i<=NumEntries(RelPath, '\\'); i++){
		Result+=Entry(i, RelPath, '\\') + '\\';
	}

	return Result;	
}
