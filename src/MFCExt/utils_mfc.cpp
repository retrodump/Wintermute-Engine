#include "stdafx.h"
#include "DCGFApp.h"
#include "DCGFStringTable.h"
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


	Filter += CString("|") + LOC("/str1003/All Files") + " (*.*)|*.*||";
	DWORD Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_PATHMUSTEXIST|OFN_NOCHANGEDIR;
	if(!AllowCreate) Flags |= OFN_FILEMUSTEXIST;

	CFileDialog dlg(TRUE, NULL, InitFile, Flags, Filter, Parent);
	dlg.m_ofn.lpstrInitialDir = LPCSTR(Dir);
	
	if(dlg.DoModal()==IDOK){
		CFileFind finder;
		if(!finder.FindFile(dlg.GetPathName())){
			if(Parent->MessageBox(dlg.GetPathName() + LOC("/str1079/\nThe specified file doesn't exist.\nDo you want to create it?"), NULL, MB_YESNO|MB_ICONQUESTION)==IDYES){
				CStdioFile File;
				if(!File.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeText)){
					Parent->MessageBox(CString(LOC("/str1080/Error creating file")) + " '" + dlg.GetPathName() + "'.", NULL, MB_OK|MB_ICONERROR);
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
	if(RelativePath.Find(':')!=-1) return RelativePath;

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
bool EditFile(CString Filename, CBGame* Game, CWnd* Parent, bool OpenOnly)
{
	if(!Game) return false;

	HWND win;
	if(Parent) win = Parent->GetSafeHwnd();
	else win = NULL;

	CString EditFile = Filename;

	// make an absolute path
	EditFile = MakeAbsolutePath(Filename, Game);

	if(EditFile!="")
	{
		((CDCGFApp*)AfxGetApp())->m_Active = false;
		if(OpenOnly || (int)ShellExecute(win, "edit", EditFile, "", "", SW_NORMAL)<=32)
			if((int)ShellExecute(win, "open", EditFile, "", "", SW_NORMAL)<=32)
			{
				char drive[_MAX_DRIVE];
				char dir[_MAX_DIR];
				CString ExploreDir;
				_splitpath(EditFile, drive, dir, NULL, NULL);
				ExploreDir.Format("%s%s", drive, dir);

				ShellExecute(win, "open", ExploreDir, "", "", SW_NORMAL);
			}
		return true;
	}
	else
	{
		if(Parent) Parent->MessageBox(CString(LOC("/str1081/File")) + " '" + Filename + "' " + LOC("/str1082/could not be found."), NULL, MB_OK|MB_ICONERROR);
		return false;
	}
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
CString GetFName(CString Filename)
{
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, fname, ext);

	CString ret;
	ret.Format("%s%s", fname, ext);
	return ret;
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



//////////////////////////////////////////////////////////////////////////
int ComparePattern(const char* pattern, const char* string)
{
  char stringc,patternc;

  for (;; ++string)
  {
    stringc=toupper(*string);
    patternc=toupper(*pattern++);
    switch (patternc)
    {
      case 0:
        return(stringc==0);
      case '?':
        if (stringc == 0)
          return(FALSE);

        break;
      case '*':
        if (!*pattern)
          return(TRUE);

        if (*pattern=='.')
        {
          char *dot;
          if (pattern[1]=='*' && pattern[2]==0)
            return(TRUE);
          dot=(char*)strchr(string,'.');
          if (pattern[1]==0)
            return (dot==NULL || dot[1]==0);
          if (dot!=NULL)
          {
            string=dot;
            if (strpbrk(pattern,"*?[")==NULL && strchr(string+1,'.')==NULL)
              return(stricmp(pattern+1,string+1)==0);
          }
        }

        while (*string)
          if (ComparePattern(pattern,string++))
            return(TRUE);
        return(FALSE);
      default:
        if (patternc != stringc)
          if (patternc=='.' && stringc==0)
            return(ComparePattern(pattern,string));
          else
            return(FALSE);
        break;
    }
  }
}

//////////////////////////////////////////////////////////////////////////
bool RegKeyExists(HKEY hKey, CString Path)
{
	HKEY key;
	LONG lr = RegOpenKeyEx(hKey, Path, 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS)
	{
		RegCloseKey(key);
		return true;
	}
	else return false;
}

//////////////////////////////////////////////////////////////////////////
CString GetRegString(HKEY hKey, CString Path, CString Key, CString Init)
{
	HKEY key;
	DWORD type, size;
	LONG lr;

	size = MAX_PATH;
	char val[MAX_PATH];
	lr = RegOpenKeyEx(hKey, Path, 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS){
		lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)val, &size);
		RegCloseKey(key);
	}
	
	if(lr == ERROR_SUCCESS) return CString(val);
	else return Init;
}


//////////////////////////////////////////////////////////////////////////
int GetRegInt(HKEY hKey, CString Path, CString Key, int Init)
{
	HKEY key;
	DWORD type, size;
	LONG lr;

	size = sizeof(int);
	int val;
	lr = RegOpenKeyEx(hKey, Path, 0, KEY_READ, &key);
	if(lr == ERROR_SUCCESS){
		lr = RegQueryValueEx(key, Key, 0, &type, (BYTE*)&val, &size);
		RegCloseKey(key);
	}
	
	if(lr == ERROR_SUCCESS) return val;
	else return Init;
}


//////////////////////////////////////////////////////////////////////////
bool SetRegString(HKEY hKey, CString Path, CString Key, CString Value)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_SZ, (BYTE*)LPCSTR(Value), Value.GetLength()+1);
		RegCloseKey(key);
	}

	return (lr==ERROR_SUCCESS);
}


//////////////////////////////////////////////////////////////////////////
bool SetRegInt(HKEY hKey, CString Path, CString Key, int Value)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegSetValueEx(key, Key, 0, REG_DWORD, (BYTE*)&Value, sizeof(int));
		RegCloseKey(key);
	}

	return (lr==ERROR_SUCCESS);
}



//////////////////////////////////////////////////////////////////////////
bool DelRegKey(HKEY hKey, CString Path, CString Key)
{
	HKEY key;

	LONG lr = RegCreateKeyEx(hKey, Path, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &key, NULL);
	if(lr == ERROR_SUCCESS){
		lr = RegDeleteValue(key, Key);
	}

	return (lr==ERROR_SUCCESS);
}
